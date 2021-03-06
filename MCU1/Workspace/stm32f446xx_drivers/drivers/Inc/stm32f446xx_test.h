/*
 * stm32f446xx_test.h
 *
 *  Created on: 06-Feb-2021
 *      Author: DELL
 */

/*
 * Include section
 */
#include <stdint.h>

#ifndef INC_STM32F446XX_TEST_H_
#define INC_STM32F446XX_TEST_H_

#define FLASH_BASE_ADDR								0x08000000
#define SRAM1_BASE_ADDR								0x20000000
#define SRAM_BASE_ADDR								SRAM1_BASE_ADDR

#define ROM_BASE_ADDR								0x1FFF0000

#define	APB1_BASE_ADDR								0x40000000
#define	APB2_BASE_ADDR								0x40010000
#define AHB1_BASE_ADDR								0x40020000
#define AHB2_BASE_ADDR								0x50000000

/*
 * Base addresses of peripherals under AHB1 bus
 */

#define GPIOA_BASE_ADDR								(AHB1_BASE_ADDR + 0x0000)
#define GPIOB_BASE_ADDR								(AHB1_BASE_ADDR + 0x0400)
#define GPIOC_BASE_ADDR								(AHB1_BASE_ADDR + 0x0800)
#define GPIOD_BASE_ADDR								(AHB1_BASE_ADDR + 0x0C00)
#define GPIOE_BASE_ADDR								(AHB1_BASE_ADDR + 0x1000)
#define GPIOF_BASE_ADDR								(AHB1_BASE_ADDR + 0x1400)
#define GPIOG_BASE_ADDR								(AHB1_BASE_ADDR + 0x1800)
#define GPIOH_BASE_ADDR								(AHB1_BASE_ADDR + 0x1C00)
#define RCC_BASE_ADDR								(AHB1_BASE_ADDR + 0x3800)

/*
 * Base addresses of peripherals under APB2 bus
 */
#define SYSCFG_BASE_ADDR							0x40013800

/*
 * Base addresses of peripherals under APB1 bus
 */

/***************************** START: Register definitions ************************************/

/*
 * GPIO Register definition
 */

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDER;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
}GPIO_RegDef_t;

typedef struct{
	volatile uint32_t CR;
	volatile uint32_t PLL_CFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1_RSTR;
	volatile uint32_t AHB2_RSTR;
	volatile uint32_t AHB3_RSTR;
	volatile uint32_t RESERVED1;
	volatile uint32_t APB1_RSTR;
	volatile uint32_t APB2_RSTR;
	volatile uint32_t RESERVED2[2];
	volatile uint32_t AHB1_ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t RESERVED3;
	volatile uint32_t APB1_ENR;
	volatile uint32_t APB2_ENR;
	volatile uint32_t RESERVED4[2];
	volatile uint32_t AHB1_LPENR;
	volatile uint32_t AHB2_LPENR;
	volatile uint32_t AHB3_LPENR;
	volatile uint32_t RESERVED5;
	volatile uint32_t APB1_LPENR;
	volatile uint32_t APB2_LPENR;
	volatile uint32_t RESERVED6[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t RESERVED7[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2_SCFGR;
	volatile uint32_t PLL_SAI_CFGR;
	volatile uint32_t DCK_CFGR;
	volatile uint32_t CK_GATENR;
	volatile uint32_t DCK_CFGR2;
}RCC_RegDef_t;

/****************************************END: Register Definitions**********************/

#define GPIOA							((GPIO_RegDef_t *)GPIOA_BASE_ADDR)
#define GPIOB							((GPIO_RegDef_t *)GPIOB_BASE_ADDR)
#define GPIOC							((GPIO_RegDef_t *)GPIOC_BASE_ADDR)
#define GPIOD							((GPIO_RegDef_t *)GPIOD_BASE_ADDR)
#define GPIOE							((GPIO_RegDef_t *)GPIOE_BASE_ADDR)
#define GPIOF							((GPIO_RegDef_t *)GPIOF_BASE_ADDR)
#define GPIOG							((GPIO_RegDef_t *)GPIOG_BASE_ADDR)
#define GPIOH							((GPIO_RegDef_t *)GPIOH_BASE_ADDR)

#define RCC								((RCC_RegDef_t *)RCC_BASE_ADDR)

/*****************************************START: Peripheral clock Enable and Disable*********/
#define GPIOA_PCLK_EN()					(RCC->AHB1_ENR |= 1<<0)
#define GPIOB_PCLK_EN()					(RCC->AHB1_ENR |= 1<<1)
#define GPIOC_PCLK_EN()					(RCC->AHB1_ENR |= 1<<2)
#define GPIOD_PCLK_EN()					(RCC->AHB1_ENR |= 1<<3)
#define GPIOE_PCLK_EN()					(RCC->AHB1_ENR |= 1<<4)
#define GPIOF_PCLK_EN()					(RCC->AHB1_ENR |= 1<<5)
#define GPIOG_PCLK_EN()					(RCC->AHB1_ENR |= 1<<6)
#define GPIOH_PCLK_EN()					(RCC->AHB1_ENR |= 1<<7)

#define GPIOA_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<7))

#define GPIOA_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<0); (RCC->AHB1_RSTR &= ~(1<<0));}while(0)
#define GPIOB_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<1); (RCC->AHB1_RSTR &= ~(1<<1));}while(0)
#define GPIOC_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<2); (RCC->AHB1_RSTR &= ~(1<<2));}while(0)
#define GPIOD_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<3); (RCC->AHB1_RSTR &= ~(1<<3));}while(0)
#define GPIOE_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<4); (RCC->AHB1_RSTR &= ~(1<<4));}while(0)
#define GPIOF_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<5); (RCC->AHB1_RSTR &= ~(1<<5));}while(0)
#define GPIOG_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<6); (RCC->AHB1_RSTR &= ~(1<<6));}while(0)
#define GPIOH_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<7); (RCC->AHB1_RSTR &= ~(1<<7));}while(0)

//Some commonly used #defines
#define DISABLE							0
#define ENABLE							1
#define RESET							DISABLE
#define SET								ENABLE

#include "stm32f446xx_gpio_driver_test.h"

#endif /* INC_STM32F446XX_TEST_H_ */
