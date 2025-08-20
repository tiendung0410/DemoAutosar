#ifndef GPT_TYPES_H
#define GPT_TYPES_H

#include <stdint.h>
#include "Std_Types.h"

typedef uint8  Gpt_ChannelType;
/* Tick counter type */
typedef uint64 Gpt_ValueType;

/* AUTOSAR-like mode of the driver  */
typedef enum {
    GPT_MODE_NORMAL = 0,
    GPT_MODE_SLEEP
} Gpt_ModeType;

/* Channel mode: one-shot or periodic */
typedef enum {
    GPT_CH_MODE_CONTINUOUS = 0,
    GPT_CH_MODE_ONESHOT
} Gpt_ChannelMode;

/* Notification callback prototype */
typedef void (*Gpt_NotificationType)(void);

/* Per-channel configuration */
typedef struct {
    Gpt_ChannelType       channelId;
    Gpt_ChannelMode       channelMode;
    uint32                tickFrequencyHz;   
    Gpt_ValueType         tickMaxValue;        
    Gpt_NotificationType  notification;        /* callback when timeout */
} Gpt_ChannelConfigType;

/* Top-level config */
typedef struct {
    const Gpt_ChannelConfigType* channels;
    uint8                        numOfChannels;
} Gpt_ConfigType;

#endif /* GPT_TYPES_H */
