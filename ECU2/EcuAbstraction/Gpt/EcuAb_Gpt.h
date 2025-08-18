#ifndef ECUAB_GPT_H
#define ECUAB_GPT_H

#include "EcuAb_Gpt_Types.h"

/* ==== Lifecycle ==== */
void EcuAb_Gpt_Init(void);        /* Gọi cả Gpt_Init(...) bên dưới */
void EcuAb_Gpt_DeInit(void);

/* ==== Control ==== */
void EcuAb_Gpt_StartTicks(EcuAb_Gpt_ChannelType ch, EcuAb_Gpt_TicksType ticks);
void EcuAb_Gpt_StartMs(EcuAb_Gpt_ChannelType ch, uint32_t ms);
void EcuAb_Gpt_StartUs(EcuAb_Gpt_ChannelType ch, uint32_t us);
void EcuAb_Gpt_Stop(EcuAb_Gpt_ChannelType ch);

/* ==== Query ==== */
EcuAb_Gpt_TicksType EcuAb_Gpt_GetElapsed(EcuAb_Gpt_ChannelType ch);
EcuAb_Gpt_TicksType EcuAb_Gpt_GetRemaining(EcuAb_Gpt_ChannelType ch);

/* ==== Notification ==== */
/* App đăng ký callback theo kênh logic; ECUA sẽ bật notification ở MCAL luôn */
int  EcuAb_Gpt_RegisterCallback(EcuAb_Gpt_ChannelType ch, EcuAb_Gpt_AppCbkType cb);
void EcuAb_Gpt_EnableNotification(EcuAb_Gpt_ChannelType ch);
void EcuAb_Gpt_DisableNotification(EcuAb_Gpt_ChannelType ch);

/* ==== Mode (tuỳ nhu cầu) ==== */
void EcuAb_Gpt_SetMode(EcuAb_Gpt_ModeType mode);  /* ảnh hưởng semantic ECUA; MCAL vẫn dùng config kênh */

#endif /* ECUAB_GPT_H */
