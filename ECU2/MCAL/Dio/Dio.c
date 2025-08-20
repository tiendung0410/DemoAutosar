#include "Dio.h"
#include <gpiod.h>
#include <string.h>

/* ================== Private state ================== */
typedef struct {
  struct gpiod_chip* chip;
} Dio_ChipHandle;

typedef struct {
  struct gpiod_line* line;
  Dio_DirectionType  dir;
} Dio_LineHandle;

static const Dio_ConfigType* s_cfg = NULL;

#define MAX_OPEN_CHIPS 16
static Dio_ChipHandle s_openChips[MAX_OPEN_CHIPS];
static char           s_openChipNames[MAX_OPEN_CHIPS][32];
static uint8          s_numOpenChips = 0;

/* Line array requested base on ChannelId logic */
static Dio_LineHandle* s_lines = NULL;

/* ==== Helpers ==== */
static int find_or_open_chip(const char* name) {
  for (uint8 i=0;i<s_numOpenChips;i++) {
    if (strncmp(s_openChipNames[i], name, sizeof(s_openChipNames[i])) == 0) return i;
  }
  if (s_numOpenChips >= MAX_OPEN_CHIPS) return -1;
  struct gpiod_chip* chip = gpiod_chip_open_by_name(name);
  if (!chip) return -1;
  strncpy(s_openChipNames[s_numOpenChips], name, sizeof(s_openChipNames[0])-1);
  s_openChipNames[s_numOpenChips][sizeof(s_openChipNames[0])-1] = '\0';
  s_openChips[s_numOpenChips].chip = chip;
  return (int)(s_numOpenChips++);
}

static struct gpiod_line* request_line(const char* chipName, unsigned int offset, Dio_DirectionType dir) {
  int idx = find_or_open_chip(chipName);
  if (idx < 0) return NULL;
  struct gpiod_line* line = gpiod_chip_get_line(s_openChips[idx].chip, offset);

  if (!line) return NULL;

  int ret;
  if (dir == DIO_DIRECTION_OUT) {
    ret = gpiod_line_request_output(line, "autosar-mcal-dio", 0);
  } else {
    ret = gpiod_line_request_input(line, "autosar-mcal-dio");
  }
  if (ret < 0) return NULL;
  return line;
}

/* ================== API ================== */
void Dio_Init(const Dio_ConfigType* ConfigPtr) {
  s_cfg = (ConfigPtr != NULL) ? ConfigPtr : &Dio_Config;

  if (s_lines) {
    for (uint16 ch=0; ch<s_cfg->NumChannels; ch++) {
      if (s_lines[ch].line) {
        gpiod_line_release(s_lines[ch].line);
        s_lines[ch].line = NULL;
      }
    }
    free(s_lines);
    s_lines = NULL;
  }
  for (uint8 i=0;i<s_numOpenChips;i++) {
    if (s_openChips[i].chip) gpiod_chip_close(s_openChips[i].chip);
    s_openChips[i].chip = NULL;
  }
  s_numOpenChips = 0;
  memset(s_openChipNames, 0, sizeof(s_openChipNames));

  /* Request channels */
  s_lines = (Dio_LineHandle*)calloc(s_cfg->NumChannels, sizeof(Dio_LineHandle));
  for (uint16 ch=0; ch<s_cfg->NumChannels; ch++) {
    const Dio_ChannelCfgType* c = &s_cfg->Channels[ch];
    struct gpiod_line* line = request_line(c->chip, c->offset, c->dir);
    s_lines[ch].line = line;
    s_lines[ch].dir  = c->dir;
  }
}

Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId) {
  if (!s_cfg || ChannelId >= s_cfg->NumChannels) return (Dio_LevelType)0;
  if (!s_lines[ChannelId].line) return (Dio_LevelType)0;
  int v = gpiod_line_get_value(s_lines[ChannelId].line);
  return (v > 0) ? (Dio_LevelType)1 : (Dio_LevelType)0;
}

void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level) {
  if (!s_cfg || ChannelId >= s_cfg->NumChannels) return;
  if (!s_lines[ChannelId].line) return;
  if (s_lines[ChannelId].dir != DIO_DIRECTION_OUT) return; /* Optional: DET */
  gpiod_line_set_value(s_lines[ChannelId].line, (Level != 0) ? 1 : 0);
}

Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId) {
  if (!s_cfg || ChannelId >= s_cfg->NumChannels) return (Dio_LevelType)0;
  if (!s_lines[ChannelId].line) return (Dio_LevelType)0;
  if (s_lines[ChannelId].dir != DIO_DIRECTION_OUT) return Dio_ReadChannel(ChannelId);
  int cur = gpiod_line_get_value(s_lines[ChannelId].line);
  int nxt = (cur > 0) ? 0 : 1;
  gpiod_line_set_value(s_lines[ChannelId].line, nxt);
  return (Dio_LevelType)(nxt ? 1 : 0);
}

/* === Port operations  === */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId) {
  if (!s_cfg || PortId >= s_cfg->NumPorts) return 0;
  const Dio_PortCfgType* p = &s_cfg->Ports[PortId];
  Dio_PortLevelType v = 0;
  for (uint8 i=0;i<p->length;i++) {
    /* Find ChannelId corresponding to offset to get requested line */
    for (uint16 ch=0; ch<s_cfg->NumChannels; ch++) {
      const Dio_ChannelCfgType* c = &s_cfg->Channels[ch];
      if (strcmp(c->chip, p->chip)==0 && c->offset==p->offsets[i]) {
        int r = s_lines[ch].line ? gpiod_line_get_value(s_lines[ch].line) : 0;
        if (r > 0) v |= (1u << i);
        break;
      }
    }
  }
  return v;
}

void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level) {
  if (!s_cfg || PortId >= s_cfg->NumPorts) return;
  const Dio_PortCfgType* p = &s_cfg->Ports[PortId];
  for (uint8 i=0;i<p->length;i++) {
    uint8 bit = (Level >> i) & 0x1;
    for (uint16 ch=0; ch<s_cfg->NumChannels; ch++) {
      const Dio_ChannelCfgType* c = &s_cfg->Channels[ch];
      if (strcmp(c->chip, p->chip)==0 && c->offset==p->offsets[i]) {
        if (s_lines[ch].line && s_lines[ch].dir==DIO_DIRECTION_OUT) {
          gpiod_line_set_value(s_lines[ch].line, bit ? 1 : 0);
        }
        break;
      }
    }
  }
}

void Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask) {
  Dio_PortLevelType cur = Dio_ReadPort(PortId);
  Dio_PortLevelType newv = (cur & ~Mask) | (Level & Mask);
  Dio_WritePort(PortId, newv);
}

/* === Channel group (mask/offset on 1 port) === */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* grp) {
  if (!grp) return 0;
  Dio_PortLevelType portv = Dio_ReadPort(grp->port);
  return (portv & grp->mask) >> grp->offset;
}

void Dio_WriteChannelGroup(const Dio_ChannelGroupType* grp, Dio_PortLevelType Level) {
  if (!grp) return;
  Dio_PortLevelType val = (Level << grp->offset) & grp->mask;
  Dio_MaskedWritePort(grp->port, val, grp->mask);
}

/* === Version info === */
void Dio_GetVersionInfo(uint16* major, uint16* minor, uint16* patch) {
  if (major) *major = DIO_SW_MAJOR_VERSION;
  if (minor) *minor = DIO_SW_MINOR_VERSION;
  if (patch) *patch = DIO_SW_PATCH_VERSION;
}
