#ifndef GPT_H
#define GPT_H

#include "Gpt_Types.h"
#include "Gpt_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

void           Gpt_Init(const Gpt_ConfigType* ConfigPtr);
void           Gpt_DeInit(void);

void           Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value);
void           Gpt_StopTimer (Gpt_ChannelType Channel);

Gpt_ValueType  Gpt_GetTimeElapsed  (Gpt_ChannelType Channel);
Gpt_ValueType  Gpt_GetTimeRemaining(Gpt_ChannelType Channel);

void           Gpt_EnableNotification (Gpt_ChannelType Channel);
void           Gpt_DisableNotification(Gpt_ChannelType Channel);

void           Gpt_SetMode(Gpt_ModeType Mode);
Gpt_ModeType   Gpt_GetMode(void);

#ifdef __cplusplus
}
#endif
#endif /* GPT_H */
