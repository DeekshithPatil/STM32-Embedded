/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: 23-Mar-2021
 *      Author: DELL
 */

#include "stm32f446xx_i2c_driver.h"


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_ClearADDRFlagIT(I2C_Handle_t *pI2CHandle);

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);

static void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	//To generate START condition, the START bit in the CR1 field should be set. It is cleared by the hardware when START signal has been sent or when PE=0
	pI2Cx->CR1 |= 1 << I2C_CR1_START;
}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx) //prototype declared in header file
{
	//To generate STOP condition, the STOP bit in the CR1 field should be set. It is cleared by the hardware when START signal has been sent or when PE=0
	pI2Cx->CR1 |= 1 << I2C_CR1_STOP;
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1; //Slave address is 7 bits (7:1)

	SlaveAddr &= ~(1); //Clear the last bit. Meaning Write mode

	pI2Cx->DR = SlaveAddr;
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1; //Slave address is 7 bits (7:1)

	SlaveAddr |= (1); //Set the last bit. Meaning Read mode

	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}

static void I2C_ClearADDRFlagIT(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummyRead;
	//check for device mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		//device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxLen == 1)
			{
				//first disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx,DISABLE);

				//clear the ADDR flag (read SR1, read SR2)
				dummyRead = pI2CHandle->pI2Cx->SR1;
				dummyRead = pI2CHandle->pI2Cx->SR2;
				(void)dummyRead;
			}
			else
			{
				//clear the ADDR flag (read SR1, read SR2)
				dummyRead = pI2CHandle->pI2Cx->SR1;
				dummyRead = pI2CHandle->pI2Cx->SR2;
				(void)dummyRead;
			}
		}
	}
	else
	{
		//device is in slave mode
		//clear the ADDR flag (read SR1, read SR2)
		dummyRead = pI2CHandle->pI2Cx->SR1;
		dummyRead = pI2CHandle->pI2Cx->SR2;
		(void)dummyRead;
	}
}



/*
 * This function is used to Enable or Disable I2C peripheral clock
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}

	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}

		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}

	    else if(pI2Cx == I2C3)
		{
	    	I2C2_PCLK_DI();
		}
	}
}



/*
 * Used to enable or disable ACK bit
 */
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		//Enable acking
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}

	else
	{
		//Disable ACK
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	}
}

/*
 * Used to get the APB1 Pclk value
 */
//static uint32_t RCC_GetPCLK1Value(void)
//{
//	uint32_t pclk1, SystemClk;
//	uint8_t clksrc, temp;
//	uint8_t ahbp,apb1p; //prescalers for ahb and apb1
//
//	// APB1 Pclk (freq) = Freq of (HSI/HSE/PLL/PLL_R, whichever is selected) / (AHBPrescaler * APB1Prescaler)
//
//	//1. Obtain system clk value
//	//Refer to RCC_CFGR in reference manual to obtain more info
//	//SWS[1:0] (System clock switch status): Indicates which clock source is as the system clock
//
//	clksrc = (RCC->CFGR >> 2) & 0x3; //obtain values of bit number 2 and 3
//	if(clksrc == 0)
//	{
//		SystemClk = 16000000; //Internal clock source is of 16MHz
//	}
//	else if(clksrc == 1)
//	{
//		SystemClk = 8000000; //External clock frequency in case of Nucleo boards comes from STLink circuitry, which is 8MHz
//	}
//	else if(clksrc == 2)
//	{
//		SystemClk = RCC_GetPLLClockOutput();
//	}
//
//	//2. Obtain AHB pre-scalar value
//	//Refer to HPRE bit fields in RCC_CFGR register
//	temp = (RCC->CFGR >> 4) & 0xF; //Obtain HPRE value by masking
//	if(temp < 8)
//	{
//		ahbp = 1; //NO division
//	}
//	else
//	{
//		ahbp = AHB_PreScaler[temp-8]; //use temp (which is greater than 8) as index to obtain ahb pre-scaler value from array
//	}
//
//	//3. Obtain the APB1 pre-scalar
//	//Refer to the PPRE bit fields in RCC_CFGR register
//	temp = (RCC->CFGR >> 10) & 0x7;
//
//	if(temp<7)
//	{
//		apb1p = 1;
//	}
//	else
//	{
//		apb1p = APB1_PreScaler[temp - 4];
//	}
//
//	//4. Calculate the value of pclk1
//	pclk1 = SystemClk / (apb1p * ahbp);
//	return pclk1;
//}

/*
 * I2C Init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg =0;

	//Enable the clock for the peripheral
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	//1. Configure the ACK bit in CR1
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK;
	pI2CHandle->pI2Cx->CR1 |= tempreg;


	//2. Configure the FREQ bit fields in CR2 with APB1 Pclk value
	// APB1 Pclk (freq) = Freq of (HSI/HSE/PLL/PLL_R, whichever is selected) / (AHBPrescaler * APB1Prescaler)
	tempreg=0;
	tempreg = (RCC_GetPCLK1Value() / 1000000) << I2C_CR2_FREQ;

	pI2CHandle->pI2Cx->CR2 |= tempreg & 0x3F; //Mask all the bits except the first six (0 to 5) bits

	//3. Store the slave address in the OAR1 register
	//If ADDMODE field is 0 it means the slave address is 7 bit, else (if it is 1) slave address in 10bit.
	//Accordingly, slave address is stored from 7:1 (7 bits) or 9:0 (10 bits)
	tempreg=0;
	tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << 1; //Only 7 bit address is covered in this
	pI2CHandle->pI2Cx->OAR1 |= tempreg;
	//The reference manual states that the reserved bit 14 should always be kept at 1 by the software
	pI2CHandle->pI2Cx->OAR1 |= 1 << 14;

	//4. Program CCR register
	uint16_t ccrValue = 0;
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Standard mode
		//In standard mode, the 15th bit(F/S) of CCR register should be configured to 0. since ccrValue is initialised to 0, we need not do it.

		//For the formula below, refer to the lecture 190. or notes of lecture 190.
		ccrValue =  RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		tempreg |= (ccrValue & 0xFFF); //Mask out all other bits apart from the first 12 bits;

	}

	else
	{
		//Fast mode
		//1. Set bit number 15 as 1. It indicates that the mode is Fast Mode
		tempreg |= 1 << I2C_CCR_F_S;

		//2. Program the duty cycle
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY);

		//Refer to lecture number 190 for the below formulas
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccrValue =  RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		else
		{
			ccrValue =  RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}

		tempreg |= (ccrValue & 0xFFF); //Mask out all other bits apart from the first 12 bits;
	}

	pI2CHandle->pI2Cx->CCR |= tempreg;

	//5. Program the TRISE register
	//Refer to lecture number 198 or it's notes for the below formula
	tempreg=0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Mode is standard mode

		tempreg = (RCC_GetPCLK1Value() / 1000000) + 1; //Trise = 1000 nS = 1uS. Thus we have divided by 1M instead of multiplying by 1u.

	}
	else
	{
		//Mode is fast mode
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000) +1; //Trise = 300ns. Therefore multiply by 300 and divide by 10^9
	}

	pI2CHandle->pI2Cx->TRISE |= (tempreg & 0x3F); //Mask all other except 	5:0 bits
}


/*
 * I2C De-Init
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}

	else if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}

	else if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}


/*
 * Enable of disable I2C peripheral from I2C Control register 1
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR1 |=  (1 << I2C_CR1_PE);
	}else
	{
		pI2Cx->CR1 &=  ~(1 << I2C_CR1_PE);
	}
}

/*
 * Send data (Blocking)
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1.
	//NOTE: Until SB is cleared SCL will be stretched
	while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB) == FLAG_RESET);
	//The procedure to clear the SB blag is read SR1(which is already done by the above command) and write the address into DR

	//3. Send the address of the slave with r/nw bit set to 0(write) (total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Confirm that the address phase is completed by checking the ADDR flag in the SR1
	while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR) == FLAG_RESET);

	//5. clear the ADDR flag according to its software sequence
	//The procedure to clear ADDR flag is read SR1 and SR2, or by hardware when PE=0
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. Send the data until len becomes 0
	while(Len > 0)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE)); //Wait till TXE is set
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	//7. When Len = 0, wait for TXE=1, and BTF=1 before generating the stop condition
	//NOTE: TXE=1, BTF=1, means that both SR and DR are empty and next transmission should begin
	//When BTF=1, SCL will be stretched (Pulled to LOW)

	while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) == FLAG_RESET); //Wait till TXE is set
	while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) == FLAG_RESET); //Wait till BTF is set

	//8. Generate the stop condition
	//NOTE: Generating the STOP, automatically clears BTF
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	//1. Generate the start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Wait till SB is set
	while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB) == RESET){}

	//3. Execute address phase with read mode.
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Wait till ADDR=1 i.e confirm if address phase has been completed and ACK is received
	while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR) == FLAG_RESET);

	//5. Start Reception
	//if len = 1, then ACK should be disabled right now and stop bit should be set after RXNE is set
	if(Len == 1)
	{
		//Disable ACK
		pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);

		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//Wait till RXNE is set
		while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) == FLAG_RESET){}
		//Program STOP condition
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//Read data into buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;

	}

	if(Len > 1)
	{
		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		while(Len>0)
		{
			//Wait till RXNE is set
			while(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) == FLAG_RESET){}

			//If length is 2, then stop condition must be generated in the following manner
			if(Len == 2)
			{
				//Clear the ack bit
				pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);

				//generate stop condition
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			}

			//Read data into buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//Increment RxBuffer address and decrement length
			pRxBuffer++;
			Len--;

		}

	}

	//6. re-enable acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}

}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
/*
 * Enable or disable I2C interrupt on the NVIC (Processor side)
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}

	else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}
}

/*
 * Interrupt priority config
 */
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t temp1 = IRQNumber / 4;
	uint8_t temp2 = IRQNumber % 4;

	NVIC_IPR->IPR[temp1] |= (IRQPriority << ((8*temp2)+4));
	//NVIC_PR has 8 bits for setting priority of the associated IRQ. Out of this only the higher 4 bits actually set the value.
	//The lower 4 bits do not have any significance.
	//E.g: Lets say IRQ number is 0, then into IPR0, the priority value needs to be written into 7:4 and 3:0.

}

/*
 * Rx and Transmit via Interrupts
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t peripheralState = pI2CHandle->TxRxState;

	if((peripheralState != I2C_BUSY_IN_RX) && (peripheralState != I2C_BUSY_IN_TX))
	{
		//Store the received variables into global variables (i.e Handle structure)
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Mark the Peripheral state as busy in Tx
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;

		//Generate Start condition, which sets the SB and inturn should trigger an Interrupt
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Enable ITBUFEN Control bit, to receive buffer based interrupts
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Enable ITEVTEN Control bit, to receive event based interrupts
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable ITERREN Control bit, to receive Error based interrupts
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return peripheralState;
}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t peripheralState = pI2CHandle->TxRxState;

	if((peripheralState != I2C_BUSY_IN_RX) && (peripheralState != I2C_BUSY_IN_TX))
	{
		//Store the received variables into global variables (i.e Handle structure)
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Mark the Peripheral state as busy in Rx
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;

		//Generate Start condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Enable ITBUFEN Control bit, to receive buffer based interrupts
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Enable ITEVTEN Control bit, to receive event based interrupts
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable ITERREN Control bit, to receive Error based interrupts
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return peripheralState;
}

/*
 * IRQ Handling for Event based and Error based Interrupts
 */

//Helper functions
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen > 0)
	{
		//1. load the data into DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		//2. Decrement the TxLen
		pI2CHandle->TxLen--;

		//3. Increment the buffer address
		pI2CHandle->pTxBuffer++;
	}
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	//We have to do the data reception
	if (pI2CHandle->RxLen == 1) {
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen--;
	} else if (pI2CHandle->RxLen > 1) {

		//If length is 2, then stop condition must be generated in the following manner
		if (pI2CHandle->RxLen == 2) {
			//Clear the ack bit
			pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);

		}

		//Read data into buffer
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;

		//Increment RxBuffer address and decrement length
		pI2CHandle->pRxBuffer++;
		pI2CHandle->RxLen--;
	}

	if (pI2CHandle->RxLen == 0) {
		//close the I2C data reception and notify the application

		//1. Generate the stop condition
		if (pI2CHandle->Sr == I2C_DISABLE_SR)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//2. reset all the member elements of the handle structure
		I2C_CloseReceiveData(pI2CHandle);

		//3. Notify the application about Transmission complete
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);

	}
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	//Interrupt handling function for both master and slave mode of a device
	uint32_t temp1, temp2, temp3;
	//Variable to hold the state of I2C_CR2_ITEVTEN, I2C_CR2_ITBUFEN
	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	//variable to hold different flag values
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB); //Check the SB flag

	//1. Handle for interrupt genertaed by SB event
	//Note: SB Flag is only applicable in master mode
	if(temp1 && temp3) //check I2C_CR2_ITEVTEN and SB
	{
		//SB flag is set and thus the interrupt has occured
		//In this block let's execute the address phase depending on RXTxState
		if(pI2CHandle->TxRxState ==  I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,pI2CHandle->DevAddr);

		}

		else if(pI2CHandle->TxRxState ==  I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,pI2CHandle->DevAddr);

		}


	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR); //Check the address flag
	//2. Handle for interrupt genertaed by ADDR event
	//NOte: When Master mode: Address is sent
	//		When Slave mode : Address matched with own address
	if(temp1 && temp3) //check I2C_CR2_ITEVTEN and ADDR
	{
		//ADDR flag is set
		I2C_ClearADDRFlagIT(pI2CHandle);

	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
	//3. Handle for Interrupt generated by BTF(Byte Transfer Finished) event
	if(temp1 && temp3) //check I2C_CR2_ITEVTEN and BTF
	{
		//BTF flag is set

		//In case of Transmission it means that the last byte of data has been received
		if(pI2CHandle->TxRxState ==  I2C_BUSY_IN_TX)
		{
			//make sure that TXE is also set
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE))
			{
				//Then it is confirmed that BTF and TXE, both are set
				if(pI2CHandle->TxLen == 0)
				{
					//1. generate the STOP condition based on repeated start if len =0
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					//2. reset all the member elements of the handle structure
					I2C_CloseSendData(pI2CHandle);

					//3. Notify the application about Transmission complete
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}
		}

		else if(pI2CHandle->TxRxState ==  I2C_BUSY_IN_RX)
		{
			//Nothing to do here
		}

	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
	//4. Handle for interrupt generated by STOPF event
	//NOTE: Stop detection flag is applicable only in slave mode
	if(temp1 && temp3) //check I2C_CR2_ITEVTEN and STOPF
	{
		//STOPF flag is set
		//Clear the STOPF by its software sequence i.e Read SR1 and write to CR1
		//Read is done already in temp3 above
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//Notify the application that STOP is detected
		I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_STOP);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);
	//5. Handle for interrupt generated by TXE event
	if(temp1 && temp2 && temp3) //check I2C_CR2_ITEVTEN,I2C_CR2_ITBUFEN and TXE
	{
		//TXE flag is set
		//Indicates the data register is empty, therefore write into data register

		//Check if the device is master
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{

			//Do the transmission only if I2C State is Busy in Tx
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE))
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}
		else
		{
			//Slave mode

			//Check the TRA bit to understand if the device is in Transmitter mode
			//TRA bit is influenced by the Read/Write bit of the address phase
			//TRA = 1 indicates that device is in transmitter mode
			if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
			{
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_SLAVE_DATA_REQ);
			}
		}

	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);
	//6. Handle for interrupt generated by RXNE event
	if(temp1 && temp2 && temp3) //check I2C_CR2_ITEVTEN,I2C_CR2_ITBUFEN and RXNE
	{
		//Check if the device is master
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			//Device is master
			//RXNE flag is set
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}
		else
		{
			//Device is slave
			//Check if TRA=0, that device is in receiver mode

			if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_SLAVE_DATA_RCV);
			}
		}

	}

}

//Helper functions to close Reception and Transmission
static void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable the ITBUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;

	//Enable Acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
		I2C_ManageAcking(pI2CHandle->pI2Cx,ENABLE);
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable the ITBUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}

/*********************************************************************
 * @fn      		  - I2C_ER_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Complete the code also define these macros in the driver
						header file
						#define I2C_ERROR_BERR  3
						#define I2C_ERROR_ARLO  4
						#define I2C_ERROR_AF    5
						#define I2C_ERROR_OVR   6
						#define I2C_ERROR_TIMEOUT 7

 */

void I2C_ERR_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);


	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);

	}

}

/************************************************************
 * 				SLave functionalities
 ***********************************************************/
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx,uint8_t data)
{
	pI2Cx->DR = data;
}
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx)
{
	return (uint8_t)pI2Cx->DR;
}

void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	else
	{
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);
	}
}
