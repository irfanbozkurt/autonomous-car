/*
 * process_joystick.c
 *
 *  Created on: Jan 13, 2023
 *      Author: irfan
 */

#include <stdint.h>
#include "macros.h"
#include "config.h"
#include "functions.h"

double joystick_ALPHA = 1;

void dir_speed_control(uint16_t x, uint16_t y) {

	if (x >= XY_MID && y >= XY_MID) {

		x -= XY_MID;
		y -= XY_MID;

		if (y==0) y++;
		double proportion = (double)x/y;

		if (proportion >= joystick_ALPHA) {
			motor_l(1, 0, x);
			motor_r(1, 0, x-y);
			front_leds_on();
		} else {
			motor_l(1, 0, y);
			motor_r(0, 1, y-x);
			blink_right_leds();
		}
	} else if (x < XY_MID && y >= XY_MID) {

		x = XY_MID - x;
		y -= XY_MID;

		if (y==0) y++;
		double proportion = (double)x/y;

		if (proportion >= joystick_ALPHA) {
			motor_l(0, 1, x);
			motor_r(0, 1, x-y);
			rear_leds_on();
		} else {
			motor_l(0, 1, y);
			motor_r(1, 0, y-x);
			blink_left_leds();	
		}
	} else if (x < XY_MID && y < XY_MID) {

		x = XY_MID - x;
		y = XY_MID - y;

		if (y==0) y++;
		double proportion = (double)x/y;

		if (proportion >= joystick_ALPHA) {
			motor_r(0, 1, x);
			motor_l(0, 1, x-y);
			rear_leds_on();
		} else {
			motor_r(0, 1, y);
			motor_l(1, 0, y-x);
			blink_right_leds();
		}
	} else if (x >= XY_MID && y < XY_MID) {

		x -= XY_MID;
		y = XY_MID - y;

		if (y==0) y++;
		double proportion = (double)x/y;

		if (proportion >= joystick_ALPHA) {
			motor_r(1, 0, x);
			motor_l(1, 0, x-y);
			front_leds_on();
		} else {
			motor_r(1, 0, y);
			motor_l(0, 1, y-x);
			blink_left_leds();
		}
	}
	if(x < 10 && y < 10)
		all_leds_off();
}

void break_motor() {
	motor_l(1, 1, 2000);
	motor_r(1, 1, 2000);
	all_leds_off();
}