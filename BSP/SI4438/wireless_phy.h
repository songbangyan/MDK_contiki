#ifndef WIRELESS_PHY_H__
#define WIRELESS_PHY_H__
#include "common.h"
#include "Low_Rate_Check.h"

#define PUBLIC_CHANNEL  0x00
#define CHANNEL_BUILDNET   0


#define RSSI_OFFSET  143

//发送标志 0:发送失败，1:发送状态，2:发送成功
typedef enum
{
    TX_FAIL   = 0, 
    TX_ON   = 1,
    TX_OFF   = 2,
    TX_EMPTY  = 3,
    TX_OK  = 4,
} TX_FLAG;

//接收标志 0：接收失败，1：接收状态，2：查找到前导，3：查找到同步字节，4：接收成功,5:CRC错误，6：超时
typedef enum
{
    RX_FAIL   = 0, 
    RX_ON   = 1,
    RX_PRE  = 2,
    RX_SYNC=3,
    RX_OK=4,
    RX_CRC_ERR=5,
    RX_DONE=6,
} RX_FLAG;
//无线状态标志，RF_IDLE_FLAG：空闲模式，RF_RX_FLAG：接收模式， RF_TX_FLAG：发送模式

typedef enum
{
    RF_IDLE_FLAG   = 0, 
    RF_RX_FLAG =1,
    RF_TX_FLAG =2,
} RF_FLAG;
/* define channel*/
#define CHANNEL_GROUP_0_0      24
#define CHANNEL_GROUP_0_1      24+42

#define CHANNEL_GROUP_1_0      0
#define CHANNEL_GROUP_SPACING  1
#define CHANNEL_SPACING        39

#define MAX_CHANNEL_GROUP_NUM  32

#define CHANNEL_NUM 2   //最多可以监听的通道组数

/*define  RSSI*/
#define MAX_RSSI  40
#define MIN_RSSI  120

/*define CCA*/
#define CCA_TH  (-96)  //UNIT  dBm
/*define  PPDU */
#define PREAMBLE 0xAA

#define SFDL 0x98
#define SFDH 0xF3

#define PREAMBLEBYTES  80

#define IDENTIFY 1

struct  PPDU_STRUCT
{
  UINT8 PreambleBytes;
  UINT8 Sfd[2];
  UINT8 FrameLength;
  UINT8 ChannelIndex;
  UINT8 identify;
  UINT8 FrameHeadCheck;
  UINT8 *PHYPayload;
  UINT8  crc[2];
};


#define MAX_PRIVATECHANNEL     32          //定义私有信道最大信道组号

struct WIRELESS_PHY
{
  UINT8 phyCurrentChannel;  //当前信道号:0~1
  UINT8 phyCurrentChannelGroup; //当前信道组号:0~32
  UINT8 PhyRSSI;     //接收到的RSSI值40~120
  UINT8 PhyRSSI_TH;  //针对空闲信道评估CCA的阀值
//  UINT8 phyCCAMode;
  UINT8 SI4438CurrentChannel;  //SI4438基于基准频率471.5MHZ信道间隔200KHZ的信道号
  UINT8 PhyPrivateChannel;     //私有信道组号，对该值赋值后，SI4438接同时监听公共信道组号0和该私有信道组号，即4个信道。
  struct PPDU_STRUCT ppdu;
  RX_FLAG rx_flag;
  TX_FLAG tx_flag;
  RF_FLAG RF_status;
  RATE_FLAG ReceiveRate;  //接收信号速率标志，当正确接收到数据或接收数据CRC错误时设置该标志，RX_LOW_RATE:为低速率；RX_HIGH_RATE:为高速率
  RATE_FLAG SendRate; //发送数据速率标志:TX_LOW_RATE:选择低速率发送;TX_HIGH_RATE:选择高速率发送
  UINT8  RF_int_flag;
  UINT8  SI4438_reset_flag; //SI4438复位标志；复位成功=1，复位失败=0;SI4438最多初始化4次；
  UINT8 timer_out_flag;  //数据包接收超时标志，为1时，程序在200ms内没有收到正常的数据，数据丢包。
  unsigned int rx_tick_tmp;
};
extern volatile  struct WIRELESS_PHY  si4438_phy;

extern volatile U8 RF_receive_flag;  //0:没有收到任何有效数据，1:收到同步头，2:CRC错误 3：从SI4438中读取40个字节后在3的基础上累加1；
extern volatile unsigned int rx_tick_tmp;  //系统TICK备份
extern volatile unsigned int rx_tick_time_out;       



UINT8 GetLatchRSSIValue(void);
UINT8 GET_CCA(UINT8 ChannelGroup ,UINT8 ChannelNum);
/*信道频率选择
入口参数: ChannelGroup,信道组号:0~32；ChannelNum，信道号:0~1;
出口参数：成功 0；失败 -1
*/
UINT8 ChannelSelect(UINT8 ChannelGroup ,UINT8 ChannelNum);
/*能量检测信道
入口参数: ChannelGroup,信道组号；ChannelNum，信道号
出口参数：12次ED采样，去除最高与最低两个极值后进行取平均数处理得到ED值
*/
UINT8 PLME_ED_request(UINT8 ChannelGroup ,UINT8 ChannelNum);

/*按照 PhyChannel  的函数设置的通道 发送数据
入口参数：*buf，待发送数据的指针；length；发送数据长度  最大aMaxPHYPayloadSize个字节
出口参数：成功 1；失败0
*/
UINT8  Si4438_SendFrame(UINT8 *buf, UINT8 length);
/*接收数据
对设置的私有通道si4438_phy.PhyPrivateChannel及公共通道进行检测，正确接收到数据后，
入口参数：*buf，待接收数据的指针；*length；接收到数据的长度指针,
si4438_phy.SendRate://发送数据速率标志:TX_LOW_RATE:选择低速率发送;TX_HIGH_RATE:选择高速率发送

出口参数：成功 1；失败 0 ；
si4438_phy.ReceiveRate  //接收信号速率标志，当正确接收到数据或接收数据CRC错误时设置该标志，RX_LOW_RATE:为低速率；RX_HIGH_RATE:为高速率

si4438_phy.phyCurrentChannelGroup为在该信道组接收到数据
si4438_phy.phyCurrentChannel为在该信道号接收到数据

*/
UINT8  Si4438_ReceiveFrame(UINT8 *buf, UINT8  *length);
void Receive_init(void) ;

void  wireless_phy_init(void);
/*
函数功能：检测SI4438相关参数是否正常，如果异常调用wireless_phy_init函数将其初始化，检测跳频表值，如果芯片处于接受模式，检测芯片当前状态是否为接受状态。
芯片正常时处理时间：1.3mS;
出口参数：1：芯片正常，0：芯片异常，
*/
unsigned char Si4438_check(void );

UINT8 Cal_CCA_TH(void);
extern volatile  struct WIRELESS_PHY  si4438_phy;
extern const UINT8 ChannelsSupported[MAX_CHANNEL_GROUP_NUM+1][2];
extern unsigned char Si4438HopRegister[12];  //跳频表寄存器备份；
#endif


