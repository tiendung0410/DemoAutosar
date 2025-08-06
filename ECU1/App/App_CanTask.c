#include "App_CanTask.h"
#include "Rte_Can.h"   // Gọi RTE interface (dưới là Rte_Read)
#include <stdio.h>

void App_CanTask_Init(void) {
    // Initialize CAN task resources if needed
    // Reserve 
}

void App_CanTask_Run(void) {
    ButtonStatusInfoType buttonInfo;
    //Reading button status from App_ButtonTask through RTE
    Rte_Read_RP_ButtonStatusInfo_ButtonStatus(&buttonInfo);

    // Send button status to COM layer
    Rte_Write_SigButtonStatusInfo(buttonInfo);
}
