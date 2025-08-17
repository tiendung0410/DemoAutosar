#ifndef GPT_CFG_H
#define GPT_CFG_H

#include "Gpt_Types.h"

/* Khai báo Channel IDs thống nhất toàn hệ */
typedef enum {
    GPT_CHANNEL_BUTTON = 0,
    GPT_CHANNEL_LED    = 1,
    GPT_CHANNEL_NUM
} Gpt_ChannelId;

/* Extern cấu hình */
extern const Gpt_ConfigType Gpt_Config;

#endif /* GPT_CFG_H */
