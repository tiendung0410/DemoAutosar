#ifndef IOHWAB_H
#define IOHWAB_H

#include "Std_Types.h"
#include "IoHwAb_Cfg.h"
#include <stddef.h>

/* === Button IDs === */
typedef uint8 IoHwAb_ButtonIdType;

/* === API === */
Std_ReturnType IoHwAb_Button_Read(IoHwAb_ButtonIdType id, uint8* level);
void IoHwAb_Init(void);

#endif /* IOHWAB_H */
