/*
 * stm32f446xx_gpio_driver_test.c
 *
 *  Created on: 07-Feb-2021
 *      Author: DELL
 */

#include "stm32f446xx_gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		//Enable peripheral clock
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}

	}
	else
	{
		//Disable peripheral clock
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
	}
}


void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	if(pGPIOHandle->GPIO_PinConfig.Mode <= GPIO_MODE_ANALOG)
	{
		// Non Interrupt Mode
		temp = pGPIOHandle->GPIO_PinConfig.Mode;
		temp = (temp << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;
	}

	else
	{
		//Interrupt mode

		//1. Configure edge selection
		if(pGPIOHandle->GPIO_PinConfig.Mode == GPIO_MODE_IT_RET)
		{
			EXTI->RTSR |= 1 << pGPIOHandle->GPIO_PinConfig.PinNumber;
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}

		else if(pGPIOHandle->GPIO_PinConfig.Mode == GPIO_MODE_IT_FET)
		{
			EXTI->FTSR |= 1 << pGPIOHandle->GPIO_PinConfig.PinNumber;
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}

		else if(pGPIOHandle->GPIO_PinConfig.Mode == GPIO_MODE_IT_RFET)
		{
			EXTI->FTSR |= 1 << pGPIOHandle->GPIO_PinConfig.PinNumber;
			EXTI->RTSR |= 1 << pGPIOHandle->GPIO_PinConfig.PinNumber;

		}

		//2. Select the appropriate port to pass through SYSCFG reg
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.PinNumber % 4;
		uint8_t portCode = GPIOtoPortCode(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] |= (portCode << (4 * temp2));

		//3. Enable the interrupt delivery on the MCU side
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.PinNumber;
	}

	temp = 0;

	temp = pGPIOHandle->GPIO_PinConfig.Otype;
	temp = (temp << (1 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (1 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->OTYPER |=temp;

	temp = 0;

	temp = pGPIOHandle->GPIO_PinConfig.Ospeed;
	temp = (temp << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDER |= temp;

	temp=0;

	temp = pGPIOHandle->GPIO_PinConfig.Pupd;
	temp = (temp << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |=temp;

	if(pGPIOHandle->GPIO_PinConfig.Mode == GPIO_MODE_ALTFN)
	{
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.PinNumber % 8;

		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= ((pGPIOHandle->GPIO_PinConfig.Altfn) << (4 * temp2));
	}
}


void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
			if(pGPIOx == GPIOA)
			{
				GPIOA_REG_RST();
			}
			else if(pGPIOx == GPIOB)
			{
				GPIOB_REG_RST();
			}
			else if(pGPIOx == GPIOC)
			{
				GPIOC_REG_RST();
			}
			else if(pGPIOx == GPIOD)
			{
				GPIOD_REG_RST();
			}
			else if(pGPIOx == GPIOE)
			{
				GPIOE_REG_RST();
			}
			else if(pGPIOx == GPIOF)
			{
				GPIOF_REG_RST();
			}
			else if(pGPIOx == GPIOG)
			{
				GPIOG_REG_RST();
			}
			else if(pGPIOx == GPIOH)
			{
				GPIOH_REG_RST();
			}
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Val)
{
	if(Val == 1)
	{
		pGPIOx->ODR |= 1 << PinNumber;
	}

	else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Val)
{
	pGPIOx->ODR = Val;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}


uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint16_t temp;

	temp = ((pGPIOx->IDR) >> PinNumber) & 0x0001;

	return temp;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t temp;

	temp = (pGPIOx->IDR);

	return temp;
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi) // Enable interrupt on MPU side
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber >=0 && IRQNumber < 32)
		{
			NVIC_ISER->ISER[0] |= (1 << IRQNumber);
		}

		else if(IRQNumber >=32 && IRQNumber < 64)
		{
			NVIC_ISER->ISER[1] |= (1 << (IRQNumber%32));
		}

		else if(IRQNumber >=64 && IRQNumber < 96)
		{
			NVIC_ISER->ISER[2] |= (1 << (IRQNumber%64));
		}
	}

	else if(EnorDi == DISABLE)
	{
		if(IRQNumber >=0 && IRQNumber < 32)
		{
			NVIC_ICER->ICER[0] |= (1 << IRQNumber);
		}

		else if(IRQNumber >=32 && IRQNumber < 64)
		{
			NVIC_ICER->ICER[1] |= (1 << (IRQNumber%32));
		}

		else if(IRQNumber >=64 && IRQNumber < 96)
		{
			NVIC_ICER->ICER[2] |= (1 << (IRQNumber%64));
		}
	}
}

void GPIO_IRQPriority(uint8_t IRQNumber, uint32_t Priority)
{
	uint8_t temp1 = IRQNumber / 4;
	uint8_t temp2 = IRQNumber % 4;

	NVIC_IPR->IPR[temp1] |= (Priority << ((8*temp2)+4));
	//NVICPR has 8 bits for setting priority of the associated IRQ. Out of this only the higher 4 bits actually set the value.
	//The lower 4 bits do not have any significance.
	//E.g: Lets say IRQ number is 0, then into IPR0, the priority value needs to be written into 7:4 and 3:0.
}

void GPIO_IRQHandle(uint8_t PinNumber)
{
	/*if(((EXTI->PR >> PinNumber)& (1) == 1))
	{
		EXTI->PR &= ~(1<< PinNumber);
	}*/
	if(((EXTI->PR >> PinNumber) & 0x1))
	{
		EXTI->PR |= 1 << PinNumber;
	}
}

uint8_t GPIOtoPortCode(GPIO_RegDef_t *pGPIOx)
{
	uint8_t PortCode = 0;
	if(pGPIOx == GPIOA){PortCode =  0;}
	else if(pGPIOx == GPIOB){PortCode = 1;}
	else if(pGPIOx == GPIOC){PortCode = 2;}
	else if(pGPIOx == GPIOD){PortCode = 3;}
	else if(pGPIOx == GPIOE){PortCode = 4;}
	else if(pGPIOx == GPIOF){PortCode = 5;}
	else if(pGPIOx == GPIOG){PortCode = 6;}
	else if(pGPIOx == GPIOH){PortCode = 7;}

	return PortCode;
}
