#ifndef COM_H_
#define COM_H_

#include "Com_Cfg.h"

Std_ReturnType Com_SendSignal(Com_SignalIdType signalId, const void* SignalDataPtr);
Std_ReturnType Com_ReceiveSignal(Com_SignalIdType signalId, void* SignalDataPtr);

void Com_RxIndication(Com_PduIdType pduId, const uint8* SduDataPtr, uint8 length);
void Com_TxConfirmation(Com_PduIdType pduId);

void Com_Init(void);

#endif /* COM_H_ */
