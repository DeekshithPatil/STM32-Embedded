/*
 * 007spi_tx_testing.c
 *
 *  Created on: 06-Mar-2021
 *      Author: DELL
 */

#include "stm32f446xx.h"
#include <string.h>
#include <stdio.h>


/*
 * SPI2:
 * NSS ->  P^B12
 * SCLK -> P^B13
 * MISO -> P^B14
 * MOSI -> P^B15
 * Alternate functionality mode: 5
 */

//Command code
#define COMMAND_LED_CTRL					0x50
#define COMMAND_SENOSR_READ					0x51
#define COMMAND_LED_READ					0x52
#define COMMAND_PRINT						0x53
#define COMMAND_ID_READ						0x54

#define LED_ON								1
#define LED_OFF								0

//Arduino Analog pins
#define ANALOG_PIN0							0
#define ANALOG_PIN1							1
#define ANALOG_PIN2							2
#define ANALOG_PIN3							3
#define ANALOG_PIN4							4

//Arduino Led pin
#define LED_PIN								13

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
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

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

uint8_t SPI_VerifyResponse(uint8_t ackByte)
{
	if(ackByte == 0xF5)
	{
		//ack
		return 1;
	}

	return 0;
}

uint8_t dummyWrite = 0xFF;
uint8_t dummyRead;
uint8_t toggleCounter = 0;
uint8_t ackByte;
uint8_t args[2];
uint8_t commandCode;

void sendLedCtrl();
void sendSensorRead();
void sendLedRead();
void sendPrint();
void readID();

int main(void)
{
	// call the extern function and enable semi-hosting for printf style debugging
	printf("Started program\n");

	GPIO_ButtonInit();

	//This function is used to initialize the GPIO pins as SPI2
	SPI2_GPIOInits();

	//This function is used to intialise the SPI2 parameters
	SPI2_Inits();

	//NOTE: SSI is needed only in software slave management mode (SSM). But in hardware mode SSOE need to enabled
	SPI_SSOEConfig(SPI2,ENABLE);

	while(1)
	{
		printf("Waiting for button press\n");
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect


		//Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL	<pinno (1)>		<value (1)>
		sendLedCtrl();

		//Wait till button is pressed
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//2. CMD_SENSOR_READ	<pinno (1)>
		sendSensorRead();

		//Wait till button is pressed
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//3. CMD_LED_READ		<pinno (1)>
		sendLedRead();

		//Wait till button is pressed
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//4. CMD_PRINT	<len (1)>	<message (len)>
		sendPrint();

		//Wait till button is pressed
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//5. CMD_ID_READ: Read 10 bytes of board ID
		readID();


		//lets confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );

		//Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);

	}


	return 0;
}

void sendLedCtrl()
{
	commandCode = COMMAND_LED_CTRL;

	//Send command
	SPI_SendData(SPI2,&commandCode,1);
	//Do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummyRead, 1);

	//Send some dummy byte to fetch the response from the slave
	SPI_SendData(SPI2,&dummyWrite,1);
	//Read the ack byte
	SPI_ReceiveData(SPI2, &ackByte, 1);

	if(SPI_VerifyResponse(ackByte))
	{
		args[0] = LED_PIN;
		args[1] = (toggleCounter%2 == 0)?LED_ON:LED_OFF;

		SPI_SendData(SPI2, args,2);
		SPI_ReceiveData(SPI2, args, 2);

		toggleCounter++;
	}

	printf("Completed COMMAND_LED_CTRL\n");

}

void sendSensorRead()
{
	commandCode = COMMAND_SENOSR_READ;
	uint8_t analog_read;
	//Send command
	SPI_SendData(SPI2,&commandCode,1);
	//Do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummyRead, 1);

	//Send some dummy byte to fetch the response from the slave
	SPI_SendData(SPI2,&dummyWrite,1);
	//Read the ack byte
	SPI_ReceiveData(SPI2, &ackByte, 1);

	if(SPI_VerifyResponse(ackByte))
	{
		args[0] = ANALOG_PIN0;

		SPI_SendData(SPI2, args,1);
		SPI_ReceiveData(SPI2, args, 1); //Dummy read

		//Give a small delay to allow the Arduino to read ADC value and then place onto to its tx buffer
		delay();

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummyWrite,1);

		SPI_ReceiveData(SPI2,&analog_read,1);


	}

	printf("Completed COMMAND_SENOSR_READ. Value= %d\n",analog_read);
}

void sendLedRead()
{
	commandCode = COMMAND_LED_READ;
	uint8_t digital_read;

	//Send command
	SPI_SendData(SPI2,&commandCode,1);
	//Do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummyRead, 1);

	//Send some dummy byte to fetch the response from the slave
	SPI_SendData(SPI2,&dummyWrite,1);
	//Read the ack byte
	SPI_ReceiveData(SPI2, &ackByte, 1);

	if(SPI_VerifyResponse(ackByte))
	{
		args[0] = LED_PIN;

		//Send the pin no to read from
		SPI_SendData(SPI2, args,1);
		SPI_ReceiveData(SPI2, args, 1);

		//wait till Arduino performs a digital read on the requested pin number
		delay();

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummyWrite,1);

		SPI_ReceiveData(SPI2,&digital_read,1);

	}
	printf("Completed COMMAND_LED_READ. Value = %d\n", digital_read);

}

void sendPrint()
{
	commandCode = COMMAND_PRINT;
	char tx_data[] = "Hi!";
	uint8_t * tx_data_pointer = (uint8_t *)tx_data;
	uint8_t len = sizeof(tx_data);

	//Send command
	SPI_SendData(SPI2,&commandCode,1);
	//Do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummyRead, 1);

	//Send some dummy byte to fetch the response from the slave
	SPI_SendData(SPI2,&dummyWrite,1);
	//Read the ack byte
	SPI_ReceiveData(SPI2, &ackByte, 1);

	if(SPI_VerifyResponse(ackByte))
	{
		//Transmit the length of data
		SPI_SendData(SPI2, &len,1);
		SPI_ReceiveData(SPI2, &dummyRead, 1); //Dummy read

		//Data
		//Read one dummy byte for every byte transmitted.
		//Repeat this for "length of tx_data" times
		for(int i=0; i<len;i++)
		{
			SPI_SendData(SPI2, tx_data_pointer, 1); //convert the base address of tx_data to uint8_t
			SPI_ReceiveData(SPI2,&dummyRead,1); //Dummy read
			//Point to the next character in tx_data
			tx_data_pointer++;
		}

		//Give some time to print the data
		delay();


	}

	printf("Completed COMMAND_PRINT\n");

}

void readID()
{
	commandCode = COMMAND_ID_READ;
	uint8_t idLen =10;
	uint8_t arduinoID[idLen +1]; // +1 is to append null character
	uint8_t *pointerToID = arduinoID;
	//Send command
	SPI_SendData(SPI2,&commandCode,1);
	//Do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummyRead, 1);

	//Send some dummy byte to fetch the response from the slave
	SPI_SendData(SPI2,&dummyWrite,1);
	//Read the ack byte
	SPI_ReceiveData(SPI2, &ackByte, 1);

	if(SPI_VerifyResponse(ackByte))
	{
		for(int i=0; i<idLen; i++)
		{
			//Transmit a dummy byte
			SPI_SendData(SPI2,&dummyWrite,1);

			//Read the data
			SPI_ReceiveData(SPI2,pointerToID,1);

			//Point to the next element
			pointerToID++;


		}

		//append null character
		arduinoID[11] = '\0';

		delay();

	}

	printf("Completed COMMAND_ID_READ\n");

}
