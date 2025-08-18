#include "PduR.h"
#include "CanIf.h"
#include <stdio.h>

// Callback pointer lên App/COM
static PduR_RxIndicationCbkType RxCbk = NULL;
static PduR_TxConfirmationCbkType TxCbk = NULL;

// Đăng ký callback
void PduR_RegisterRxIndicationCallback(PduR_RxIndicationCbkType cb) {
    RxCbk = cb;
}
void PduR_RegisterTxConfirmationCallback(PduR_TxConfirmationCbkType cb) {
    TxCbk = cb;
}

// Khởi tạo PduR, đăng ký callback lên CanIf
void PduR_Init(void) {
    // Đăng ký callback lên CanIf
    CanIf_RegisterRxIndicationCallback(PduR_CanIfRxIndication);
    CanIf_RegisterTxConfirmationCallback(PduR_CanIfTxConfirmation);
    // Khởi tạo pointer lên App/COM
    RxCbk = NULL;
    TxCbk = NULL;
}

// App/COM gọi để gửi PDU qua CAN
Std_ReturnType PduR_CanIfTransmit(uint32 pduId, const CanIf_TxPduInfoType* PduInfoPtr) {
    // Ở đây mapping trực tiếp pduId sang CanIf_PduId (project nhỏ)
    return CanIf_Transmit(pduId, PduInfoPtr);
}

// Callback được CanIf gọi khi nhận được CAN frame
void PduR_CanIfRxIndication(uint32 pduId, const uint8* SduPtr, uint8 length) {
    if (RxCbk) {
        RxCbk(pduId, SduPtr, length); // Route lên App/COM
    } else {
        // printf("[PduR] RX: pduId=0x%X, len=%d, data:", pduId, length);
        for (uint8 i = 0; i < length; ++i)
            printf(" %02X", SduPtr[i]);
        printf("\n");
    }
}

// Callback được CanIf gọi khi gửi xong CAN frame
void PduR_CanIfTxConfirmation(uint32 pduId) {
    if (TxCbk) {
        TxCbk(pduId); // Route lên App/COM
    } else {
        // printf("[PduR] TX Confirm: pduId=0x%X\n", pduId);
    }
}
