#include "Can.h"
#include "Can_Cfg.h"
#include "Spi.h"
#include "Spi_Cfg.h"
#include <string.h>
#include <stdio.h>

/* ====== State Variables ====== */
static const Can_ConfigType* Can_GlobalConfig = NULL;
static Can_ControllerStateType Can_ControllerState[CAN_CONTROLLER_COUNT] = { CAN_CS_UNINIT };
static uint8 Can_RxBuf[14];

/* ====== API Implementations ====== */
void Can_Init(const Can_ConfigType* Config) {
    if (Config == NULL) return;
    Can_GlobalConfig = Config;
    Spi_Init(NULL);
    for (uint8 i=0; i<Config->controllerCount; i++) {
        Can_ControllerState[i] = CAN_CS_STOPPED;
    }
}

void Can_DeInit(void) {
    Spi_DeInit();
    for (uint8 i=0; i<CAN_CONTROLLER_COUNT; i++) {
        Can_ControllerState[i] = CAN_CS_UNINIT;
    }
    Can_GlobalConfig = NULL;
}

Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition) {
    if (Controller >= CAN_CONTROLLER_COUNT) return E_NOT_OK;
    Can_ControllerState[Controller] = Transition;
    return E_OK;
}

Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo) {
    (void)Hth;
    if (PduInfo == NULL) return E_NOT_OK;

    uint8 txBuf[14] = {0};
    txBuf[0] = (uint8)(PduInfo->id >> 3);
    txBuf[1] = (uint8)((PduInfo->id & 0x07) << 5);
    txBuf[2] = PduInfo->length;
    memcpy(&txBuf[3], PduInfo->sdu, PduInfo->length);

    Spi_WriteIB(SPI_CH_MCP2515_DATA, txBuf);
    Spi_AsyncTransmit(SPI_SEQ_WRITE_MCP2515);
    return E_OK;
}

void Can_MainFunction_Read(void) {
    Spi_AsyncTransmit(SPI_SEQ_READ_MCP2515);
    Spi_ReadIB(SPI_CH_MCP2515_DATA, Can_RxBuf);
    Can_RxIndication(Can_RxBuf, Can_RxBuf[2]);
}

void Can_MainFunction_Write(void) {
    /* Optionally check TX status via MCP2515 if needed */
}

void Can_GetVersionInfo(Can_VersionInfoType* versioninfo) {
    if (versioninfo == NULL) return;
    versioninfo->vendorID = 0xFFFF;
    versioninfo->moduleID = 0x0020; /* Example Module ID */
    versioninfo->sw_major_version = 1;
    versioninfo->sw_minor_version = 0;
    versioninfo->sw_patch_version = 0;
}

/* Optional APIs */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType* State) {
    if (Controller >= CAN_CONTROLLER_COUNT || State == NULL) return E_NOT_OK;
    *State = Can_ControllerState[Controller];
    return E_OK;
}

Std_ReturnType Can_GetControllerErrorState(uint8 Controller, uint8* ErrorState) {
    if (Controller >= CAN_CONTROLLER_COUNT || ErrorState == NULL) return E_NOT_OK;
    *ErrorState = 0; /* Stub: No error */
    return E_OK;
}

Std_ReturnType Can_GetControllerTxErrorCounter(uint8 Controller, uint8* TxErrorCounter) {
    if (Controller >= CAN_CONTROLLER_COUNT || TxErrorCounter == NULL) return E_NOT_OK;
    *TxErrorCounter = 0; /* Stub */
    return E_OK;
}

Std_ReturnType Can_GetControllerRxErrorCounter(uint8 Controller, uint8* RxErrorCounter) {
    if (Controller >= CAN_CONTROLLER_COUNT || RxErrorCounter == NULL) return E_NOT_OK;
    *RxErrorCounter = 0; /* Stub */
    return E_OK;
}

/* ====== Callback ====== */
void Can_RxIndication(uint8* data, uint8 len) {
    printf("CAN RX: DLC=%u, Data=", len);
    for (uint8 i=0; i<len; i++) {
        printf("%02X ", data[3+i]);
    }
    printf("\n");
}
