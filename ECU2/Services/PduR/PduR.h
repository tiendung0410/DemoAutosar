#ifndef PDUR_H_
#define PDUR_H_

#include "PduR_Cfg.h"
#include "CanIf_Types.h"

// Định nghĩa callback lên App/COM
typedef void (*PduR_RxIndicationCbkType)(uint8 pduId, const uint8* SduPtr, uint8 length);
typedef void (*PduR_TxConfirmationCbkType)(uint8 pduId);

// Đăng ký callback
void PduR_RegisterRxIndicationCallback(PduR_RxIndicationCbkType cb);
void PduR_RegisterTxConfirmationCallback(PduR_TxConfirmationCbkType cb);

void PduR_Init(void);

// App/COM gọi để gửi (giả sử pduId mapping luôn với CanIf)
Std_ReturnType PduR_CanIfTransmit(uint32 pduId, const CanIf_TxPduInfoType* PduInfoPtr);

// Callback của CanIf gọi khi nhận/gửi xong
void PduR_CanIfRxIndication(uint32 pduId, const uint8* SduPtr, uint8 length);
void PduR_CanIfTxConfirmation(uint32 pduId);

#endif /* PDUR_H_ */
