#include "bsp.h"



void delay(volatile unsigned int s) {
    for(; s>0; s--);
}

void BSP_system_init(void){

	__disable_irq();
	BSP_led_init();
	BSP_button_init();
	BSP_UART_init(9600);
	__enable_irq();


}



void uart_tx(uint8_t c){

	USART2->TDR = (uint16_t)c;

	while (!(USART2->ISR & (1 << 6)));

}

uint8_t uart_rx(void){

	while (!(USART2->ISR & (1 << 5))); //RXFNE bit of ISR, receive data ready to read
	return (uint8_t)USART2->RDR;
	//RXNE is automatically cleared when read
}


/*
 * PA2 PA3 are connected to the Virtual COM port
 * USART2 module
 */


void BSP_UART_init(uint32_t baud ){

	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 17);

	//setup PA2 as AF1
    GPIOA->MODER &= ~(3U << 2*2);
	GPIOA->MODER |= (2U << 2*2);
	// choose AF1 from mux
	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1U << 4*2);


	//setup PA3 as AF1
	GPIOA->MODER &= ~(3U << 2*3);
	GPIOA->MODER |= (2U << 2*3);
	// choose AF1 from mux
	GPIOA->AFR[0] &= ~(0xFU << 4*3);
	GPIOA->AFR[0] |= (1U << 4*3);

	//setup uart2
	//reset uart cr1
	USART2->CR1 = 0;
	USART2->CR1 |= (1 << 3);  // TE
	USART2->CR1 |= (1 << 2);  // RE
	USART2->CR1 |= (1 << 5);  // RXNEIE

	USART2->BRR = (uint16_t)(SystemCoreClock / baud);


	USART2->CR1 |= (1 << 0);  // UE

}


void BSP_led_init(void){


    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);


    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /*Clear led*/
    GPIOC->BRR |= (1U << 6);

}
void BSP_led_set(void){

	/*trun-on led*/
    GPIOC->ODR |= (1U << 6);
}
void BSP_led_clear(void){

    /*Clear led*/
    GPIOC->BRR |= (1U << 6);
}
void BSP_led_toggle(void){

    /* Toggle LED */
    GPIOC->ODR ^= (1U << 6);
}

void BSP_button_init(void){
	 /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);

	    /* Setup PA1 as input */
	    GPIOA->MODER &= ~(3U << 2*1);
	    GPIOA->PUPDR |= (2U << 2*1); // Pull-down mode

	    /*setup interrrupts for inputs*/
	    EXTI->EXTICR[0] |=(0U << 8*1);//PA1	 EXTI1 mux ta PA1 için EXTICR0'ın 9.biti 0 yapıldı

	    /* MASK*/
	    EXTI->IMR1 |= (1U << 1);

	    /*rising edge*/
	    EXTI->RTSR1 |= (1U << 1);

	    /*NVIC*/
	    NVIC_SetPriority(EXTI0_1_IRQn,0);	// buton interruptı PA1 için EXTI1 in içerisinde olduğundan EXTI0_1_IRQn kullanıldı.
	    NVIC_EnableIRQ(EXTI0_1_IRQn);	//buton interrupt ı nvic içerisinde enable edildi
}
// returns 1 if button is pressed
int BSP_button_read(void){
	int b = ((GPIOA->IDR >> 1) & 0x0001);

	if (b) return 1;
	else return 0;
}






