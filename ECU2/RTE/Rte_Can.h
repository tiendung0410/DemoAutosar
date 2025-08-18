#ifndef RTE_CAN_H_
#define RTE_CAN_H_

#include "Rte_Types.h"
#include "Com.h"

// --- Đọc ButtonStatusInfoType từ COM (CAN) ---
Std_ReturnType Rte_Read_SigButtonStatusInfo(ButtonStatusInfoType* value);

// --- Gửi ButtonStatusInfoType sang AppLed ---
Std_ReturnType Rte_Write_PP_ButtonStatusInfo_ButtonStatus(const ButtonStatusInfoType* value);


#endif /* RTE_CAN_H_ */
