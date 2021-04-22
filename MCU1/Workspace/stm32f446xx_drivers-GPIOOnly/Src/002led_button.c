/*
 * 002led_button.c
 *
 *  Created on: 25-Jan-2021
 *      Author: DELL
 */

#include "stm32f446xx.h"

void delay(void)
{
	uint32_t i;

	for(i=0;i<500000/2;i++)
	{

	}
}


int main(void)
{
	GPIO_Handle_t GPIOLed, GPIOBtn;

	GPIOLed.pGPIOx = GPIOA;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;

	GPIOBtn.pGPIOx = GPIOC;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; //Output type does not matter when pin is configured as input
	GPIOBtn.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD; //There already exists an external pull down


	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_PeriClockControl(GPIOC, ENABLE);

	GPIO_Init(&GPIOLed);
	GPIO_Init(&GPIOBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == 0) //Button is pressed. Toggle only when pressed
		{
			delay(); // To avoid debouncing of switch
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
		}

	}


}
