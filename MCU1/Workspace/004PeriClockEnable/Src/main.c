/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include<stdint.h> //this is used for uint32_t declaration

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define ADC_BASE_ADDR 				0x40012000UL //Obtained from memory map section of reference manual
#define ADC_CR1_REG_OFFSET 			0X04UL//Obtained form ADC section of reference manual

#define ADC_CR1_REG_ADDR 			(ADC_BASE_ADDR + ADC_CR1_REG_OFFSET)

#define RCC_BASE_ADDR 0X40023800UL
#define RCC_APB2_ENR_OFFSET 0X44UL
#define RCC_APB2_ENR_ADDR (RCC_BASE_ADDR + RCC_APB2_ENR_OFFSET)


//Step-1: Run this main program and check ADC register in debug mode from SFRs section.
//Note: Since clock is not yet enabled, although we write data, the peripheral does not get written

/*
int main(void)
{
	uint32_t *pAdcCr1Reg = (uint32_t*) ADC_CR1_REG_ADDR ; //This pointer now points to ADC CR1 register
	// on the RHS uint32_t* is used to convert hex into unsigned integer pointer of 32 bits.

	*pAdcCr1Reg |= (1<<8); //Here we are trying to set the Scan bit in ADC CR1 register, which is the 8th bit.

	for(;;);


}*/

//Step-2: Enable peripheral clock to ADC
int main(void)
{
	uint32_t *pAdcCr1Reg = (uint32_t*) ADC_CR1_REG_ADDR ; //This pointer now points to ADC CR1 register
	// on the RHS uint32_t* is used to convert hex into unsigned integer pointer of 32 bits.

	uint32_t *pRccApb2Enr = (uint32_t*) RCC_APB2_ENR_ADDR; //Points to RCC APB2 Enable register

	//Enable the peripheral clock for ADC1
	*pRccApb2Enr |= (1<<8);

	//Modify the ADC1 CR register
	*pAdcCr1Reg |= (1<<8); //Here we are trying to set the Scan bit in ADC CR1 register, which is the 8th bit.





	for(;;);


}
