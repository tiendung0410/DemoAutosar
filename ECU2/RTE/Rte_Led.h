#ifndef RTE_LED_H_
#define RTE_LED_H_

#include "Rte_Types.h"
#include "IoHwAb.h"
#include "EcuAb_Gpt.h"

// Biến buffer nhận từ AppCan
extern ButtonStatusInfoType g_Rte_ButtonStatusInfo_Buffer;

// Đọc struct từ buffer (do AppCan gửi)
Std_ReturnType Rte_Read_RP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* value);

// Điều khiển LED qua IoHwAb (call port)
Std_ReturnType Rte_Call_PP_LedControl_LedSet(IoHwAb_LedIdType id, uint8 level);

// API sử dụng GPT cho AppLed
void Rte_Call_PP_GptControl_GptStartTicks(EcuAb_Gpt_ChannelType ch, uint32_t ticks);
void Rte_Call_PP_GptControl_GptStop(EcuAb_Gpt_ChannelType ch);
int  Rte_Call_PP_GptControl_GptRegisterCb(EcuAb_Gpt_ChannelType ch, EcuAb_Gpt_AppCbkType cb);

#endif
