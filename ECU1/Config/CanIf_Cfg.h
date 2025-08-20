#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_

#include "Std_Types.h"
#include "CanIf_Types.h"

#define CANIF_MAX_CHANNELS      1
#define CANIF_MAX_TX_PDU        1   // Transmit PDU number
#define CANIF_MAX_RX_PDU        1   // Receive PDU number


typedef struct {
    CanIf_HwHandleType hth;
    CanIf_PduIdType    pduId;
    uint32             canId;
} CanIf_TxPduCfgType;


typedef struct {
    CanIf_HwHandleType hrh;
    CanIf_PduIdType    pduId;
    uint32             canId;
} CanIf_RxPduCfgType;

/* extern array config defined at CanIf_Cfg.c */ 
extern const CanIf_TxPduCfgType CanIf_TxPduConfig[CANIF_MAX_TX_PDU];
extern const CanIf_RxPduCfgType CanIf_RxPduConfig[CANIF_MAX_RX_PDU];

#endif /* CANIF_CFG_H_ */
