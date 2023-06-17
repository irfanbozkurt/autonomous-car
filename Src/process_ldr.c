/*
 * process_ldr.c
 *
 *  Created on: Jan 13, 2023
 *      Author: irfan
 */

#include <stdint.h>
#include "macros.h"
#include "config.h"
#include "functions.h"


void processLDR(uint16_t l, uint16_t r) {

	//dir_speed_control(l, r);
	uint16_t x = 2000;
 	uint16_t y = 2000;
// 	int diff = l-r;
// 	if(diff < LDR_MEDIUM_RANGE || diff > -LDR_MEDIUM_RANGE){
// //		r = SCALE_ADC_MED;
// //		l = SCALE_ADC_MED;
// 	}
	if(r > l){
		y -= (r-l)*100;
	}
	else if(l > r){
		x -= (l-r)*100;
	}
	motor_l(1, 0, x);
	motor_r(1, 0, y);
}
