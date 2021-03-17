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

/*******************************************MPU Details*****************************************/
#define NVIC_ISER_BASE_ADDR							0xE000E100
#define NVIC_ICER_BASE_ADDR							0xE000E180
#define NVIC_IPR_BASE_ADDR							0xE000E400

typedef struct
{
	volatile uint32_t ISER[8];
}NVIC_ISER_RegDef_t;

typedef struct
{
	volatile  uint32_t ICER[8];
}NVIC_ICER_RegDef_t;

typedef struct
{
	volatile uint32_t IPR[60];
}NVIC_IPR_RegDef_t;


#define NVIC_ISER									((NVIC_ISER_RegDef_t *)NVIC_ISER_BASE_ADDR)
#define NVIC_ICER									((NVIC_ICER_RegDef_t *)NVIC_ICER_BASE_ADDR)
#define NVIC_IPR									((NVIC_IPR_RegDef_t *)NVIC_IPR_BASE_ADDR)

/*******************************************END: MPU Details************************************/
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
#define EXTI_BASE_ADDR								0x40013C00
#define SPI1_BASE_ADDR								(APB2_BASE_ADDR + 0x33FF)


/*
 * Base addresses of peripherals under APB1 bus
 */
#define SPI2_BASE_ADDR								(APB1_BASE_ADDR + 0x3800)
#define SPI3_BASE_ADDR								(APB1_BASE_ADDR + 0x3C00)

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

/*
 * RCC Register definition
 */
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

/*
 * EXTI Register definition
 */
typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_RegDef_t;

/*
 * SYSCFG Register definition
 */
typedef struct
{
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	volatile uint32_t RESERVED[2];
	volatile uint32_t CMPCR;
	volatile uint32_t CFGR;
}SYSCFG_RegDef_t;

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;
}SPI_RegDef_t;


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

#define SYSCFG							((SYSCFG_RegDef_t *)SYSCFG_BASE_ADDR)
#define EXTI							((EXTI_RegDef_t *)EXTI_BASE_ADDR)

#define SPI1							((SPI_RegDef_t *)SPI1_BASE_ADDR)
#define SPI2							((SPI_RegDef_t *)SPI2_BASE_ADDR)
#define SPI3							((SPI_RegDef_t *)SPI3_BASE_ADDR)

/*****************************************START: Peripheral clock Enable and Disable*********/
#define GPIOA_PCLK_EN()					(RCC->AHB1_ENR |= 1<<0)
#define GPIOB_PCLK_EN()					(RCC->AHB1_ENR |= 1<<1)
#define GPIOC_PCLK_EN()					(RCC->AHB1_ENR |= 1<<2)
#define GPIOD_PCLK_EN()					(RCC->AHB1_ENR |= 1<<3)
#define GPIOE_PCLK_EN()					(RCC->AHB1_ENR |= 1<<4)
#define GPIOF_PCLK_EN()					(RCC->AHB1_ENR |= 1<<5)
#define GPIOG_PCLK_EN()					(RCC->AHB1_ENR |= 1<<6)
#define GPIOH_PCLK_EN()					(RCC->AHB1_ENR |= 1<<7)

#define SYSCFG_PCLK_EN()				(RCC->APB2_ENR |= 1<<14)

#define SPI1_PCLK_EN()					(RCC->APB2_ENR |= 1<<12)
#define SPI2_PCLK_EN()					(RCC->APB1_ENR |= 1<<14)
#define SPI3_PCLK_EN()					(RCC->APB1_ENR |= 1<<15)

#define GPIOA_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()					(RCC->AHB1_ENR &= ~(1<<7))

#define SYSCFG_PCLK_DI()				(RCC->APB2_ENR &= ~(1<<14))

#define SPI1_PCLK_DI()					(RCC->APB2_ENR &= ~(1<<12))
#define SPI2_PCLK_DI()					(RCC->APB1_ENR &= ~(1<<14))
#define SPI3_PCLK_DI()					(RCC->APB1_ENR &= ~(1<<15))


#define GPIOA_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<0); (RCC->AHB1_RSTR &= ~(1<<0));}while(0)
#define GPIOB_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<1); (RCC->AHB1_RSTR &= ~(1<<1));}while(0)
#define GPIOC_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<2); (RCC->AHB1_RSTR &= ~(1<<2));}while(0)
#define GPIOD_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<3); (RCC->AHB1_RSTR &= ~(1<<3));}while(0)
#define GPIOE_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<4); (RCC->AHB1_RSTR &= ~(1<<4));}while(0)
#define GPIOF_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<5); (RCC->AHB1_RSTR &= ~(1<<5));}while(0)
#define GPIOG_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<6); (RCC->AHB1_RSTR &= ~(1<<6));}while(0)
#define GPIOH_REG_RST()					do{(RCC->AHB1_RSTR |= 1<<7); (RCC->AHB1_RSTR &= ~(1<<7));}while(0)

#define SPI1_REG_RST()					do{(RCC->APB2_RSTR |= 1<<12); (RCC->APB2_RSTR &= ~(1<<12));}while(0)
#define SPI2_REG_RST()					do{(RCC->APB1_RSTR |= 1<<14); (RCC->APB1_RSTR &= ~(1<<14));}while(0)
#define SPI3_REG_RST()					do{(RCC->APB1_RSTR |= 1<<15); (RCC->APB1_RSTR &= ~(1<<15));}while(0)

/*
 * SPI Bit positions
 */

#define SPI_CR1_CPHA_BITPOS					0
#define SPI_CR1_CPOL_BITPOS					1
#define SPI_CR1_MSTR_BITPOS					2
#define SPI_CR1_BR_BITPOS					3
#define SPI_CR1_SPE_BITPOS					6
#define SPI_CR1_LSBFIRST_BITPOS				7
#define SPI_CR1_SSI_BITPOS					8
#define SPI_CR1_SSM_BITPOS					9
#define SPI_CR1_RXONLY_BITPOS				10
#define SPI_CR1_DFF_BITPOS					11
#define SPI_CR1_CRCNEXT_BITPOS				12
#define SPI_CR1_CRCEN_BITPOS				13
#define SPI_CR1_BIDIOE_BITPOS				14
#define SPI_CR1_BIDIMODE_BITPOS				15

#define SPI_CR2_RXDMAEN_BITPOS				0
#define SPI_CR2_TXDMAEN_BITPOS				1
#define SPI_CR2_SSOE_BITPOS					2
#define SPI_CR2_FRF_BITPOS					4
#define SPI_CR2_ERRIE_BITPOS				5
#define SPI_CR2_RXNEIE_BITPOS				6
#define SPI_CR2_TXEIE_BITPOS				7

#define SPI_SR_RXNE_BITPOS					0
#define SPI_SR_TXE_BITPOS					1
#define SPI_SR_CHSIDE_BITPOS				2
#define SPI_SR_UDR_BITPOS					3
#define SPI_SR_CRCERR_BITPOS				4
#define SPI_SR_MODF_BITPOS					5
#define SPI_SR_BSY_BITPOS					6
#define SPI_SR_FRE_BITPOS					7

#define TXE_FLAG							(1 << SPI_SR_TXE_BITPOS)
#define MODF_FLAG							(1 << SPI_SR_MODF_BITPOS)
#define BSY_FLAG							(1 << SPI_SR_BSY_BITPOS)

//Some commonly used #defines
#define DISABLE								0
#define ENABLE								1
#define RESET								DISABLE
#define SET									ENABLE
#define FLAG_SET							SET
#define FLAG_RESET							RESET

#define EXTI0_IRQ_NO						6
#define	EXTI1_IRQ_NO						7
#define EXTI2_IRQ_NO						8
#define EXTI3_IRQ_NO						9
#define	EXTI4_IRQ_NO						10
#define EXTI9_5_IRQ_NO						23
#define EXTI15_10_IRQ_NO					40

#define NVIC_IRQ_PRIO_0						0
#define NVIC_IRQ_PRIO_1						1
#define NVIC_IRQ_PRIO_2						2
#define NVIC_IRQ_PRIO_3						3
#define NVIC_IRQ_PRIO_4						4
#define NVIC_IRQ_PRIO_5						5
#define NVIC_IRQ_PRIO_6						6
#define NVIC_IRQ_PRIO_7						7
#define NVIC_IRQ_PRIO_8						8
#define NVIC_IRQ_PRIO_9						9
#define NVIC_IRQ_PRIO_10					10
#define NVIC_IRQ_PRIO_11					11
#define NVIC_IRQ_PRIO_12					12
#define NVIC_IRQ_PRIO_13					13
#define NVIC_IRQ_PRIO_14					14
#define NVIC_IRQ_PRIO_15					15



#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"

#endif /* INC_STM32F446XX_TEST_H_ */
