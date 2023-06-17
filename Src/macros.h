/*
 * macros.h
 *
 *  Created on: Jan 7, 2023
 *      Author: dogukan
 */

#ifndef MACROS_H_
#define MACROS_H_

#define RCC 0x40021000
#define RCC_AHB2ENR *((volatile uint32_t *) (RCC + 0x4C))
#define RCC_APB1ENR *((volatile uint32_t *) (RCC + 0x58))
#define RCC_APB2ENR *((volatile uint32_t *) (RCC + 0x60))
#define RCC_CCIPR1 *((volatile uint32_t *) (RCC + 0x88))

#define GPIO_base 0x42020000
#define GPIOA			((GPIO_TypeDef *)	GPIO_base)
#define GPIOB 			((GPIO_TypeDef *)(GPIO_base + 0x400UL))
#define GPIOC 			((GPIO_TypeDef *)(GPIO_base + 0x800UL))
#define GPIOD 			((GPIO_TypeDef *)(GPIO_base + 3*0x400UL))
#define GPIOE 			((GPIO_TypeDef *) 0x42021000)

#define TIM2            ((TIM_TypeDef *)	0x40000000)
#define TIM3            ((TIM_TypeDef *)	0x40000400)
#define TIM4            ((TIM_TypeDef *)	0x40000800)
#define TIM5            ((TIM_TypeDef *)	0x40000C00)
#define TIM6            ((TIM_TypeDef *)	0x40001000)
#define TIM15            ((TIM_TypeDef *)	0x50014000)
#define TIM16            ((TIM_TypeDef *)	0x50014400)
#define TIM17            ((TIM_TypeDef *)	0x50014800)

#define ADC_BASE 0x42028000
#define ADC1_ISR *((volatile uint32_t *) (ADC_BASE + 0x00))
#define ADC1_IER *((volatile uint32_t *) (ADC_BASE + 0x04))
#define ADC1_CR *((volatile uint32_t *) (ADC_BASE + 0x08))
#define ADC1_CFGR *((volatile uint32_t *) (ADC_BASE + 0x0C))
#define ADC1_SQR1 *((volatile uint32_t *) (ADC_BASE + 0x30))
#define ADC1_DR *((volatile uint32_t *) (ADC_BASE + 0x40))
#define ADC1_CCR *((volatile uint32_t *) (ADC_BASE + 0x308))

#define SET(pin) |= 1<<(pin)
#define RESET(pin) &= ~(1<<(pin))
#define GET(value, pin) ((value >> pin) % 2)

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
	volatile uint32_t BRR;
	uint32_t RESERVED;
	volatile uint32_t SECCFGR;
} GPIO_TypeDef;

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR1;
	volatile uint32_t CCMR3;
	volatile uint32_t CCR5;
	volatile uint32_t CCR6;
	volatile uint32_t OR2;
	volatile uint32_t OR3;
} TIM_TypeDef;

typedef struct {
	volatile uint32_t ISER[16];
	volatile uint32_t ICER[16];
	volatile uint32_t ISPR[16];
	volatile uint32_t ICPR[16];
	volatile uint32_t IABR[16];
	volatile uint32_t ITNS[16];
	volatile uint32_t IPR[120];
	volatile uint32_t STIR;
} NVIC_TypeDef;

#define NVIC ((NVIC_TypeDef *) 0xE000E100)


#endif /* MACROS_H_ */
