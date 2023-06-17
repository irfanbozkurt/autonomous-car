#include <stdint.h>
#include "macros.h"
#include "shared.h"
#include "functions.h"
#include "config.h"

uint16_t LDR1_VAL = ADC_l_middle_high;
uint16_t LDR2_VAL = ADC_l_middle_high;
uint16_t JOY_X_VAL = XY_MID;
uint16_t JOY_Y_VAL = XY_MID;
uint16_t JOY_BTN_VAL = 0;
uint16_t DIST = 0;

void init_GPIO_PIN(
	GPIO_TypeDef *gpio,
	uint32_t rcc_bit,
	uint32_t pin_number,
	uint8_t mode
) {
	RCC_AHB2ENR |= (1<<rcc_bit);
	gpio->MODER &= ~(0x03 << (pin_number * 2));
	gpio->MODER |= (mode << (pin_number * 2));
}


void init_joystick_button() {
	init_GPIO_PIN(GPIOA, 0, 8, 0);
	GPIOA->PUPDR |= 1 << 16;
}

// states: 0 = manual, moving with joystick
//         1 = manual, stopped after obstacle detected
//         2 = auto, started
// 	       3 = auto, moving
//         4 = auto, stopped after obstacle detected
uint8_t obstacle_count = 0;
void transition_to_manual() {
	obstacle_count = 0;
	GPIOA->ODR RESET(9);
	GPIOB->ODR RESET(7);
	GPIOC->ODR SET(7);
}
uint8_t manual(uint16_t ldr1_val, uint16_t ldr2_val, uint16_t joy_x_val, uint16_t joy_y_val, uint16_t joy_btn_val, uint16_t user_btn_val, uint16_t dist) {
	if (user_btn_val) {
		return 2;
	}
	if ((dist > 0 && dist < 10) || joy_btn_val) {
		return 1;
	}
	dir_speed_control(joy_x_val, joy_y_val);
	return 0;
}
void transition_to_stopped() {
	break_motor();
	GPIOA->ODR RESET(9);
	GPIOB->ODR SET(7);
	GPIOC->ODR RESET(7);
}
uint8_t manual_stopped(uint16_t ldr1_val, uint16_t ldr2_val, uint16_t joy_x_val, uint16_t joy_y_val, uint16_t joy_btn_val, uint16_t user_btn_val, uint16_t dist) {
	if (user_btn_val) {
		return 2;
	}
	if (joy_x_val - 2000 > 400 || joy_x_val - 2000 < -400
		|| joy_y_val - 2000 > 400 || joy_y_val - 2000 < -400) {
		return 0;
	}
	return 1;

}

void transition_to_auto_started() {
	dir_speed_control(XY_MID, XY_MID);
	GPIOA->ODR RESET(9);
	GPIOB->ODR SET(7);
	GPIOC->ODR SET(7);
}
uint8_t auto_started(uint16_t ldr1_val, uint16_t ldr2_val, uint16_t joy_x_val, uint16_t joy_y_val, uint16_t joy_btn_val, uint16_t user_btn_val, uint16_t dist) {
	if (user_btn_val) {
		return 0;
	}
	if(joy_x_val > 3000) {
		return 3;
	}
	return 2;
}

void transition_to_auto() {	
	front_leds_on();
	GPIOA->ODR SET(9);
	GPIOB->ODR RESET(7);
	GPIOC->ODR RESET(7);
}
uint8_t auto_moving(uint16_t ldr1_val, uint16_t ldr2_val, uint16_t joy_x_val, uint16_t joy_y_val, uint16_t joy_btn_val, uint16_t user_btn_val, uint16_t dist) {
	if (user_btn_val) {
		return 0;
	}
	if (dist < 10) {
		return 4;
	}
	if(joy_btn_val) {
		return 2;
	}
	processLDR(ldr1_val, ldr2_val);
	return 3;
	// call the function that moves the robot
}
void transition_to_auto_stopped() {
	break_motor();
	all_leds_off();
	GPIOA->ODR SET(9);
	GPIOB->ODR SET(7);
	GPIOC->ODR SET(7);
}
uint8_t auto_stopped(uint16_t ldr1_val, uint16_t ldr2_val, uint16_t joy_x_val, uint16_t joy_y_val, uint16_t joy_btn_val, uint16_t user_btn_val, uint16_t dist) {
	if (user_btn_val) {
		return 0;
	}
	if (dist > 10) {
		return 2;
	}
	return 3;
}


void loop(){
	static uint8_t state = 0;
	static uint16_t push_button_high = 0;
	uint16_t ldr1_val = LDR1_VAL, ldr2_val = LDR2_VAL,
 			joy_x_val = JOY_X_VAL, joy_y_val = JOY_Y_VAL,
			joy_btn_val = !GET(GPIOA->IDR, 8), dist = DIST,
			push_btn_val = GET(GPIOC->IDR, 13);
	uint16_t push_btn_pressed = push_btn_val && !push_button_high;
	push_button_high = push_btn_val;
	uint8_t new_state = 0;
	switch (state)
	{
		case 0:
			new_state = manual(ldr1_val, ldr2_val, joy_x_val, joy_y_val, joy_btn_val, push_btn_pressed, dist);
			break;
		case 1:
			new_state = manual_stopped(ldr1_val, ldr2_val, joy_x_val, joy_y_val, joy_btn_val, push_btn_pressed, dist);
			break;
		case 2:
			new_state = auto_started(ldr1_val, ldr2_val, joy_x_val, joy_y_val, joy_btn_val, push_btn_pressed, dist);
			break;
		case 3:
			new_state = auto_moving(ldr1_val, ldr2_val, joy_x_val, joy_y_val, joy_btn_val, push_btn_pressed, dist);
			break;
		case 4:
			new_state = auto_stopped(ldr1_val, ldr2_val, joy_x_val, joy_y_val, joy_btn_val, push_btn_pressed, dist);
			break;
		default:
			break;
	}
	if (new_state == state)
	    return;
	switch (new_state)
	{
		case 0:
			transition_to_manual();
			break;
		case 1:
			transition_to_stopped();
			break;
		case 2:
			transition_to_auto_started();
			break;
		case 3:
			transition_to_auto();
			break;
		case 4:
			transition_to_auto_stopped();
			break;
		default:
			break;
	}
	state = new_state;		
}

void init_user_button() {
	init_GPIO_PIN(GPIOC, 2, 13, 0);
}

void init_user_leds() {
	init_GPIO_PIN(GPIOA, 0, 9, 1);
	init_GPIO_PIN(GPIOB, 1, 7, 1);
	init_GPIO_PIN(GPIOC, 2, 7, 1);
}

int main(void) {

	init_adc();

	init_motor_controller();

	init_ultrasonic_sensor();

	init_user_leds();

	init_user_button();

	init_joystick_button(); //nedense inputu algılamıyor.

	__asm volatile(
		 "mov r0, #0 \n\t"
		 "msr primask, r0 \n\t"
	);
	transition_to_manual();
	while(1) loop();
}
