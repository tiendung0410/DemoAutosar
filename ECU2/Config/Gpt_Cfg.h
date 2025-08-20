#ifndef GPT_CFG_H
#define GPT_CFG_H

#include "Gpt_Types.h"


typedef enum {
    GPT_CHANNEL_BUTTON = 0,
    GPT_CHANNEL_LED    = 1,
    GPT_CHANNEL_NUM
} Gpt_ChannelId;

/* Extern configuration */
extern const Gpt_ConfigType Gpt_Config;

#endif /* GPT_CFG_H */
