/*
 * stm32f446xx_i2c_driver.h
 *
 *  Created on: 23-Mar-2021
 *      Author: DELL
 */

#ifndef INC_STM32F446XX_I2C_DRIVER_H_
#define INC_STM32F446XX_I2C_DRIVER_H_

#include "stm32f446xx.h"


/*
 * I2C config structure
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t	 I2C_DeviceAddress;
	uint8_t	 I2C_ACKControl;
	uint16_t I2C_FMDutyCycle;
}I2C_Config_t;


/*
 * I2C Handle structure
 */
typedef struct
{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;

	uint8_t		*pTxBuffer;
	uint8_t		*pRxBuffer;
	uint32_t	TxLen;
	uint32_t	RxLen;
	uint8_t 	TxRxState; //Because I2C is half duplex, only one variable for Tx and Rx
	uint8_t		DevAddr; //To store device/slave address
	uint8_t		Sr; //To store repeated start

}I2C_Handle_t;

/*
 * @I2C States (Rx and Tx)
 */
#define I2C_READY												0
#define I2C_BUSY_IN_RX											1
#define I2C_BUSY_IN_TX											2

/*
 * @I2C Sr(Repeated Start)
 */
#define I2C_DISABLE_SR											0
#define I2C_ENABLE_SR											1

/*
 * I2C SR flags
 */
#define I2C_FLAG_TXE				(1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE				(1 << I2C_SR1_RXNE)
#define I2C_FLAG_SB					(1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR				(1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF				(1 << I2C_SR1_BTF)
#define I2C_FLAG_ADD10				(1 << I2C_SR1_ADD10)
#define I2C_FLAG_STOPF				(1 << I2C_SR1_STOPF)
#define I2C_FLAG_BERR				(1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO				(1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF					(1 << I2C_SR1_AF)
#define I2C_FLAG_OVR				(1 << I2C_SR1_OVR)
#define I2C_FLAG_PECERR				(1 << I2C_SR1_PECERR)
#define I2C_FLAG_TIMEOUT			(1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_SMBALERT			(1 << I2C_SR1_SMBALERT)

/*
 * I2C Application Callback events macros
 */
#define I2C_EV_TX_CMPLT								0
#define I2C_EV_RX_CMPLT								1
#define I2C_EV_STOP									2
#define I2C_ERROR_BERR 							    3
#define I2C_ERROR_ARLO							    4
#define I2C_ERROR_AF                                5
#define I2C_ERROR_OVR                               6
#define I2C_ERROR_TIMEOUT                           7
#define I2C_EV_SLAVE_DATA_REQ						8
#define I2C_EV_SLAVE_DATA_RCV						9

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM								100000 //Standard mode: 100KHz
#define I2C_SCL_SPEED_FM4K								400000 //Fast mode with 400KHz
#define I2C_SCL_SPEED_FM2K								200000 //Fast mode with 200KHz

/*
 * @I2C_DeviceAddress:
 */

//It needs to be mentioned by the user. We dont have any control over this;

/*
 * @I2C_ACKControl
 */
#define I2C_ACK_ENABLE									1
#define I2C_ACK_DISABLE									0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2									0
#define I2C_FM_DUTY_16_9								1

/*
 * @I2C Execute address phase
 */
#define I2C_WRITE										0
#define I2C_READ										1

/*
 * Peripheral Clock Setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);


/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle); //Used to initialize the I2Cx registers
void I2C_DeInit(I2C_RegDef_t *pI2Cx); //Used to de-Initialize the I2Cx registers

/*
 * Send and receive data
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr);

//Slave
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

/*
 * Send and receive data through interrupt
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);


/*
 * IRQ configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi); //IRQ number and enable or disable
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ERR_IRQHandling(I2C_Handle_t *pI2CHandle);


//other
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);

//Application Callback functions
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv); //This function has to be implemented by the application.
//If application doesnt implement this function then the compiler may issue an error.
//To avoid this we need to give a weak implementation of this function in the I2C_driver.c file

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);


#endif /* INC_STM32F446XX_I2C_DRIVER_H_ */
