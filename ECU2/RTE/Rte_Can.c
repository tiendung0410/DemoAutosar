#include "Rte_Can.h"

// Đọc struct từ CAN (qua COM)
Std_ReturnType Rte_Read_SigButtonStatusInfo(ButtonStatusInfoType* value)
{
    // Đọc từ COM signalId=0 (mapping struct)
    return Com_ReceiveSignal(0, value); 
}

// Gửi struct sang AppLed (qua local variable/buffer hoặc fake RTE, tuỳ hệ thống)
ButtonStatusInfoType g_Rte_ButtonStatusInfo_Buffer;

Std_ReturnType Rte_Write_PP_ButtonStatusInfo_ButtonStatus(const ButtonStatusInfoType* value)
{
    // Lưu vào buffer dùng chung cho AppLed đọc lại (giả lập RTE giữa SWC nội bộ)
    if (value == NULL) return E_NOT_OK;
    g_Rte_ButtonStatusInfo_Buffer = *value;
    return E_OK;
}

