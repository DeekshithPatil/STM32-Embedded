/*
 * 001led_toggle_test.c
 *
 *  Created on: 13-Feb-2021
 *      Author: DELL
 */

#include "stm32f446xx_test.h"

void delay()
{
	uint32_t i;

	for(i = 0; i<0xFFFFF; i++)
	{

	}
}

int main()
{
	GPIO_Handle_t Led;

	Led.pGPIOx = GPIOA;
	Led.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_5;
	Led.GPIO_PinConfig.Mode = GPIO_MODE_OUTPUT;
	Led.GPIO_PinConfig.Ospeed = GPIO_OSPEED_FAST;
	Led.GPIO_PinConfig.Otype = GPIO_OPTYPE_PP;
	Led.GPIO_PinConfig.Pupd = GPIO_NO_PUPD;

	GPIO_PeriClockControl(Led.pGPIOx, ENABLE);
	GPIO_Init(&Led);

	while(1)
	{
		//GPIO_ToggleOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.PinNumber);
		//GPIO_WriteToOutputPort(Led.pGPIOx, 0x0020);
		GPIO_WriteToOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.PinNumber, ENABLE);
		delay();
		//GPIO_ToggleOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.PinNumber);
		//GPIO_WriteToOutputPort(Led.pGPIOx, 0x0000);
		GPIO_WriteToOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.PinNumber, DISABLE);
		delay();
	}
	return 0;
}

