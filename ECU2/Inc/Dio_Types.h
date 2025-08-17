#ifndef DIO_TYPES_H
#define DIO_TYPES_H

#include "Std_Types.h"

/* === Kiểu dữ liệu AUTOSAR === */
typedef uint16 Dio_ChannelType;        /* ID logic cho 1 line */
typedef uint16 Dio_PortType;           /* ID logic cho 1 port */
typedef uint8  Dio_LevelType;          /* STD_LOW(0)/STD_HIGH(1) */
typedef uint32 Dio_PortLevelType;      /* bitmap các line trong 1 port */

typedef struct {
  uint32 mask;       /* bitmask các channel thuộc group */
  uint8  offset;     /* bit offset thấp nhất trong group */
  Dio_PortType port; /* port owner */
} Dio_ChannelGroupType;

/* === Cấu hình binding sang libgpiod === */
typedef enum {
  DIO_DIRECTION_IN  = 0,
  DIO_DIRECTION_OUT = 1
} Dio_DirectionType;

typedef struct {
  const char* chip;          /* "gpiochip0", "gpiochip1", ... */
  unsigned int offset;       /* gpiod line offset trong chip */
  Dio_DirectionType dir;     /* IN/OUT để request line */
} Dio_ChannelCfgType;

typedef struct {
  const char* chip;              /* chip chứa các line của port */
  const unsigned int* offsets;  /* mảng offset các line theo thứ tự bit */
  uint8 length;                 /* số line trong port */
} Dio_PortCfgType;

typedef struct {
  const Dio_ChannelCfgType* Channels;
  uint16 NumChannels;
  const Dio_PortCfgType*    Ports;
  uint16 NumPorts;
  const Dio_ChannelGroupType* ChannelGroups;
  uint16 NumChannelGroups;
} Dio_ConfigType;

#endif /* DIO_TYPES_H */
