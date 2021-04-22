/*
 *
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
#define SLAVE_ADDRESS					0x69
#define MY_ADDRESS						SLAVE_ADDRESS

uint8_t commandCode = 0;
uint8_t Cnt=0;

I2C_Handle_t I2C1Handle;

uint8_t tx_buf[32] = "STM32 Slave Mode Testing...";


void delay(void)
{
	uint32_t i;

	for(i=0;i<1000000/2;i++)
	{

	}
}

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

	//I2C IRQ Configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV,ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ERR,ENABLE);


	//Note, let the interrupt's priority is not configured. Thus, they are left at their default values

	//The below function is used to enable and disable I2C interrupts
	I2C_SlaveEnableDisableCallbackEvents(I2C1Handle.pI2Cx, ENABLE);

	//Enable the I2C Peripheral
	I2C_PeripheralControl(I2C1Handle.pI2Cx,ENABLE);

	//Enable ACK
	I2C_ManageAcking(I2C1Handle.pI2Cx,ENABLE);

	while(1)
	{

	}


	return 0;
}

void I2C1_EV_IRQHandler(void)
{
	//Call the I2C event IRQ Handling
	I2C_EV_IRQHandling(&I2C1Handle);

}

void I2C1_ER_IRQHandler(void)
{
	//Call the I2C error IRQ Handling
	I2C_ERR_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{
	if(AppEv == I2C_EV_SLAVE_DATA_REQ)
	{
		//Master wants some data, slave has to send it
		if(commandCode == 0x51)
		{
			//send the length information to the master
			I2C_SlaveSendData(pI2CHandle->pI2Cx,strlen((char*)tx_buf));

		}
		else if(commandCode == 0x52)
		{
			//Send the contents of tx_buf
			I2C_SlaveSendData(pI2CHandle->pI2Cx,tx_buf[Cnt++]);

		}
	}

	else if(AppEv == I2C_EV_SLAVE_DATA_RCV)
	{
		//Slave has received data and is ready to be read
		commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
	}

	else if(AppEv == I2C_ERROR_AF)
	{
		//This happens only during slave transmitting
		//Master has sent the NACK, sho slave should understand that master doesnt need more data
		commandCode = 0xFF; //Invalildating it.
		Cnt = 0;
	}

	else if(AppEv == I2C_EV_STOP)
	{
		//This happens only during slave reception
		//Master has ended the I2C communication with the slave
	}


}
