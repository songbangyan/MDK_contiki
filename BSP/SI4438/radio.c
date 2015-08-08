/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */
#include "STM32F10x.h"
#include "common.h"
#include "wireless_phy.h"
#include "radio_config.h"
#include "si446x_api_lib.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si446x_defs.h"
#include "si446x_patch.h"
#include "SI4438_config.h"
#include "radio.h"
//#include "systick.h"
//#include "iwdg.h"
#include "Low_Rate_Check.h"

/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

const U8 Radio_Configuration_Data_Array[]=RADIO_CONFIGURATION_DATA_ARRAY;
const tRadioConfiguration RadioConfiguration=RADIO_CONFIGURATION_DATA;
const tRadioConfiguration *pRadioConfiguration=&RadioConfiguration;

void Clear_int_status(void)
{
  U8 send_cmd;
  send_cmd=SI446X_CMD_ID_GET_INT_STATUS;
  radio_comm_SendCmd(0x01,&send_cmd);
}
U8 set_property(const U8* pSetPropCmd,U8 length)
{
//  U8 chip_pend;
  if (radio_comm_SendCmdGetResp(length, Pro2Cmd, 0, 0) != 0xFF)
    {
      /* Timeout occured */
    DBUG("CTS_TIMEOUT");
      return SI446X_CTS_TIMEOUT;
    }

    if (radio_hal_NirqLevel() == 0)
    {
      /* Get and clear all interrupts.  An error has occured... */
      si446x_get_int_status(0, 0, 0);
    
  //  bApi_GetFastResponseRegister(SI446X_CMD_ID_FRR_B_READ,SI446X_CMD_ARG_COUNT_FRR_B_READ,&chip_pend);
  //  if (chip_pend & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
    if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
    {
        DBUG("COMMAND_ERROR");
        return SI446X_COMMAND_ERROR;
    }
    }
  return SI446X_SUCCESS;
}
#define NOT_CHECK_REGISTER 35,36
unsigned char get_all_property(void)
{
  U8 i,j,col;
  const U8 *pSetPropCmd=pRadioConfiguration->Radio_ConfigurationArray;
  U8 numOfBytes;
  U8 buf[20],length;
  U16 Pregister=0;
  U8 not_check_register[2]={NOT_CHECK_REGISTER};
  
  numOfBytes = *pSetPropCmd++;
  pSetPropCmd+=numOfBytes;
  numOfBytes = *pSetPropCmd++;
  pSetPropCmd+=numOfBytes;
  while (*pSetPropCmd != 0x00)
  {
    numOfBytes = *pSetPropCmd++;

    for (col = 0u; col < numOfBytes; col++)
    {
      Pro2Cmd[col] = *pSetPropCmd;
      pSetPropCmd++;
    }
    Pro2Cmd[0]=0x12;

    length=Pro2Cmd[2];
    
    if (radio_comm_SendCmdGetResp(4, Pro2Cmd, length, buf) != 0xFF)
    {
      DBUG("SI446X_CTS_TIMEOUT\r\n");
      return FALSE;
    }
    //DBUG("\r\n");
    for(i=0;i<length;i++)
    {
      
      Pregister++;
      //DBUG("%02x ",buf[i]);
      if(Pro2Cmd[i+4]!=buf[i])
      {
        for(j=0;j<2;j++)
        {
          
          if(Pregister==not_check_register[j])
          {
            j=0xee;
            break;
          }
        }
        if(j!=0xee)
        {
          DBUG("Pregister=%d ",Pregister);
          return FALSE;
        }

      }
      
    }
  }

  return TRUE;
}

unsigned char get_property(void)
{
  U8 cmd_length,i,device_state;

    i=0;
    device_state=0;
  
    Pro2Cmd[i++]=0x12;
    Pro2Cmd[i++]=0x50;
    Pro2Cmd[i++]=0x0C;
    Pro2Cmd[i++]=0x00;
    cmd_length=0x0C;
    if(radio_comm_SendCmdGetResp(4, Pro2Cmd, cmd_length, Pro2Cmd)!= 0xFF)  //获取跳频表参数
    {
      DBUG("get_hop cmd err\r\n");
      return FALSE;
    }
    for(i=0;i<(cmd_length);i++)
    {
    //  DBUG("%02x %02x\r\n",Pro2Cmd[i],Si4438HopRegister[i]);
      if(Pro2Cmd[i]!=Si4438HopRegister[i])  //判断跳频表参数
      {
          DBUG("get_hop err\r\n");
          return FALSE;
      }
    }
    if(si4438_phy.RF_status==RF_RX_FLAG)
    {
      si446x_request_device_state();
      device_state=Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE;  //获取设备当前状态是否为读状态
      if(device_state!=SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_RX)
      {
          DBUG("CURR_STATE err\r\n");
          return FALSE;
      }

    }
/*        
    tick_tmp=0;
    device_state=0x00;

    if(radio_hal_Gpio0Level())  //判断晶振是否启振；
    {
      for(tick_tmp=0;tick_tmp<5;tick_tmp++)
      {
        if(!radio_hal_Gpio0Level())
          break;
      }
    }
    else
    {
      for(tick_tmp=0;tick_tmp<5;tick_tmp++)
      {
        if(radio_hal_Gpio0Level())
          break;
      }
    }
    
    if(tick_tmp>=20)
    {
      DBUG("READY state err");
      return TRUE;
    }
*/
    return TRUE;
}

volatile U8 set_pchannel=0;


U8 set_hop_table(U8 UserChannel)
{
    U8 i=0,j=0;
    if(UserChannel>32)
    {
      UserChannel = PUBLIC_CHANNEL;
      
    }
    si4438_phy.RF_status=RF_IDLE_FLAG;
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
    Clear_int_status();
    Pro2Cmd[i++]=0x11;
    Pro2Cmd[i++]=0x50;
    Pro2Cmd[i++]=0x0C;
    Pro2Cmd[i++]=0x00;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0x30; 
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0x04;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=ChannelsSupported[0][0]; 
    Pro2Cmd[i++]=Si4438HopRegister[j++]=ChannelsSupported[0][1];
    Pro2Cmd[i++]=Si4438HopRegister[j++]=ChannelsSupported[UserChannel][0];
    Pro2Cmd[i++]=Si4438HopRegister[j++]=ChannelsSupported[UserChannel][1];
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0xFF;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0xFF;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0xFF;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0xFF;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0xFF;
    Pro2Cmd[i++]=Si4438HopRegister[j++]=0xFF;
      
    if(set_property(Pro2Cmd,0x10)==SI446X_SUCCESS)
    {
      DBUG("set_hop_table=%d\r\n",UserChannel);
      set_pchannel=UserChannel;
      si4438_phy.PhyPrivateChannel=UserChannel;
      Clear_int_status();
      vRadio_StartRX(ChannelsSupported[0][0]);
      return TRUE;
    }
    return FALSE;
  
    
}

unsigned char Rf_set(void)
{ 
  U8 SetPA[]={ RF_PA_MODE_5};
  si4438_phy.RF_status=RF_IDLE_FLAG;
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  
  
  if (radio_comm_SendCmdGetResp(0x09, SetPA, 0, 0) != 0xFF)
  {
    DBUG("rf_power_up SI446X_CTS_TIMEOUT\r\n");
    return FALSE;
  }

  return TRUE;
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */

void vRadio_Init(void)
{
  volatile    unsigned int tick_tmp;
  unsigned char times=0;
  /* Power Up the radio chip */
  si446x_reset();
//  si446x_apply_patch();
//  vRadio_PowerUp();
#ifdef SI446X_PATCH_CMDS
  if( si446x_apply_patch()==SI446X_SUCCESS)
  {
      DBUG("si446x_apply_patch SI446X_SUCCESS \r\n");
      si446x_power_up(0x01,0x00,0x01C9C380);
  }
  else
  {
    DBUG("si446x_apply_patch fail \r\n");
    si446x_power_up(0x01,0x00,0x01C9C380);
  }
#else
  DBUG(" no si446x_apply_patch  \r\n");
  si446x_power_up(0x01,0x00,0x01C9C380);
#endif
  /* Load radio configuration */
  si4438_phy.SI4438_reset_flag=1;
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))
  {
    times++;
    if(times>4)
    {
      si4438_phy.SI4438_reset_flag=0;
      DBUG("si446x_configuration_init fail");
      break;
    }
    Iwdg_Init();
    tick_tmp=GET_SYS_TICK;
    while(GET_SYS_TICK < (tick_tmp+TICK_50MS));
    SI4438_Config();
    si446x_reset();
  }

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
}

void sync_word_int(void)
{
  RXD_LED(1);
  si4438_phy.rx_flag=RX_SYNC;
  RF_receive_flag=1;
  si4438_phy.timer_out_flag=1; //启动接收超时标志
  rx_tick_tmp=GET_SYS_TICK;
  GetLatchRSSIValue();

}
/*!
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 */
BIT gRadio_CheckReceived_VariablePacket(void)
{
 U8 ph_pend=0;
 si4438_phy.RF_int_flag=0;

 if ((IRQ_PORT->IDR & IRQ_PIN) == FALSE)
 {
    bApi_GetFastResponseRegister(SI446X_CMD_ID_FRR_A_READ,SI446X_CMD_ARG_COUNT_FRR_A_READ,&ph_pend);
//    bApi_GetFastResponseRegister(SI446X_CMD_ID_FRR_B_READ,SI446X_CMD_ARG_COUNT_FRR_B_READ,&chip_pend);
//    if (chip_pend & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
//    {
//        DBUG("COMMAND ERR\r\n");
//        wireless_phy_init();
//        return TRUE;
//    
//    }

    if((ph_pend&0xC4)!=0x00)  //判断中断标示有效性
    {
      //wireless_phy_init();
      return FALSE;
    }
    Clear_int_status();
    if(ph_pend & SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
    {

      si4438_phy.tx_flag=TX_OK;
      si4438_phy.RF_status=RF_IDLE_FLAG;
      return SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT;
    }
    
    if (ph_pend & SI446X_CMD_GET_INT_STATUS_REP_TX_FIFO_ALMOST_EMPTY_PEND_BIT)
    {
      si4438_phy.tx_flag=TX_EMPTY;
      return TRUE;
    }
   
    if (ph_pend & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {
      RXD_LED(0);
      si4438_phy.rx_flag=RX_DONE;
      return TRUE;
    }
    
   if (ph_pend & SI446X_CMD_GET_INT_STATUS_REP_RX_FIFO_ALMOST_FULL_BIT)
   {
  
      RXD_LED(1);
      si4438_phy.rx_flag=RX_OK;
      return TRUE;
  
   }
   
  }

  return FALSE;
}

U8 bRadio_GetCurrentChannel(void)
{
  // Requests the current state of the device
  si446x_request_device_state();

  return Si446xCmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL;
}

/*!
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 */
void vRadio_StartRX(U8 channel)
{
//  volatile unsigned int  tick_tmp=1000; //延时220uS;
//  while(tick_tmp--);
#ifdef LOW_RATE_CHECK

  SetHighRate();
#endif
  PA_Rxen();
  si4438_phy.RF_status=RF_IDLE_FLAG;
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  si4438_phy.rx_flag=RX_ON;
  si4438_phy.RF_status=RF_RX_FLAG;
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  /* Reset FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
  /* Start Receiving packet, channel 0, START immediately, Packet length according to PH */
  si446x_start_rx(channel, 0u, 0x00,
            SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
            SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
            SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
}


/*!
 *  Set Radio to TX mode, variable packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent length of of the packet sent to TXFIFO
 *
 *  @note
 *
 */
void set_txd_mode(void)
{
  volatile unsigned int  tick_tmp=1000; //延时220uS;
  PA_Txen();
  RXD_LED(0);
  si4438_phy.RF_status=RF_IDLE_FLAG;
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  while(tick_tmp--);
  si446x_get_int_status(0u, 0u, 0u);
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
  /* Reset the Tx Fifo */
  si4438_phy.tx_flag=TX_ON;
  si4438_phy.RF_status=RF_TX_FLAG;
  
}

void vRadio_StartTx_Variable_Packet(U8 channel, U8 *pioRadioPacket, U8 length)
{
//  DBUG("send channel=%d pioRadioPacket=%d length=%d\r\n",channel,pioRadioPacket,length);
  /* Read ITs, clear pending ones */
  Clear_int_status();
  /* Reset the Tx Fifo */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX fifo with datas */
  si446x_write_tx_fifo(length, pioRadioPacket);

  /* Start sending packet, channel 0, START immediately */
   si446x_start_tx(channel, 0x80, length);  
  
  si4438_phy.tx_flag=TX_ON;
  si4438_phy.RF_status=RF_TX_FLAG;
}

void PA_Rxen(void)
{
  TE_PORT->BSRR=TE_PIN;
  RE_PORT->BRR=RE_PIN;
}

void PA_Txen(void)
{
  RE_PORT->BSRR=RE_PIN;
  TE_PORT->BRR=TE_PIN;
}




