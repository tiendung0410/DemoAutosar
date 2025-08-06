#ifndef IOHWAB_H
#define IOHWAB_H

#include "Std_Types.h"

/* === LED IDs === */
typedef uint8 IoHwAb_LedIdType;

#define IOHWAB_LED_STATUS      0u
#define IOHWAB_NUM_LEDS        1u

/* === API === */
Std_ReturnType IoHwAb_Led_Set(IoHwAb_LedIdType id, uint8 level);
void IoHwAb_Init(void);

#endif /* IOHWAB_H */
