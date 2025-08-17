#ifndef CAN_H_
#define CAN_H_

#include "Std_Types.h"
#include "Can_Cfg.h"


#define CAN_SW_MAJOR_VERSION  1
#define CAN_SW_MINOR_VERSION  0
#define CAN_SW_PATCH_VERSION  0

typedef enum {
    CAN_CS_UNINIT,
    CAN_CS_STARTED,
    CAN_CS_STOPPED,
    CAN_CS_SLEEP
} Can_ControllerStateType;

typedef uint8 Can_ReturnType;
#define CAN_OK      0U
#define CAN_NOT_OK  1U
#define CAN_BUSY    2U

typedef uint32 Can_IdType;

typedef struct {
    Can_IdType id;
    uint8 length;
    uint8* sdu;
    uint32 swPduHandle;
} Can_PduType;

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
