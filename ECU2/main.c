#include "OS.h"
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

void System_Init(void)
{
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
}

// Task cyclic 10ms
void CyclicTask10ms(void* arg)
{
    printf("CyclicTask10ms running...\n");
    (void)arg;
    while (1) {
        Can_MainFunction_Read();

        App_Can_Run();    // Receive CAN data, forwarding struct
        App_Led_Run();    // Blink LED base on Button info

        Os_Sleep(100); // period 100ms
    }
}

int main(void)
{
  
    // Init system
    System_Init();

    // OS task init
    OsTaskType cyclic10msTask;

    Os_CreateTask(&cyclic10msTask, CyclicTask10ms, NULL, 10, "Cyclic10msTask");
    // Start  task
    Os_StartTask(&cyclic10msTask);
    printf("===== ECU2 Init Done =====\n");

    while (1) {
        Os_Sleep(1000); 
    }

    return 0;
}
