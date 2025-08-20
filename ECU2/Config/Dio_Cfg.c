#include "Dio_Cfg.h"


static const Dio_ChannelCfgType kChannels[] = {
  /* ChannelId 0: Led1     */ { "gpiochip0", 31, DIO_DIRECTION_OUT },  /*  P9_13m  */

};

static const unsigned int kPort0_Offsets[] = { 31 };
static const Dio_PortCfgType kPorts[] = {
  { "gpiochip0", kPort0_Offsets, 1 }  
};


static const Dio_ChannelGroupType kGroups[] = {
  { .mask = 0x1u, .offset = 0u, .port = 0u },
};

const Dio_ConfigType Dio_Config = {
  .Channels = kChannels,
  .NumChannels = (uint16)(sizeof(kChannels)/sizeof(kChannels[0])),
  .Ports = kPorts,
  .NumPorts = (uint16)(sizeof(kPorts)/sizeof(kPorts[0])),
  .ChannelGroups = kGroups,
  .NumChannelGroups = (uint16)(sizeof(kGroups)/sizeof(kGroups[0]))
};
