#ifndef CANIF_H_
#define CANIF_H_

#include "CanIf_Cfg.h"

typedef void (*CanIf_RxIndicationCbkType)(CanIf_PduIdType pduId, const uint8* SduPtr, uint8 length);
typedef void (*CanIf_TxConfirmationCbkType)(CanIf_PduIdType pduId);

void CanIf_RegisterRxIndicationCallback(CanIf_RxIndicationCbkType cb);
void CanIf_RegisterTxConfirmationCallback(CanIf_TxConfirmationCbkType cb);

void CanIf_Init(void);
Std_ReturnType CanIf_Transmit(CanIf_PduIdType TxPduId, const CanIf_TxPduInfoType* PduInfoPtr);
void CanIf_RxIndication(uint8 Hrh, uint32 CanId, uint8 CanDlc, const uint8 *CanSduPtr);
void CanIf_TxConfirmation(CanIf_PduIdType TxPduId);
void CanIf_MainFunction(void);

#endif /* CANIF_H_ */
