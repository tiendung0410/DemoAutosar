#include "Rte_Led.h"


Std_ReturnType Rte_Read_RP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* value)
{
    if (value) 
    {   
        *value = g_Rte_ButtonStatusInfo_Buffer;
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Call_PP_LedControl_LedSet(IoHwAb_LedIdType id, uint8 level)
{
    return IoHwAb_Led_Set(id, level);
}

// ==== GPT interface for AppLed ====
void Rte_Call_PP_GptControl_GptStartTicks(EcuAb_Gpt_ChannelType ch, uint32_t ticks)
{
    EcuAb_Gpt_StartTicks(ch, ticks);
}
void Rte_Call_PP_GptControl_GptStop(EcuAb_Gpt_ChannelType ch)
{
    EcuAb_Gpt_Stop(ch);
}
int Rte_Call_PP_GptControl_GptRegisterCb(EcuAb_Gpt_ChannelType ch, EcuAb_Gpt_AppCbkType cb)
{
    return EcuAb_Gpt_RegisterCallback(ch, cb);
}
