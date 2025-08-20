#include "App_LedTask.h"
#include "Rte_Led.h"
#include <stdio.h>

static uint8 s_ledState = 0;
static uint8 s_lastCount = 0;
static uint8 s_lastSysState = 0;
static uint32_t s_periodMs = 1000; // ms

// GPT callback: Toggle LED
static void AppLed_GptCb(void)
{
    s_ledState ^= 1;
    Rte_Call_PP_LedControl_LedSet(0, s_ledState);
}

void App_Led_Init(void)
{
    Rte_Call_PP_LedControl_LedSet(0, 0);
    s_ledState = 0;
    s_lastCount = 0;
    s_lastSysState = 0;
    s_periodMs = 1000;
    // Callback register GPT
    Rte_Call_PP_GptControl_GptRegisterCb(0, AppLed_GptCb);
    printf("App_LED init OK\n");
}

void App_Led_Run(void)
{
    ButtonStatusInfoType buttonInfo;
    if (Rte_Read_RP_ButtonStatusInfo_ButtonStatus(&buttonInfo) == E_OK) 
    {
        printf("Button State: %d, Count: %d\n", buttonInfo.ButtonState, buttonInfo.ButtonCount);
        uint8 curSysState = buttonInfo.ButtonState;
        uint8 curCount = buttonInfo.ButtonCount;

        if(curSysState != s_lastSysState) {
            s_lastSysState = curSysState;
            if (curSysState) {
                s_ledState = 0;
                Rte_Call_PP_LedControl_LedSet(0, 0);
                Rte_Call_PP_GptControl_GptStartTicks(0, 1000); // Start with 1s period
                printf("[App_LED] System ON\n");
            } else {
                s_ledState = 0;
                s_lastCount = 0; 
                Rte_Call_PP_LedControl_LedSet(0, 0);
                Rte_Call_PP_GptControl_GptStop(0);
                printf("[App_LED] System OFF\n");
            }
        }

        if(s_lastSysState)
        {
            if (curCount != s_lastCount) {
            s_lastCount = curCount;
            s_periodMs = 1024U >> curCount;
            if (s_periodMs == 0) s_periodMs = 1; // avoid division by zero
            s_ledState = 0;
            Rte_Call_PP_LedControl_LedSet(0, 0);
            Rte_Call_PP_GptControl_GptStop(0); 
            Rte_Call_PP_GptControl_GptStartTicks(0, s_periodMs);
            printf("[App_LED] Toggle LED with Freg %u Hz (period %u ms)\n",
                1000U / s_periodMs, s_periodMs);
            }
        }
    }
}


