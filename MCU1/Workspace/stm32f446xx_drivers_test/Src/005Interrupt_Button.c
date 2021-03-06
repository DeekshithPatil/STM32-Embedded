/*
 * 005Interrupt_Button.c
 *
 *  Created on: 28-Feb-2021
 *      Author: DELL
 */


#include "stm32f446xx.h"


GPIO_Handle_t Button, Led;

int main()
{


	Led.pGPIOx = GPIOA;
	Led.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_5;
	Led.GPIO_PinConfig.Mode = GPIO_MODE_OUTPUT;
	Led.GPIO_PinConfig.Otype = GPIO_OTYPE_PP;
	Led.GPIO_PinConfig.Ospeed = GPIO_OSPEED_FAST;
	Led.GPIO_PinConfig.Pupd = GPIO_NO_PUPD;

	Button.pGPIOx = GPIOC;
	Button.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_13;
	Button.GPIO_PinConfig.Mode = GPIO_MODE_IT_FET;
	Button.GPIO_PinConfig.Pupd = GPIO_NO_PUPD;
	Button.GPIO_PinConfig.Ospeed = GPIO_OSPEED_FAST;

	GPIOA_PCLK_EN();
	GPIOC_PCLK_EN();

	GPIO_Init(&Button);
	GPIO_Init(&Led);

	GPIO_IRQConfig(EXTI15_10_IRQ_NO, ENABLE);
	GPIO_IRQPriority(EXTI15_10_IRQ_NO, NVIC_IRQ_PRIO_15);
	while(1)
	{

	}





	return 0;
}

void EXTI15_10_IRQHandler(void)
{
	GPIO_IRQHandle(Button.GPIO_PinConfig.PinNumber);

	GPIO_ToggleOutputPin(Led.pGPIOx, Led.GPIO_PinConfig.PinNumber);
}
