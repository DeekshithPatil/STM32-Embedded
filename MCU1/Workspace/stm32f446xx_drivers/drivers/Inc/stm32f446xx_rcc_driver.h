/*
 * stm32f446xx_rcc_driver.h
 *
 *  Created on: 10-Apr-2021
 *      Author: DELL
 */

#ifndef INC_STM32F446XX_RCC_DRIVER_H_
#define INC_STM32F446XX_RCC_DRIVER_H_

#include "stm32f446xx.h"


uint32_t RCC_GetPCLK1Value(void); //Returns peripheral clock value of APB1
uint32_t RCC_GetPCLK2Value(void); //Return peripheral clock value of APB2
uint32_t RCC_GetPLLClockOutput();

uint32_t RCC_GetPLLClockOutput();

#endif /* INC_STM32F446XX_RCC_DRIVER_H_ */
