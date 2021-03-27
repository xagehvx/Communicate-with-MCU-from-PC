#include "bsp.h"


void BSP_system_init(void){

	__disable_irq();

	init_timer2();

	__enable_irq();


}

void init_timer2(){

	RCC->IOPENR |= 7;
	RCC->APBENR1 |= (1U<< 1);// enable time3 module clock

	//setup PA6 as AF2
	GPIOA->MODER &= ~(3U << 2*6);
	GPIOA->MODER |= (2U << 2*6);

	// choose AF2 from mux
	GPIOA->AFR[0] &= ~(0xFU << 4*6);
	GPIOA->AFR[0] |= (1U << 4*6);

	TIM3->CR1=0;// zero out the control register just in case
	TIM3->CR1 |= (1U << 7);	// ARPE
	TIM3->CNT=0;// zero out counter

	/*10 Msecond interrupt	 */

	TIM3->PSC = 10;
	TIM3->ARR = 16000;

	TIM3->DIER |= (1 << 0);// update interrupt enable


	TIM3->CCMR1 |=(1 << 3); // output compare preload enable
	TIM3->CCMR1 &= ~(1U << 16); //0
	TIM3->CCMR1 &= ~(0xFU << 4);
	TIM3->CCMR1 |= (0x6U << 4); // mode 1 enable
	TIM3->CCER |= (1U << 0);
	TIM3->CCR1	= 1600; //duty cycle

	TIM3->CR1 |= (1 << 0);// 	tım2 enable

	NVIC_SetPriority(TIM3_IRQn,3);
	NVIC_EnableIRQ(TIM3_IRQn);

}


void TIM3_IRQHandler(void){


	TIM3->SR &= ~(1U << 0); //clear update status register

	//TIM2->SR &=  ~(1U<<1);
}
