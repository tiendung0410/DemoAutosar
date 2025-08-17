#include "Dio_Cfg.h"

/* Button1, Button2 là input; Led1 là output */
static const Dio_ChannelCfgType kChannels[] = {
  /* ChannelId 0: Button1  */ { "gpiochip0", 60, DIO_DIRECTION_IN  },  /* ví dụ P9_12 -> offset 60 (minh họa) */
  /* ChannelId 1: Button2  */ { "gpiochip0", 48, DIO_DIRECTION_IN  },  /* ví dụ P9_15 */
};

/* Port0 gom 3 line ở trên theo thứ tự bit [2:0] = {Led1, Button2, Button1} */
static const unsigned int kPort0_Offsets[] = { 60, 48 };
static const Dio_PortCfgType kPorts[] = {
  { "gpiochip0", kPort0_Offsets, 2 }  /* chú ý: Led1 ở chip1 trong ví dụ này -> nếu trộn chip, tách ra 2 port là thực tế hơn */
};

/* ChannelGroup ví dụ: nhóm 2 button (bit0..1) trên Port0 */
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
