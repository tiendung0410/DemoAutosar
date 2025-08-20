#include "App_LedTask.h"
#include "Rte_Led.h"
#include <stdio.h>

static uint8 s_ledState = 0;
static uint8 s_lastCount = 0;
static uint8 s_lastSysState = 0;
static uint32_t s_periodMs = 1000; // ms

// GPT callback: Toggle LED, chạy mãi nếu buttonCount>0
static void AppLed_GptCb(void)
{

    s_ledState ^= 1;
    Rte_Call_PP_LedControl_LedSet(0, s_ledState);
    // Timer đã ở chế độ periodic, không cần restart lại.
}

void App_Led_Init(void)
{
    Rte_Call_PP_LedControl_LedSet(0, 0);
    s_ledState = 0;
    s_lastCount = 0;
    s_lastSysState = 0;
    s_periodMs = 1000;
    // Đăng ký callback GPT
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
                // Khi hệ thống ON, reset LED
                s_ledState = 0;
                Rte_Call_PP_LedControl_LedSet(0, 0);
                Rte_Call_PP_GptControl_GptStartTicks(0, 1000); // Bắt đầu với 1s 
                printf("[App_LED] Bật hệ thống, tắt LED\n");
            } else {
                // Khi hệ thống OFF, tắt LED và dừng timer
                s_ledState = 0;
                s_lastCount = 0; // Reset đếm
                Rte_Call_PP_LedControl_LedSet(0, 0);
                Rte_Call_PP_GptControl_GptStop(0);
                printf("[App_LED] Tắt hệ thống, dừng LED\n");
            }
        }

        if(s_lastSysState)
        {
            if (curCount != s_lastCount) {
            s_lastCount = curCount;
            s_periodMs = 1024U >> curCount;
            if (s_periodMs == 0) s_periodMs = 1; // tránh chia nhỏ quá (bảo vệ)
            s_ledState = 0;
            Rte_Call_PP_LedControl_LedSet(0, 0);
            Rte_Call_PP_GptControl_GptStop(0); // Đảm bảo không bị trùng timer cũ
            Rte_Call_PP_GptControl_GptStartTicks(0, s_periodMs);
            printf("[App_LED] Nháy LED tần số %u Hz (chu kỳ %u ms)\n",
                1000U / s_periodMs, s_periodMs);
            }
        }
    }
}


