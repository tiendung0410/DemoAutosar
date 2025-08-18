#ifndef COM_CFG_H_
#define COM_CFG_H_

#include "Std_Types.h"

#define COM_MAX_SIGNALS   1    
#define COM_MAX_PDU       1    

typedef uint8 Com_SignalIdType;
typedef uint8 Com_PduIdType;

typedef struct {
    Com_SignalIdType signalId;
    Com_PduIdType    pduId;
    uint8            startBit; // offset trong PDU (bit)
    uint8            bitLength;
} Com_SignalCfgType;

typedef struct {
    Com_PduIdType    pduId;
    uint8            length;   // chiều dài PDU (byte)
} Com_PduCfgType;

extern const Com_SignalCfgType Com_SignalConfig[COM_MAX_SIGNALS];
extern const Com_PduCfgType    Com_PduConfig[COM_MAX_PDU];

#endif /* COM_CFG_H_ */
