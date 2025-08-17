#include "App_LedTask.h"
#include "Rte_Led.h"
#include "IoHwAb.h"
#include <unistd.h> // usleep
#include <stdio.h>

static uint8 ledState = 0;
static uint32 tick = 0;
static uint32 togglePeriodMs = 500; // default = 2Hz

void App_LedTask_Init(void)
{
    IoHwAb_Led_Set(IOHWAB_LED_STATUS, 0); // Đảm bảo LED tắt lúc khởi động
    ledState = 0;
    tick = 0;
    togglePeriodMs = 500;
}

void App_LedTask_Run(void)
{
    uint8 system = 0, freqCount = 0;
    Rte_Read_RP_ButtonStateInfo_ButtonState(&system);
    Rte_Read_RP_ButtonCountInfo_ButtonCount(&freqCount);

    if (!system) {
        // Nếu hệ thống OFF thì tắt LED, reset state
        IoHwAb_Led_Set(IOHWAB_LED_STATUS, 0);
        ledState = 0;
        tick = 0;
        return;
    }

    // Tính tần số nháy: Freq = 2 * (2^freqCount)
    uint32 freq = 2 << freqCount;       // 2, 4, 8, 16 Hz...
    togglePeriodMs = 500 / freq;        // Chu kỳ đổi trạng thái (ms)

    tick += 100; // Giả định mỗi 100ms gọi 1 lần
    if (tick >= togglePeriodMs) {
        tick = 0;
        ledState ^= 1;
        IoHwAb_Led_Set(IOHWAB_LED_STATUS, ledState);
        printf("LED toggled: %d Hz, ledState=%d\n", freq, ledState);
    }

    // Nếu task cycle không cố định, nên dùng timer hoặc sleep phù hợp!
    usleep(100 * 1000);
}
