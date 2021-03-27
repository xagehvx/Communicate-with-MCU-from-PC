#ifndef	BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

void delay(volatile unsigned int);


void BSP_system_init();
void BSP_UART_init(uint32_t);
void uart_tx(uint8_t);

uint8_t uart_rx();



void BSP_led_init();
void BSP_led_set();
void BSP_led_clear();
void BSP_led_toggle();


//Button related function
void BSP_button_init();
int BSP_button_read();



#endif
