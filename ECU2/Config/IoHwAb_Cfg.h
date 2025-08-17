#ifndef IOHWAB_CFG_H
#define IOHWAB_CFG_H

#include "Std_Types.h"

typedef struct {
    uint8 DioChannelId;
} IoHwAb_LedConfigType;

extern const IoHwAb_LedConfigType IoHwAb_LedConfig[];

#endif /* IOHWAB_CFG_H */
