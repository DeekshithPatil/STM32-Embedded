/*
 * stm32f446xx_gpio_driver_test.c
 *
 *  Created on: 07-Feb-2021
 *      Author: DELL
 */

#include "stm32f446xx_gpio_driver_test.h"

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
	uint8_t temp;

	temp = ((pGPIOx->IDR) >> PinNumber);
	return temp;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t temp;

	temp = (pGPIOx->IDR);

	return temp;
}


