#include "App_LedTask.h"
#include "Rte_Led.h"
#include <stdio.h>

static uint8 s_ledBlinkCount = 0;
static uint8 s_ledBlinkRemain = 0;
static uint8 s_ledState = 0;

// GPT callback: toggle LED mỗi tick, khi hết số lần thì tắt timer
static void AppLed_GptCb(void)
{
    if (s_ledBlinkRemain > 0) {
        s_ledState ^= 1;
        Rte_Call_PP_LedControl_LedSet(0, s_ledState);
        --s_ledBlinkRemain;
    }
    if (s_ledBlinkRemain == 0) {
        // Tắt LED và dừng timer
        Rte_Call_PP_LedControl_LedSet(0, 0);
        Rte_Call_PP_GptControl_GptStop(0); // dừng timer kênh 0
    }
}

void App_Led_Init(void)
{
    Rte_Call_PP_LedControl_LedSet(0, 0);
    s_ledBlinkCount = 0;
    s_ledBlinkRemain = 0;
    s_ledState = 0;
    // Đăng ký callback GPT
    Rte_Call_PP_GptControl_GptRegisterCb(0, AppLed_GptCb);
    printf("App_LED init OK\n");
}

void App_Led_Run(void)
{
    ButtonStatusInfoType buttonInfo;
    if (Rte_Read_RP_ButtonStatusInfo_ButtonStatus(&buttonInfo) == E_OK) {
        // Khi ButtonCount thay đổi, nháy LED0 buttonInfo.ButtonCount lần
        static uint8 lastCount = 0;
        if (buttonInfo.ButtonCount != lastCount) {
            lastCount = buttonInfo.ButtonCount;
            s_ledBlinkCount = buttonInfo.ButtonCount;
            s_ledBlinkRemain = s_ledBlinkCount * 2; // 2 lần mỗi nháy (on/off)
            s_ledState = 0;
            Rte_Call_PP_LedControl_LedSet(0, 0);
            // Start timer mỗi 200ms, kênh 0 (blink chậm)
            Rte_Call_PP_GptControl_GptStartMs(0, 200);
        }
    }
}
