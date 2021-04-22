/*
 * 005button_interrupt.c
 *
 *  Created on: Feb 16, 2021
 *      Author: DELL
 */

#include<string.h>
#include "stm32f446xx.h"

GPIO_Handle_t Led, Button;

int main()
{

	memset(&Led, 0, sizeof(Led));
	memset(&Button, 0, sizeof(Button));
	//LED GPIO Configuration
	Led.pGPIOx = GPIOA;
	Led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	Led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	Led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	Led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	Led.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;

	// Button Configuration
	Button.pGPIOx = GPIOC;
	Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	Button.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(Led.pGPIOx, ENABLE);
	GPIO_PeriClockControl(Button.pGPIOx, ENABLE);

	GPIO_Init(&Led);
	GPIO_Init(&Button);

	//IRQ Configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRIO15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

	while(1)
	{

	}

	return 0;
}

void EXTI15_10_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_13);
	GPIO_ToggleOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.GPIO_PinNumber);

}
