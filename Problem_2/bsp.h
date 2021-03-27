#ifndef	BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

void delay(volatile unsigned int);

void BSP_system_init();


void init_timer2();

#endif
