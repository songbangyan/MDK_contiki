#ifndef _USART_H__
#define _USART_H__

#include "stm32f10x.h"  
#include "bsp.h"
//#include <data_type.h>
#include <stdio.h>
#include <string.h>
/***********���ڷ���************/
#define  COM1 1                 //uart
#define  COM2 2                 //debug 
#define  COM3 3                 //none

#define COM_TTL  COM1 
// ������
#define CBR_110             110
#define CBR_300             300
#define CBR_600             600
#define CBR_1200            1200
#define CBR_2400            2400
#define CBR_4800            4800
#define CBR_7200            7200
#define CBR_9600            9600
#define CBR_14400           14400
#define CBR_19200           19200
#define CBR_38400           38400
#define CBR_56000           56000
#define CBR_57600           57600
#define CBR_115200          115200
#define CBR_128000          128000
#define CBR_256000          256000
// ����λ

#define DATABITS_8        USART_WordLength_8b
#define DATABITS_9        USART_WordLength_9b

// У��λ
#define NOPARITY            USART_Parity_No
#define ODDPARITY           USART_Parity_Odd
#define EVENPARITY          USART_Parity_Even

// ֹͣλ
#define ONESTOPBIT    USART_StopBits_1
#define O5STOPBITS    USART_StopBits_0_5
#define ONE5STOPBITS  USART_StopBits_1_5
#define TWOSTOPBITS   USART_StopBits_2
// ����
#define HARDCON_NO      USART_HardwareFlowControl_None
#define HARDCON_RTS     USART_HardwareFlowControl_RTS
#define HARDCON_CTS     USART_HardwareFlowControl_CTS
#define HARDCON_RC      USART_HardwareFlowControl_RTS_CTS




#define BufferSize(a) (countof(a)-1)
#define countof(a) (sizeof(a) / sizeof(*(a)))

typedef struct _SCOMMINFO
  {
  u8  nPort ;       //���ں�
  u32 nBaudRate ;     //������
  u16 ByteSize;        //����λ
  u16 nParity;          //У��λ
  u16 StopBits;        //ֹͣλ
  u16 HardCon;
  }SCOMMINFO, *PSCOMMINFO;    //���ڲ���
  
  
void BSP_USART_Init(void);
void USART_Configuration(void); 
void InitCom(PSCOMMINFO psComminfo);
void USART_NVIC_Config(u8 COM);
void SetUartState1(u8 com, u32 BaudRate,u16 Parity,u16 StopBits);

void error(unsigned char error_num);
void uart_send_init(void);
void uart_receive_init(void);
unsigned char uart_phy_receive(unsigned char *pdata); 
unsigned char uart_phy_send(unsigned char *pdata,unsigned char length);
void uart1_send(unsigned char *pdata,unsigned char length);

void fill_uart_receive_buf(unsigned char *buf,unsigned char len);
  

#endif