#include "stm32l476xx.h"
#include "SysClock.h"
#include "Tim.h"

void gpio_initialise(){
	

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;		// enable clock for A group of GPIO
	GPIOA->MODER &= ~3 ; 										// clear out bits 0 and 1 for PA0
																					// PA0 is now in input mode
	GPIOA->MODER |= 2;

	
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_0 ;
	
	GPIOA->BSRR |= GPIO_BSRR_BR1;
	//GPIOA->BSRR |= GPIO_BSRR_BS0;
}
void timer_initialize(){
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->CCR1 |= TIM_CCR1_CCR1;
	
	TIM2->PSC |= 79;
	TIM2->EGR |=TIM_EGR_UG;
	//TIM2->EGR |= TIM_EGR_CC1G;
	
	TIM2->CCER &= ~(0x01);
	TIM2->CCMR1 = 0x01;
	
	
	TIM2->CCMR1 &= ~(0x70);
	
	//TIM2->CCMR1 |= TIM_CCMR1_IC1F_1;
	//TIM2->CCMR1 |= TIM_CCMR1_IC1F_0;
	
	
	TIM2->CCER |= 0x01;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	//TIM2->CCMR1 |= (uint32_t)0x00000000;
	//TIM2->CCER |= TIM_CCER_CC1E;
	
	

}

int read_pa0( void )
{
	return GPIOA->IDR & 1 ;	
}	




