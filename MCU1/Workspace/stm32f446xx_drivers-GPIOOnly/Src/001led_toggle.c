/*
 * 001led_toggle.c
 *
 *  Created on: 24-Jan-2021
 *      Author: DELL
 */
#include "stm32f446xx.h"


void delay(void)
{
	for(uint32_t i=0; i<500000; i++ )
	{

	}
}


int main(void)
{
	/*
	 * Data Declaration of GPIO_TaskHandle structure
	 */
	GPIO_Handle_t GpioLed;


	/*
	 * Data Initialisation of GPIO_TaskHandle structure
	 */

	/* ************************* Case-1: Push pull type
	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5; 		// Pin number = 5
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT; 		// Pin Mode = 1
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 	// Pin Speed = 2 (Fast)
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	// Output type = 0 (Push pull)
	GpioLed.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;	// Pullup / Pulldown = 0 (Nothing)
	*/

	/*
	 * ************************** Case-2: Open Drain configuration
	 */
	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5; 		// Pin number = 5
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT; 		// Pin Mode = 1
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 	// Pin Speed = 2 (Fast)
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;	// Output type = 1 (Open Drain)
	GpioLed.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_PIN_PU;	// Pullup / Pulldown = 0 (Nothing)
	// Since internal pull up resistor value is 50K, it limits a lot of current.

	/*
	 * Clock Enable
	 */

	GPIO_PeriClockControl(GPIOA, ENABLE);						//Enable clock to GPIOA

	/*
	 * GPIO Init
	 */

	GPIO_Init(&GpioLed);										//Initialize GPIO with the above mentioned params

	/*
	 * Toggle function implemented in a infinite while loop
	 */
	while(1)
	{
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5); 		//Toggle output pin
		delay();											//Call a delay

	}


	return 0;
}

