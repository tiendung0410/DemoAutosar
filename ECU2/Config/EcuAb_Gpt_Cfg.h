#ifndef ECUAB_GPT_CFG_H
#define ECUAB_GPT_CFG_H

#include "EcuAb_Gpt_Types.h"
#include "Gpt.h"        /* dùng Gpt_ChannelType, API MCAL */
#include "Gpt_Cfg.h"    /* extern Gpt_Config (nếu bạn tách riêng ở Config/) */

/* Số kênh logic mà ECUA export cho upper layer */
#define ECUAB_GPT_MAX_CHANNELS   2u

/* Map kênh logic (ECUA) -> kênh MCAL (GPT) + tickHz mặc định */
typedef struct {
    EcuAb_Gpt_ChannelType  logicCh;
    Gpt_ChannelType        mcalCh;
    uint32_t               tickHz;      /* phải trùng với tickFrequency của MCAL kênh tương ứng */
    EcuAb_Gpt_ModeType     defaultMode; /* ONESHOT/CONTINUOUS ở tầng ECUA */
} EcuAb_Gpt_ChannelMapType;

/* Khai báo bảng map (định nghĩa ở EcuAb_Gpt.c cho đơn giản) */
extern const EcuAb_Gpt_ChannelMapType EcuAb_Gpt_ChannelMap[ECUAB_GPT_MAX_CHANNELS];

#endif /* ECUAB_GPT_CFG_H */
