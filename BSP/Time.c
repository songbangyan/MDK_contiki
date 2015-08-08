#include "Time.h"

volatile u32 Tim3_systick = 0;
/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Configures the TIM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_TIM3_init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* ---------------------------------------------------------------
  TIM3 Configuration: Output Compare Inactive Mode:
  TIM3CLK = 36 MHz, Prescaler = 7299, TIM3 counter clock = 10 KHz
  --------------------------------------------------------------- */  
  /* Time base configuration */ 
  TIM_TimeBaseStructure.TIM_Period = 14;
  TIM_TimeBaseStructure.TIM_Prescaler = 7199; //max=65535
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//0
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ENABLE DISABLE
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
  /* Enable the TIM3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
  
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    /* Clear TIM3 Update interrupt pending bit*/
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    Tim3_systick ++;
  }
}

