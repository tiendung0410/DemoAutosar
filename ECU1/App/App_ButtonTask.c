#include "App_ButtonTask.h"
#include "Rte_Types.h"         // Include file RTE chứa các kiểu dữ liệu
#include "Rte_Button.h"        // Include file RTE chứa API struct
#include <stdio.h>

static uint8 prevSystem = 0;
static uint8 systemState = 0;  // 0 = OFF, 1 = ON
static uint8 prevFreq = 0;
static uint8 freqCount = 0;

void App_ButtonTask_Init(void) {
    systemState = 0;
    prevSystem = 0;
    prevFreq = 0;
    freqCount = 0;
    printf("ButtonTask init OK\n");
}

void App_ButtonTask_Run(void) {
    uint8 systemNow = 0, freqNow = 0;

    // Đọc trạng thái các nút (từ IO abstraction hoặc driver)
    Rte_Read_RP_AbButtonStateInfo_AbButtonState(&systemNow);
    Rte_Read_RP_AbButtonCountInfo_AbButtonFreq(&freqNow);

    // Toggle ON/OFF
    if (systemNow && !prevSystem) {
        systemState ^= 1;
        if(!systemState)
        {
            freqCount=0;
        }
        printf("System State toggled: %s\n", systemState ? "ON" : "OFF");
    }
    prevSystem = systemNow;

    // Đếm số lần nhấn freq button
    if (freqNow && !prevFreq) {
        freqCount++;
        printf("Freq Button pressed, count=%d\n", freqCount);
    }
    prevFreq = freqNow;

    // Đóng gói struct và GỬI QUA RTE (1 port duy nhất)
    ButtonStatusInfoType buttonInfo;
    buttonInfo.ButtonState = systemState;
    buttonInfo.ButtonCount = freqCount;

    Rte_Write_PP_ButtonStatusInfo_ButtonStatus(&buttonInfo);

    printf("App: SystemButton=%d, FreqButton_Count=%d\n", systemState, freqCount);
}
