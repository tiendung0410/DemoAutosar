#include "App_CanTask.h"
#include "Rte_Can.h"
#include <stdio.h>

void App_Can_Init(void) {
    // Nếu cần khởi tạo biến hoặc state nội bộ, thêm ở đây
    printf("App_CAN init OK\n");
}

void App_Can_Run(void) {
    ButtonStatusInfoType buttonInfo;

    // Đọc dữ liệu đã được gửi lên CAN từ ECU1
    if (Rte_Read_SigButtonStatusInfo(&buttonInfo) == E_OK) {
        printf("[App_CAN] Received: ButtonState=%d, ButtonCount=%d\n",
               buttonInfo.ButtonState, buttonInfo.ButtonCount);

        // Gửi struct này sang cho AppLed (qua RTE)
        Rte_Write_PP_ButtonStatusInfo_ButtonStatus(&buttonInfo);
    }
}
