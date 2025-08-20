#include "App_CanTask.h"
#include "Rte_Can.h"
#include <stdio.h>

void App_Can_Init(void) {
    // Reserve buffer or any initialization if needed
}

void App_Can_Run(void) {
    ButtonStatusInfoType buttonInfo;
    // Read button info from Com through RTE
    if (Rte_Read_SigButtonStatusInfo(&buttonInfo) == E_OK) {
        printf("[App_CAN] Received: ButtonState=%d, ButtonCount=%d\n",
               buttonInfo.ButtonState, buttonInfo.ButtonCount);

        // Send button info to App Led SWC
        Rte_Write_PP_ButtonStatusInfo_ButtonStatus(&buttonInfo);
    }
}
