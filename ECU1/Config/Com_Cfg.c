#include "Com_Cfg.h"

// Mapping từng signalId với vị trí trong PDU 0
const Com_SignalCfgType Com_SignalConfig[COM_MAX_SIGNALS] = {
    { .signalId = 0, .pduId = 0, .startBit = 0, .bitLength = 8 },  // ButtonState
};

// PDU 0:  tổng dài 2 byte (16 bit)
const Com_PduCfgType Com_PduConfig[COM_MAX_PDU] = {
    { .pduId = 0, .length = 2 },
};
