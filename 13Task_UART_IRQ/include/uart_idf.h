#ifndef UART_IDF_H_
#define UART_IDF_H_

#include "driver/uart.h"

#define EX_UART_NUM UART_NUM_0
#define BUF_SIZE 70

#define LED1  2

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

#endif // UART_IDF_H_