#ifndef IOHWAB_CFG_H
#define IOHWAB_CFG_H

#include "Std_Types.h"

#define IOHWAB_LED_STATUS      0u
#define IOHWAB_NUM_LEDS        1u

typedef struct {
    uint8 DioChannelId;
} IoHwAb_LedConfigType;

extern const IoHwAb_LedConfigType IoHwAb_LedConfig[];

#endif /* IOHWAB_CFG_H */
