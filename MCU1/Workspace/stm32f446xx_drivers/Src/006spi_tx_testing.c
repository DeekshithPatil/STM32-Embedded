/*
 * 006spi_tx_testing.c
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
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins);

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
	SPI2Handle.SPIConfig.SPI_SclSpeed = SPI_SCLK_SPEED_DIV16; //generates sclk of 8 MHz
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN; //SOftware slave management enabled for NSS

	SPI_Init(&SPI2Handle);

}


int main(void)
{

	char user_data[] = "Hello world!";
	//This function is used to initialize the GPIO pins as SPI2
	SPI2_GPIOInits();

	//This function is used to intialise the SPI2 parameters
	SPI2_Inits();

	//This makes NSS Signal internally high and avoid MODF Error
	SPI_SSIConfig(SPI2, ENABLE);

	//Enable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, ENABLE);


	//This function is used to transmit the data
	SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

	//lets confirm SPI is not busy
	while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );

	//Enable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, DISABLE);


	while(1);

	return 0;
}
