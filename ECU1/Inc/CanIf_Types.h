#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_

#include "Std_Types.h"

typedef uint8 CanIf_HwHandleType;
typedef uint32 CanIf_PduIdType;

typedef struct {
    uint32 CanId;
    uint8  CanDlc;
    uint8* Sdu;
    CanIf_HwHandleType Hrh;   // Hardware Receive Handle
} CanIf_RxPduInfoType;

typedef struct {
    uint32 CanId;
    uint8  CanDlc;
    uint8* Sdu;
    CanIf_HwHandleType Hth;   // Hardware Transmit Handle
    CanIf_PduIdType PduId;
} CanIf_TxPduInfoType;

#endif
