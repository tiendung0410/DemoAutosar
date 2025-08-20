#include "App_ButtonTask.h"
#include "Rte_Types.h"      
#include "Rte_Button.h"        
#include <stdio.h>

static uint8 prevSystem = 0;
static uint8 systemState = 0; 
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
    /* Read Button Info from RTE */
    Rte_Read_RP_AbButtonStateInfo_AbButtonState(&systemNow);
    Rte_Read_RP_AbButtonCountInfo_AbButtonFreq(&freqNow);

    /* State Button pressed */ 
    if (systemNow && !prevSystem) {
        systemState ^= 1;
        if(!systemState)
        {
            freqCount=0;
        }
    }
    prevSystem = systemNow;

    /* Freq Button pressed */
    if (freqNow && !prevFreq) {
        freqCount++;
        printf("Freq Button pressed, count=%d\n", freqCount);
    }
    prevFreq = freqNow;

    /* Send Button Info to AppCan SWC */
    ButtonStatusInfoType buttonInfo;
    buttonInfo.ButtonState = systemState;
    buttonInfo.ButtonCount = freqCount;
    Rte_Write_PP_ButtonStatusInfo_ButtonStatus(&buttonInfo);

    printf("App: SystemButton=%d, FreqButton_Count=%d\n", systemState, freqCount);
}
