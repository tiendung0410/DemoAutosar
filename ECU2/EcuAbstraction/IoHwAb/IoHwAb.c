#include "IoHwAb.h"
#include "IoHwAb_Cfg.h"
#include "Dio.h"

void IoHwAb_Init(void) {
    //Reserve
}

Std_ReturnType IoHwAb_Led_Set(IoHwAb_LedIdType id, uint8 level) {
    if (id >= IOHWAB_NUM_LEDS) return E_NOT_OK;
    Dio_ChannelType ch = IoHwAb_LedConfig[id].DioChannelId;
    Dio_WriteChannel(ch, (level != 0) ? 1 : 0);
    return E_OK;
}
