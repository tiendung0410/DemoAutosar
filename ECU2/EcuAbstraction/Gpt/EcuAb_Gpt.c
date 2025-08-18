#define _POSIX_C_SOURCE 200809L
#include "EcuAb_Gpt.h"
#include "EcuAb_Gpt_Cfg.h"
#include "Gpt.h"
#include "Gpt_Cfg.h"
#include <stddef.h>

/* --------- MAP LOGIC->MCAL (khớp với Gpt_Config của bạn) ---------- */
const EcuAb_Gpt_ChannelMapType EcuAb_Gpt_ChannelMap[ECUAB_GPT_MAX_CHANNELS] = {
    /* logicCh, mcalCh, tickHz,   defaultMode */
    { 0u,       0u,     1000000u, ECUAB_GPT_CONTINUOUS }, /* GPT0 periodic 1us tick */
    { 1u,       1u,     1000000u, ECUAB_GPT_ONESHOT    }, /* GPT1 one-shot 1us tick */
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

/* Convert helpers */
static inline EcuAb_Gpt_TicksType ms_to_ticks(uint32_t ms, uint32_t hz)
{   /* ticks = ms * 1e-3 * hz = ms * (hz/1000) */
    return (EcuAb_Gpt_TicksType)((uint64_t)ms * (hz/1000u));
}
static inline EcuAb_Gpt_TicksType us_to_ticks(uint32_t us, uint32_t hz)
{   /* ticks = us * 1e-6 * hz = us * (hz/1e6) ; với hz=1MHz => ticks = us */
    return (EcuAb_Gpt_TicksType)((uint64_t)us * (hz/1000000u));
}

/* ========= ECUA API ========= */
void EcuAb_Gpt_Init(void)
{
    /* Khởi tạo MCAL GPT với cấu hình toàn cục */
    Gpt_Init(&Gpt_Config);

    /* Bật notification mặc định cho tất cả kênh (app sẽ có thể nhận ngay nếu đăng ký) */
    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        Gpt_EnableNotification(EcuAb_Gpt_ChannelMap[i].mcalCh);
    }
}

void EcuAb_Gpt_DeInit(void)
{
    /* Stop toàn bộ kênh trước khi DeInit */
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

    /* Ở ECUA, mode mang tính semantic; MCAL mode (oneshot/continuous) đã được fix trong Gpt_Config.
       Nếu muốn ép oneshot giả lập trên kênh continuous, có thể disable notification sau lần đầu, v.v.
       Bản tối giản: tin tưởng config MCAL. */
    Gpt_StartTimer(m->mcalCh, (Gpt_ValueType)ticks);
}

void EcuAb_Gpt_StartMs(EcuAb_Gpt_ChannelType ch, uint32_t ms)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return;
    EcuAb_Gpt_StartTicks(ch, ms_to_ticks(ms, m->tickHz));
}

void EcuAb_Gpt_StartUs(EcuAb_Gpt_ChannelType ch, uint32_t us)
{
    const EcuAb_Gpt_ChannelMapType* m = map_of(ch);
    if (!m) return;
    EcuAb_Gpt_StartTicks(ch, us_to_ticks(us, m->tickHz));
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
    /* Tối giản: chỉ lưu lại. Nếu muốn, có thể stop/restart theo mode mới. */
}

/* ========= BRIDGE CALLBACK từ MCAL GPT -> ECUA -> APP =========
   Lưu ý: Trong Gpt_Cfg.c, bạn đã cấu hình:
     { .channelId = 0, .callback = Gpt0_Notification, ... }
     { .channelId = 1, .callback = Gpt1_Notification, ... }
   ECUA sẽ cung cấp các hàm này, forward lên app callback đã register. */

static void EcuAb_Gpt_OnMcalCb(Gpt_ChannelType mcalCh)
{
    /* tìm entry map; gọi app callback nếu có */
    for (unsigned i=0; i<ECUAB_GPT_MAX_CHANNELS; ++i) {
        if (EcuAb_Gpt_ChannelMap[i].mcalCh == mcalCh) {
            if (s_appCbk[i]) s_appCbk[i]();
            break;
        }
    }
}

/* MCAL GPT callback symbols (đừng định nghĩa ở nơi khác) */
void Gpt0_Notification(void) { EcuAb_Gpt_OnMcalCb(0u); }
void Gpt1_Notification(void) { EcuAb_Gpt_OnMcalCb(1u); }

