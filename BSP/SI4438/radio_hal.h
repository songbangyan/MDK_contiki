/*!
 * File:
 *  radio_hal.h
 *
 * Description:
 *  This file contains RADIO HAL.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

#ifndef _RADIO_HAL_H_
#define _RADIO_HAL_H_

#include "SI4438_config.h"
            
//#define SPI_SIMULATION3
//#define SPI_SIMULATION2
#define SPI_SIMULATION



#define radio_hal_ClearNsel() RF_SEL_PORT->BRR=RF_SEL_PIN;
#define radio_hal_SetNsel() RF_SEL_PORT->BSRR=RF_SEL_PIN;
#define radio_hal_NirqLevel() ((IRQ_PORT->IDR & IRQ_PIN) ? 1 : 0)

#ifdef SPI_SIMULATION2
#define radio_hal_SpiWriteByte(byteToWrite) vSpi_WriteBitsBitbangSpi1(byteToWrite, 8u)



#define radio_hal_SpiReadByte()  bSpi_ReadByteBitbangSpi1();

#else
#define radio_hal_SpiWriteByte(byteToWrite) ReadWriteByte_SL(byteToWrite)


#define radio_hal_SpiReadByte(x)  vSpi_ReadDataSpi0(0x01,x)

//#define radio_hal_SpiReadByte()  ReadWriteByte_SL(0xFF)
#endif


#define radio_hal_Gpio0Level() ((GDIO0_PORT->IDR & GDIO0_PIN) ? 1 : 0)

#define radio_hal_Gpio1Level() ((GDIO1_PORT->IDR & GDIO1_PIN) ? 1 : 0)
#define radio_hal_Gpio2Level() ((GDIO2_PORT->IDR & GDIO2_PIN) ? 1 : 0)
#define radio_hal_Gpio3Level() ((GDIO3_PORT->IDR & GDIO3_PIN) ? 1 : 0)



#endif //_RADIO_HAL_H_
