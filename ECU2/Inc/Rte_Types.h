#ifndef RTE_TYPES_H_
#define RTE_TYPES_H_

#include "Std_Types.h"

// Struct dùng làm Data Element cho Sender-Receiver interface
typedef struct {
    uint8 ButtonState;   // 0: released, 1: pressed
    uint8 ButtonCount;   // số lần nhấn
} ButtonStatusInfoType;

// Có thể khai báo thêm các type cho SWC khác nếu cần

#endif /* RTE_TYPES_H_ */
