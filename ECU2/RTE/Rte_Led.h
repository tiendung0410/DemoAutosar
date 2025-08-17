#ifndef RTE_LED_H
#define RTE_LED_H

#include "Std_Types.h"

/* Đọc trạng thái hệ thống (ON/OFF) do ECU1 gửi sang */
Std_ReturnType Rte_Read_RP_ButtonStateInfo_ButtonState(uint8* state);
/* Đọc số lần nhấn nút FREQ (FreqCount) do ECU1 gửi sang */
Std_ReturnType Rte_Read_RP_ButtonCountInfo_ButtonCount(uint8* count);

#endif /* RTE_LED_H */
