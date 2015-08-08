
#include "bsp.h"


//Time_Def rtc_real;




volatile  u32  g_sec;

  
//u8 RTC_GetWeek(u16 year,u8 month,u8 day);
//�ж��Ƿ������꺯��
//�·� 1 2 3 4 5 6 7 8 9 10 11 12
//���� 31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1����.0������
u8 Is_LeapYear(u16 year)
{
  if(((year%4==0)&&(year%100!=0))||(year%400==0))
    return 1;
  else
    return 0;
}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970 ��1 ��1 ��Ϊ��׼
//1970~2099 ��Ϊ�Ϸ����
//����ֵ:1���ɹ�;����:�������.
//�·����ݱ�
const u8 week_table[12]={0,3,3,6,1,4,6,2,5,0,3,5};
//ƽ����·����ڱ�
const u8 month_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(Time_Def *rtc)
{
  u16 i;
  u8 tpmon;
  u32 seccount = 0;
  if(rtc->year<1970||rtc->year>2099)//����
  return 0;
  for(i=1970;i<rtc->year;i++)//������ݵ�����
  {
    if(Is_LeapYear(i))
      seccount+=31622400;//�����������
    else
      seccount+=31536000; //ƽ��������� 
  }
  tpmon = rtc->mon - 1;
  for(i=0;i<tpmon;i++)
  {
    seccount+=(u32)month_table[i]*86400;//�·����������
    if(Is_LeapYear(rtc->year)&&(i==1))
      seccount+=86400;//����2 �·�����һ���������
  }
  seccount += (u32)(rtc->day-1)*86400;//��ǰ�����ڵ����������
  seccount += (u32)rtc->hour*3600;//Сʱ������
  seccount += (u32)rtc->min*60; //����������
  seccount += rtc->sec;//�������Ӽ���ȥ
  //����ʱ��
  g_sec = seccount;
  return 1;
}
//�õ���ǰ��ʱ��
//����ֵ:1���ɹ�;����:�������.
u8 RTC_Get(Time_Def *rtc_real)
{
  u32 temp=0;
  u16 temp1=0;
  u32 seccount=0;

  seccount = g_sec;//��ȡ���������ֵ
  temp=seccount/86400; //�õ�����(��������Ӧ��)
  if(temp>0)//����һ��
  {
    temp1=1970;
    while(temp>=365)
    {
      if(Is_LeapYear(temp1))//����
      {
        if(temp>=366)
         temp-=366;
        else //��ǰ��ݼ�Ϊ������
         break;
     }
     else
        temp-=365;
    temp1++;
    }
  }
  rtc_real->year=temp1;//�õ����
  temp1=0;
  while(temp>=28)//������һ����
  {
    if(Is_LeapYear(rtc_real->year)&&(temp1==1))//�����ǲ�������/2 �·�
    {
      if(temp>=29)
        temp-=29;//�����2������
      else
        break;
    }else
    {
      if(temp>=month_table[temp1])
        temp-=month_table[temp1];//ƽ��
      else
        break;
    }
    temp1++;
  }
  rtc_real->mon=temp1+1;//�õ��·�
  rtc_real->day=temp+1; //�õ�����
  temp=seccount%86400; //�õ�������
  rtc_real->hour=temp/3600; //Сʱ
  rtc_real->min=(temp%3600)/60; //����
  rtc_real->sec=(temp%60); //����
  rtc_real->week=RTC_GetWeek(rtc_real->year,rtc_real->mon,rtc_real->day); 
  return 1;
}
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//�������������������
//����ֵ�����ں�                      
u8 RTC_GetWeek(u16 year,u8 month,u8 day)
{ 
  u16 temp;
  u8 yearH,yearL; 
  yearH=year/100; yearL=year%100;
  // ���Ϊ21����,�������100 
  if (yearH>19)
  yearL+=100;
  // ����������ֻ��1900��֮��� 
  temp=yearL+yearL/4;
  temp=temp%7;
  temp=temp+day+week_table[month-1];
  if (yearL%4==0&&month<3)
    temp--;
  return(temp%7);
}
//���㴫���ʱ�������ʱ��Ա��Ѿ����˶���ʱ��
//����Ϊ����
u32 Time_Pasted(Time_Def *Oldtime)
{
  u16 i;
  u8 tpmon;
  u32 seccount = 0;
  if(Oldtime->year<1970||Oldtime->year>2099)//����
  return 0;
  for(i=1970;i<Oldtime->year;i++)//������ݵ�����
  {
    if(Is_LeapYear(i))
      seccount+=31622400;//�����������
    else
      seccount+=31536000; //ƽ��������� 
  }
  tpmon = Oldtime->mon - 1;
  for(i=0;i<tpmon;i++)
  {
    seccount+=(u32)month_table[i]*86400;//�·����������
    if(Is_LeapYear(Oldtime->year)&&(i==1))
      seccount+=86400;//����2 �·�����һ���������
  }
  seccount += (u32)(Oldtime->day-1)*86400;//��ǰ�����ڵ����������
  seccount += (u32)Oldtime->hour*3600;//Сʱ������
  seccount += (u32)Oldtime->min*60; //����������
  seccount += Oldtime->sec;//�������Ӽ���ȥ
  return g_sec - seccount;
}
u32 Time_comp(Time_Def *Oldtime, Time_Def *Newtime)
{
  u16 i;
  u8 tpmon;
  u32 seccount = 0;
  u32 seccount1 = 0;
  
  if(Oldtime->year<1970||Oldtime->year>2099)//����
  return 0;
  for(i=1970;i<Oldtime->year;i++)//������ݵ�����
  {
    if(Is_LeapYear(i))
      seccount+=31622400;//�����������
    else
      seccount+=31536000; //ƽ��������� 
  }
  tpmon = Oldtime->mon - 1;
  for(i=0;i<tpmon;i++)
  {
    seccount+=(u32)month_table[i]*86400;//�·����������
    if(Is_LeapYear(Oldtime->year)&&(i==1))
      seccount+=86400;//����2 �·�����һ���������
  }
  seccount += (u32)(Oldtime->day-1)*86400;//��ǰ�����ڵ����������
  seccount += (u32)Oldtime->hour*3600;//Сʱ������
  seccount += (u32)Oldtime->min*60; //����������
  seccount += Oldtime->sec;//�������Ӽ���ȥ

  if(Newtime->year<1970||Newtime->year>2099)//����
  return 0;
  for(i=1970;i<Newtime->year;i++)//������ݵ�����
  {
    if(Is_LeapYear(i))
      seccount1+=31622400;//�����������
    else
      seccount1+=31536000; //ƽ��������� 
  }
  tpmon = Newtime->mon - 1;
  for(i=0;i<tpmon;i++)
  {
    seccount1+=(u32)month_table[i]*86400;//�·����������
    if(Is_LeapYear(Newtime->year)&&(i==1))
      seccount1+=86400;//����2 �·�����һ���������
  }
  seccount1 += (u32)(Newtime->day-1)*86400;//��ǰ�����ڵ����������
  seccount1 += (u32)Newtime->hour*3600;//Сʱ������
  seccount1 += (u32)Newtime->min*60; //����������
  seccount1 += Newtime->sec;//�������Ӽ���ȥ
  
  return seccount1 - seccount;
}/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_RTC_init(void)
{
  Time_Def rtc_real;
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);  
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);  
  /* Reset Backup Domain */
  BKP_DeInit(); 
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);  
  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);  
  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro(); 
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();  
  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE); 
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();  
  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(62499);  
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();  
  /* To output second signal on Tamper pin, the tamper functionality
     must be disabled (by default this functionality is disabled) */
  BKP_TamperPinCmd(DISABLE);  
  
  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  GetBuildTime(&rtc_real);
  rtc_real.year += 2000;
  RTC_Set(&rtc_real);
}
/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);    
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    g_sec++;
  }
}

