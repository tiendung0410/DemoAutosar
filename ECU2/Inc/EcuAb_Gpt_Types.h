#ifndef ECUAB_GPT_TYPES_H
#define ECUAB_GPT_TYPES_H

#include <stdint.h>

typedef uint8_t  EcuAb_Gpt_ChannelType;    /* kênh logic ở ECUA */
typedef uint32_t EcuAb_Gpt_TicksType;
typedef void   (*EcuAb_Gpt_AppCbkType)(void);

typedef enum {
    ECUAB_GPT_ONESHOT = 0,
    ECUAB_GPT_CONTINUOUS
} EcuAb_Gpt_ModeType;

#endif /* ECUAB_GPT_TYPES_H */
