/*
 * 007spi_tx_testing.c
 *
 *  Created on: 06-Mar-2021
 *      Author: DELL
 */

#include "stm32f446xx.h"
#include <string.h>
/*
 * SPI2:
 * NSS ->  P^B12
 * SCLK -> P^B13
 * MISO -> P^B14
 * MOSI -> P^B15
 * Alternate functionality mode: 5
 */

void delay(void)
{
	uint32_t i;

	for(i=0;i<1000000/2;i++)
	{

	}
}

void SPI2_GPIOInits()
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5; //Refering data sheet, Port B pins 12, 13, 14,15, need to be initialised with ALTFN =5
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; //Push pull is default
	SPIPins.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//COnfigure NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

	//Configure SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//Configure MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//Configure MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits()
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclSpeed = SPI_SCLK_SPEED_DIV32; //generates sclk of 2 MHz (16/8)
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI; //Hardware slave management enabled for NSS

	SPI_Init(&SPI2Handle);

}

void GPIO_ButtonInit()
{
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOC;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; //Output type does not matter when pin is configured as input
	GPIOBtn.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD; //There already exists an external pull down


	GPIO_PeriClockControl(GPIOC, ENABLE);

	GPIO_Init(&GPIOBtn);

}


int main(void)
{

	char user_data[] = "Hello! This code is working fine";

	GPIO_ButtonInit();

	//This function is used to initialize the GPIO pins as SPI2
	SPI2_GPIOInits();

	//This function is used to intialise the SPI2 parameters
	SPI2_Inits();

	//NOTE: SSI is needed only in software slave management mode (SSM). But in hardware mode SSOE need to enabled
	SPI_SSOEConfig(SPI2,ENABLE);

	while(1)
	{

		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//First let's send length information
		uint8_t dataLength = strlen(user_data);
		SPI_SendData(SPI2, &dataLength, sizeof(dataLength));

		//This function is used to transmit the data
		SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

		//lets confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );

		//Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);

	}


	return 0;
}
