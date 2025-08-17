#ifndef IOHWAB_CFG_H
#define IOHWAB_CFG_H

#include "Std_Types.h"

typedef struct {
    uint8 DioChannelId;
} IoHwAb_ButtonConfigType;

extern const IoHwAb_ButtonConfigType IoHwAb_ButtonConfig[];

#endif /* IOHWAB_CFG_H */
