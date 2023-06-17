/*
 * us.c
 *
 *  Created on: Jan 13, 2023
 *      Author: irfan
 */

#include <stdint.h>
#include "macros.h"
#include "shared.h"
#include "functions.h"

void init_trigger_timer(
	TIM_TypeDef *tim,
	uint32_t arr_value,
	uint32_t iser_location,
	uint32_t rcc_bit
) {
	NVIC->ISER[iser_location/32] |= 1 << (iser_location%32);
	RCC_APB1ENR |= 1 << rcc_bit;
	tim->PSC = 3;
	tim->CR1 &= ~(1<<1);
	tim->ARR = arr_value;
	tim->SR=0;
	tim->DIER |= 1;
}


void init_TIM_echo(
	TIM_TypeDef *tim,
	uint32_t arr_value,
	uint32_t iser_location,
	uint32_t rcc_bit
) {
	NVIC->ISER[iser_location/32] |= 1 << (iser_location%32);
	RCC_APB2ENR |= 1 << rcc_bit;
	tim->PSC = 3;

	tim->CCMR1 |= 1;
	tim->CCER |= 8;
	tim->CCER |= 2;
	tim->CCER |= 1;

	tim->DIER |= 2;
	tim->CR1 |= 1;
}


void init_TIM15(
	TIM_TypeDef *tim,
	uint32_t arr_value,
	uint32_t iser_location,
	uint32_t rcc_bit
) {
	NVIC->ISER[iser_location/32] |= 1 << (iser_location%32);
	RCC_APB2ENR |= 1 << rcc_bit;
	tim->CR1 &= ~1;
	tim->PSC = 3999;
	tim->ARR = 50;
	tim->SR=0;
	tim->DIER |= 2;
	tim->CR1 &= ~(1<<1);
	tim->CR1 |= 1;
}

void init_echo(){
	init_GPIO_PIN(GPIOA, 0, 6, 2);
	GPIOA->AFR[0] |= 14 << (6*4);
	init_TIM_echo(TIM16, 200, 70, 17);
}

void init_trigger(){
	init_GPIO_PIN(GPIOA, 0, 1, 1);
	init_trigger_timer(TIM5, 200, 48, 3);
}

void start_trigger(TIM_TypeDef *tim, uint32_t arr_value){
	GPIOA->ODR |= (1<<1);
	tim->SR=0;
	tim->CNT=0;
	tim->ARR = arr_value;
	tim->CR1 |= 1;
}

void TIM15_IRQHandler(void)
{
	start_trigger(TIM5, 11);
	TIM15->SR=0;
}

void TIM5_IRQHandler(void)
{
	GPIOA->ODR &= ~(1<<1);
	TIM5->CR1 &= ~1;
	TIM5->SR=0;
}

void TIM16_IRQHandler(void)
{
	static int high = 1;
	static uint16_t first_time = 0;
	static uint16_t second_time = 0;
	if(!(TIM16->SR & 2)){
		TIM16->SR=0;
		return;
	}
	TIM16->SR=0;
	if(high){
		first_time = TIM16->CCR1;
		high = 0;
	} else {
		second_time = TIM16->CCR1;
		uint16_t dist = (second_time - first_time) / 58;
		DIST = dist;
		high = 1;
	}
}


void init_ultrasonic_sensor() {
	init_echo();
	init_trigger();
	init_TIM15(TIM15, 50, 69, 16);
}


