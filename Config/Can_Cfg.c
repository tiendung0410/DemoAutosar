#include "Can_Cfg.h"

/* Ở đây trong AUTOSAR thực sự sẽ có cấu hình mapping HTH, HRH, mailbox... 
   Với demo này mình để trống hoặc tạo mảng cấu hình giả lập */

typedef struct {
    uint8 controllerId;
    uint8 hthId;
} Can_HthConfigType;

typedef struct {
    uint8 controllerId;
    uint8 hrhId;
} Can_HrhConfigType;

/* Config giả lập */
static const Can_HthConfigType Can_HthConfig[] = {
    { CAN_CTRL_0, CAN_HTH_0 }
};

static const Can_HrhConfigType Can_HrhConfig[] = {
    { CAN_CTRL_0, CAN_HRH_0 }
};

/* Getter */
const void* Can_GetHthConfig(uint8 idx) { return &Can_HthConfig[idx]; }
const void* Can_GetHrhConfig(uint8 idx) { return &Can_HrhConfig[idx]; }
