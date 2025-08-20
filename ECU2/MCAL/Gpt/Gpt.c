#define _POSIX_C_SOURCE 199309L  // << cần khai báo dòng này trước include

#include <time.h>
#include <signal.h>
#include <pthread.h>   // nếu dùng SIGEV_THREAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Gpt.h"

typedef struct {
    timer_t                 posixTimer;
    struct timespec         startTs;
    Gpt_ValueType           programmedTicks;
    uint64_t                programmedNs;   /* thời gian mục tiêu ns */
    boolean                 running;
    boolean                 notifyEnabled;
    pthread_mutex_t         lock;
    const Gpt_ChannelConfigType* cfg;
} Gpt_ChCtx;

static struct {
    const Gpt_ConfigType* cfg;
    Gpt_ModeType          mode;
    Gpt_ChCtx*            ch;   /* mảng context có size = numOfChannels */
} Gpt_Drv = {0};

static uint64_t ts_to_ns(const struct timespec* ts) {
    return (uint64_t)ts->tv_sec * 1000000000ULL + (uint64_t)ts->tv_nsec;
}
static void ns_to_itimerspec(uint64_t ns, struct itimerspec* its, int periodic) {
    its->it_value.tv_sec  = ns / 1000000000ULL;
    its->it_value.tv_nsec = ns % 1000000000ULL;
    if (periodic) {
        its->it_interval = its->it_value;  /* period = value */
    } else {
        its->it_interval.tv_sec  = 0;
        its->it_interval.tv_nsec = 0;
    }
}

/* Callback thread của POSIX timer → gọi notification ứng dụng */
static void gpt_timer_sigev_thread(union sigval sv) {
    Gpt_ChannelType chId = (Gpt_ChannelType)(uintptr_t)sv.sival_ptr;
    Gpt_ChCtx* c = &Gpt_Drv.ch[chId];

    /* One-shot → running = FALSE sau khi fired */
    pthread_mutex_lock(&c->lock);
    if (c->cfg->channelMode == GPT_CH_MODE_ONESHOT) {
        c->running = FALSE;
    }
    boolean doNotify = (c->notifyEnabled && c->cfg->notification != NULL);
    Gpt_NotificationType cb = c->cfg->notification;
    pthread_mutex_unlock(&c->lock);

    if (doNotify) {
        cb();
    }
}

static uint64_t ticks_to_ns(const Gpt_ChannelConfigType* cfg, Gpt_ValueType ticks) {
    /* ns = ticks * (1e9 / freq) ; tránh tràn: nhân trước rồi chia ULL */
    uint64_t num = (uint64_t)ticks * 1000000000ULL;
    return num / cfg->tickFrequencyHz;
}

void Gpt_Init(const Gpt_ConfigType* ConfigPtr) {
    if (!ConfigPtr || !ConfigPtr->channels || ConfigPtr->numOfChannels == 0) return;

    memset(&Gpt_Drv, 0, sizeof(Gpt_Drv));
    Gpt_Drv.cfg  = ConfigPtr;
    Gpt_Drv.mode = GPT_MODE_NORMAL;
    Gpt_Drv.ch   = (Gpt_ChCtx*)calloc(ConfigPtr->numOfChannels, sizeof(Gpt_ChCtx));

    for (uint8_t i = 0; i < ConfigPtr->numOfChannels; ++i) {
        Gpt_ChCtx* c = &Gpt_Drv.ch[i];
        c->cfg = &ConfigPtr->channels[i];
        pthread_mutex_init(&c->lock, NULL);

        struct sigevent sev;
        memset(&sev, 0, sizeof(sev));
        sev.sigev_notify          = SIGEV_THREAD;
        sev.sigev_notify_function = gpt_timer_sigev_thread;
        sev.sigev_value.sival_ptr = (void*)(uintptr_t)c->cfg->channelId;

        if (timer_create(CLOCK_MONOTONIC, &sev, &c->posixTimer) != 0) {
            /* lỗi tạo timer → vẫn cho phép init tiếp các channel khác */
        }
        c->notifyEnabled = TRUE;  /* default bật notify */
    }
}

void Gpt_DeInit(void) {
    if (!Gpt_Drv.cfg) return;
    for (uint8_t i = 0; i < Gpt_Drv.cfg->numOfChannels; ++i) {
        Gpt_ChCtx* c = &Gpt_Drv.ch[i];
        if (c->posixTimer) {
            struct itimerspec its = {0};
            timer_settime(c->posixTimer, 0, &its, NULL);
            timer_delete(c->posixTimer);
        }
        pthread_mutex_destroy(&c->lock);
    }
    free(Gpt_Drv.ch);
    memset(&Gpt_Drv, 0, sizeof(Gpt_Drv));
}

void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value) {
    if (!Gpt_Drv.cfg || Channel >= Gpt_Drv.cfg->numOfChannels) return;
    Gpt_ChCtx* c = &Gpt_Drv.ch[Channel];

    pthread_mutex_lock(&c->lock);

    if (Value == 0 || Value > c->cfg->tickMaxValue) {
        pthread_mutex_unlock(&c->lock);
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, &c->startTs);
    c->programmedTicks = Value;
    c->programmedNs    = ticks_to_ns(c->cfg, c->programmedTicks);

    struct itimerspec its;
    ns_to_itimerspec(c->programmedNs, &its, c->cfg->channelMode == GPT_CH_MODE_CONTINUOUS);
    if (timer_settime(c->posixTimer, 0, &its, NULL) != 0) {
        printf("[GPT] Error setting timer for channel %d: %s\n", Channel, strerror(errno));
    }

    
    c->running = TRUE;
    pthread_mutex_unlock(&c->lock);
}

void Gpt_StopTimer(Gpt_ChannelType Channel) {
    if (!Gpt_Drv.cfg || Channel >= Gpt_Drv.cfg->numOfChannels) return;
    Gpt_ChCtx* c = &Gpt_Drv.ch[Channel];
    pthread_mutex_lock(&c->lock);
    struct itimerspec its = {0};
    timer_settime(c->posixTimer, 0, &its, NULL);
    c->running = FALSE;
    pthread_mutex_unlock(&c->lock);
}

Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel) {
    if (!Gpt_Drv.cfg || Channel >= Gpt_Drv.cfg->numOfChannels) return 0;
    Gpt_ChCtx* c = &Gpt_Drv.ch[Channel];

    pthread_mutex_lock(&c->lock);
    if (!c->running) {
        pthread_mutex_unlock(&c->lock);
        return 0;
    }
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t elapsedNs = ts_to_ns(&now) - ts_to_ns(&c->startTs);

    /* Giới hạn về programmedNs nếu one-shot */
    if (c->cfg->channelMode == GPT_CH_MODE_ONESHOT && elapsedNs > c->programmedNs) {
        elapsedNs = c->programmedNs;
    }
    /* ticks = elapsedNs * freq / 1e9 */
    uint64_t ticks = (elapsedNs * c->cfg->tickFrequencyHz) / 1000000000ULL;
    if (ticks > c->cfg->tickMaxValue) ticks = c->cfg->tickMaxValue;
    pthread_mutex_unlock(&c->lock);
    return (Gpt_ValueType)ticks;
}

Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel) {
    if (!Gpt_Drv.cfg || Channel >= Gpt_Drv.cfg->numOfChannels) return 0;
    Gpt_ChCtx* c = &Gpt_Drv.ch[Channel];

    pthread_mutex_lock(&c->lock);
    if (!c->running) {
        pthread_mutex_unlock(&c->lock);
        return 0;
    }

    struct itimerspec cur;
    timer_gettime(c->posixTimer, &cur);
    uint64_t remNs = (uint64_t)cur.it_value.tv_sec * 1000000000ULL + (uint64_t)cur.it_value.tv_nsec;
    uint64_t remTicks = (remNs * c->cfg->tickFrequencyHz) / 1000000000ULL;
    if (remTicks > c->cfg->tickMaxValue) remTicks = c->cfg->tickMaxValue;

    pthread_mutex_unlock(&c->lock);
    return (Gpt_ValueType)remTicks;
}

void Gpt_EnableNotification(Gpt_ChannelType Channel) {
    if (!Gpt_Drv.cfg || Channel >= Gpt_Drv.cfg->numOfChannels) return;
    Gpt_ChCtx* c = &Gpt_Drv.ch[Channel];
    pthread_mutex_lock(&c->lock);
    c->notifyEnabled = TRUE;
    pthread_mutex_unlock(&c->lock);
}

void Gpt_DisableNotification(Gpt_ChannelType Channel) {
    if (!Gpt_Drv.cfg || Channel >= Gpt_Drv.cfg->numOfChannels) return;
    Gpt_ChCtx* c = &Gpt_Drv.ch[Channel];
    pthread_mutex_lock(&c->lock);
    c->notifyEnabled = FALSE;
    pthread_mutex_unlock(&c->lock);
}

void Gpt_SetMode(Gpt_ModeType Mode) {
    /* Ở Linux user-space không có low-power native → lưu lại cho đúng API */
    Gpt_Drv.mode = Mode;
}

Gpt_ModeType Gpt_GetMode(void) {
    return Gpt_Drv.mode;
}
