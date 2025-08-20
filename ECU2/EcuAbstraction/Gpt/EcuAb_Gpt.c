#define _POSIX_C_SOURCE 200809L
#include "EcuAb_Gpt.h"
#include "EcuAb_Gpt_Cfg.h"
#include "Gpt.h"
#include "Gpt_Cfg.h"
#include <stddef.h>

/* --------- MAP LOGIC->MCAL (Match with Gpt_Config) ---------- */
const EcuAb_Gpt_ChannelMapType EcuAb_Gpt_ChannelMap[ECUAB_GPT_MAX_CHANNELS] = {
    /* logicCh, mcalCh, tickHz,   defaultMode */
    { 0u,       0u,     1000u, ECUAB_GPT_CONTINUOUS }, /* GPT0 periodic 1us tick */
    { 1u,       1u,     1000u, ECUAB_GPT_ONESHOT    }, /* GPT1 one-shot 1us tick */
};

/* ---------- STATE ECUA ---------- */
static EcuAb_Gpt_AppCbkType s_appCbk[ECUAB_GPT_MAX_CHANNELS] = {0};
static EcuAb_Gpt_ModeType   s_mode = ECUAB_GPT_CONTINUOUS; /* global ECUA “hint” */

/* Find map by logic channel */
static inline const EcuAb_Gpt_ChannelMapType* map_of(EcuAb_Gpt_ChannelType ch)
{
    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        if (EcuAb_Gpt_ChannelMap[i].logicCh == ch) return &EcuAb_Gpt_ChannelMap[i];
    }
    return NULL;
}


/* ========= ECUA API ========= */
void EcuAb_Gpt_Init(void)
{

    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        Gpt_EnableNotification(EcuAb_Gpt_ChannelMap[i].mcalCh);
    }
}

void EcuAb_Gpt_DeInit(void)
{
    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        Gpt_StopTimer(EcuAb_Gpt_ChannelMap[i].mcalCh);
        s_appCbk[i] = NULL;
    }
    Gpt_DeInit();
}

void EcuAb_Gpt_StartTicks(EcuAb_Gpt_ChannelType ch, EcuAb_Gpt_TicksType ticks)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return;

    Gpt_StartTimer(m->mcalCh, (Gpt_ValueType)ticks);
}


void EcuAb_Gpt_Stop(EcuAb_Gpt_ChannelType ch)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return;
    Gpt_StopTimer(m->mcalCh);
}

EcuAb_Gpt_TicksType EcuAb_Gpt_GetElapsed(EcuAb_Gpt_ChannelType ch)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return 0u;
    return (EcuAb_Gpt_TicksType)Gpt_GetTimeElapsed(m->mcalCh);
}

EcuAb_Gpt_TicksType EcuAb_Gpt_GetRemaining(EcuAb_Gpt_ChannelType ch)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return 0u;
    return (EcuAb_Gpt_TicksType)Gpt_GetTimeRemaining(m->mcalCh);
}

int EcuAb_Gpt_RegisterCallback(EcuAb_Gpt_ChannelType ch, EcuAb_Gpt_AppCbkType cb)
{
    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        if (EcuAb_Gpt_ChannelMap[i].logicCh == ch) {
            s_appCbk[i] = cb;
            return 0; /* OK */
        }
    }
    return -1; /* not found */
}

void EcuAb_Gpt_EnableNotification(EcuAb_Gpt_ChannelType ch)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return;
    Gpt_EnableNotification(m->mcalCh);
}

void EcuAb_Gpt_DisableNotification(EcuAb_Gpt_ChannelType ch)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return;
    Gpt_DisableNotification(m->mcalCh);
}

void EcuAb_Gpt_SetMode(EcuAb_Gpt_ModeType mode)
{
    s_mode = mode;
}

/* ========= BRIDGE CALLBACK từ MCAL GPT -> ECUA -> APP =========
 */

static void EcuAb_Gpt_OnMcalCb(Gpt_ChannelType mcalCh)
{
    /* find entry map; call app callback */
    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        if (EcuAb_Gpt_ChannelMap[i].mcalCh == mcalCh) {
            if (s_appCbk[i]) s_appCbk[i]();
            break;
        }
    }
}

/* MCAL GPT callback symbols */
void Gpt0_Notification(void) { EcuAb_Gpt_OnMcalCb(0u); }
void Gpt1_Notification(void) { EcuAb_Gpt_OnMcalCb(1u); }

