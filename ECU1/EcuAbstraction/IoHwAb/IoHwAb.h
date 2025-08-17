#ifndef IOHWAB_H
#define IOHWAB_H

#include "Std_Types.h"

/* === Button IDs === */
typedef uint8 IoHwAb_ButtonIdType;

#define IOHWAB_BUTTON_SYSTEM   0u
#define IOHWAB_BUTTON_FREQ     1u
#define IOHWAB_NUM_BUTTONS     2u

/* === API === */
Std_ReturnType IoHwAb_Button_Read(IoHwAb_ButtonIdType id, uint8* level);
void IoHwAb_Init(void);

#endif /* IOHWAB_H */
