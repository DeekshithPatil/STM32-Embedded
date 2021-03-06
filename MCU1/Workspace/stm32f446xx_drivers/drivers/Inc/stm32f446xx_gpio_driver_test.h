/*
 * stm32f446xx_gpio_driver_test.h
 *
 *  Created on: 07-Feb-2021
 *      Author: DELL
 */

#ifndef INC_STM32F446XX_GPIO_DRIVER_TEST_H_
#define INC_STM32F446XX_GPIO_DRIVER_TEST_H_

#include "stm32f446xx_test.h"
typedef struct
{
	uint8_t PinNumber;
	uint8_t Mode;
	uint8_t Otype;
	uint8_t Ospeed;
	uint8_t Pupd;
	uint8_t Altfn;

}GPIO_PinConfig_t;


typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

/*
 * Possible Values for GPIO PinNumber
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
 * Possible Values for GPIO Mode
 */
#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALTFN 2
#define GPIO_MODE_ANALOG 3
//user defined modes
#define GPIO_MODE_IT_RET 4
#define GPIO_MODE_IT_FET 5
#define GPIO_MODE_IT_RFET 6

/*
 * Possible Values for GPIO Output type
 */
#define GPIO_OPTYPE_PP 0
#define GPIO_OPTYPE_OD 1

/*
 * Possible Values for GPIO Output Speed
 */
#define GPIO_OSPEED_LOW 0
#define GPIO_OSPEED_MEDIUM 1
#define GPIO_OSPEED_FAST 2
#define GPIO_OSPEED_HIGH 3

/*
 * Possible Values for GPIO PUPD
 */
#define GPIO_NO_PUPD 0
#define GPIO_PU 1
#define GPIO_PD 2

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Val);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Val);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

#endif /* INC_STM32F446XX_GPIO_DRIVER_TEST_H_ */
