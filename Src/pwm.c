#include <stdint.h>
#include "macros.h"
#include "functions.h"

void init_TIM_pwm(
	TIM_TypeDef *tim,
	uint32_t iser_location,
	uint32_t rcc_bit
) {
	NVIC->ISER[iser_location/32] |= 1 << (iser_location%32);
	RCC_APB1ENR |= 1 << rcc_bit;
	tim->CR1 &= ~1;

	tim->CCMR1 |= 3<<5;
	tim->CCER |= 1;
	tim->CCR1 |= 60;

	tim->PSC = 20-1;
	tim->ARR = 2000-1;

	tim->SR=0;
	tim->DIER |= 2;

	tim->CR1 &= ~(1<<1);
	tim->CR1 |= 1;
}

void init_right_pwm() {
	init_GPIO_PIN(GPIOB, 1, 6, 2);
	GPIOB->AFR[0] |= 2 << (6*4);
	init_TIM_pwm(TIM4, 47, 2);
}

void init_left_pwm() {
	init_GPIO_PIN(GPIOB, 1, 4, 2);
	GPIOB->AFR[0] |= 2 << (4*4);
	init_TIM_pwm(TIM3, 46, 1);
}

void set_pulse_width(TIM_TypeDef *tim, uint32_t pulse_width) {
	tim->CCR1 = pulse_width;
}


void TIM4_IRQHandler(void)
{
	TIM4->SR=0;
}
void TIM3_IRQHandler(void)
{
	TIM3->SR=0;
}
