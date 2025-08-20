#include "Com_Cfg.h"

const Com_SignalCfgType Com_SignalConfig[COM_MAX_SIGNALS] = {
    { .signalId = 0, .pduId = 0, .startBit = 0, .bitLength = 16 },  // Led
};


const Com_PduCfgType Com_PduConfig[COM_MAX_PDU] = {
    { .pduId = 0, .length = 2 },
};
