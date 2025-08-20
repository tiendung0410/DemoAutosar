#ifndef ECUAB_GPT_CFG_H
#define ECUAB_GPT_CFG_H

#include "EcuAb_Gpt_Types.h"
#include "Gpt.h"        /* dùng Gpt_ChannelType, API MCAL */
#include "Gpt_Cfg.h"   

/* Logic channel number export for upper layer */
#define ECUAB_GPT_MAX_CHANNELS   2u

/* Map logic channel (ECUA) ->  MCAL channel (GPT) and default tick rate */
typedef struct {
    EcuAb_Gpt_ChannelType  logicCh;
    Gpt_ChannelType        mcalCh;
    uint32_t               tickHz;      /* must equal to tickFrequency of respective MCAL channel */
    EcuAb_Gpt_ModeType     defaultMode; /* ONESHOT/CONTINUOUS */
} EcuAb_Gpt_ChannelMapType;

/* Mapping table declaration (defined at EcuAb_Gpt.c ) */
extern const EcuAb_Gpt_ChannelMapType EcuAb_Gpt_ChannelMap[ECUAB_GPT_MAX_CHANNELS];

#endif /* ECUAB_GPT_CFG_H */
