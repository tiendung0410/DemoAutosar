#ifndef IOHWAB_H
#define IOHWAB_H

#include "Std_Types.h"

typedef uint8 IoHwAb_LedIdType;

Std_ReturnType IoHwAb_Led_Set(IoHwAb_LedIdType id, uint8 level);
void IoHwAb_Init(void);

#endif /* IOHWAB_H */
