#include "stm32f10x.h"
#include "SI4438_config.h"
#include "radio_hal.h"
#include "wireless_phy.h"
#include "si446x_api_lib.h"
#include "si446x_defs.h"
#include "radio.h"
//#include "systick.h"
//#include "iwdg.h"

#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x01, 0x00, 0x0B
#define RF_GPIO_PIN_CFG 0x13, 0x04, 0x10, 0x14, 0x08, 0x00, 0x00, 0x00

#define RF_DIRECT_MODE_CONFIG_DATA_ARRAY {\
0x08, RF_GPIO_PIN_CFG,\
0x05, RF_MODEM_MOD_TYPE_12,\
0x00 \
}

#define TX_CLK_PORT GDIO1_PORT
#define TX_CLK_PIN  GDIO1_PIN

#define TX_DATA_PORT GDIO0_PORT
#define TX_DATA_PIN  GDIO0_PIN


#define TX_CLK()     radio_hal_Gpio1Level()

#define TX_DATA(x)  x==1 ? (TX_DATA_PORT->BSRR = TX_DATA_PIN) : (TX_DATA_PORT->BRR=TX_DATA_PIN )


#define WAIT_SERIAL_CLK(x)  while (TX_CLK()!=x)

const unsigned char DirectModefiguration[]=RF_DIRECT_MODE_CONFIG_DATA_ARRAY;

unsigned char Si4438_Tx__Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  SetHighRate();  //设置成高速率模式
  //配置相应发送GPIO口
  GPIO_InitStructure.GPIO_Pin = TX_DATA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TX_DATA_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.GPIO_Pin = TX_CLK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TX_CLK_PORT, &GPIO_InitStructure);

  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  si4438_phy.RF_status=RF_IDLE_FLAG;
  
  NVIC_DisableIRQ(GDIO1_EXTI_IRQn);
  NVIC_DisableIRQ(GDIO0_EXTI_IRQn);
  
  if (SI446X_SUCCESS != si446x_configuration_init(DirectModefiguration))
  {
    DBUG("Si4438_Tx__Config fail\r\n");
    wireless_phy_init();
    return 0;
  }
  return 1;
}
UINT8 SendByte(UINT8 dataByte)
{
  
  UINT8 i;
   for (i = 0; i < 8; i++) {
        WAIT_SERIAL_CLK(1);
        if ((dataByte & 0x01) == 0){
            TX_DATA(0);
        }
    else{
            TX_DATA(1);
    }
    
            dataByte >>=  1;
      WAIT_SERIAL_CLK(0);
            
        }
   return TRUE;
}

void SendTestStream(unsigned char ChannelIndex ,unsigned char Stream_M,unsigned char Send_S)
{
  UINT8 ChannelGroup , ChannelNum,SendCode;
  UINT32 Send_MS;
  unsigned int counter_tmp;
  
  ChannelGroup=ChannelIndex/2;
  ChannelNum= ChannelIndex%2;

  if(!Si4438_Tx__Config())
  {
    Si4438_Tx__Config();
  }

  Send_MS=Send_S*1000;
  ChannelSelect(ChannelGroup, ChannelNum);
  PA_Txen();
  RXD_LED(0);
  si446x_start_tx(si4438_phy.SI4438CurrentChannel,0u,0u);  
    
  si4438_phy.RF_status=RF_IDLE_FLAG;

  TXD_LED(1);
  SendByte(0x00);
  SendByte(SFDL);
  SendByte(SFDH);
  counter_tmp=GET_SYS_TICK;
  if(Stream_M==0x04)
  {
    SendCode=0x55;
  }
  else if(Stream_M==0x00)
  {
    SendCode=0x00;
  }
  else if(Stream_M==0x01)
  {
    SendCode=0xff;
  }
  else
  { 
    ;
  }
  
  do{
    Iwdg_Init();  
    SendByte(SendCode); 
  }while((GET_SYS_TICK-counter_tmp)<Send_MS);
  wireless_phy_init();
  TXD_LED(0);
}

