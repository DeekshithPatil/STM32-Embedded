/*
 * stm32f446xx_spi_driver.h
 *
 *  Created on: 12-Mar-2021
 *      Author: DELL
 */

#ifndef INC_STM32F446XX_SPI_DRIVER_H_
#define INC_STM32F446XX_SPI_DRIVER_H_

#include "stm32f446xx.h"

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
}SPI_Handle_t;


/*
 * #defines
 */

//Device Mode
#define SPI_DeviceMode_FD								0
#define SPI_DeviceMode_HD								1
#define SPI_DeviceMode_SimplexRxOnly					2

/*
 * Function declarations
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

//Transmit and Receive Data
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

//Other Functions
uint8_t SPI_CheckFlagStatus(SPI_RegDef_t *pSPIx, uint8_t FlagName);
void SPI_PeriControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_SSIControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_SSOEControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

#endif /* INC_STM32F446XX_SPI_DRIVER_H_ */
