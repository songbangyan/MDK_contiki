#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "STM32F10x.h" 

void I2C_EE_Init(void);
u32 EEPROM_GetTotalsize(void);
void eeprom_write(uint32_t addr,uint16_t num,uint8_t *p_data);
void eeprom_read(uint32_t addr,uint16_t num,uint8_t *p_data);
#endif
