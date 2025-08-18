#ifndef RTE_BUTTON_H
#define RTE_BUTTON_H

#include "Rte_Types.h"

Std_ReturnType Rte_Read_RP_AbButtonStateInfo_AbButtonState(uint8* state);
Std_ReturnType Rte_Read_RP_AbButtonCountInfo_AbButtonFreq(uint8* freq);

Std_ReturnType Rte_Write_PP_ButtonStatusInfo_ButtonStatus(ButtonStatusInfoType* buttonInfo);

#endif /* RTE_BUTTON_H */
