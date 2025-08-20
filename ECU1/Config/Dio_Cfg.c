#include "Dio_Cfg.h"

/* Dio Channel configuration */
static const Dio_ChannelCfgType kChannels[] = {
  /* ChannelId 0: Button1  */ { "gpiochip1", 28, DIO_DIRECTION_IN  },  /* P9_12 -> gpop 60  */
  /* ChannelId 1: Button2  */ { "gpiochip1", 16, DIO_DIRECTION_IN  },  /* P9_15 -> gpio 48  */
};

/* Port configuration */
static const unsigned int kPort0_Offsets[] = { 60, 48 };
static const Dio_PortCfgType kPorts[] = {
  { "gpiochip1", kPort0_Offsets, 2 }  
};

/* ChannelGroup configuration */
static const Dio_ChannelGroupType kGroups[] = {
  { .mask = 0x2u, .offset = 0u, .port = 0u },
};

const Dio_ConfigType Dio_Config = {
  .Channels = kChannels,
  .NumChannels = (uint16)(sizeof(kChannels)/sizeof(kChannels[0])),
  .Ports = kPorts,
  .NumPorts = (uint16)(sizeof(kPorts)/sizeof(kPorts[0])),
  .ChannelGroups = kGroups,
  .NumChannelGroups = (uint16)(sizeof(kGroups)/sizeof(kGroups[0]))
};
