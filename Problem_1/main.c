/*
 * main.c
 *
 * author: Mehmet Akif GÜMÜŞ

 */

#include "bsp.h"

#define DELAY    16000U



int main(void) {

	BSP_system_init();

	while(1){

		uart_tx(uart_rx());

}
    return 0;
}






