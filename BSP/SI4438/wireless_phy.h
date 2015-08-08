#ifndef WIRELESS_PHY_H__
#define WIRELESS_PHY_H__
#include "common.h"
#include "Low_Rate_Check.h"

#define PUBLIC_CHANNEL  0x00
#define CHANNEL_BUILDNET   0


#define RSSI_OFFSET  143

//���ͱ�־ 0:����ʧ�ܣ�1:����״̬��2:���ͳɹ�
typedef enum
{
    TX_FAIL   = 0, 
    TX_ON   = 1,
    TX_OFF   = 2,
    TX_EMPTY  = 3,
    TX_OK  = 4,
} TX_FLAG;

//���ձ�־ 0������ʧ�ܣ�1������״̬��2�����ҵ�ǰ����3�����ҵ�ͬ���ֽڣ�4�����ճɹ�,5:CRC����6����ʱ
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
//����״̬��־��RF_IDLE_FLAG������ģʽ��RF_RX_FLAG������ģʽ�� RF_TX_FLAG������ģʽ

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

#define CHANNEL_NUM 2   //�����Լ�����ͨ������

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


#define MAX_PRIVATECHANNEL     32          //����˽���ŵ�����ŵ����

struct WIRELESS_PHY
{
  UINT8 phyCurrentChannel;  //��ǰ�ŵ���:0~1
  UINT8 phyCurrentChannelGroup; //��ǰ�ŵ����:0~32
  UINT8 PhyRSSI;     //���յ���RSSIֵ40~120
  UINT8 PhyRSSI_TH;  //��Կ����ŵ�����CCA�ķ�ֵ
//  UINT8 phyCCAMode;
  UINT8 SI4438CurrentChannel;  //SI4438���ڻ�׼Ƶ��471.5MHZ�ŵ����200KHZ���ŵ���
  UINT8 PhyPrivateChannel;     //˽���ŵ���ţ��Ը�ֵ��ֵ��SI4438��ͬʱ���������ŵ����0�͸�˽���ŵ���ţ���4���ŵ���
  struct PPDU_STRUCT ppdu;
  RX_FLAG rx_flag;
  TX_FLAG tx_flag;
  RF_FLAG RF_status;
  RATE_FLAG ReceiveRate;  //�����ź����ʱ�־������ȷ���յ����ݻ��������CRC����ʱ���øñ�־��RX_LOW_RATE:Ϊ�����ʣ�RX_HIGH_RATE:Ϊ������
  RATE_FLAG SendRate; //�����������ʱ�־:TX_LOW_RATE:ѡ������ʷ���;TX_HIGH_RATE:ѡ������ʷ���
  UINT8  RF_int_flag;
  UINT8  SI4438_reset_flag; //SI4438��λ��־����λ�ɹ�=1����λʧ��=0;SI4438����ʼ��4�Σ�
  UINT8 timer_out_flag;  //���ݰ����ճ�ʱ��־��Ϊ1ʱ��������200ms��û���յ����������ݣ����ݶ�����
  unsigned int rx_tick_tmp;
};
extern volatile  struct WIRELESS_PHY  si4438_phy;

extern volatile U8 RF_receive_flag;  //0:û���յ��κ���Ч���ݣ�1:�յ�ͬ��ͷ��2:CRC���� 3����SI4438�ж�ȡ40���ֽں���3�Ļ������ۼ�1��
extern volatile unsigned int rx_tick_tmp;  //ϵͳTICK����
extern volatile unsigned int rx_tick_time_out;       



UINT8 GetLatchRSSIValue(void);
UINT8 GET_CCA(UINT8 ChannelGroup ,UINT8 ChannelNum);
/*�ŵ�Ƶ��ѡ��
��ڲ���: ChannelGroup,�ŵ����:0~32��ChannelNum���ŵ���:0~1;
���ڲ������ɹ� 0��ʧ�� -1
*/
UINT8 ChannelSelect(UINT8 ChannelGroup ,UINT8 ChannelNum);
/*��������ŵ�
��ڲ���: ChannelGroup,�ŵ���ţ�ChannelNum���ŵ���
���ڲ�����12��ED������ȥ����������������ֵ�����ȡƽ��������õ�EDֵ
*/
UINT8 PLME_ED_request(UINT8 ChannelGroup ,UINT8 ChannelNum);

/*���� PhyChannel  �ĺ������õ�ͨ�� ��������
��ڲ�����*buf�����������ݵ�ָ�룻length���������ݳ���  ���aMaxPHYPayloadSize���ֽ�
���ڲ������ɹ� 1��ʧ��0
*/
UINT8  Si4438_SendFrame(UINT8 *buf, UINT8 length);
/*��������
�����õ�˽��ͨ��si4438_phy.PhyPrivateChannel������ͨ�����м�⣬��ȷ���յ����ݺ�
��ڲ�����*buf�����������ݵ�ָ�룻*length�����յ����ݵĳ���ָ��,
si4438_phy.SendRate://�����������ʱ�־:TX_LOW_RATE:ѡ������ʷ���;TX_HIGH_RATE:ѡ������ʷ���

���ڲ������ɹ� 1��ʧ�� 0 ��
si4438_phy.ReceiveRate  //�����ź����ʱ�־������ȷ���յ����ݻ��������CRC����ʱ���øñ�־��RX_LOW_RATE:Ϊ�����ʣ�RX_HIGH_RATE:Ϊ������

si4438_phy.phyCurrentChannelGroupΪ�ڸ��ŵ�����յ�����
si4438_phy.phyCurrentChannelΪ�ڸ��ŵ��Ž��յ�����

*/
UINT8  Si4438_ReceiveFrame(UINT8 *buf, UINT8  *length);
void Receive_init(void) ;

void  wireless_phy_init(void);
/*
�������ܣ����SI4438��ز����Ƿ�����������쳣����wireless_phy_init���������ʼ���������Ƶ��ֵ�����оƬ���ڽ���ģʽ�����оƬ��ǰ״̬�Ƿ�Ϊ����״̬��
оƬ����ʱ����ʱ�䣺1.3mS;
���ڲ�����1��оƬ������0��оƬ�쳣��
*/
unsigned char Si4438_check(void );

UINT8 Cal_CCA_TH(void);
extern volatile  struct WIRELESS_PHY  si4438_phy;
extern const UINT8 ChannelsSupported[MAX_CHANNEL_GROUP_NUM+1][2];
extern unsigned char Si4438HopRegister[12];  //��Ƶ��Ĵ������ݣ�
#endif


