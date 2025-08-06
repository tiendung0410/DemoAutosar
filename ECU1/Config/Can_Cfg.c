#include "Can_Cfg.h"
const Can_ControllerConfigType CanControllerConfig_BBB = {
    .ifName = "can1",
    .ControllerId = 0,
    .Baudrate = 500000
};

const Can_ConfigType Can_Config = {
    .ControllerConfigs = &CanControllerConfig_BBB,
    .NumControllers = 1
};
