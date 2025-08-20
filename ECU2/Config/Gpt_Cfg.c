#include "Gpt.h"
#include "Gpt_Cbk.h"

static const Gpt_ChannelConfigType Gpt_ChannelCfgs[] = {
    /* BUTTON tick: period 1ms @1kHz ticks, periodic */
    {
        .channelId       = GPT_CHANNEL_BUTTON,
        .channelMode     = GPT_CH_MODE_CONTINUOUS,
        .tickFrequencyHz = 1000u,               /* 1 tick = 1ms */
        .tickMaxValue    = 0xFFFFFFFFu,
        .notification    = Gpt0_Notification
    },
    /* Reserve tick: one-shot theo yêu cầu */
    {
        .channelId       = GPT_CHANNEL_LED,
        .channelMode     = GPT_CH_MODE_ONESHOT,
        .tickFrequencyHz = 1000u,               /* 1 tick = 1ms */
        .tickMaxValue    = 0xFFFFFFFFu,
        .notification    = Gpt1_Notification
    }
};

const Gpt_ConfigType Gpt_Config = {
    .channels      = Gpt_ChannelCfgs,
    .numOfChannels = (uint8)(sizeof(Gpt_ChannelCfgs)/sizeof(Gpt_ChannelCfgs[0]))
};
