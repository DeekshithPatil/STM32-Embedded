/*
 * stm32f446xx.h
 *
 *  Created on: Jan 5, 2021
 *      Author: DELL
 */

#include<stdint.h>

#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#define __vo							volatile //To use a shorthand for volatile

/********************************************START: Processor Specific Details **************************/

/*
 *ARM Cortex Mx Processor NVIC ISERx register Addresses
 */

#define NVIC_ISER0						((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1						((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2						((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3						((volatile uint32_t*)0xE000E10C)

/*
 *ARM Cortex Mx Processor NVIC ICERx register Addresses
 */

#define NVIC_ICER0						((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1						((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2						((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3						((volatile uint32_t*)0xE000E18C)

/*
 *ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR				((volatile uint32_t *)0xE000E400)

/*
 * Number of priority bits implemented in priority register
 */
#define NO_PR_BITS_IMPLEMENTED			4


/*
 * Base address of Flash and SRAM1 memories
 */
#define FLASH_BASEADDR					0x08000000U
#define SRAM1_BASEADDR					0x20000000U //SRAM1 is the main SRAM and SRAM2 is not used in this course
#define SRAM 							SRAM1_BASEADDR

/*
 * Base address of SRAM2 and ROM memories
 */
#define SRAM2_BASEADDR					0x2001C000U // SRAM2 is secondary SRAM
#define ROM								0x1FFF0000U // ROM is nothing but system memory

/*
 * Base address of AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASEADDR						0x40000000U
#define APB1PERIPH_BASEADDR					PERIPH_BASEADDR //Becase APB1 bus is the first address in all the bus peripherals, thus it is same as PERIPH_BASE
#define APB2PERIPH_BASEADDR					0x40010000U
#define AHB1PERIPH_BASEADDR 				0x40020000U
#define AHB2PERIPH_BASEADDR					0x50000000U

/*
 * Base address of peripherals which are hanging on AHB1 bus
 * Note: Here we are using relative addressing (w.r.t AHB1 base address) instead of direct
 */

#define GPIOA_BASEADDR					(AHB1PERIPH_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR					(AHB1PERIPH_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR					(AHB1PERIPH_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR					(AHB1PERIPH_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR					(AHB1PERIPH_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR					(AHB1PERIPH_BASEADDR + 0x1400U)
#define GPIOG_BASEADDR					(AHB1PERIPH_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR					(AHB1PERIPH_BASEADDR + 0x1C00U)
#define RCC_BASEADDR					(AHB1PERIPH_BASEADDR + 0x3800U)


/*
 * Base address of peripherals which are hanging on APB1 bus
 * Note: Here we are using relative addressing (w.r.t APB1 base address) instead of direct
 */
#define I2C1_BASEADDR					(APB1PERIPH_BASEADDR + 0x5400U)
#define I2C2_BASEADDR					(APB1PERIPH_BASEADDR + 0x5800U)
#define I2C3_BASEADDR					(APB1PERIPH_BASEADDR + 0x5C00U)
#define SPI2_BASEADDR					(APB1PERIPH_BASEADDR + 0x3800U)
#define SPI3_BASEADDR					(APB1PERIPH_BASEADDR + 0x3C00U)
#define USART2_BASEADDR					(APB1PERIPH_BASEADDR + 0x4400U)
#define USART3_BASEADDR					(APB1PERIPH_BASEADDR + 0x4800U)
#define UART4_BASEADDR					(APB1PERIPH_BASEADDR + 0x4C00U)
#define UART5_BASEADDR					(APB1PERIPH_BASEADDR + 0x5000U)

/*
 * Base address of peripherals which are hanging on APB2 bus
 * Note: Here we are using relative addressing (w.r.t APB2 base address) instead of direct
 */
#define EXTI_BASEADDR					(APB2PERIPH_BASEADDR + 0x3C00U)
#define USART1_BASEADDR					(APB2PERIPH_BASEADDR + 0x1000U)
#define USART6_BASEADDR					(APB2PERIPH_BASEADDR + 0x1400U)
#define SPI1_BASEADDR					(APB2PERIPH_BASEADDR + 0x3000U)
#define SPI4_BASEADDR					(APB2PERIPH_BASEADDR + 0x3400U)
#define SYSCFG_BASEADDR					(APB2PERIPH_BASEADDR + 0x3800U)

/**************************************peripheral register definition structures***********************/

/*
 * Note: Registers of a peripheral are specific to a MCU
 * e.g: Number of Registers of SPI peripheral of STM32F4x family of MCUs may be different(more or less)
 * Compared to number of registers of SPI peripheral of STM32Lx or STM32F0x family of MCUs
 * Please check your device RM
 */

typedef struct
{
	volatile uint32_t MODER; //GPIO port mode register
	volatile uint32_t OTYPER; //GPIO port output type register
	volatile uint32_t OSPEEDER; //GPIO port output speed register
	volatile uint32_t PUPDR; //GPIO port pull-up/pull-down register
	volatile uint32_t IDR; //GPIO port input data register
	volatile uint32_t ODR; //GPIO port output data register
	volatile uint32_t BSRR; //GPIO port bit set/reset register
	volatile uint32_t LCKR; //GPIO port configuration lock register
	volatile uint32_t AFR[2]; //GPIO alternate function low and high register. 0 -> Low register, 1 -> High register

}GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	uint32_t		  RESERVED0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t		  RESERVED1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	uint32_t		  RESERVED2;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t		  RESERVED3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	uint32_t		  RESERVED4;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t		  RESERVED5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t		  RESERVED6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;
	volatile uint32_t CKGATENR;
	volatile uint32_t DCKCFGR2;


}RCC_RegDef_t;


typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_RegDef_t;

typedef struct
{
	volatile uint32_t MEMRMP;
	volatile uint32_t SPMC;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED1[2];
	volatile uint32_t CMPCR;
	uint32_t RESERVED2[2];
	volatile uint32_t CFGR;

}SYSCFG_RegDef_t;

/*
 * peripheral definitions (Peripheral base addresses type casted to xxx_RegDef_t)
 * Note: Type casting is used to tell the compiler specifically that the value is an address and not an integer data
 */
#define GPIOA							((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB							((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC							((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD							((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE							((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF							((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG							((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH							((GPIO_RegDef_t *)GPIOH_BASEADDR)

#define RCC								((RCC_RegDef_t *) RCC_BASEADDR)
#define EXTI							((EXTI_RegDef_t *) EXTI_BASEADDR)
#define SYSCFG							((SYSCFG_RegDef_t *) SYSCFG_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()					(RCC->AHB1ENR |= (1<<0)) //Putting 0th bit of AHB1ENR as 1
#define GPIOB_PCLK_EN()					(RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()					(RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()					(RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()					(RCC->AHB1ENR |= (1<<4))
#define GPIOF_PCLK_EN()					(RCC->AHB1ENR |= (1<<5))
#define GPIOG_PCLK_EN()					(RCC->AHB1ENR |= (1<<6))
#define GPIOH_PCLK_EN()					(RCC->AHB1ENR |= (1<<7))

/*
 * Clock Enable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()					(RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()					(RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()					(RCC->APB1ENR |= (1<<23))

/*
 * Clock Enable Macros for SPIx peripherals
 */

#define SPI1_PCLK_EN()					(RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()					(RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()					(RCC->APB1ENR |= (1<<15))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()				(RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN()				(RCC->APB1ENR |= (1<<17))
#define USART3_PCLK_EN()				(RCC->APB1ENR |= (1<<18))

/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN()				(RCC->APB2ENR |= (1<<14))



/*
 * Clock disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<0)) //Putting 0th bit of AHB1ENR as 0
#define GPIOB_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()					(RCC->AHB1ENR &= ~(1<<7))

/*
 * Clock disable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_DI()					(RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI()					(RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DI()					(RCC->APB1ENR &= ~(1<<23))

/*
 * Clock disable Macros for SPIx peripherals
 */

#define SPI1_PCLK_DI()					(RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI()					(RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI()					(RCC->APB1ENR &= ~(1<<15))

/*
 * Clock disable Macros for USARTx peripherals
 */
#define USART1_PCLK_DI()				(RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DI()				(RCC->APB1ENR &= ~(1<<17))
#define USART3_PCLK_DI()				(RCC->APB1ENR &= ~(1<<18))

/*
 * Clock disable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_DI()				(RCC->APB2ENR &= ~(1<<14))

/*
 * Macros to reset GPIOx peripherals
 * The first operation is to set the corresponding bit and the second operation is to reset the corresponding bit
 * To use two statements in one single macro, we should use do while loop
 * we don't have to give a semi-colon after while because because while calling it we give
 * NOTE: Since it is whil(0) it is executed only once
 */
#define GPIOA_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<0));	(RCC->AHB1RSTR &= ~(1<<0));}while(0)
#define GPIOB_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<1));	(RCC->AHB1RSTR &= ~(1<<1));}while(0)
#define GPIOC_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<2));	(RCC->AHB1RSTR &= ~(1<<2));}while(0)
#define GPIOD_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<3));	(RCC->AHB1RSTR &= ~(1<<3));}while(0)
#define GPIOE_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<4));	(RCC->AHB1RSTR &= ~(1<<4));}while(0)
#define GPIOF_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<5));	(RCC->AHB1RSTR &= ~(1<<5));}while(0)
#define GPIOG_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<6));	(RCC->AHB1RSTR &= ~(1<<6));}while(0)
#define GPIOH_REG_RESET()				do{ (RCC->AHB1RSTR |= (1<<7));	(RCC->AHB1RSTR &= ~(1<<7));}while(0)

/*
 * return port code for selecting relevant port in EXTCR for given GPIOx base address
 */
#define GPIO_BASEADDR_TO_CODE(x)		((x == GPIOA) ? 0:\
										 (x == GPIOB) ? 1:\
										 (x == GPIOC) ? 2:\
										 (x == GPIOD) ? 3:\
										 (x == GPIOE) ? 4:\
										 (x == GPIOF) ? 5:\
										 (x == GPIOG) ? 6:\
										 (x == GPIOH) ? 7:0	)

//Defining IRQ number macros for EXTI
#define IRQ_NO_EXTI0					6 //RHS Indicates IRQ number from IVT
#define IRQ_NO_EXTI1					7
#define IRQ_NO_EXTI2					8
#define IRQ_NO_EXTI3					9
#define IRQ_NO_EXTI4					10
#define IRQ_NO_EXTI9_5					23
#define IRQ_NO_EXTI15_10				40





//some generic macros
#define ENABLE 						1
#define DISABLE 					0
#define SET 						ENABLE
#define RESET 						DISABLE
#define GPIO_PIN_SET				SET
#define GPIO_PIN_RESET				RESET

//Include driver files
#include "stm32f446xx_gpio_driver.h"


#endif /* INC_STM32F446XX_H_ */
