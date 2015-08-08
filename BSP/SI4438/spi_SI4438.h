/*! @file spi.h
 * @brief This file is the interface file for SPI0 and SPI1 routines.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef SPI_H
#define SPI_H
#include "SI4438_Platform_configuration.h"
 
void vSpi_WriteDataSpi0(U8 biDataInLength, U8 *pabiDataIn);

void vSpi_ReadDataSpi0(U8 biDataOutLength, U8 *paboDataOut);
U8 ReadWriteByte_SL(U8 DataByte);

void SpiInit( void );


#endif
