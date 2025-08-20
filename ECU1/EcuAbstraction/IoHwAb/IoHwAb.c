#include "IoHwAb.h"
#include "IoHwAb_Cfg.h"
#include "Dio.h"

void IoHwAb_Init(void) {
    //Reserve space for button configuration
    
}

Std_ReturnType IoHwAb_Button_Read(IoHwAb_ButtonIdType id, uint8* level) {
    if (id >= IOHWAB_NUM_BUTTONS || level == NULL) {
        return E_NOT_OK;
    }

    Dio_ChannelType ch = IoHwAb_ButtonConfig[id].DioChannelId;
    *level = Dio_ReadChannel(ch);
    printf("level=%d\n", *level);
    return E_OK;
}
