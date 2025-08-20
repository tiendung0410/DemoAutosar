#include "CanIf.h"
#include "Can.h"

#include <stdio.h>
#include <string.h>

// Callback pointer (registered by uppper layers)
static CanIf_RxIndicationCbkType RxIndicationCbk = NULL;
static CanIf_TxConfirmationCbkType TxConfirmationCbk = NULL;

void CanIf_RegisterRxIndicationCallback(CanIf_RxIndicationCbkType cb) {
    RxIndicationCbk = cb;
}

void CanIf_RegisterTxConfirmationCallback(CanIf_TxConfirmationCbkType cb) {
    TxConfirmationCbk = cb;
}

void CanIf_Init(void) {
    RxIndicationCbk = NULL;
    TxConfirmationCbk = NULL;
}

Std_ReturnType CanIf_Transmit(CanIf_PduIdType TxPduId, const CanIf_TxPduInfoType* PduInfoPtr)
{
    for (int i = 0; i < CANIF_MAX_TX_PDU; ++i) {
        if (CanIf_TxPduConfig[i].pduId == TxPduId) {
            Can_PduType pdu;
            pdu.id = CanIf_TxPduConfig[i].canId;
            pdu.length = PduInfoPtr->CanDlc;
            pdu.sdu = PduInfoPtr->Sdu;
            pdu.swPduHandle = TxPduId;

            return Can_Write(CanIf_TxPduConfig[i].hth, &pdu);
        }
    }
    return E_NOT_OK;
}

void CanIf_RxIndication(uint8 Hrh, uint32 CanId, uint8 CanDlc, const uint8 *CanSduPtr)
{
    for (int i = 0; i < CANIF_MAX_RX_PDU; ++i) {
        if (CanIf_RxPduConfig[i].hrh == Hrh && CanIf_RxPduConfig[i].canId == CanId) {
            if (RxIndicationCbk) {
                RxIndicationCbk(CanIf_RxPduConfig[i].pduId, CanSduPtr, CanDlc);
            } else {
                printf("[CanIf] RX Indication: (No RxIndicationCbk)\n");
            }
            break;
        }
    }
}

void CanIf_TxConfirmation(CanIf_PduIdType TxPduId)
{
    if (TxConfirmationCbk) {
        TxConfirmationCbk(TxPduId);
    } else {
        printf("[CanIf] TX Confirm\n");
    }
}

void CanIf_MainFunction(void)
{
    //Reserve for future use, currently no periodic tasks
}
