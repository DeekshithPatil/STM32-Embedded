/*
 * stm32f446xx_gpio_driver.c
 *
 *  Created on: 23-Jan-2021
 *      Author: DELL
 */

#include "stm32f446xx_gpio_driver.h"

/*
 * Peripheral Clock Setup
 */
/*****************************************************************************
 * @fn 				- GPIO_PeriClockControl
 *
 * @brief			- This function enables or disbales peripheral clock for the given GPIO port
 *
 * @param[in]		- base address of the GPIO peripheral
 * @param[in]		- Enable or Disable macros in MCU specific header file
 * @param[in]		-
 *
 * @return			- none
 *
 * @Note			- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
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


/*
 * Init and De-init
 */
/*
 * Peripheral Clock Setup
 */
/*****************************************************************************
 * @fn 				- GPIO_Init
 *
 * @brief			- This function initialises a gpio port
 *
 * @param[in]		- gpio handle structure
 * @param[in]		-
 * @param[in]		-
 *
 * @return			- none
 *
 * @Note			- none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp=0;

	//Enable the GPIO Peripheral clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
	//1. configure the mode of gpio pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) //Which means it is working in non-interrupt mode
	{
		//the non-interrupt mode
		temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Each pin takes 2 bit fields in the GPIO mode register. That's why pinNumber is multiplied by 2
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); //Clearing. Note: 0x3 corresponds to 1 in binary, thus clears those two fields only
		pGPIOHandle->pGPIOx->MODER |= temp; //setting

	}
	else
	{
		// This is the interrupt mode

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1. Configure the Falling Trigger Selection Register (FTSR)
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}

		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1. Configure the Rising Trigger Selection Register (RTSR)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);


		}

		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//1. Configure both FTSR and RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. Configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);

		//3. Enable the EXTI interrupt delivery using IMR (Interrupt mask register)
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	  }

	temp = 0;
	//2. Configure the speed
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); //Clearing
	pGPIOHandle->pGPIOx->OSPEEDER |= temp; //Setting

	temp=0;
	//3. configure the pupd settings
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPupdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp=0;
	//4. configure the output type
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (1 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Each pin takes 1 bit field in the register
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (1 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	temp=0;

	//5. configure the alternate functionality: There are two alternate function registers, low and high i.e AFR[0] and AFR[1] respectively
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//configure the alternate function registers
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8; //This is an integer division and is used to decide whether the low register or high register needs to be selected based on the pin number

		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8; //This is modulo operation and is used to select which particular section (4 bits) of the 32 bit register needs to be written into

		//E.g 8/8 = 1 and 8%8 =0, therefore, value needs to be written into high register (result of '/' operation) and in the 0th section (the first 4 bits)
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2);
	}

}

/*****************************************************************************
 * @fn 				- GPIO_DeInit
 *
 * @brief			- This function De-initialises a gpio port
 *
 * @param[in]		- gpio regdef
 * @param[in]		-
 * @param[in]		-
 *
 * @return			- none
 *
 * @Note			- We are resetting to default values using the register available in RCC and not directly hardcoding the reset values into the registers
 * The register used to reset here is RCC_AHB1RSTR. This is used to reset peripherals lying on AHB1 bus.
 * @Note			- The particular bit field needs to be set and then reset, so that it doesnt keep writing default values into the register
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}

}


/*
 * Data read and write
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001) ;

	return value;

}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;

}

void GPIO_WriteToOuputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{

	if(Value == GPIO_PIN_SET) //GPIO_PIN_SET macro is defined in MCU specific header and its value is 1
	{
		//write 1 at the bit field corresponding to the pin number
		pGPIOx->ODR |= 1 << PinNumber;
	}
	else
	{
		//write 0 at the bit field corresponding to the pin number
		pGPIOx->ODR &= ~(1 << PinNumber);
	}


}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= 1<<PinNumber;
}


/*
 * IRQ configuration and ISR handling
 */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	//This function is written to take care of interrupt configuration on the processor side i.e NVIC registers

   if (EnorDi == ENABLE)
   {
	   if(IRQNumber<=31)
	   {
		   // Program ISER0 register
		   *NVIC_ISER0 |= (1 << IRQNumber);
	   }
	   else if(IRQNumber>31 && IRQNumber<64)
	   {
		   // Program ISER1 register
		   *NVIC_ISER1 |= (1 << (IRQNumber % 32));
	   }
	   else if(IRQNumber >= 64 && IRQNumber < 96)
	   {
		   // Program ISER2 register
		   *NVIC_ISER2 |= (1 << (IRQNumber % 32));
	   }

   }
   else
   {
	   if(IRQNumber<=31)
	   	   {
	   		   // Program ICER0 register
		   	   *NVIC_ICER0 |= (1 << IRQNumber);
	   	   }
	   	   else if(IRQNumber>31 && IRQNumber<64)
	   	   {
	   		   // Program ICER1 register
			   *NVIC_ICER1 |= (1 << (IRQNumber % 32));

	   	   }
	   	   else if(IRQNumber >= 64 && IRQNumber < 96)
	   	   {
	   		   // Program ICER2 register
			   *NVIC_ICER2 |= (1 << (IRQNumber % 32));

	   	   }
   }

}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t Priority)
{
	uint8_t temp1 = IRQNumber / 4;
	uint8_t temp2 = IRQNumber % 4;

	NVIC_IPR->IPR[temp1] |= (Priority << ((8*temp2)+4));
	//NVIC_PR has 8 bits for setting priority of the associated IRQ. Out of this only the higher 4 bits actually set the value.
	//The lower 4 bits do not have any significance.
	//E.g: Lets say IRQ number is 0, then into IPR0, the priority value needs to be written into 7:4 and 3:0.
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	//Clear the pending register
			//EXTI->PR |= (1 << PinNumber); // the procedure to clear the PR register is to write 1
	//clear the exti pr register corresponding to the pin number
	if((EXTI->PR & (1 << PinNumber)))
	{
		//Clear the pending register
		EXTI->PR |= (1 << PinNumber); // the procedure to clear the PR register is to write 1
	}
	/*if(((EXTI->PR >> PinNumber) & 0x1))
	{
		EXTI->PR |= 1 << PinNumber;
	}*/
}

