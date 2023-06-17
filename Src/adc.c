/*
 * adc.c
 *
 *  Created on: Jan 7, 2023
 *      Author: irfan
 */

#include <stdint.h>
#include "macros.h"
#include "functions.h"
#include "shared.h"
#include "config.h"

void configure_clocks() {
	//Enable Clock for GPIO C
	RCC_AHB2ENR |= (1 << 2);
	//Enable Clock for ADC
	RCC_AHB2ENR |= (1 << 13);
	//Select ADC clock as System clock
	RCC_CCIPR1 |= (0b11 << 28);
	ADC1_CCR |= (11 << 18);
}

void configure_sqr_registers() {
	ADC1_SQR1 &= ~(0xF);
	ADC1_SQR1 |= (3UL << (0 * 6)); // 2 -> 3 conversions

	ADC1_SQR1 |= (1U << (1 * 6));
	ADC1_SQR1 |= (2U << (2 * 6));
	ADC1_SQR1 |= (3U << (3 * 6));
	ADC1_SQR1 |= (4U << (4 * 6));
}

void configure_PC0() {
	GPIOC->MODER |= (0b11 << (2 * 0));
	GPIOC->PUPDR &= ~(0b11 << (2 * 0));
}

void configure_PC1() {
	GPIOC->MODER |= (0b11 << (2 * 1));
	GPIOC->PUPDR &= ~(0b11 << (2 * 1));
}

void configure_PC2() {
	GPIOC->MODER |= (0b11 << (2 * 2));
	GPIOC->PUPDR &= ~(0b11 << (2 * 2));
}

void configure_PC3() {
	GPIOC->MODER |= (0b11 << (2 * 3));
	GPIOC->PUPDR &= ~(0b11 << (2 * 3));
}

void enable_adc() {
	ADC1_CR |= (1 << 0);
	while(!(ADC1_ISR & (1 << 0)));
}

void enable_adc_interrupts() {
	// Vector table "position" of ADC1_2 global interrupt is 37
	NVIC->ISER[1] |= (1 << 5);
	//Enable interrupt for end of regular conversion
	ADC1_IER |= (1 << 2);
}

void init_adc() {

	configure_clocks();

	ADC1_CR &= ~(1 << 29); //Disable Deep-power-down for ADC
	ADC1_CR |= (1 << 28); //Enable ADC Voltage regulator

	configure_PC0();
	configure_PC1();
	configure_PC2();
	configure_PC3();

	configure_sqr_registers();
	enable_adc();
	enable_adc_interrupts();

	//Start regular conversion of ADC
	ADC1_CR |= (1 << 2);
}


uint16_t scale_joystick_X(uint16_t adc_x) {

	if (adc_x > ADC_xy_MAX) {
		adc_x = SCALE_ADC_MAX;
	} else if (adc_x < ADC_x_middle_high && adc_x > ADC_x_middle_low) {
		adc_x = SCALE_ADC_MED;
	} else if (adc_x < ADC_xy_MIN) {
		adc_x = SCALE_ADC_MIN;
	}

	return SCALE_ADC_MAX - adc_x;
}

uint16_t scale_joystick_Y(uint16_t adc_y) {

	if (adc_y > ADC_xy_MAX) {
		adc_y = SCALE_ADC_MAX;
	} else if (adc_y < ADC_y_middle_high && adc_y > ADC_y_middle_low) {
		adc_y = SCALE_ADC_MED;
	} else if (adc_y < ADC_xy_MIN) {
		adc_y = SCALE_ADC_MIN;
	}

	return adc_y;
}

#define ADC_lr_MAX 3900




uint16_t scale_ldr_left(uint16_t adc_ldr_l) {

	if (adc_ldr_l > ADC_lr_MAX) {
		adc_ldr_l = SCALE_ADC_MAX;
	} else if (adc_ldr_l < ADC_l_middle_high && adc_ldr_l > ADC_l_middle_low) {
		adc_ldr_l = SCALE_ADC_MED;
	} else if (adc_ldr_l < ADC_l_MIN) {
		adc_ldr_l = SCALE_ADC_MIN;
	}

	return SCALE_ADC_MAX - adc_ldr_l;
}

uint16_t scale_ldr_right(uint16_t adc_ldr_r) {

	if (adc_ldr_r > ADC_lr_MAX) {
		adc_ldr_r = SCALE_ADC_MAX;
	} else if (adc_ldr_r < ADC_r_middle_high && adc_ldr_r > ADC_r_middle_low) {
		adc_ldr_r = SCALE_ADC_MED;
	} else if (adc_ldr_r < ADC_r_MIN) {
		adc_ldr_r = SCALE_ADC_MIN;
	}

	return SCALE_ADC_MAX - adc_ldr_r;
}

uint16_t joy_X = 0;
uint16_t joy_Y = 0;

uint16_t ldr_left = 0;
uint16_t ldr_right = 0;

uint16_t adc_counter = 0;
void ADC1_2_IRQHandler() {

	if ((ADC1_ISR & (1 << 2)) == (1 << 2)) {
		uint16_t adcVal = ADC1_DR;

		if (adc_counter == 0) {
			joy_X = scale_joystick_X(adcVal);
			JOY_X_VAL = joy_X;
			adc_counter++;
		} else if (adc_counter == 1) {
			joy_Y = scale_joystick_Y(adcVal);
			JOY_Y_VAL = joy_Y;
			adc_counter++;
		} else if (adc_counter == 2) {
			ldr_right = scale_ldr_right(adcVal);
			LDR1_VAL = ldr_right;
			adc_counter++;
		} else {
			ldr_left = scale_ldr_left(adcVal);
			LDR2_VAL = ldr_left;
			adc_counter = 0;
		}
	}

	ADC1_CR |= (1 << 2);
}

