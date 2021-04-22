/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: 27-Mar-2021
 *      Author: DELL
 */

#include <stdio.h>
#include <string.h>
#include "stm32f446xx.h"

/*
 * I2C1: Port B, ALternate Function number 4
 * SCL: PB^6 / PB^8 --> PB^6 is selected
 * SDA: PB^7 / PB^9 --> PB^9 is selected
 */
#define MY_ADDRESS						0x61
#define SLAVE_ADDRESS					0x68

I2C_Handle_t I2C1Handle;

uint8_t TxData[] = "We are testing I2C Master Tx\n"; //I2C wire.h library used in Arduino side does not take more than 32 bytes in one transaction.
//Therefore, make sure that the above data is not above 32 bytes



void I2C1_GPIOInits()
{
	GPIO_Handle_t I2CPins;
	memset(&I2CPins,0,sizeof(I2CPins));
	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;

	//SCL = PB^6;
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	//SDA = PB^9
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&I2CPins);
}

void I2C1_Inits()
{
	memset(&I2C1Handle,0,sizeof(I2C1Handle));

	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDRESS; //This doesnt matter as our device is master. Address is only for slave.
	//Also refer to table 3 of specification to obtain some of the reserved addresses
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2; //This too doesnt matter as we are using in Standard mode and not Fast mode

	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM; //SCL speed is set to standard mode. i.e 100 KHz

	I2C_Init(&I2C1Handle);

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

	//This function is used to Configure the button as input
	GPIO_ButtonInit();

	//This function is used to configure the GPIO to the required alternate funciton mode to perform as I2C1 pins
	I2C1_GPIOInits();

	//This function is used to initialise I2C function in Standard mode
	I2C1_Inits();

	//Enable the I2C Peripheral
	I2C_PeripheralControl(I2C1Handle.pI2Cx,ENABLE);



	while(1)
	{
		//Wait till button is pressed
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//Send the data to the slave
		I2C_MasterSendData(&I2C1Handle,TxData,strlen((char *)TxData),SLAVE_ADDRESS);
	}




	return 0;
}
