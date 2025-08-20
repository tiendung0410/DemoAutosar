#include "Rte_Can.h"
#include "Rte_Button.h" 
#include "Com.h"


Std_ReturnType Rte_Write_SigButtonStatusInfo(ButtonStatusInfoType buttonInfo) {
    return Com_SendSignal(0, &buttonInfo); 
}


Std_ReturnType Rte_Read_RP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* buttonInfo)
{
    if (buttonInfo) {
        buttonInfo->ButtonState = g_ButtonStatusInfo.ButtonState;
        buttonInfo->ButtonCount = g_ButtonStatusInfo.ButtonCount;
        return E_OK;
    } else {
        return E_NOT_OK;
    }
}