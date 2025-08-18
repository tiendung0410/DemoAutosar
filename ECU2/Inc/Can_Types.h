#ifndef CAN_TYPES_H_
#define CAN_TYPES_H_

#include "Std_Types.h"

typedef enum {
    CAN_CS_UNINIT,
    CAN_CS_STARTED,
    CAN_CS_STOPPED,
    CAN_CS_SLEEP
} Can_ControllerStateType;

typedef uint8 Can_ReturnType;
#define CAN_OK      0U
#define CAN_NOT_OK  1U
#define CAN_BUSY    2U

typedef uint32 Can_IdType;

typedef struct {
    Can_IdType id;
    uint8 length;
    uint8* sdu;
    uint32 swPduHandle;
} Can_PduType;

#endif /* CANIF_TYPES_H_ */
