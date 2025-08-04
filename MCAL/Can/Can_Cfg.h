#ifndef CAN_CFG_H
#define CAN_CFG_H

#include "Can.h"

/* ====== Config Defines ====== */
#define CAN_CONTROLLER_COUNT     1
#define CAN_CTRL_0               0

#define CAN_HTH_0                0
#define CAN_HRH_0                0

#define CAN_MAX_DLC              8

/* ====== External Config Reference ====== */
extern const Can_ConfigType CanDriver_Config;

#endif /* CAN_CFG_H */
