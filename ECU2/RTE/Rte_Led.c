#include "Rte_Led.h"
#include "Com.h" // layer trung gian nhận dữ liệu từ CAN

Std_ReturnType Rte_Read_RP_ButtonStateInfo_ButtonState(uint8* state)
{
    return Com_ReceiveSignal(SIGNAL_ID_BUTTON_STATE, state);
}

Std_ReturnType Rte_Read_RP_ButtonCountInfo_ButtonCount(uint8* count)
{
    return Com_ReceiveSignal(SIGNAL_ID_BUTTON_COUNT, count);
}
