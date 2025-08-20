#define _POSIX_C_SOURCE 200809L
#include "Rte_Gpt.h"
#include "EcuAb_Gpt.h"   /* ECUA GPT bạn đã có: Start/Stop/GetElapsed/.../Delay */

void Rte_Gpt_Init(void)
{
    /* Nếu bạn đã init ECUA ở main rồi thì có thể để trống. */
    EcuAb_Gpt_Init();
}

void Rte_Gpt_DeInit(void)
{
    EcuAb_Gpt_DeInit();
}

/* ==== Start/Stop ==== */
void Rte_Call_RP_Gpt_StartTicks(uint8_t logicCh, uint32_t ticks)
{
    EcuAb_Gpt_StartTicks(logicCh, ticks);
}

void Rte_Call_RP_Gpt_Stop(uint8_t logicCh)
{
    EcuAb_Gpt_Stop(logicCh);
}

/* ==== Query wrapper (GetElapsed / GetRemaining) ==== */
uint32_t Rte_Call_RP_Gpt_GetElapsed(uint8_t logicCh)
{
    return (uint32_t)EcuAb_Gpt_GetElapsed(logicCh);
}
uint32_t Rte_Call_RP_Gpt_GetRemaining(uint8_t logicCh)
{
    return (uint32_t)EcuAb_Gpt_GetRemaining(logicCh);
}

