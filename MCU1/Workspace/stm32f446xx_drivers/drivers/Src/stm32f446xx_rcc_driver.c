/*
 * stm32f446xx_rcc_driver.c
 *
 *  Created on: 10-Apr-2021
 *      Author: DELL
 */

#include "stm32f446xx_rcc_driver.h"

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler[4] = {2,4,8,16};
uint8_t APB2_PreScaler[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;
	uint8_t clksrc, temp;
	uint8_t ahbp,apb1p; //prescalers for ahb and apb1

	// APB1 Pclk (freq) = Freq of (HSI/HSE/PLL/PLL_R, whichever is selected) / (AHBPrescaler * APB1Prescaler)

	//1. Obtain system clk value
	//Refer to RCC_CFGR in reference manual to obtain more info
	//SWS[1:0] (System clock switch status): Indicates which clock source is as the system clock

	clksrc = (RCC->CFGR >> 2) & 0x3; //obtain values of bit number 2 and 3
	if(clksrc == 0)
	{
		SystemClk = 16000000; //Internal clock source is of 16MHz
	}
	else if(clksrc == 1)
	{
		SystemClk = 8000000; //External clock frequency in case of Nucleo boards comes from STLink circuitry, which is 8MHz
	}
	else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLClockOutput();
	}

	//2. Obtain AHB pre-scalar value
	//Refer to HPRE bit fields in RCC_CFGR register
	temp = (RCC->CFGR >> 4) & 0xF; //Obtain HPRE value by masking
	if(temp < 8)
	{
		ahbp = 1; //NO division
	}
	else
	{
		ahbp = AHB_PreScaler[temp-8]; //use temp (which is greater than 8) as index to obtain ahb pre-scaler value from array
	}

	//3. Obtain the APB1 pre-scalar
	//Refer to the PPRE bit fields in RCC_CFGR register
	temp = (RCC->CFGR >> 10) & 0x7;

	if(temp<7)
	{
		apb1p = 1;
	}
	else
	{
		apb1p = APB1_PreScaler[temp - 4];
	}

	//4. Calculate the value of pclk1
	pclk1 = SystemClk / (apb1p * ahbp);
	return pclk1;
}

uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk;
	uint8_t clksrc, temp;
	uint8_t ahbp,apb2p; //prescalers for ahb and apb2

	// APB2 Pclk (freq) = Freq of (HSI/HSE/PLL/PLL_R, whichever is selected) / (AHBPrescaler * APB2Prescaler)

	//1. Obtain system clk value
	//Refer to RCC_CFGR in reference manual to obtain more info
	//SWS[1:0] (System clock switch status): Indicates which clock source is as the system clock

	clksrc = (RCC->CFGR >> 2) & 0x3; //obtain values of bit number 2 and 3
	if(clksrc == 0)
	{
		SystemClk = 16000000; //Internal clock source is of 16MHz
	}
	else if(clksrc == 1)
	{
		SystemClk = 8000000; //External clock frequency in case of Nucleo boards comes from STLink circuitry, which is 8MHz
	}
	else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLClockOutput();
	}

	//2. Obtain AHB pre-scalar value
	//Refer to HPRE bit fields in RCC_CFGR register
	temp = (RCC->CFGR >> 4) & 0xF; //Obtain HPRE value by masking
	if(temp < 8)
	{
		ahbp = 1; //NO division
	}
	else
	{
		ahbp = AHB_PreScaler[temp-8]; //use temp (which is greater than 8) as index to obtain ahb pre-scaler value from array
	}

	//3. Obtain the APB1 pre-scalar
	//Refer to the PPRE bit fields in RCC_CFGR register
	temp = (RCC->CFGR >> 10) & 0x7;

	if(temp<7)
	{
		apb2p = 1;
	}
	else
	{
		apb2p = APB2_PreScaler[temp - 4];
	}

	//4. Calculate the value of pclk1
	pclk2 = SystemClk / (apb2p * ahbp);
	return pclk2;
}

uint32_t RCC_GetPLLClockOutput()
{
	uint32_t temp;

	temp = 0;
	//NOTE: PLL is not used

	return temp;
}
