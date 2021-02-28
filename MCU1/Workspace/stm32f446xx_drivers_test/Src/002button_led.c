/*
 * 002button_led.c
 *
 *  Created on: Feb 13, 2021
 *      Author: DELL
 */

#include "stm32f446xx.h"

#define BUTTON_PRESSED 0
#define BUTTON_NOT_PRESSED 1

void delay()
{
	uint16_t i;

	for(i=0;i<0xFFFF;i++)
	{

	}
}

int main()
{
	GPIO_Handle_t Led, Button;

	Led.pGPIOx = GPIOA;
	Led.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_5;
	Led.GPIO_PinConfig.Mode = GPIO_MODE_OUTPUT;
	Led.GPIO_PinConfig.Ospeed = GPIO_OSPEED_FAST;
	Led.GPIO_PinConfig.Otype = GPIO_OTYPE_PP;
	Led.GPIO_PinConfig.Pupd = GPIO_NO_PUPD;

	Button.pGPIOx = GPIOC;
	Button.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_13;
	Button.GPIO_PinConfig.Mode = GPIO_MODE_INPUT;
	Button.GPIO_PinConfig.Pupd = GPIO_NO_PUPD;
	Button.GPIO_PinConfig.Ospeed = GPIO_OSPEED_FAST;

	GPIO_PeriClockControl(Led.pGPIOx, ENABLE);
	GPIO_PeriClockControl(Button.pGPIOx, ENABLE);

	GPIO_Init(&Button);
	GPIO_Init(&Led);

	while(1)
	{
		uint16_t Val;

		Val = GPIO_ReadFromInputPin(Button.pGPIOx, Button.GPIO_PinConfig.PinNumber);

		if(Val == BUTTON_PRESSED)
		{
			GPIO_ToggleOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.PinNumber);
			//delay();
			while((GPIO_ReadFromInputPin(Button.pGPIOx, Button.GPIO_PinConfig.PinNumber)!=BUTTON_NOT_PRESSED))
			{

			}
		}
	}


	return 0;
}
