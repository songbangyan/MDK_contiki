/*! @file spi.c
 * @brief This file contains functions for controlling SPI0 and SPI1 interfaces.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "STM32F10x.h"
#include "spi_SI4438.h"
#include "SI4438_config.h"
#include "radio_hal.h"

#define SPI_DELAY(x)// delay(x)

#define SPI_INTERFACE                               SPI2
#define SPI_CLK                                     RCC_APB1Periph_SPI2

#define SPI_PIN_SCK_PORT                            RF_SCLK_PORT
#define SPI_PIN_SCK_PORT_CLK                        RCC_APB2Periph_GPIOA
#define SPI_PIN_SCK                                 RF_SCLK_PIN

#define SPI_PIN_MISO_PORT                           RF_SDO_PORT
#define SPI_PIN_MISO_PORT_CLK                       RCC_APB2Periph_GPIOB
#define SPI_PIN_MISO                                RF_SDO_PIN

#define SPI_PIN_MOSI_PORT                           RF_SDI_PORT
#define SPI_PIN_MOSI_PORT_CLK                       RCC_APB2Periph_GPIOB
#define SPI_PIN_MOSI                                RF_SDI_PIN

void delay(u8 i)
{
	while(i--);
}
void SpiInit( void )
{
	
}


#if 1
void vSpi_WriteDataSpi0(u8 biDataInLength, u8 *pabiDataIn)
{
	u8 i,DataByte;
	SPI_DELAY(5);
 while (biDataInLength--) {
  	DataByte=*pabiDataIn++;
  	for(i = 0; i < 8; i++)
	{
		RF_SCLK_PORT->BRR= RF_SCLK_PIN;
		if(DataByte & 0x80)
			RF_SDI_PORT->BSRR=RF_SDI_PIN;
		else
			RF_SDI_PORT->BRR=RF_SDI_PIN;
		DataByte <<= 1;
		
		RF_SCLK_PORT->BSRR= RF_SCLK_PIN;
	}
	
  }
 
 RF_SCLK_PORT->BRR= RF_SCLK_PIN;
 SPI_DELAY(5);
}


#else
void vSpi_WriteDataSpi0(U8 biDataInLength, U8 *pabiDataIn)
{
  while (biDataInLength--) {
    ReadWriteByte_SL(*pabiDataIn++);
  }
}
#endif



#if 1
void vSpi_ReadDataSpi0(u8 biDataOutLength, u8 *paboDataOut)
{
	unsigned char i = 0;
	u8 SdoData=0;
		SPI_DELAY(5);
	while (biDataOutLength--){
	  	SdoData=0x00;
	  	for(i = 0; i < 8; i++)
		{
			RF_SCLK_PORT->BRR= RF_SCLK_PIN;
			SdoData<<=1;
			RF_SCLK_PORT->BSRR= RF_SCLK_PIN;
			SdoData |= (RF_SDO_PORT->IDR & RF_SDO_PIN) ? 1 : 0;
		}
		
	  *paboDataOut++ = SdoData;
	  }
	RF_SCLK_PORT->BRR= RF_SCLK_PIN;
	SPI_DELAY(5);
}

#else

/*!
 * This function is used to read data from SPI0.
 *
 *  \param[in] biDataOutLength  The length of the data.
 *  \param[out] *paboDataOut    Pointer to the first element of the response.
 *
 *  \return None
 */
void vSpi_ReadDataSpi0(U8 biDataOutLength, U8 *paboDataOut)
{
  while (biDataOutLength--){
    *paboDataOut++ = ReadWriteByte_SL(0xFF);
  }
}
#endif

#if 1


u8 ReadWriteByte_SL(u8 DataByte)
{
#ifdef SPI_SIMULATION
	unsigned char i = 0;
	u8 SdoData=0;
	SPI_DELAY(5);
	for(i = 0; i < 8; i++)
	{
		RF_SCLK_PORT->BRR= RF_SCLK_PIN;
		if(DataByte & 0x80)
			RF_SDI_PORT->BSRR=RF_SDI_PIN;
		else
			RF_SDI_PORT->BRR=RF_SDI_PIN;
		
		//SPI_DELAY(2);
		DataByte <<= 1;
		
		SdoData<<=1;
		//SPI_CLK = LOW;
		
		
		
		RF_SCLK_PORT->BSRR= RF_SCLK_PIN;
		SdoData |= (RF_SDO_PORT->IDR & RF_SDO_PIN) ? 1 : 0;
	}
	
	RF_SCLK_PORT->BRR= RF_SCLK_PIN;
	SPI_DELAY(5);
	//DBUG(" %x",SdoData);
	return SdoData;
#else
	SPI_I2S_SendData( SPI_INTERFACE, DataByte );
	//DBUG("%02x\r\n",outData);
    while( SPI_I2S_GetFlagStatus( SPI_INTERFACE, SPI_I2S_FLAG_RXNE ) == RESET );
    return SPI_I2S_ReceiveData( SPI_INTERFACE );

#endif
}
#endif


