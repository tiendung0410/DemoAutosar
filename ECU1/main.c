#include "OS.h"

#include "App_ButtonTask.h"
#include "App_CanTask.h"
#include "Dio.h"
#include "Can.h"
#include "IoHwAb.h"
#include "CanIf.h"

// Nếu có config/init riêng cho mỗi module, thêm vào đây
void System_Init(void)
{
    // MCAL Init
    Dio_Init(&Dio_Config);
    Can_Init(&Can_Config);

    // Ecu Abstraction/IoHwAb layer
    IoHwAb_Init();
    CanIf_Init();

    // Application layer Init
    App_ButtonTask_Init();
    App_CanTask_Init();
}

// Task cyclic 10ms: chạy các runnable mỗi 10ms
void CyclicTask10ms(void* arg)
{
    (void)arg;
    while (1) {
        App_ButtonTask_Run();
        App_CanTask_Run();
        Os_Sleep(10); // period 10ms
    }
}


int main(void)
{
    // Init hệ thống
    System_Init();

    // Khởi tạo các OS task
    OsTaskType cyclic10msTask;

    Os_CreateTask(&cyclic10msTask, CyclicTask10ms, NULL, 10, "Cyclic10msTask");

    // Start các task
    Os_StartTask(&cyclic10msTask);

    // Main idle loop
    while (1) {
        Os_Sleep(1000); // sleep lâu, chỉ giữ chương trình không thoát
    }
    return 0;
}
