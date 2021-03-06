/*
 * stm32f446xx_gpio_driver.h
 *
 *  Created on: 23-Jan-2021
 *      Author: DELL
 */

#ifndef INC_STM32F446XX_GPIO_DRIVER_H_
#define INC_STM32F446XX_GPIO_DRIVER_H_

#include "stm32f446xx.h"

/*
 * This is a Configuration structure for a GPIO pin
 */

typedef struct
{
	uint8_t GPIO_PinNumber;			// possible values from @GPIO_PIN_NUMBERS
	uint8_t GPIO_PinMode;			// possible values from @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;			// possible values from @GPIO_PIN_SPEED
	uint8_t GPIO_PinPupdControl;	// possible values from @GPIO_PIN_PUPD
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

/*
 * This is a Handle structure for a GPIO pin
 */

typedef struct
{
	GPIO_RegDef_t *pGPIOx;				// Holds the base address of the GPIO port to which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;	// Holds GPIO pin configuration settings
}GPIO_Handle_t;

/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin possible numbers
 */
#define GPIO_PIN_NO_0 0
#define GPIO_PIN_NO_1 1
#define GPIO_PIN_NO_2 2
#define GPIO_PIN_NO_3 3
#define GPIO_PIN_NO_4 4
#define GPIO_PIN_NO_5 5
#define GPIO_PIN_NO_6 6
#define GPIO_PIN_NO_7 7
#define GPIO_PIN_NO_8 8
#define GPIO_PIN_NO_9 9
#define GPIO_PIN_NO_10 10
#define GPIO_PIN_NO_11 11
#define GPIO_PIN_NO_12 12
#define GPIO_PIN_NO_13 13
#define GPIO_PIN_NO_14 14
#define GPIO_PIN_NO_15 15



/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 * These modes and their numbers are obtained from GPIOx Mode register in reference manual
 */

#define GPIO_MODE_IN 				0
#define GPIO_MODE_OUT 				1
#define GPIO_MODE_ALTFN 			2
#define GPIO_MODE_ANALOG			3
//The below modes are user defined for interrupt
#define GPIO_MODE_IT_FT				4 //Interrupt falling edge trigger
#define GPIO_MODE_IT_RT				5 //Interrupt rising edge trigger
#define GPIO_MODE_IT_RFT			6 //Interrupt rising edge, falling edge trigger

/*
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP 0 //Push pull output type
#define GPIO_OP_TYPE_OD 1 //Open drain output type

/*
 * @GPIO_PIN_SPEED
 * GPIO Pin possible output speeds
 */
#define GPIO_SPEED_LOW				0
#define GPIO_SPEED_MEDIUM			1
#define GPIO_SPEED_FAST				2
#define GPIO_SPEED_HIGH				3

/*
 * @GPIO_PIN_PUPD
 * GPIO pin pull up and pull down macros
 */
#define GPIO_NO_PUPD				0
#define GPIO_PIN_PU					1
#define GPIO_PIN_PD					2

/**************************************************************************************
 * 					APIs supported by this driver
 * 		For more information about the APIs check the function definitions
 */

/*
 * Peripheral Clock Setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDI); //First argument holds the base address of the GPIO and the second is used to enable or disbale


/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle); //Used to initialise the GPIOx registers
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx); //Used to de-Initialize the GPIOx registers
//De-initialising a register means sending that register back to its reset value
//To do this, there is one important register in RCC that makes it easy for us, instead of re-writing every register with its default value
//Check "RCC AHB1 peripheral reset register (RCC_AHB1RSTR) in reference manual

//Therefore, DeInit function takes only the base addres of the peripheral which needs to be reset

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber); //base address and pin number and returns 0 or 1 depending on the value read
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
//Port is of 16 pins, hence the return value is of uint16_t
void GPIO_WriteToOuputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value); //base address, pin number and value (1 or 0)
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value); //base address and 16 bit value to write to the 16 pins
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi); //IRQ number and enable or disable
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);





#endif /* INC_STM32F446XX_GPIO_DRIVER_H_ */
