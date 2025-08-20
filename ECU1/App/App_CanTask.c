#include "App_CanTask.h"
#include "Rte_Can.h"   
#include <stdio.h>

void App_CanTask_Init(void) {
    // Reserve 
}

void App_CanTask_Run(void) {
    ButtonStatusInfoType buttonInfo;
    
    /* Reading button info from App_ButtonTask through RTE */
    Rte_Read_RP_ButtonStatusInfo_ButtonStatus(&buttonInfo);
    printf("App_CanTask: ButtonState=%d, ButtonCount=%d\n", 
           buttonInfo.ButtonState, buttonInfo.ButtonCount);
    /* Send button info to COM */
    Rte_Write_SigButtonStatusInfo(buttonInfo);

}
