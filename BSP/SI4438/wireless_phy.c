#include "wireless_phy.h"
#include "SI4438_config.h"
//#include "systick.h"
#include "radio.h"
#include "si446x_defs.h"
#include "radio_comm.h"
#include "si446x_api_lib.h"
//#include "iwdg.h"


volatile  struct WIRELESS_PHY  si4438_phy;

//物理层常量
//物理层常量
#define aMaxPHYPayloadSize   252
#define aTurnaroundTime    500

void Receive_init(void) ;

const UINT8 ChannelsSupported[MAX_CHANNEL_GROUP_NUM+1][2]={
    {CHANNEL_GROUP_0_0,CHANNEL_GROUP_0_1},{0,0+CHANNEL_SPACING},{1,1+CHANNEL_SPACING},{2,2+CHANNEL_SPACING},
    {3,3+CHANNEL_SPACING},{4,4+CHANNEL_SPACING},{5,5+CHANNEL_SPACING},{6,6+CHANNEL_SPACING},{7,7+CHANNEL_SPACING},
    {8,8+CHANNEL_SPACING},{9,9+CHANNEL_SPACING},{10,10+CHANNEL_SPACING},{11,11+CHANNEL_SPACING},{12,12+CHANNEL_SPACING},
    {13,13+CHANNEL_SPACING},{14,14+CHANNEL_SPACING},{15,15+CHANNEL_SPACING},{16,16+CHANNEL_SPACING},{17,17+CHANNEL_SPACING},
    {18,18+CHANNEL_SPACING},{19,19+CHANNEL_SPACING},{20,20+CHANNEL_SPACING},{21,21+CHANNEL_SPACING},{22,22+CHANNEL_SPACING},
    {23,23+CHANNEL_SPACING},{25,24+CHANNEL_SPACING},{26,25+CHANNEL_SPACING},{27,26+CHANNEL_SPACING},{28,28+CHANNEL_SPACING},
    {29,29+CHANNEL_SPACING},{30,30+CHANNEL_SPACING},{31,31+CHANNEL_SPACING},{32,32+CHANNEL_SPACING}


};
#if 1   //查表法计算CRC,计算时间可以节省,处理读40个字节并进行CRC校验的时间为320us
#define CRC_POLY 0x8408

const unsigned int CRC_Tbl[256] = {
0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E,
0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD,
0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C,
0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9,
0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738,
0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7,
0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036,
0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5,
0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134,
0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3,
0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1,
0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330,
0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78,
};
UINT16 cal_crc(UINT8 *ptr, UINT16 len,UINT16 crc)
{
  while(len--)
  {
  crc = (crc >> 8 ) ^ CRC_Tbl [ (crc ^ *ptr++) & 0xFF];
  }
  return crc;
}

#else
#define CRC_POLY 0x8408

//移位法计算CRC校验码 处理读40个字节并进行CRC校验的时间为410us
UINT16 cal_crc(UINT8 *ptr, UINT16 len,UINT16 crc)
{
  UINT8 i;
  while(len--)
  {
    crc^=*ptr++;
    for(i=0;i<8;i++)
    {
      if(crc&0x0001)
      {
        crc >>= 1;
        crc^= CRC_POLY;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  
  return crc;
}

#endif

UINT8 GetLatchRSSIValue(void)
{
  UINT8 rssi_dBm;
  
  bApi_GetFastResponseRegister(SI446X_CMD_ID_FRR_C_READ,SI446X_CMD_ARG_COUNT_FRR_C_READ,&rssi_dBm);
  if(rssi_dBm==0x00)
  {
    rssi_dBm=80;
  }
  rssi_dBm=RSSI_OFFSET-rssi_dBm/2;
//  rssi_dBm=rssi_dBm/2-100;
  //RSSI测量值取绝对值
  if(rssi_dBm>=MIN_RSSI)    
    rssi_dBm=MIN_RSSI;
  else if(rssi_dBm<MAX_RSSI)
    rssi_dBm=MAX_RSSI;
  else
    rssi_dBm=rssi_dBm;
  si4438_phy.PhyRSSI=rssi_dBm;
  return rssi_dBm;

}
UINT8 GetRSSIValue(void)
{
  UINT8 rssi_dBm;
  si446x_get_modem_status(NULL);
  rssi_dBm=Si446xCmd.GET_MODEM_STATUS.CURR_RSSI;
  rssi_dBm=RSSI_OFFSET-rssi_dBm/2;
//  rssi_dBm=rssi_dBm/2-100;
  //RSSI测量值取绝对值
  if(rssi_dBm>=MIN_RSSI)    
    rssi_dBm=MIN_RSSI;
  else if(rssi_dBm<MAX_RSSI)
    rssi_dBm=MAX_RSSI;
  else
    rssi_dBm=rssi_dBm;
  si4438_phy.PhyRSSI=rssi_dBm;
  return rssi_dBm;

}


UINT8 PLME_ED_request(UINT8 ChannelGroup ,UINT8 ChannelNum)
{
  UINT8 i, ED_value[12];
  UINT16 ED_average=0;
  unsigned int tick_tmp;
    
  ChannelSelect(ChannelGroup,ChannelNum);
  vRadio_StartRX(si4438_phy.phyCurrentChannel);
  tick_tmp=GET_SYS_TICK;
  while(GET_SYS_TICK<(tick_tmp+TICK_100MS));  //延时10mS；
  for(i=0;i<12;i++)
  {
    ED_value[i]=GetRSSIValue();
//    printf("%d  ",ED_value[i]);
    tick_tmp=GET_SYS_TICK;
    while(GET_SYS_TICK<(tick_tmp+TICK_10MS));  //延时10mS；
  }
  
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  si4438_phy.RF_status=RF_IDLE_FLAG;
  
  
  for(i=1;i<12;i++)   //取最大值保存到 ED_value[0]
  {
    if(ED_value[0]<ED_value[i])
    {
      ED_value[0]=ED_value[i];
      ED_value[i]=ED_value[0];
    }
  }

  for(i=2;i<12;i++)   //取最小值保存到 ED_value[1]
  {
    if(ED_value[1]>ED_value[i])
    {
      ED_value[1]=ED_value[i];
      ED_value[i]=ED_value[1];
    }
  }
  for(i=2;i<12;i++) //从ED_value[2]  开始计算平均值
  {
    ED_average+=ED_value[i];
  }
  ED_average/=10;
  return ED_average;
}
/*
函数功能：模块静态的RSSI值，该值等于12次采样，去除最高与最低两个极值后进行取平均数处理，再减5个DB的偏移得到的值，
出口参数：1，设置成功；0，设置失败，PhyRSSI_TH设置为80
*/
UINT8 Cal_CCA_TH(void)
{
  UINT8 i, Max_RSSI,Min_RSSI,RSSI_value[12];
  unsigned int tick_tmp;
  UINT16 RSSI_average=0;
  Max_RSSI=0;
  Min_RSSI=0xff;
  if(si4438_phy.RF_status!=RF_RX_FLAG)
  {
    vRadio_StartRX(si4438_phy.SI4438CurrentChannel);
    tick_tmp=GET_SYS_TICK;
    while(GET_SYS_TICK<(tick_tmp+TICK_10MS));  //延时10mS；
  }

  for(i=0;i<12;i++)
  {
  //  tick_tmp=GET_SYS_TICK;
  //  while(GET_SYS_TICK<(tick_tmp+TICK_10MS));  //延时10mS；
    if((RF_receive_flag==0)||(RF_receive_flag==2))
    {
      RSSI_value[i]=GetRSSIValue();
      //DBUG("%d ",RSSI_value[i]);
      if(RSSI_value[i]>Max_RSSI)
      {
        Max_RSSI=RSSI_value[i];
      }
      if(RSSI_value[i]<Min_RSSI)
      {
        Min_RSSI=RSSI_value[i];
      }
      RSSI_average+=RSSI_value[i];
    }
    else
    {
      si4438_phy.PhyRSSI_TH=83;
      return FALSE;
    }
    
  }

  //DBUG("\r\n");
  RSSI_average-=Max_RSSI;
  RSSI_average-=Min_RSSI;
  RSSI_average/=10;
  //DBUG("RSSI_average=%d \r\n",RSSI_average);
  si4438_phy.PhyRSSI_TH=RSSI_average-5;
  if(si4438_phy.PhyRSSI_TH > 96)
  {
    si4438_phy.PhyRSSI_TH=96;
  }
  if(si4438_phy.PhyRSSI_TH < 80)
    si4438_phy.PhyRSSI_TH=80;
  return TRUE;
  
}
UINT8 GET_CCA(UINT8 ChannelGroup ,UINT8 ChannelNum)
{
  UINT8 i,rssi_threshold;
  
  ChannelSelect(ChannelGroup,ChannelNum);
  vRadio_StartRX(si4438_phy.phyCurrentChannel);
//  si4438_phy.phyCCAMode=1;
  
  for(i=0;i<3;i++)  //一次测试RSSI在110Us以上，
  {
    rssi_threshold=GetRSSIValue();
    if(rssi_threshold<si4438_phy.PhyRSSI_TH)
    {
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
      si4438_phy.RF_status=RF_IDLE_FLAG;
      return 1;
    }
  }
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  si4438_phy.RF_status=RF_IDLE_FLAG;
  return 0;
}

UINT8 ChannelSelect(UINT8 ChannelGroup ,UINT8 ChannelNum)
{
  
  if((ChannelGroup<=32)&&(ChannelNum<=1))
  {
    si4438_phy.SI4438CurrentChannel=ChannelsSupported[ChannelGroup][ChannelNum];
  }
  else
  {
    ChannelGroup=PUBLIC_CHANNEL;
    ChannelNum=0;
    si4438_phy.SI4438CurrentChannel=ChannelsSupported[ChannelGroup][ChannelNum];
  }
  
  si4438_phy.phyCurrentChannel=ChannelNum;
  si4438_phy.phyCurrentChannelGroup=ChannelGroup;
  //si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  //si4438_phy.RF_status=RF_IDLE_FLAG;
  return TRUE;
}
volatile unsigned int Txticktimeout;       

void wait_tx_ok(UINT8 tx_flag)
{
  unsigned int tick_tmp;
  
  tick_tmp=GET_SYS_TICK;
  si4438_phy.tx_flag=TX_ON;
  while(((tick_tmp+Txticktimeout)>GET_SYS_TICK)&&(si4438_phy.tx_flag!=tx_flag))
  {
    if(si4438_phy.RF_int_flag)
    {
      gRadio_CheckReceived_VariablePacket();
    }

  }
  
  if((si4438_phy.tx_flag!=tx_flag))
  {
    
    DBUG("tx_flag ERR %d \r\n",tx_flag);
    wireless_phy_init();
  
  }
}

extern unsigned char Speed;
extern host_t host;
UINT8  Si4438_SendFrame(UINT8 *buf, UINT8 length)
{
  unsigned char remainder=0;
  
  UINT16 crc16_result;
  //DBUG("SendFrame");
  if((length>aMaxPHYPayloadSize)||(length==0))
  {
    Receive_init();
    return FALSE;
  }
  if(Rf_set()==FALSE)       //set RF_PA_MODE_5
  {
    DBUG("Rf_set");
    wireless_phy_init();
  }
#ifdef LOW_RATE_CHECK
  if((host.work_status == AFN_13_F1)&&(Speed != 0))
  {
    switch(Speed)
    {
      case 1:
        si4438_phy.SendRate = MEDIUM_RATE;
        break;
      case 2:
        si4438_phy.SendRate = LOW_RATE;
        break;
      default:
        si4438_phy.SendRate = HIGH_RATE;
        break;        
    }      
  }

  if(si4438_phy.SendRate==LOW_RATE)
  {
    
    SetLowRate();
    Txticktimeout = TICK_4S;
  }
  else if(si4438_phy.SendRate == MEDIUM_RATE)
  {
    SetMediumRate();
    Txticktimeout = TICK_2S;
  }
  else
  {
    //SetHighRate();
    Txticktimeout = TICK_500MS;
  }
#else
  Txticktimeout = TICK_500MS; 
#endif

  si4438_phy.ppdu.FrameLength=length+3; //??=PSDU+3
  si4438_phy.ppdu.ChannelIndex=si4438_phy.phyCurrentChannelGroup*2+si4438_phy.phyCurrentChannel;    //????
  si4438_phy.ppdu.identify=IDENTIFY;  //?????
  si4438_phy.ppdu.FrameHeadCheck=(si4438_phy.ppdu.FrameLength^si4438_phy.ppdu.ChannelIndex)^si4438_phy.ppdu.identify;  //????
  si4438_phy.ppdu.PHYPayload=buf;   //??????
if(host.dbg_switch&(DBGINFO_PRINTF))
{
  unsigned char cont;
  printf("\r\n  SendFrame Data -->");
  if(si4438_phy.SendRate == LOW_RATE)
  {
    printf("  LOW_RATE");
  }else if(si4438_phy.SendRate == MEDIUM_RATE)
  {
    printf("  MEDIUM_RATE");
  }else
  {
    printf("  HIGH_RATE");
  }
  printf("\r\nChannelGroup:%02d  Channel:%02d",si4438_phy.phyCurrentChannelGroup,si4438_phy.phyCurrentChannel);
  printf("\r\nlength:%d\r\n",length);
  for(cont=0;cont<length;cont++)
  {
    printf("%02X",buf[cont]);
  } 
  printf("\r\n");
}
  TXD_LED(1);
  //PHR CRC check
  crc16_result=cal_crc((U8 *)&si4438_phy.ppdu.FrameLength, 4,0xffff);
  //PSDU CRC check
  crc16_result=cal_crc(buf,length, crc16_result);   //???
  crc16_result^=0xffff;
  si4438_phy.ppdu.crc[0]=crc16_result&0x00ff;
  si4438_phy.ppdu.crc[1]=(crc16_result&0xff00)>>8;
  set_txd_mode();
  remainder=length;

  si446x_write_tx_fifo(4, (U8 *)&si4438_phy.ppdu.FrameLength);
  
  if(length<=MAX_PACKET_LENGTH)
  {
    si446x_write_tx_fifo(length, buf);
    si446x_write_tx_fifo(2,(U8 *)si4438_phy.ppdu.crc);     //??CRC??
    si446x_start_tx(si4438_phy.SI4438CurrentChannel, 0x30, length+4+2);
    wait_tx_ok(TX_OK);
    if(si4438_phy.tx_flag!=TX_OK)
    {
      Clear_int_status();
      Receive_init();
      TXD_LED(0);
      return FALSE;
    }
  }
  else
  {
    si446x_write_tx_fifo(MAX_PACKET_LENGTH, buf);
    si446x_start_tx(si4438_phy.SI4438CurrentChannel, 0x30, length+4+2);
    remainder-=MAX_PACKET_LENGTH;
    buf+=MAX_PACKET_LENGTH;
    
    si4438_phy.tx_flag=TX_ON;
    while(remainder)
    {
      wait_tx_ok(TX_EMPTY);
      
      if(si4438_phy.tx_flag==TX_EMPTY)
      {
        if(remainder<=RADIO_TX_ALMOST_EMPTY_THRESHOLD)
        {
          
          si446x_write_tx_fifo(remainder, buf);
          si446x_write_tx_fifo(2,(U8 *)si4438_phy.ppdu.crc);   //??CRC??
          remainder=0;
          buf+=RADIO_TX_ALMOST_EMPTY_THRESHOLD;
        }
        else
        {
          si446x_write_tx_fifo(RADIO_TX_ALMOST_EMPTY_THRESHOLD, buf);
          remainder-=RADIO_TX_ALMOST_EMPTY_THRESHOLD;
          buf+=RADIO_TX_ALMOST_EMPTY_THRESHOLD;
        }
        si4438_phy.tx_flag=TX_ON;
      }
      else
      {
        Clear_int_status();
        Receive_init();
        TXD_LED(0);
        return FALSE;
      }
    }
    wait_tx_ok(TX_OK);
    if(si4438_phy.tx_flag!=TX_OK)
    {
      
      Clear_int_status();
      Receive_init();
      TXD_LED(0);
      return FALSE;
    }
    
  }

  Clear_int_status();
  Receive_init();
  TXD_LED(0);
  return TRUE;
    
}


U8  *pPositionInPayload; //接收数据BUF指针
volatile unsigned int rx_tick_tmp;  //系统TICK备份
volatile unsigned int rx_tick_time_out;       

volatile U8 RF_receive_flag;       //0:没有收到任何有效数据，1:收到同步头，2:CRC错误 3：从SI4438中读取40个字节后在3的基础上累加1；
U16 crc16_result =0xffff;

//初始化接收，重新接收数据
void Receive_init(void) 
{
  RXD_LED(0);
  si4438_phy.SendRate = HIGH_RATE;
  RF_receive_flag=0;
  si4438_phy.timer_out_flag=0;
  vRadio_StartRX(si4438_phy.SI4438CurrentChannel);
}

UINT8 ReadFrameHead(void)
{
  U8 HeadCheck;
  crc16_result =0xffff;
//  GetLatchRSSIValue();
  
  si4438_phy.SI4438CurrentChannel=bRadio_GetCurrentChannel();
  
  si446x_read_rx_fifo(4,(U8 *)&si4438_phy.ppdu.FrameLength);

  HeadCheck=(si4438_phy.ppdu.FrameLength^si4438_phy.ppdu.ChannelIndex)^si4438_phy.ppdu.identify;
  if(si4438_phy.ppdu.FrameHeadCheck!=HeadCheck)
  {
    Receive_init();
    return FALSE;
  } 
  return TRUE;
}
#define GOOD_CRC 0xF0B8

UINT8 receive_done(UINT8 *buf, UINT8  *length)
{
  U16 crc_r;
  crc_r = crc_r;
  si4438_phy.ReceiveRate = PreambleCheck.CurrentRate;
  
  
  *length=si4438_phy.ppdu.FrameLength-3;
  
  if(crc16_result == GOOD_CRC)
  { 
    Receive_init();
    si4438_phy.rx_flag=RX_DONE;
    si4438_phy.phyCurrentChannelGroup=si4438_phy.ppdu.ChannelIndex/2;
    si4438_phy.phyCurrentChannel=si4438_phy.ppdu.ChannelIndex%2;
    return TRUE;
  }
  else
  {
    Receive_init();
    RXD_LED(0);
    crc_r=(buf[*length+1]<<8)+buf[*length];
    DBUG("crc_result=%04x crc_r=%04x ",crc_result,crc_r);
    RF_receive_flag=2;
    si4438_phy.rx_flag= RX_CRC_ERR;
    return FALSE;
  }

}
unsigned char Si4438HopRegister[12]={0x30, 0x04, 0x18, 0x42, 0x18, 0x42, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

unsigned char Si4438_check(void)
{
  static UINT32 static_tick_tmp=0;
  if((RF_receive_flag==0)||(RF_receive_flag==2))
  {   
    if(GET_SYS_TICK>si4438_phy.rx_tick_tmp+30*60*TICK_1S)
    {     
      wireless_phy_init();
      si4438_phy.rx_tick_tmp=GET_SYS_TICK;      
      return 1;
    }
    if(GET_SYS_TICK>static_tick_tmp+60*TICK_1S)
    {
      DBUG("Si4438_check");
      if(get_property()==FALSE)
      {
        DBUG("Si4438_check");
        wireless_phy_init();
        return 1;
      }
      static_tick_tmp=GET_SYS_TICK;     
    }
  }
  return 1;
}
#ifdef LOW_RATE_CHECK

U8* gpSetPropCmd;

void DoRateConfigurationProcess(void)
{
  unsigned char i,j;
  if(ConfigRate.ConfigState==STOP_CONFIG)
  {
    return;
  }
  switch (ConfigRate.ConfigState) 
  {
    case START_CONFIG :
      CLK_EXTI_Configuration(DISABLE);
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
      gpSetPropCmd=(unsigned char *)LowRateRadioComfiguration;
      ConfigRate.ConfigState=NEXT1_CONFIG;
      break;
    case NEXT1_CONFIG:
      for(j=0;j<2;j++)
      {
        if (*gpSetPropCmd != 0x00)
        {
          ConfigRate.numOfBytes = *gpSetPropCmd++;
          for (i = 0u; i < ConfigRate.numOfBytes; i++)
            {
              Pro2Cmd[i] = *gpSetPropCmd;
              gpSetPropCmd++;
            }
          if(set_property(Pro2Cmd,ConfigRate.numOfBytes)!=SI446X_SUCCESS)
          {
            ConfigRate.ConfigState=STOP_CONFIG;
                return ;
          }
          /*
          if (radio_comm_SendCmdGetResp(ConfigRate.numOfBytes, Pro2Cmd, 0, 0) != 0xFF)
            {
            ConfigRate.ConfigState=STOP_CONFIG;
              return ;
            }

            if (radio_hal_NirqLevel() == 0)
            {
              si446x_get_int_status(0, 0, 0);
            
              if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
              {
              ConfigRate.ConfigState=STOP_CONFIG;
                return ;
              }
            }
          */
        }
        else
        {
          
          ConfigRate.ConfigState=NEXT2_CONFIG;
        }
      }
      
      break;  
    case NEXT2_CONFIG:
      PA_Rxen();
      si446x_get_int_status(0u, 0u, 0u);
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
      si446x_start_rx(PreambleCheck.channel, 0u, 0x00,
              SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
              SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
              SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
      si4438_phy.RF_status=RF_RX_FLAG;
      rx_tick_time_out = TICK_500MS;
      PreambleCheck.rx_tick_tmp=GET_SYS_TICK;
      PreambleCheck.CurrentRate = HIGH_RATE;
      ConfigRate.ConfigState=STOP_CONFIG;
      break;
    case STOP_CONFIG:
      break;
    default:
      ConfigRate.ConfigState=STOP_CONFIG;
      break;

     
  
  } 
}
#endif
UINT8  Si4438_ReceiveFrame(UINT8 *buf, UINT8  *length)
{
#ifdef LOW_RATE_CHECK
#ifdef DOUBLE_MOD_SLAVE

  DoRateConfigurationProcess();
#endif

#endif
  if(set_pchannel!=si4438_phy.PhyPrivateChannel)
  {
    set_hop_table(si4438_phy.PhyPrivateChannel);
  }
  if(si4438_phy.RF_status!=RF_RX_FLAG)
  {
    vRadio_StartRX(si4438_phy.SI4438CurrentChannel);
    
  }
  si4438_phy.rx_flag=RX_ON;
  if(si4438_phy.RF_int_flag)
  {
    gRadio_CheckReceived_VariablePacket();
  }
  if((si4438_phy.rx_flag==RX_OK)||(si4438_phy.rx_flag==RX_DONE))
  {

    if(RF_receive_flag==1) //接收到同步头
    {
    
      RF_receive_flag=3;  //标志位置接收数据模式
      if(si4438_phy.rx_flag==RX_DONE)
      {
        if(ReadFrameHead()==TRUE)
        {
          pPositionInPayload=buf;
          crc16_result=cal_crc((U8 *)&si4438_phy.ppdu.FrameLength, 4,0xffff);
          *length=si4438_phy.ppdu.FrameLength-3;
          si446x_fifo_info(0u);
          
          if((pPositionInPayload+Si446xCmd.FIFO_INFO.RX_FIFO_COUNT-buf)<=254)
          {
            si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT,pPositionInPayload); //read PHR
            crc16_result=cal_crc(pPositionInPayload,Si446xCmd.FIFO_INFO.RX_FIFO_COUNT,crc16_result);
          }
          pPositionInPayload+=Si446xCmd.FIFO_INFO.RX_FIFO_COUNT;
          
        }
        else
        {
          pPositionInPayload=buf;
          Receive_init();
          return FALSE;
        }
      }
      else
      {
        if(ReadFrameHead()==TRUE)
        {
          pPositionInPayload=buf;
          crc16_result=cal_crc((U8 *)&si4438_phy.ppdu.FrameLength, 4,0xffff);
          *length=si4438_phy.ppdu.FrameLength-3;
          if((pPositionInPayload+20-buf)<=254)
          {
            si446x_read_rx_fifo(20,pPositionInPayload);
            crc16_result=cal_crc(pPositionInPayload,20,crc16_result);
          }
          
          pPositionInPayload+=20;
          
        }
        else
        {
          pPositionInPayload=buf;
          Receive_init();
          return FALSE;
        }
      }
    }
    else if(RF_receive_flag>=3)
    {
      
      //RF_receive_flag++;
      if(si4438_phy.rx_flag==RX_DONE)
      {
        si446x_fifo_info(0u);
        if((pPositionInPayload+Si446xCmd.FIFO_INFO.RX_FIFO_COUNT-buf)<=254)
        {
          si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT,pPositionInPayload); //read PHR
          crc16_result=cal_crc(pPositionInPayload,Si446xCmd.FIFO_INFO.RX_FIFO_COUNT,crc16_result);
        }
        
        pPositionInPayload+=Si446xCmd.FIFO_INFO.RX_FIFO_COUNT;
        
      }
      else
      {
  //      si4438_phy.rx_flag=RX_ON;
        if((pPositionInPayload+RADIO_RX_ALMOST_FULL_THRESHOLD-buf)<=254)
        {
          si446x_read_rx_fifo(RADIO_RX_ALMOST_FULL_THRESHOLD,pPositionInPayload);
          crc16_result=cal_crc(pPositionInPayload,RADIO_RX_ALMOST_FULL_THRESHOLD,crc16_result);
        }
        
        pPositionInPayload+=RADIO_RX_ALMOST_FULL_THRESHOLD;
        
      } 
    }
    else
    {
      DBUG("RF_receive_flag !=1 && <3");
      wireless_phy_init();
      pPositionInPayload=buf;
      Receive_init();
      return FALSE;
    }
    
    if((*length+2)<=(pPositionInPayload-buf)||(pPositionInPayload-buf)>=254)
    {
      pPositionInPayload=buf;
      return receive_done(buf,length);
    }
    si4438_phy.timer_out_flag=1; //启动接收超时标志
    rx_tick_tmp=GET_SYS_TICK;
    
    return FALSE;
  }
#ifndef LOW_RATE_CHECK
  rx_tick_time_out = TICK_100MS;
#endif
  if(si4438_phy.timer_out_flag)
  {
    if(GET_SYS_TICK>(rx_tick_tmp+rx_tick_time_out))  //在时间内未收到任何中断标志，
    {
      if(RF_receive_flag>=3)
      {
        si446x_fifo_info(0u);
        if((pPositionInPayload+Si446xCmd.FIFO_INFO.RX_FIFO_COUNT-buf)<=254)
        {
          if(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT!=0)  //如果FIFO中还有数据，读出并CRC 判断
          {
            //DBUG("length=%d RX_FIFO_COUNT=%d \r\n",si4438_phy.ppdu.FrameLength+3,Si446xCmd.FIFO_INFO.RX_FIFO_COUNT);
            RF_receive_flag++;
            if((pPositionInPayload+Si446xCmd.FIFO_INFO.RX_FIFO_COUNT-buf)<=254)
            {
              si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT,pPositionInPayload); //read PHR
              crc16_result=cal_crc(pPositionInPayload,Si446xCmd.FIFO_INFO.RX_FIFO_COUNT,crc16_result);
            }
            
            pPositionInPayload+=Si446xCmd.FIFO_INFO.RX_FIFO_COUNT;
            
          }
          else
          {
            
            si4438_phy.RF_status=RF_IDLE_FLAG;
          }
          if((*length+2)<=(pPositionInPayload-buf)||(pPositionInPayload-buf)>=254)
          {
            pPositionInPayload=buf;
            return receive_done(buf,length);
          }
        }
      }
      DBUG("rx_tick_time_out RF_receive_flag=%d ",RF_receive_flag);
      wireless_phy_init();
      pPositionInPayload=buf;

      return FALSE;
    }
    
  }
  
#ifdef LOW_RATE_CHECK
  if(PreambleCheck.CurrentRate!=HIGH_RATE)
  {
    if(GET_SYS_TICK>(PreambleCheck.rx_tick_tmp+rx_tick_time_out))  //在1s时间内未收到任何中断标志，
    {
      if((RF_receive_flag==0)||(RF_receive_flag==2))
      {
        Receive_init();
        DBUG("PreambleCheck FAILE %d\r\n",PreambleCheck.CurrentRate);
      }
    }
  }
#endif
  Si4438_check();
  return FALSE;
}


