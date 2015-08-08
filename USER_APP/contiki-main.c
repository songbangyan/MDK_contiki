#include "stm32f10x.h"
#include "bsp.h" 
#include <stdint.h>  
#include <stdio.h>  
//#include <debug-uart.h>  //??????????????
#include <clock.h>  
#include <sys/process.h>  
#include <sys/procinit.h>  
#include <etimer.h>  
#include <sys/autostart.h>  
#include "contiki.h"


unsigned int idle_count = 0;
//�¼������� 
static process_event_t event_ledoff;
static process_event_t event_ledon;


//�������� 
void Delay_us(__IO u32 nCount);


////�������̵�����
//PROCESS(led_on, "led_on");
//PROCESS(led_off, "led_off");   


////���������̼���AUTOSTART_PROCESSES
//AUTOSTART_PROCESSES(&led_on, &led_off);  

////����1�ĺ������� 
//PROCESS_THREAD(led_on, ev, data)  
//{   
//  //���̿�ʼ
//  PROCESS_BEGIN();
//  //���һ���¼������ǳ�ʼ��
//  event_ledoff = process_alloc_event(); 
//  while(1)  
//  {  
//    //�ȴ��¼��������룬��û���¼�����ϵͳ���ȵ������ط�
//    PROCESS_WAIT_EVENT_UNTIL(ev == event_ledon);
//    //��LED ע����ʱ  
//    LED_On(LED_POW); 
//    //Delay(0xFFFEF);
//    Delay_us(2);
//    //����һ���¼��������ǻ����˵ڶ�������
//    process_post(&led_off, event_ledoff, NULL);  
//  } 
//  //���̽���
//  PROCESS_END();  
//}  


//PROCESS_THREAD(led_off, ev, data)  
//{  
//  PROCESS_BEGIN();
//  event_ledon = process_alloc_event();
//  while(1)  
//  {  
//    PROCESS_WAIT_EVENT_UNTIL(ev == event_ledoff);
//    //��LED ע����ʱ  
//    LED_Off(LED_POW);
//    //Delay(0xFFFEF);
//    Delay_us(2);
//    process_post(&led_on, event_ledon, NULL); 
//  } 
//  PROCESS_END();  
//} 



//int main(void)
//{
//  bsp_init();
//  //��ָʾ�� 
////  LED2(ON);
//  clock_init();  
//  process_init();  
//  //etimer_process ��ϵͳ���� 
//  process_start(&etimer_process,NULL);  
//  //����������Ķ�����ִ��,�����Ϳ��� 
//  autostart_start(autostart_processes);
//  event_ledon = process_alloc_event();
//  process_post(&led_on, event_ledon, NULL); 
//  while(1) {  
//  do  
//  {  
//    }  
//    while(process_run()> 0);  
//    idle_count++;  
//    }  
//  return 0;  
//}  
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);

PROCESS_THREAD(blink_process, ev, data)
{
  
  PROCESS_BEGIN();
  while(1) 
  {
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    //??LED
    LED_On(LED_POW); 
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    //??LED
    LED_Off(LED_POW); 
  }
   PROCESS_END();
}


int main()
{
//  dbg_setup_uart();
  bsp_init();
//  printf("Initialising\r\n");
  
  clock_init();
  process_init();
  process_start(&etimer_process, NULL);
  autostart_start(autostart_processes);
  //process_start(&blink_process,NULL);
//  printf("Processes running\r\n");
  while(1) {
    do 
    {
    } 
    while(process_run() > 0);
    idle_count++;
    /* Idle! */
    /* Stop processor clock */
    /* asm("wfi"::); */ 
  }
  return 0;
}


void Delay_us(__IO u32 nCount)
{
  for(;nCount != 0; nCount--);
}


//int
//main()
//{
//  dbg_setup_uart();
//  printf("Initialising\n");
//  
//  clock_init();
//  process_init();
//  process_start(&etimer_process, NULL);
//  autostart_start(autostart_processes);
//  printf("Processes running\n");
//  while(1) {
//    do {
//    } while(process_run() > 0);
//    idle_count++;
//    /* Idle! */
//    /* Stop processor clock */
//    /* asm("wfi"::); */ 
//  }
//  return 0;
//}




