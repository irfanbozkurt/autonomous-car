/*
 * motor.c
 *
 *  Created on: Jan 13, 2023
 *      Author: irfan
 */
#include <stdint.h>
#include "macros.h"
#include "functions.h"

void init_motor_input_GPIO() {
	init_GPIO_PIN(GPIOB, 1, 0, 1);
	init_GPIO_PIN(GPIOB, 1, 1, 1);
	init_GPIO_PIN(GPIOB, 1, 2, 1);
	init_GPIO_PIN(GPIOB, 1, 3, 1);
}

void init_motor_controller() {
	init_LEDs();
	init_motor_input_GPIO();
	init_right_pwm();
	init_left_pwm();
}


void motor_l_speed(uint16_t val) {
	if (val == 2000)
		val -= 5;
	else if (val == 0)
		val += 5;
	set_pulse_width(TIM3, val);
}

void motor_l(uint8_t forward, uint8_t backward, uint16_t speed) {

	motor_l_speed(speed);

	if (backward == 0) {
		GPIOB->ODR &= ~(1 << 0);
	} else {
		GPIOB->ODR |= (1 << 0);
	}

	if (forward == 0) {
		GPIOB->ODR &= ~(1 << 1);
	} else {
		GPIOB->ODR |= (1 << 1);
	}
}

void motor_r_speed(uint16_t val) {
	if (val == 2000)
		val -= 5;
	else if (val == 0)
		val += 5;
	set_pulse_width(TIM4, val);
}

void motor_r(uint8_t forward, uint8_t backward, uint16_t speed) {

	motor_r_speed(speed);

	if (backward == 0) {
		GPIOB->ODR &= ~(1 << 2);
	} else {
		GPIOB->ODR |= (1 << 2);
	}

	if (forward == 0) {
		GPIOB->ODR &= ~(1 << 3);
	} else {
		GPIOB->ODR |= (1 << 3);
	}
}


