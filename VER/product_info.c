/**
  ******************************************************************************
  * @file    product_info.c 
  * @author  �ű�ƽ
  * @version V1.1
  * @date    2014-08-6
  * @brief   ���ļ���Ҫ����������Ʒ�������Ϣ���ṩ�ӿں����������ļ����ã���
             ָ���˿������Ʒ��Ϣ
  ******************************************************************************
  */
  
  /* Includes ------------------------------------------------------------------*/
#include "bsp.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VER_BUF_LEN                       100
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char compile_date[]     = {__DATE__};                //��������
const char compile_time[]     = {__TIME__};                //����ʱ��
const char pd_mcu[]           = {"STM32F101ZET6"};         //��Ʒ��ʹ�õ���Ҫ�������ͺ�
const char pd_mcu_osc[]       = {"8.0MHz"};                //��Ʒ��������ʱ��Ƶ��
const char pd_sys_clock[]     = {"72.0MHz"};               //��Ʒϵͳʱ��
const char pd_sft_ver[]       = {"1.00.00.01469"};         //��Ʒ������汾���������
const char pd_developer[]     = {"SongBangYan"};           //��������Ϣ
const char pd_os[]            = {"RTX"};                   //�������ϵͳ
const char pd_pj_num[]        = {"BG201405"};              //��Ʒ������Ŀ���
const char pd_code[]          = {"TX16"};                  //��Ʒ����
const char pd_num[]           = {"TXJX13-BGJA"};           //��Ʒ�ͺ�
const char pd_sft_toolchain[] = {"MDK-ARM Standard Version:5.14.0.0"};//�������������Ϣ
const int pd_sft_Compiler     = __ARMCC_VERSION;           //����ı������汾
const char pd_model[]         = {"-"};                     //��Ʒ��ģ������
const char pd_model_slnum[]   = {"A"};                     //��Ʒ��ģ�����������
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
  



/**
* @brief  ��ָ����ͨ�Ŷ˿ڷ��Ͳ�Ʒ��Ϣ
*         
* @note   ��
* @param  mode��ͨ�Ŷ˿ڣ�1-����1��
* @retval ��
*/
void product_info(unsigned char com)
{
  if(com == 0x00)
  {
//    uart1_send((unsigned char *)(&pd_pj_num),sizeof(pd_pj_num));
//    uart1_send((unsigned char *)(&pd_model),sizeof(pd_model));
//    uart1_send((unsigned char *)(&pd_code),sizeof(pd_code));
//    uart1_send((unsigned char *)(&pd_model),sizeof(pd_model));
//    uart1_send((unsigned char *)(&pd_model_slnum),sizeof(pd_model_slnum));       
//    uart1_send((unsigned char *)(&pd_model),sizeof(pd_model));
//    uart1_send((unsigned char *)(&pd_sft_ver),sizeof(pd_sft_ver));
  }else
  {
    printf("\r\ncompile date:          %s",(char *)compile_date);
    printf("\r\ncompile time:          %s",(char *)compile_time);
    printf("\r\nproduct mcu:           %s",(char *)pd_mcu);
    printf("\r\nproduct mcu clock:     %s",(char *)pd_mcu_osc);
    printf("\r\nproduct sys clock:     %s",(char *)pd_sys_clock);
    printf("\r\nproduct ver:           %s",(char *)pd_sft_ver);
    printf("\r\nproduct developer:     %s",(char *)pd_developer);
    printf("\r\nproduct os:            %s",(char *)pd_os);
    printf("\r\nproject num:           %s",(char *)pd_pj_num);
    printf("\r\nproduct code:          %s",(char *)pd_code);
    printf("\r\nproduct tool chain:    %s",(char *)pd_sft_toolchain);
    printf("\r\nproduct armcc compiler:%d",pd_sft_Compiler);
    printf("\r\nproduct model:         %s",(char *)pd_model);
    printf("\r\nproduct solution num:  %s",(char *)pd_model_slnum);   
  }
}

void GetBuildTime(Time_Def *rtc)
{
  char Date[] = __DATE__;
  char Time[] = __TIME__;
  char mon_s[20]={0};
  char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  int mon=0, day=0, year=0;
  int sec=0, min=0, hour=0;
  sscanf(Date, "%s %d %d", mon_s, &day, &year);
  sscanf(Time, "%d:%d:%d", &hour, &min, &sec);  
  mon = (strstr(month_names, mon_s)-month_names)/3+1;
  rtc->day = (unsigned char)day;
  rtc->mon = ((unsigned char)mon);
  rtc->year = ((unsigned char)(year%100));  
  rtc->hour = (unsigned char)hour;
  rtc->min = (unsigned char)min;
  rtc->sec = (unsigned char)sec;
}

void GetBuildVer(u8 *Version)
{
  int ver1,ver2,ver3;
  sscanf(pd_sft_ver, "%d.%d.%d",&ver1, &ver2, &ver3);
  Version[0] = (u8)(ver3);
  Version[1] = (u8)((ver1<<4)|ver2);  
}



