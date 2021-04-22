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

uint8_t rcv_buf[100];

uint8_t rxComplt = RESET; //Used to check if Reception is complete or not

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
	uint8_t commandcode;
	uint8_t len;

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


	//Enable the I2C Peripheral
	I2C_PeripheralControl(I2C1Handle.pI2Cx,ENABLE);

	//Enable ACK
	I2C_ManageAcking(I2C1Handle.pI2Cx,ENABLE);

	while(1)
	{
		memset(rcv_buf,0,sizeof(rcv_buf));
		printf("\nWaiting for Button to be pressed \n");
		//Wait till button is pressed
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)){} //Wait until button is pressed
		delay(); //To avoid debounce effect

		//Send the command code to read len to the slave
		commandcode = 0x51;
		//I2C_MasterSendData(&I2C1Handle,TxData,strlen((char *)TxData),SLAVE_ADDRESS);
		while(I2C_MasterSendDataIT(&I2C1Handle,&commandcode,1,SLAVE_ADDRESS,I2C_ENABLE_SR) != I2C_READY); //Wait until I2C state is READY

		//Receive len
		while(I2C_MasterReceiveDataIT(&I2C1Handle,&len,1,SLAVE_ADDRESS,I2C_ENABLE_SR)!= I2C_READY);

		printf("\nReceived Length = %d\n",len);

		//send the command code to read data
		commandcode = 0x52;
		while(I2C_MasterSendDataIT(&I2C1Handle,&commandcode,1,SLAVE_ADDRESS,I2C_ENABLE_SR)!= I2C_READY);

		//read the data
		while(I2C_MasterReceiveDataIT(&I2C1Handle,rcv_buf,21,SLAVE_ADDRESS,I2C_ENABLE_SR)!= I2C_READY);

		rxComplt = RESET;

		//Wait till rxComplt is set
		while(rxComplt != SET);

		for(int i=0;i<len;i++)
		{
			printf("%d %c\n",i,rcv_buf[i]);
		}

		printf("\n");

		rxComplt = RESET;
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
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		printf("Tx is completed\n");
	}
	else if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("Rx is completed\n");
		rxComplt = SET;
	}
	else if(AppEv == I2C_ERROR_AF)
	{
		printf("Error: Ack failure\n");
		//In master ack failure happens when slave fails to send ack for the byte
		I2C_CloseSendData(pI2CHandle);

		//Generate the stop condition to release the bus
		I2C_GenerateStopCondition(I2C1);

		//Hand in infinite loop
		while(1); //Just random one, can be processed differently if needed
	}
}
