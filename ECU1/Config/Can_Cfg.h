#ifndef CAN_CFG_H_
#define CAN_CFG_H_

#include "Std_Types.h"
#include "Can_Types.h" // hoặc chỉ include khi dùng type ở đây

#define CAN_DEV_ERROR_DETECT    STD_ON
#define CAN_MAX_CONTROLLER      1U

typedef uint8 Can_HwHandleType;

typedef struct {
    char ifName[16];     
    uint8 ControllerId;
    uint32 Baudrate;
} Can_ControllerConfigType;

typedef struct {
    const Can_ControllerConfigType* ControllerConfigs;
    uint8 NumControllers;
} Can_ConfigType;

extern const Can_ConfigType Can_Config;

#endif /* CAN_CFG_H_ */
