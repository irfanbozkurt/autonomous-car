#include <stdint.h>
#include "shared.h"
#include "macros.h"




void LED_front_l(uint8_t val) {
	if (val == 0) {
		GPIOA->ODR &= ~(1 << 2);
	} else {
		GPIOA->ODR |= (1 << 2);
	}
}

void LED_front_r(uint8_t val) {
	if (val == 0) {
		GPIOA->ODR &= ~(1 << 3);
	} else {
		GPIOA->ODR |= (1 << 3);
	}
}

void LED_rear_l(uint8_t val) {
	if (val == 0) {
		GPIOA->ODR &= ~(1 << 4);
	} else {
		GPIOA->ODR |= (1 << 4);
	}
}

void LED_rear_r(uint8_t val) {
	if (val == 0) {
		GPIOA->ODR &= ~(1 << 5);
	} else {
		GPIOA->ODR |= (1 << 5);
	}
}

int leds_on = 0, led_direction = 0, blinking = 0;

void process_leds() {
    if (leds_on){
        LED_front_l(0);
        LED_front_r(0);
        LED_rear_l(0);
        LED_rear_r(0);
        leds_on = 0;
    }
    else {
        if (led_direction) {
            LED_front_l(1);
            LED_rear_l(1);
        }
        else {
            LED_front_r(1);
            LED_rear_r(1);
        }
        leds_on = 1;
    }
}

void init_TIM17(
	TIM_TypeDef *tim,
	uint32_t arr_value,
	uint32_t iser_location,
	uint32_t rcc_bit
) {
	NVIC->ISER[iser_location/32] |= 1 << (iser_location%32);
	RCC_APB2ENR |= 1 << rcc_bit;
	tim->CR1 &= ~1;
	tim->PSC = 3999;
	tim->ARR = arr_value;
	tim->SR=0;
	tim->DIER |= 2;
	tim->CR1 &= ~(1<<1);
	tim->CR1 |= 1;
}

void TIM17_IRQHandler() {
    process_leds();
    TIM17->SR = 0;
}

init_blinking() {
    init_TIM17(TIM17, 500, 71, 18);
}

void start_blinking() {
    TIM17->CR1 |= 1;
}

void stop_blinking() {
    blinking = 0;
    TIM17->CR1 &= ~1;
}

void front_leds_on() {
    stop_blinking();
    LED_front_l(1);
    LED_front_r(1);
    LED_rear_l(0);
    LED_rear_r(0);
}

void rear_leds_on() {
    stop_blinking();
    LED_front_l(0);
    LED_front_r(0);
    LED_rear_l(1);
    LED_rear_r(1);
}

void all_leds_off() {
    stop_blinking();
    LED_front_l(0);
    LED_front_r(0);
    LED_rear_l(0);
    LED_rear_r(0);
}

void blink_right_leds() {
    if (blinking && led_direction==0) return;
    blinking = 1;
    LED_front_r(1);
    LED_rear_r(1);
    LED_front_l(0);
    LED_rear_l(0);
    leds_on = 1;
    led_direction = 0;
    start_blinking();
}

void blink_left_leds() {
    if (blinking && led_direction) return;
    blinking = 1;
    LED_front_r(0);
    LED_rear_r(0);
    LED_front_l(1);
    LED_rear_l(1);
    leds_on = 1;
    led_direction = 1;
    start_blinking();
}

void init_LEDs(){
    init_blinking();
	init_GPIO_PIN(GPIOA, 0, 2, 1);
	init_GPIO_PIN(GPIOA, 0, 3, 1);
	init_GPIO_PIN(GPIOA, 0, 4, 1);
	init_GPIO_PIN(GPIOA, 0, 5, 1);
}