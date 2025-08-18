#ifndef RTE_GPT_H
#define RTE_GPT_H

#include <stdint.h>

/* Chọn kênh logic mặc định (map ở ECUA) */
#ifndef RTE_GPT_LED_LOGIC_CHANNEL
#define RTE_GPT_LED_LOGIC_CHANNEL   0u   /* ví dụ: periodic / blink scheduler */
#endif

#ifndef RTE_GPT_DELAY_LOGIC_CHANNEL
#define RTE_GPT_DELAY_LOGIC_CHANNEL 1u   /* ví dụ: ONESHOT, dùng cho delay */
#endif

/* ==== Khởi tạo / Hủy ==== */
void Rte_Gpt_Init(void);     /* gọi ECUA init bên dưới nếu bạn muốn gói chung */
void Rte_Gpt_DeInit(void);

/* ==== Start/Stop theo ticks hoặc theo thời gian ==== */
void Rte_Call_RP_Gpt_StartTicks(uint8_t logicCh, uint32_t ticks);
void Rte_Call_RP_Gpt_StartMs   (uint8_t logicCh, uint32_t ms);
void Rte_Call_RP_Gpt_StartUs   (uint8_t logicCh, uint32_t us);
void Rte_Call_RP_Gpt_Stop      (uint8_t logicCh);

/* ==== Query wrapper (bạn yêu cầu) ==== */
uint32_t Rte_Call_RP_Gpt_GetElapsed  (uint8_t logicCh);
uint32_t Rte_Call_RP_Gpt_GetRemaining(uint8_t logicCh);



#endif /* RTE_GPT_H */
