#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
void init_TIM_pwm(TIM_TypeDef *tim, uint32_t iser_location, uint32_t rcc_bit);
void set_pulse_width(TIM_TypeDef *tim,uint32_t pulse_width);
void init_left_pwm();
void init_right_pwm();

void init_GPIO_PIN(
	GPIO_TypeDef *gpio,
	uint32_t rcc_bit,
	uint32_t pin_number,
	uint8_t mode
);

void init_LEDs();
void front_leds_on();
void rear_leds_on();
void blink_left_leds();
void blink_right_leds();
void all_leds_off();

void init_motor_controller();
void dir_speed_control(uint16_t x, uint16_t y) ;
void break_motor();

void process_joystick(uint16_t x, uint16_t y);
void processLDR(uint16_t l, uint16_t r);


void init_adc();
void init_ultrasonic_sensor();
#endif /* FUNCTIONS_H_ */
