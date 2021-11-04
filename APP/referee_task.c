//
// Created by WSJ on 2021/11/4.
//

#include <cmsis_os.h>
#include "referee_task.h"
#include "referee_data.h"
#include "fifo.h"
#include "protocol.h"
#include "bsp_usart.h"

extern fifo_s_t referee_fifo;
extern uint8_t referee_fifo_buf[REFEREE_FIFO_BUF_LENGTH];
extern unpack_data_t referee_unpack_obj;
extern uint8_t usart6_buf[2][USART_RX_BUF_LENGHT];

void referee_task(void *pvParameters) {
    init_referee_struct_data();
    fifo_s_init(&referee_fifo, referee_fifo_buf, REFEREE_FIFO_BUF_LENGTH);
    usart6_init(usart6_buf[0], usart6_buf[1], USART_RX_BUF_LENGHT);

    while (1) {
        referee_unpack_fifo_data();
        osDelay(10);
    }
}