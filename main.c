#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Can.h"

int main(void)
{
    printf("Init CAN MCAL...\n");
    Can_Init(&Can_Config);

    // Chuẩn bị dữ liệu gửi: "hello" (5 byte)
    uint8 data[8] = {0}; // classic CAN: tối đa 8 bytes
    strcpy((char*)data, "hello");

    Can_PduType pdu;
    pdu.id = 0x123;         // CAN ID mà STM32 sẽ nhận
    pdu.length = 5;         // Độ dài data gửi (5 byte)
    pdu.sdu = data;         // Trỏ vào buffer chứa "hello"
    pdu.swPduHandle = 0;    // Demo, không cần dùng

    printf("Sending CAN frame: ID=0x%03X Data=\"%s\"\n", pdu.id, data);

    Std_ReturnType ret = Can_Write(0, &pdu);  // Hth=0 nếu chỉ có 1 controller/mailbox

    if (ret == CAN_OK) {
        printf("CAN frame sent OK!\n");
    } else if (ret == CAN_BUSY) {
        printf("CAN TX mailbox busy!\n");
    } else {
        printf("CAN frame send failed!\n");
    }

    // Nếu muốn gửi liên tục:
    
    while(1) {
        Std_ReturnType ret = Can_Write(0, &pdu);  // Hth=0 nếu chỉ có 1 controller/mailbox

        if (ret == CAN_OK) {
            printf("CAN frame sent OK!\n");
        } else if (ret == CAN_BUSY) {
            printf("CAN TX mailbox busy!\n");
        } else {
            printf("CAN frame send failed!\n");
        }
        usleep(1000000); // 100ms giữa các lần gửi
    }
    

    Can_DeInit();
    return 0;
}
