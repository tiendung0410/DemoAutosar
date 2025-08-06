#ifndef RTE_CAN_H
#define RTE_CAN_H

#include "Rte_Types.h"

Rte_Read_RP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* buttonInfo);

Std_ReturnType Rte_Write_SigButtonStatusInfo(ButtonStatusInfoType buttonInfo);

extern ButtonStatusInfoType g_ButtonStatusInfo;
#endif /* RTE_CAN_H */
