#include "PduR.h"
#include "CanIf.h"
#include <stdio.h>

// Callback pointer to COM
static PduR_RxIndicationCbkType RxCbk = NULL;
static PduR_TxConfirmationCbkType TxCbk = NULL;

// Callback register
void PduR_RegisterRxIndicationCallback(PduR_RxIndicationCbkType cb) {
    RxCbk = cb;
}
void PduR_RegisterTxConfirmationCallback(PduR_TxConfirmationCbkType cb) {
    TxCbk = cb;
}


void PduR_Init(void) {
    CanIf_RegisterRxIndicationCallback(PduR_CanIfRxIndication);
    CanIf_RegisterTxConfirmationCallback(PduR_CanIfTxConfirmation);

    RxCbk = NULL;
    TxCbk = NULL;
}

Std_ReturnType PduR_CanIfTransmit(uint32 pduId, const CanIf_TxPduInfoType* PduInfoPtr) {

    return CanIf_Transmit(pduId, PduInfoPtr);
}

// Callback called by CanIf when receive CAN frame
void PduR_CanIfRxIndication(uint32 pduId, const uint8* SduPtr, uint8 length) {
    if (RxCbk) {
        RxCbk(pduId, SduPtr, length); // Route to COM
    } else {
        // Log for debugging
        for (uint8 i = 0; i < length; ++i)
            printf(" %02X", SduPtr[i]);
        printf("\n");
    }
}

// Callback called by CanIf when completed transmit CAN frame
void PduR_CanIfTxConfirmation(uint32 pduId) {
    if (TxCbk) {
        TxCbk(pduId); // Route to COM
    } else {
        // Log for debugging
    }
}
