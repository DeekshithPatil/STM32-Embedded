/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: 12-Mar-2021
 *      Author: DELL
 */

#include "stm32f446xx_spi_driver.h"

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}

		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}

		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}

	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}

		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}

		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

uint8_t SPI_CheckFlagStatus(SPI_RegDef_t *pSPIx, uint8_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	else
	{
		return FLAG_RESET;
	}
}

void SPI_PeriControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= 1 << SPI_CR1_SPE_BITPOS;
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE_BITPOS);
	}
}

void SPI_SSIControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= 1 << SPI_CR1_SSI_BITPOS;
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI_BITPOS);
	}
}

void SPI_SSOEControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR2 |= 1 << SPI_CR2_SSOE_BITPOS;
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE_BITPOS);
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//Enable Peripheral clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg=0;

	//1. Configure the mode
	if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DeviceMode_FD)
	{
		//Clear BIDIMODE bits
		tempreg &= ~(0x1 << SPI_CR1_BIDIMODE_BITPOS);
	}

	else if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DeviceMode_FD)
	{
		//Set BIDIMODE bits
		tempreg |= (0x1 << SPI_CR1_BIDIMODE_BITPOS);
	}

	else if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DeviceMode_SimplexRxOnly)
	{
		//Clear BIDIMODE
		tempreg &= ~(0x1 << SPI_CR1_BIDIMODE_BITPOS);

		//Set RXONLY bit
		tempreg |= 1 << SPI_CR1_RXONLY_BITPOS;
	}

	//2. Bus config (Master / Slave)
	tempreg |= pSPIHandle->SPIConfig.SPI_BusConfig << SPI_CR1_MSTR_BITPOS;

	//3. SCLK Speed
	tempreg |= pSPIHandle->SPIConfig.SPI_SclSpeed <<  SPI_CR1_BR_BITPOS;

	//4.Data Frame Format
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF_BITPOS;

	//5. CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA_BITPOS;

	//6. CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL_BITPOS;

	//7. SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM_BITPOS;

	pSPIHandle->pSPIx->CR1 |= tempreg;
}

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
			{
				SPI1_REG_RST();
			}

			else if(pSPIx == SPI2)
			{
				SPI2_REG_RST();
			}

			else if(pSPIx == SPI3)
			{
				SPI3_REG_RST();
			}
}


void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len>0)
	{
		while(!(SPI_CheckFlagStatus(pSPIx, TXE_FLAG))){}

		if(pSPIx->CR1 & 1<< SPI_CR1_DFF_BITPOS)
		{
			//16 bit transfer mode

			pSPIx->DR = *((uint16_t *)pTxBuffer);

			Len--;
			Len--;

			pTxBuffer++;
			pTxBuffer++;
		}

		else
		{
			//8 bit transfer mode
			pSPIx->DR = *pTxBuffer;

			Len--;
			pTxBuffer++;
		}
	}
}

