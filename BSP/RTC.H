#ifndef _RTC_H__
#define _RTC_H__

#include "stm32f10x.h" 
#include <data_type.h>

//extern Time_Def rtc_real;
extern volatile  u32  g_sec;

void BSP_RTC_init(void);
u8 RTC_GetWeek(u16 year,u8 month,u8 day);
u8 Is_LeapYear(u16 year);
u8 RTC_Set(Time_Def *rtc);
u8 RTC_Get(Time_Def *rtc_real);
u8 RTC_GetWeek(u16 year,u8 month,u8 day);
u32 Time_Pasted(Time_Def *Oldtime);
u32 Time_comp(Time_Def *Oldtime, Time_Def *Newtime);

#endif
