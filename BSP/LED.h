/*-----------------------------------------------------------------------------
 * Name:    LED.h
 * Purpose: Low level LED definitions
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h" 
//#include <stdint.h>

#define LED_POW 0
#define LED_RXD 1
#define LED_TXD 2
#define LED_NET 3

void BSP_LED_Init(void);
void     LED_On           (uint32_t num);
void     LED_Off          (uint32_t num);
void     LED_Toggle       (uint32_t num); 
uint32_t LED_Num          (void);

#endif /* __LED_H */
