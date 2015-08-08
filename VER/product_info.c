/**
  ******************************************************************************
  * @file    product_info.c 
  * @author  张保平
  * @version V1.1
  * @date    2014-08-6
  * @brief   本文件主要用于描述产品的相关信息，提供接口函数供其他文件调用，从
             指定端口输出产品信息
  ******************************************************************************
  */
  
  /* Includes ------------------------------------------------------------------*/
#include "bsp.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VER_BUF_LEN                       100
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char compile_date[]     = {__DATE__};                //编译日期
const char compile_time[]     = {__TIME__};                //编译时间
const char pd_mcu[]           = {"STM32F101ZET6"};         //产品所使用的主要控制器型号
const char pd_mcu_osc[]       = {"8.0MHz"};                //产品主控制器时钟频率
const char pd_sys_clock[]     = {"72.0MHz"};               //产品系统时钟
const char pd_sft_ver[]       = {"1.00.00.01469"};         //产品的软件版本及编译次数
const char pd_developer[]     = {"SongBangYan"};           //开发者信息
const char pd_os[]            = {"RTX"};                   //软件操作系统
const char pd_pj_num[]        = {"BG201405"};              //产品所属项目编号
const char pd_code[]          = {"TX16"};                  //产品编码
const char pd_num[]           = {"TXJX13-BGJA"};           //产品型号
const char pd_sft_toolchain[] = {"MDK-ARM Standard Version:5.14.0.0"};//软件开发工具信息
const int pd_sft_Compiler     = __ARMCC_VERSION;           //软件的编译链版本
const char pd_model[]         = {"-"};                     //产品子模块名称
const char pd_model_slnum[]   = {"A"};                     //产品子模块软件方案号
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
  



/**
* @brief  从指定的通信端口发送产品信息
*         
* @note   无
* @param  mode：通信端口；1-串口1，
* @retval 无
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



