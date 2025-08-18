#include "CanIf_Cfg.h"

// Mapping 1 PDU gửi (ví dụ gửi CAN ID 0x123)
const CanIf_TxPduCfgType CanIf_TxPduConfig[CANIF_MAX_TX_PDU] = {
    { .hth = 0, .pduId = 0, .canId = 0x123 },
};

// Mapping 1 PDU nhận (ví dụ nhận CAN ID 0x123)
const CanIf_RxPduCfgType CanIf_RxPduConfig[CANIF_MAX_RX_PDU] = {
    { .hrh = 0, .pduId = 0, .canId = 0x123 },
};
