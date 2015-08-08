
#include "bsp.h"


void BSP_IWDG_Init(void)
{
//  /* Enable the LSI OSC */
//  RCC_LSICmd(ENABLE);  
//  /* Wait till LSI is ready */
//  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
//  {}
//  /* Enable write access to IWDG_PR and IWDG_RLR registers */
//  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//  /* IWDG counter clock: LSI/32 */
//  IWDG_SetPrescaler(IWDG_Prescaler_32);
//  /* Set counter reload value to obtain 2000ms IWDG TimeOut.
//     Counter Reload Value = 2000ms*IWDG counter clock period
//                          = 2000ms * (LSI/32)
//                          = 2s * (LSI/32)
//                          = LSI/(4 * 4)
//                          = LSI/16
//  */
//  IWDG_SetReload(40000/16);
//  /* Reload IWDG counter */
//  IWDG_ReloadCounter();
//  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
//  IWDG_Enable();
}
void Iwdg_Init(void)
{
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}

void BSP_CRC_init(void)
{
  /* Enable CRC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  CRC_ResetDR();
}
unsigned int Calc_CRC_HW(unsigned int *dat, unsigned int len)
{
  CRC_ResetDR();
  return CRC_CalcBlockCRC(dat, len);
}
/*该文件完成对系统的初始化*/
void bsp_init(void)
{
  SystemCoreClockUpdate();
  /* System Clocks Configuration */
  BSP_IWDG_Init();
  BSP_LED_Init(); 
  BSP_TIM3_init();
  BSP_USART_Init();
  BSP_SPI_Init();
  I2C_EE_Init();
  BSP_RTC_init();
  BSP_CRC_init();
//  wireless_phy_init();
}

void sys_delay(u32 ms)
{
//  osDelay(ms);
}

u32 get_systick(void)
{
//  printf("\r\n systick = %X",osKernelSysTick());
  return Tim3_systick;
}

void real_delay(u32 millisec)
{
  u32 systick = get_systick()+millisec;
  while(get_systick() < systick);
}






