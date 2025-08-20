#include "CanIf_Cfg.h"

const CanIf_TxPduCfgType CanIf_TxPduConfig[CANIF_MAX_TX_PDU] = {
    { .hth = 0, .pduId = 0, .canId = 0x123 },
};


const CanIf_RxPduCfgType CanIf_RxPduConfig[CANIF_MAX_RX_PDU] = {
    { .hrh = 0, .pduId = 0, .canId = 0x123 },
};
