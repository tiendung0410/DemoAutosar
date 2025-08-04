#ifndef CAN_H
#define CAN_H

#include "Std_Types.h"
#include "ComStack_Types.h"

/* ====== Type Definitions (AUTOSAR) ====== */

typedef uint8 Can_ControllerStateType; /* CAN_CS_UNINIT, ... */
typedef uint8 Can_HwHandleType;        /* HRH / HTH */

typedef struct {
    PduIdType swPduHandle;
    uint8 length;
    uint8* sdu;
    uint32 id;
} Can_PduType;

typedef struct {
    uint16 vendorID;
    uint16 moduleID;
    uint8  sw_major_version;
    uint8  sw_minor_version;
    uint8  sw_patch_version;
} Can_VersionInfoType;

typedef struct {
    /* Add controller-specific config if needed */
    uint8 controllerId;
} Can_ControllerConfigType;

typedef struct {
    const Can_ControllerConfigType* controllers;
    uint8 controllerCount;
} Can_ConfigType;

/* ====== Controller State Defines ====== */
#define CAN_CS_UNINIT   0
#define CAN_CS_STOPPED  1
#define CAN_CS_STARTED  2
#define CAN_CS_SLEEP    3

/* ====== API Prototypes (AUTOSAR) ====== */
void Can_Init(const Can_ConfigType* Config);
void Can_DeInit(void);
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);
void Can_MainFunction_Read(void);
void Can_MainFunction_Write(void);
void Can_GetVersionInfo(Can_VersionInfoType* versioninfo);

/* Optional APIs (stub for now) */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType* State);
Std_ReturnType Can_GetControllerErrorState(uint8 Controller, uint8* ErrorState);
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 Controller, uint8* TxErrorCounter);
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 Controller, uint8* RxErrorCounter);

/* Callback */
void Can_RxIndication(uint8* data, uint8 len);

#endif /* CAN_H */
