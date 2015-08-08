#ifndef __BSP_H__
#define __BSP_H__

#include <data_type.h>
//#include "tool.h"
#include "stm32f10x.h"
#include "Time.h"
#include "LED.h"
#include "usart.h"
#include "SPI.h"
#include "eeprom.h"
#include "RTC.h"
#include "product_info.h"
#include "cmsis_os.h"
//#include "wireless_phy.h"
//#include "nwk_driver.h"
typedef struct
{
  unsigned char Iwdg_uartproc:1;
  unsigned char Iwdg_sysback:1;
  unsigned char Iwdg_phyproc:1;  
  unsigned char Iwdg_macproc:1;
  unsigned char Iwdg_nwkProc:1;
  unsigned char Iwdg_apsProc:1;  
  unsigned char Iwdg_maintainproc:1;  
  unsigned char Iwdg_periodproce:1;  
}Iwdg_flg_t;

extern Iwdg_flg_t Iwdg_Flg;


void BSP_IWDG_Init(void);
void Iwdg_Init(void);
unsigned int Calc_CRC_HW(unsigned int *dat, unsigned int len);
void bsp_init(void);
void sys_delay(u32 ms);
u32 get_systick(void);
void real_delay(u32 millisec);
#endif



