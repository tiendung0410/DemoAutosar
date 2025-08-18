#ifndef IOHWAB_CFG_H
#define IOHWAB_CFG_H

#include "Std_Types.h"

#define IOHWAB_BUTTON_SYSTEM   0u
#define IOHWAB_BUTTON_FREQ     1u
#define IOHWAB_NUM_BUTTONS     2u

typedef struct {
    uint8 DioChannelId;
} IoHwAb_ButtonConfigType;

extern const IoHwAb_ButtonConfigType IoHwAb_ButtonConfig[];

#endif /* IOHWAB_CFG_H */
