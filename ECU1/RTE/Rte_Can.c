#include "Rte_Can.h"
#include "Rte_Button.h"  // Dùng lại buffer trung gian đã publish ở Rte_Button
#include "Com.h"

// Mapping "ButtonState" ↔ signalId=0
Std_ReturnType Rte_Write_SigButtonStatusInfo(ButtonStatusInfoType buttonInfo) {
    return Com_SendSignal(0, &buttonInfo); // 0 = signalId của ButtonState
}


Std_ReturnType Rte_Read_RP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* buttonInfo)
{
    if (buttonInfo) {
        buttonInfo->ButtonState = g_ButtonStatusInfo.ButtonState;
        buttonInfo->ButtonCount = g_ButtonStatusInfo.ButtonCount;
        return E_OK;
    } else {
        return E_NOT_OK; // Trả về lỗi nếu con trỏ NULL
    }
}