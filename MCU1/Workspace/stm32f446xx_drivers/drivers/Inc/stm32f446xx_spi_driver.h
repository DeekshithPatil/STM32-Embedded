/*
 * stm32f446xx_spi_driver.h
 *
 *  Created on: 05-Mar-2021
 *      Author: DELL
 */

#ifndef INC_STM32F446XX_SPI_DRIVER_H_
#define INC_STM32F446XX_SPI_DRIVER_H_

#include "stm32f446xx.h"

#define __weak							__attribute__((weak)) //THis is an attribute used to define weak functions

/*
 * Configuration Structure for SPIx peripheral
 */

typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPHA;
	uint8_t SPI_CPOL;
	uint8_t SPI_SSM;

}SPI_Config_t;

typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;

	uint8_t		*pTxBuffer;
	uint8_t		*pRxBuffer;
	uint32_t	TxLen;
	uint32_t	RxLen;
	uint8_t 	TxState;
	uint8_t		RxState;
}SPI_Handle_t;

/*
 * @SPI States (Rx and Tx)
 */
#define SPI_READY												0
#define SPI_BUSY_IN_RX											1
#define SPI_BUSY_IN_TX											2

/*
 * Possible SPI application events
 */
#define SPI_EVENT_TX_CMPLT										1
#define SPI_EVENT_RX_CMPLT										2
#define SPI_EVENT_OVR_ERR										3

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER 									1
#define SPI_DEVICE_MODE_SLAVE 									0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD 										1
#define SPI_BUS_CONFIG_HD 										2
//#define SPI_BUS_CONFIG_SIMPLEX_TXONLY 						3 //Removed as simplex tx only can be achieved by configuring in Full duplex and disconnecting MISO
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY							4
//Simplex rx only (on the master side) cannot be achieved by just dropping the MOSI connection from slave to master as, master produces clock only when MOSI is active
// Since in this mode, Master's MOSI will not be active, it will not produce the SCLK
// To force the master to produce SCLK, we need to set RXONLY bit in SPI_CR1 to '1'

/*
 * @SPI_SclSPeed
 */
#define SPI_SCLK_SPEED_DIV2										0
#define SPI_SCLK_SPEED_DIV4										1
#define SPI_SCLK_SPEED_DIV8										2
#define SPI_SCLK_SPEED_DIV16									3
#define SPI_SCLK_SPEED_DIV32									4
#define SPI_SCLK_SPEED_DIV64									5
#define SPI_SCLK_SPEED_DIV128									6
#define SPI_SCLK_SPEED_DIV256									7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS											0
#define SPI_DFF_16BITS											1

/*
 * @SPI_CPOL
 */
#define SPI_CPOL_HIGH											1
#define SPI_CPOL_LOW											0

/*
 * @SPI_CPHA
 */
#define	SPI_CPHA_HIGH											1
#define SPI_CPHA_LOW											0

/*
 * @SPI_SSM
 */
#define SPI_SSM_DI												0
#define SPI_SSM_EN												1


/*
 * SPI Related status flags definitions
 */
#define SPI_TXE_FLAG				(1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG				(1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG				(1 << SPI_SR_BUSY)

/*
 * Peripheral Clock Setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Set the SPE bit (in SPIx_CR1) and enable the peripheral register
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);


/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle); //Used to initialise the SPIx registers
void SPI_DeInit(SPI_RegDef_t *pSPIx); //Used to de-Initialize the SPIx registers

/*
 * Send and receive data
 */

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len); //First parameter is to SPI registers, second paramater is a pointer to the data that needs to be transmitted
//Third paramater represents the size of the data in bytes
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * Send and receive data through interrupt
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle,  uint8_t *pRxBuffer, uint32_t Len);


/*
 * IRQ configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi); //IRQ number and enable or disable
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

//other
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

//Application Callback functions
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv); //This function has to be implemented by the application.
//If application doesnt implement this function then the compiler may issue an error.
//To avoid this we need to give a weak implementation of this function in the spi_driver.c file
#endif /* INC_STM32F446XX_SPI_DRIVER_H_ */
