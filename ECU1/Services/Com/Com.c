#include "Com.h"
#include "PduR.h"
#include <string.h>
#include <stdio.h>

// Internal buffer to hold PDU data
static uint8 Com_PduBuffer[COM_MAX_PDU][8];

// When receiving PDU via PduR, unpack to signal
void Com_RxIndication(Com_PduIdType pduId, const uint8* SduDataPtr, uint8 length)
{
    if (pduId >= COM_MAX_PDU) return;
    memcpy(Com_PduBuffer[pduId], SduDataPtr, length);

    // Log received signal for debugging
    for (int i = 0; i < COM_MAX_SIGNALS; ++i) {
        if (Com_SignalConfig[i].pduId == pduId) {
            uint8 value = Com_PduBuffer[pduId][Com_SignalConfig[i].startBit / 8];
            printf("[Com] RX SignalId=%d, Value=%d\n", Com_SignalConfig[i].signalId, value);
        }
    }
}

// Send signal from App to CAN
Std_ReturnType Com_SendSignal(Com_SignalIdType signalId, const void* SignalDataPtr)
{
    for (int i = 0; i < COM_MAX_SIGNALS; ++i) {
        if (Com_SignalConfig[i].signalId == signalId) {
            Com_PduIdType pduId = Com_SignalConfig[i].pduId;

            uint8 byteCount = Com_SignalConfig[i].bitLength / 8;
            if( byteCount > sizeof(Com_PduBuffer[pduId])) {
                return E_NOT_OK; 
            }
            uint8 byteIdx = Com_SignalConfig[i].startBit / 8;
            memcpy(&Com_PduBuffer[pduId][byteIdx], (uint8*)SignalDataPtr, byteCount);

            //Prepare PDU for transmission
            CanIf_TxPduInfoType txPdu;
            txPdu.CanId = 0; // actually not used here
            txPdu.CanDlc = Com_PduConfig[pduId].length;
            txPdu.Sdu = Com_PduBuffer[pduId];
            txPdu.Hth = 0; 
            txPdu.PduId = pduId;

            return PduR_CanIfTransmit(pduId, &txPdu);
        }
    }
    return E_NOT_OK;
}

// Receive signal from App
Std_ReturnType Com_ReceiveSignal(Com_SignalIdType signalId, void* SignalDataPtr)
{
    for (int i = 0; i < COM_MAX_SIGNALS; ++i) {
        if (Com_SignalConfig[i].signalId == signalId) {
            Com_PduIdType pduId = Com_SignalConfig[i].pduId;
            uint8 byteCount = Com_SignalConfig[i].bitLength / 8;
            if( byteCount > sizeof(Com_PduBuffer[pduId])) {
                return E_NOT_OK; 
            }
            uint8 byteIdx = Com_SignalConfig[i].startBit / 8;
            memcpy((uint8*)SignalDataPtr, &Com_PduBuffer[pduId][byteIdx], byteCount);

            return E_OK;
        }
    }
    return E_NOT_OK;
}

// Log transmission confirmation
void Com_TxConfirmation(Com_PduIdType pduId)
{
    printf("[Com] TX Confirmation for PDU=%d\n", pduId);
}

void Com_Init(void)
{
    memset(Com_PduBuffer, 0, sizeof(Com_PduBuffer));

    // Register PduR Callbacks 
    PduR_RegisterRxIndicationCallback(Com_RxIndication);
    PduR_RegisterTxConfirmationCallback(Com_TxConfirmation);
}
