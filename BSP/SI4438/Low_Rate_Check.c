#include "si446x_defs.h"
#include "radio.h"
#include "common.h"
#include "si446x_api_lib.h"
#include "wireless_phy.h"
#include "Low_Rate_Check.h"
//#include "systick.h"

volatile PREAMBLE_LOW_RATE PreambleCheck;
void CLK_EXTI_Configuration(FunctionalState State)
{
    EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = GDIO0_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = State;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(GDIO0_EXTI);


}
CONFIG_LOW_RATE ConfigRate;
const unsigned char LowRateRadioComfiguration[]=LOW_RADIO_CONFIGURATION_DATA_ARRAY;
//const unsigned char LowLowRateRadioComfiguration[]=LOW_LOW_RADIO_CONFIGURATION_DATA_ARRAY;
const unsigned char MediumRateRadioComfiguration[]=MID_RADIO_CONFIGURATION_DATA_ARRAY;
const unsigned char HighRateRadioComfiguration[]=HIGH_RADIO_CONFIGURATION_DATA_ARRAY;

	
void SetLowRate(void)
{
 
	CLK_EXTI_Configuration(DISABLE);
	DBUG("SetLowRate ");
	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
	si4438_phy.RF_status=RF_IDLE_FLAG;
	PreambleCheck.channel=bRadio_GetCurrentChannel();
	if (SI446X_SUCCESS != si446x_configuration_init(LowRateRadioComfiguration))
	{
		DBUG("SetLowRate fail\r\n");
		wireless_phy_init();
	}
	
	PA_Rxen();
	si446x_get_int_status(0u, 0u, 0u);
	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
	si446x_start_rx(PreambleCheck.channel, 0u, 0x00,
				  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
				  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
				  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
	si4438_phy.RF_status=RF_RX_FLAG;
	rx_tick_time_out = TICK_4S;
	PreambleCheck.rx_tick_tmp=GET_SYS_TICK;
	PreambleCheck.CurrentRate=LOW_RATE;
 }
void SetMediumRate(void)
{
 
	CLK_EXTI_Configuration(DISABLE);
	DBUG("SetMediumRate ");
	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
	si4438_phy.RF_status=RF_IDLE_FLAG;
	PreambleCheck.channel=bRadio_GetCurrentChannel();
	if (SI446X_SUCCESS != si446x_configuration_init(MediumRateRadioComfiguration))
	{
		DBUG("SetMediumRate fail\r\n");
		wireless_phy_init();
	}
	PA_Rxen();
	si446x_get_int_status(0u, 0u, 0u);
	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
	si446x_start_rx(PreambleCheck.channel, 0u, 0x00,
				  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
				  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
				  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
	si4438_phy.RF_status=RF_RX_FLAG;
	rx_tick_time_out = TICK_1S;
	PreambleCheck.rx_tick_tmp=GET_SYS_TICK;
	PreambleCheck.CurrentRate=MEDIUM_RATE;
 }

void SetHighRate(void)
{
#ifdef LOW_RATE_CHECK
	if(PreambleCheck.CurrentRate!=HIGH_RATE)
	{
		DBUG("SetHighRate ");
		si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
		si4438_phy.RF_status=RF_IDLE_FLAG;
		//PreambleCheck.channel=bRadio_GetCurrentChannel();
		if (SI446X_SUCCESS != si446x_configuration_init(HighRateRadioComfiguration))
		{
			DBUG("SetHighRate fail\r\n");
			wireless_phy_init();
		}
	}
	
	PreambleCheck.CurrentRate=HIGH_RATE;
	CLK_EXTI_Configuration(ENABLE);
	
	PreambleCheck.CheckStartFlag=START_CHECK;
	rx_tick_time_out = TICK_100MS;
#endif
}
#if 0
void CheckLowRatePreamble(void)
{
	switch (PreambleCheck.CheckStartFlag)
	{
		case START_CHECK:  //启动低速率前导码检测
			PreambleCheck.BitTmp=RX_DATA_PIN();
			PreambleCheck.Rate_100us=1;
			PreambleCheck.CheckStartFlag=CHECK_EQUALLYNUM;
			PreambleCheck.ValidNum=0;
			break;
		case CHECK_EQUALLYNUM:  //检测连续相同码流
			if(RX_DATA_PIN()==PreambleCheck.BitTmp)
			{
				//eambleCheck.BitTmp=radio_hal_Gpio2Level();
				PreambleCheck.Rate_100us++;
			}
			else
			{
				PreambleCheck.BitTmp=RX_DATA_PIN();
				if((PreambleCheck.Rate_100us>=CHECK_MIN_RATE)&&(PreambleCheck.Rate_100us<=CHECK_MAX_RATE))
				{
					PreambleCheck.ValidNum++;
					PreambleCheck.Rate_100us=1;
					PreambleCheck.CheckStartFlag=CHECK_VALID_NUM;
				}
				else
				{
					PreambleCheck.CheckStartFlag=START_CHECK;
				}
				
			}
			break;
		case CHECK_VALID_NUM:   //检测低速率0101码流前导码位数
			if(RX_DATA_PIN()==PreambleCheck.BitTmp)
			{
				PreambleCheck.Rate_100us++;
				if(PreambleCheck.ValidNum>=MAX_CHECK_PREAMBLE_NUM)
				{
					PreambleCheck.CheckStartFlag=CHECK_VALID;
					
				}
				else
				{
					PreambleCheck.CheckStartFlag=CHECK_EQUALLYNUM;
				}
			}
			else
			{
				PreambleCheck.CheckStartFlag=START_CHECK;
			}
			break;
		case CHECK_VALID:     //低速率前导码检测成功
			
			//SetLowRate();
			//DBUG("CHECK_VALID ");
			PreambleCheck.channel=bRadio_GetCurrentChannel();
			PreambleCheck.CheckStartFlag=STOP_CHECK;
			break;
		case STOP_CHECK:   //停止检测，即停止中断
			break;
		default:
			break;
	}
}
#endif
#define DSSS_LOW_CHECK_TH_H 20
#define DSSS_LOW_CHECK_TH_L 76

#define DSSS_MIDE_CHECK_TH_H 18
#define DSSS_MIDE_CHECK_TH_L 78

const unsigned char IntegralTable[256]={
	0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,
	1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
	1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
	2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
	1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
	2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
	2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
	3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
	1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
	2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
	2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
	3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
	2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
	3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
	3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
	4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8 
};
const unsigned char DsssLowLowData[12]={0xff,0xff,0xff,0xff, 0x00,0x00,0xff,0xff, 0x00,0x00,0x00,0x00};
//const unsigned char DsssLowData[12]={0xff,0xff,0xf0,0x00,0x00,0xff,0xff,0xf0,0x00,0x00,0xff,0xff};
const unsigned char DsssLowData[12]={0x00,0xf0,0xff,0xff,0xf0,0xff,0xff,0x00,0xff,0xff,0x00,0x00};

const unsigned char DsssMideData[12]={0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00};

unsigned int InData[3];


void CheckLowRatePreamble(void)
{
	unsigned char i,*pdata;
	unsigned int num=0;
	InData[0]<<=1;
	InData[0]+=((InData[1]&0x80000000)==0 ? 0 : 1);
	InData[1]<<=1;
	InData[1]+=((InData[2]&0x80000000)==0 ? 0 : 1);
	InData[2]<<=1;
	if(RX_DATA_PIN())
	{
		InData[2]+=1;
	}
	pdata=(unsigned char *)InData;
	/*
	num=0;
	for(i=0;i<12;i++)
	{
		
		num+=IntegralTable[pdata[i]^DsssLowLowData[i]];
	}
	if((num<=DSSS_LOW_CHECK_TH_H)||(num>=DSSS_LOW_CHECK_TH_L))
	{
//		PreambleCheck.channel=bRadio_GetCurrentChannel();
//		PreambleCheck.CheckStartFlag=STOP_CHECK;
		//DBUG("DsssMideData %08x %08x %08x\r\n",InData[0],InData[1],InData[2]);
		SetLowLowRate();

		InData[0]=0;
		InData[1]=0;
		InData[2]=0;

	}
	*/
	num=0;
	for(i=0;i<12;i++)
	{
		
		num+=IntegralTable[pdata[i]^DsssLowData[i]];
	}
	if((num<=DSSS_LOW_CHECK_TH_H)||(num>=DSSS_LOW_CHECK_TH_L))
	{
//		PreambleCheck.channel=bRadio_GetCurrentChannel();
		PreambleCheck.CheckStartFlag=CHECK_VALID;
		if(InData[0]>=0x10)
		{
				//DBUG("DsssLowData %08x %08x %08x\r\n",InData[0],InData[1],InData[2]);
				SetLowRate();
			InData[0]=0;
			InData[1]=0;
			InData[2]=0;
		}
	}
	num=0;
	for(i=0;i<12;i++)
	{
		
		num+=IntegralTable[pdata[i]^DsssMideData[i]];
	}
	if((num<=DSSS_MIDE_CHECK_TH_H)||(num>=DSSS_MIDE_CHECK_TH_L))
	{
//		PreambleCheck.channel=bRadio_GetCurrentChannel();
//		PreambleCheck.CheckStartFlag=STOP_CHECK;
		//DBUG("DsssMideData %08x %08x %08x\r\n",InData[0],InData[1],InData[2]);
		SetMediumRate();
		PreambleCheck.CheckStartFlag=CHECK_VALID;

		InData[0]=0;
		InData[1]=0;
		InData[2]=0;

	}
	
}


