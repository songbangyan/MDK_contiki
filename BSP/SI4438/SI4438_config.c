#include "SI4438_config.h"
#include "Low_Rate_Check.h"
#include "wireless_phy.h"

unsigned char SDN_IRQ_CONFIG = 0;
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the GPIOD Pins.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SI4438_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  GPIO_InitStructure.GPIO_Pin = RE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RE_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TE_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GDIO0_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GDIO0_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GDIO1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GDIO1_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GDIO2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GDIO2_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GDIO3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GDIO3_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IRQ_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RF_SDI_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RF_SDI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RF_SDO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RF_SDO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RF_SCLK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RF_SCLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RF_SEL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RF_SEL_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.GPIO_Pin = SDN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SDN_PORT, &GPIO_InitStructure);

  RF_SEL_PORT->BSRR=RF_SEL_PIN;
  RF_SCLK_PORT->BRR = RF_SCLK_PIN;
  RF_SDI_PORT->BRR = RF_SDI_PIN;
  SDN_PORT->BSRR = SDN_PIN;
}
void SI4438_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2); //连接EXTI Line8到PA8
  /* Configure  EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = IRQ_EXTI;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure   GPIO0  Line to generate an interrupt on falling edge */ 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8); //连接EXTI Line8到PA8
  EXTI_InitStructure.EXTI_Line = GDIO1_EXTI;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
#ifdef LOW_RATE_CHECK
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //连接EXTI Line8到PA8
  EXTI_InitStructure.EXTI_Line = GDIO0_EXTI;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
#endif  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //设置为抢占式优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //设置为抢占式优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void SI4438_SDN_NVIC_Configuration(void)
{
//  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = SDN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SDN_PORT, &GPIO_InitStructure);

  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource6); //连接EXTI Line8到PA8
   /* Configure EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = SDN_EXTI;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_ClearITPendingBit(SDN_EXTI);
	SDN_IRQ_CONFIG=1;  
}
void SI4438_Config()
{
    /* GPIOC clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  SI4438_GPIO_Configuration();
  SI4438_NVIC_Configuration();
}
#include "wireless_phy.h"
#include "radio.h"
void  wireless_phy_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DBUG("wireless_phy_init");
  si4438_phy.RF_status=RF_IDLE_FLAG;
  SI4438_Config();

//  DISABLE_IRQ_GPIO_NVIC();
  vRadio_Init();
  
  PreambleCheck.CurrentRate=HIGH_RATE;
  si4438_phy.ReceiveRate=HIGH_RATE;
  si4438_phy.SendRate=HIGH_RATE;
  ChannelSelect(PUBLIC_CHANNEL, 0);
  si4438_phy.PhyRSSI_TH=96;
  set_hop_table(si4438_phy.PhyPrivateChannel);
  si4438_phy.PhyRSSI=0;
  
  
  
  GPIO_InitStructure.GPIO_Pin = SDN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDN_PORT, &GPIO_InitStructure);
  
  SDN_PORT->BRR = SDN_PIN;
  DISABLE_SDN_NVIC();  //管闭SDN脚中断
  //EXTI_ClearITPendingBit(SDN_EXTI);//清除SDN中断
  EXTI_ClearITPendingBit(IRQ_EXTI);//清除IRQ中断
  EXTI_ClearITPendingBit(GDIO1_EXTI);//清除GDIO1中断
  EXTI_ClearITPendingBit(GDIO0_EXTI);//清除GDIO0中断
  ENABLE_IRQ_GPIO_NVIC(); //使能IRQ及GDIO1中断
  EXTI_ClearITPendingBit(IRQ_EXTI);//清除IRQ中断
  //SI4438_SDN_NVIC_Configuration();
  EXTI_ClearITPendingBit(GDIO0_EXTI);//清除GDIO0中断
  
//  si4438_phy.int_si4438_flag=0;
	Receive_init();
  SetHighRate();
}



extern void CheckLowRatePreamble(void);
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(IRQ_EXTI) != RESET)
  {
      /* Clear the Key Button EXTI line pending bit */
      EXTI_ClearITPendingBit(IRQ_EXTI);

  //  gRadio_CheckReceived_VariablePacket();
    si4438_phy.RF_int_flag=1;

    return ;
  }
  EXTI_ClearITPendingBit(IRQ_EXTI);
}
/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{

   if(EXTI_GetFlagStatus(GDIO1_EXTI) != RESET)
  {
    
      /* Clear  EXTI line pending bit */
      EXTI_ClearITPendingBit(GDIO1_EXTI);
    if((si4438_phy.RF_status==RF_RX_FLAG)&&((RF_receive_flag==0)||(RF_receive_flag==2)))
      sync_word_int();  //占用时间25uS
    return ;
  }
#ifdef LOW_RATE_CHECK  
  if(EXTI_GetFlagStatus(GDIO0_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(GDIO0_EXTI);
  if((si4438_phy.RF_status==RF_RX_FLAG)&&((RF_receive_flag==0)||(RF_receive_flag==2)))
    CheckLowRatePreamble();
  return ;
  }
  EXTI_ClearITPendingBit(GDIO0_EXTI);
#endif
   if(EXTI_GetFlagStatus(SDN_EXTI) != RESET)
  {

      /* Clear the Key Button EXTI line pending bit */
      EXTI_ClearITPendingBit(SDN_EXTI);

  //  gRadio_CheckReceived_VariablePacket();
//    si4438_phy.int_si4438_flag=1;
		if(SDN_IRQ_CONFIG)
		{
    	wireless_phy_init();
  	}
    return ;
//  DBUG("wireless_phy_init");
  } 
  EXTI_ClearITPendingBit(GDIO1_EXTI);
  EXTI_ClearITPendingBit(SDN_EXTI);
}




