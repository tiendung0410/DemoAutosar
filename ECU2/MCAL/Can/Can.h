#ifndef CAN_H_
#define CAN_H_

#include "Can_Types.h"
#include "Can_Cfg.h"

// Macro version, error code, function prototype
#define CAN_SW_MAJOR_VERSION  1
#define CAN_SW_MINOR_VERSION  0
#define CAN_SW_PATCH_VERSION  0

void Can_Init(const Can_ConfigType* Config);
void Can_DeInit(void);
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);
void Can_MainFunction_Write(void);
void Can_MainFunction_Read(void);
void Can_MainFunction_BusOff(void);
void Can_MainFunction_Wakeup(void);
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo);

#endif /* CAN_H_ */
