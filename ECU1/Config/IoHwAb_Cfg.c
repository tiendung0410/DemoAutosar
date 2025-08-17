#include "IoHwAb_Cfg.h"

const IoHwAb_ButtonConfigType IoHwAb_ButtonConfig[] = {
    [IOHWAB_BUTTON_SYSTEM] = { .DioChannelId = 0 },
    [IOHWAB_BUTTON_FREQ]   = { .DioChannelId = 1 },
};
