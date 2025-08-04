#include "Can_Cfg.h"

/* Controller Config Table */
static const Can_ControllerConfigType Can_ControllerConfigs[CAN_CONTROLLER_COUNT] = {
    { CAN_CTRL_0 }
};

/* Global CAN Config */
const Can_ConfigType CanDriver_Config = {
    .controllers = Can_ControllerConfigs,
    .controllerCount = CAN_CONTROLLER_COUNT
};
