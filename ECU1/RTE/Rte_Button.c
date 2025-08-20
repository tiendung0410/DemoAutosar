#include "Rte_Button.h"
#include "IoHwAb.h"

ButtonStatusInfoType g_ButtonStatusInfo;


//---------------API Send Button State and Count to CAN Task----------------
Std_ReturnType Rte_Write_PP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* buttonInfo) {
    if(buttonInfo) {
        g_ButtonStatusInfo.ButtonState = buttonInfo->ButtonState;
        g_ButtonStatusInfo.ButtonCount = buttonInfo->ButtonCount;
        return E_OK;
    }
    else {
        return E_NOT_OK; // Trả về lỗi nếu con trỏ NULL
    }
}

//---------------API Read Button State and Count From IoHwAb----------------
Std_ReturnType Rte_Read_RP_AbButtonStateInfo_AbButtonState(uint8* state) {
    if(state) {
        IoHwAb_Button_Read(IOHWAB_BUTTON_SYSTEM, state);
        printf("state=%d\n", *state);
        return E_OK;
    }
    else {
        return E_NOT_OK; // Trả về lỗi nếu con trỏ NULL
    }
}
Std_ReturnType Rte_Read_RP_AbButtonCountInfo_AbButtonFreq(uint8* freq) {
    if(freq) {
        IoHwAb_Button_Read(IOHWAB_BUTTON_FREQ, freq);
        return E_OK;
    }
    else {
        return E_NOT_OK; // Trả về lỗi nếu con trỏ NULL
    }
}
