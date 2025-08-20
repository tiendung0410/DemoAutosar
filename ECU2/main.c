#include <stdio.h>
#include <unistd.h>  // usleep
#include "Can.h"
#include "CanIf.h"
#include "PduR.h"
#include "Com.h"
#include "Dio.h"
#include "IoHwAb.h"
#include "Gpt.h"
#include "EcuAb_Gpt.h"
#include "App_CanTask.h"
#include "App_LedTask.h"

int main(void)
{
    printf("===== ECU2 Startup =====\n");

    // MCAL & BSW INIT
    Dio_Init(&Dio_Config);        // GPIO
    Can_Init(&Can_Config);        // CAN Driver
    Gpt_Init(&Gpt_Config);        // GPT Driver

    // ECU Abstraction & Service Layer
    IoHwAb_Init();                // IO Abstraction (LED)
    EcuAb_Gpt_Init();             // GPT Abstraction
    CanIf_Init();                 // CAN Interface
    PduR_Init();                  // PDU Router
    Com_Init();                   // Communication Layer

    // Application Layer Init
    App_Can_Init();
    App_Led_Init();

    printf("===== ECU2 Init Done. Entering Main Loop =====\n");

    // Main loop giả lập scheduler, mỗi 10ms
    while (1) {
        // Poll BSW nếu cần (ví dụ nếu dùng SocketCAN hoặc không interrupt)
        Can_MainFunction_Read();
        Can_MainFunction_Write();
        // Gpt có thể cần poll nếu không dùng hardware interrupt
        // Gpt_MainFunction();

        // Gọi App task định kỳ
        App_Can_Run();    // Nhận data từ CAN, forwarding struct
        App_Led_Run();    // Nháy LED theo ButtonCount

        // Tùy hệ điều hành, sleep ngắn để giảm CPU (ở đây 10ms)
        usleep(10 * 1000);
    }

    return 0;
}
