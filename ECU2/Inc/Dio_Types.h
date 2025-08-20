#ifndef DIO_TYPES_H
#define DIO_TYPES_H

#include "Std_Types.h"

/* === AUTOSAR Types === */
typedef uint16 Dio_ChannelType;        /* ID logic for 1 line */
typedef uint16 Dio_PortType;           /* ID logic for 1 port */
typedef uint8  Dio_LevelType;          /* STD_LOW(0)/STD_HIGH(1) */
typedef uint32 Dio_PortLevelType;      /* lines bitmap in 1 port */

typedef struct {
  uint32 mask;       /* channels bitmap in group */
  uint8  offset;     /* lowest bit offset in group */
  Dio_PortType port; /* port owner */
} Dio_ChannelGroupType;

typedef enum {
  DIO_DIRECTION_IN  = 0,
  DIO_DIRECTION_OUT = 1
} Dio_DirectionType;

typedef struct {
  const char* chip;          /* "gpiochip0", "gpiochip1", ... */
  unsigned int offset;       /* gpiod line offset in chip */
  Dio_DirectionType dir;     /* IN/OUT to request line */
} Dio_ChannelCfgType;

typedef struct {
  const char* chip;             /* chip store lines of port */
  const unsigned int* offsets;  /* line offset bit table */
  uint8 length;                 /* line number in port */
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
