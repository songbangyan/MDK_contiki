#include "usart.h"


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


void BSP_USART_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
  /* Enable USART1 Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  /* Enable USART2 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
  /*********Configure USART1 as UART****/   
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  /*********Configure USART2 as UART****/   
  /* Configure USART1 Rx (PA.02) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  /* Configure USART1 Tx (PA.03) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  USART_Configuration();
}
void USART_NVIC_Config(u8 COM)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 

  switch(COM)
  {
    case(COM1):
    {
      NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
      NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      break;    
    }
    case(COM2):
    {
      NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
      NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      break;    
    }
    case(COM3):
    {
      NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
      NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      break;    
    }
    default:
      break;
  }

}
void USART_Configuration(void)
{
  SCOMMINFO p;
  p.nPort = COM1;
  p.nBaudRate = CBR_9600;
  p.ByteSize = DATABITS_9;
  p.nParity = EVENPARITY;
  p.StopBits = ONESTOPBIT ;
  p.HardCon = HARDCON_NO;
  InitCom(&p);
  p.nBaudRate = CBR_115200;
  p.nPort = COM2;
  InitCom(&p);
}
void InitCom(PSCOMMINFO psComminfo)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = psComminfo->nBaudRate;
  USART_InitStructure.USART_WordLength = psComminfo->ByteSize;
  USART_InitStructure.USART_StopBits = psComminfo->StopBits;
  USART_InitStructure.USART_Parity = psComminfo->nParity;
  USART_InitStructure.USART_HardwareFlowControl = psComminfo->HardCon;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     
  switch(psComminfo->nPort)
  {
    case(COM1):
    {
      /* Configure USART1 */
      USART_Init(USART1, &USART_InitStructure);
      /* Enable USART1 */
      USART_Cmd(USART1, ENABLE);
      /* Enable USART1 Receive and Transmit interrupts */
      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
      USART_NVIC_Config(COM1);
      break;            
    }
  case(COM2):
    {
      /* Configure USART2 */
      USART_Init(USART2, &USART_InitStructure);
      /* Enable USART2 */
      USART_Cmd(USART2, ENABLE);
      /* Enable USART2 Receive and Transmit interrupts */
      USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
      USART_NVIC_Config(COM2);
      break;            
    }
  case(COM3):
    {
      /* Configure USART3 */
      USART_Init(USART3, &USART_InitStructure);
      /* Enable USART3 */
      USART_Cmd(USART3, ENABLE);
      /* Enable USART3 Receive and Transmit interrupts */
      USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
      USART_NVIC_Config(COM3);
      break;            
    }
  default:
    break;
  }
} 

void SetUartState1(u8 com, u32 BaudRate,u16 Parity,u16 StopBits)
{
  USART_InitTypeDef USART_InitStructure;
  if(Parity == NOPARITY)
  {
    USART_InitStructure.USART_WordLength = DATABITS_8;
  }
  else
  {
    USART_InitStructure.USART_WordLength = DATABITS_9;
  }
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_StopBits = StopBits;
  USART_InitStructure.USART_Parity = Parity;
  USART_InitStructure.USART_HardwareFlowControl = HARDCON_NO;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  switch(com)
  {
    case(COM1):
    {
      /* Dsable USART1 */
      USART_Cmd(USART1, DISABLE); 
      /* Configure USART1 */
      USART_Init(USART1, &USART_InitStructure);
      /* Enable USART1 */
      USART_Cmd(USART1, ENABLE);
      break;            
    }
  case(COM2):
    {
      /* Dsable USART2 */
      USART_Cmd(USART2, DISABLE); 
      /* Configure USART2 */
      USART_Init(USART2, &USART_InitStructure);
      /* Enable USART2 */
      USART_Cmd(USART2, ENABLE);
      break;            
    }
  case(COM3):
    {
      /* Dsable USART3 */
      USART_Cmd(USART3, DISABLE); 
      /* Configure USART3 */
      USART_Init(USART3, &USART_InitStructure);
      /* Enable USART3 */
      USART_Cmd(USART3, ENABLE);
      break;            
    }
  default:
    break;
  } 
}
void uart_receive_isr_disable(void)
{
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
}
void uart_receive_isr_enable(void)
{
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
void uart_send_isr_disable(void)
{
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}
void uart_send_isr_enable(void)
{
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
void error(unsigned char error_num)
{
  printf("\r\nERROR! ERROR NUM IS:%d\r\n",error_num);
}


uart_send_t uart_send;
uart_receive_t uart_receive;
unsigned int uart_receive_timer = 0;
void uart_send_init(void)
{
    uart_send.state = 0;
    uart_send.length = 0;
    uart_send.num = 0;
}

void uart_receive_init(void)
{
  uart_receive_isr_disable();
  uart_receive.state = 0;
  uart_receive.verify = 0;
  uart_receive.num = 0;
  uart_receive_timer = 0;
//  RX_LED_ON_OFF(0);
  uart_receive_isr_enable();
}


__IO uint8_t RxDataLen;
uint8_t RxData[0x100];
__IO uint8_t EndOfCom;
//USART2 串口中断接收
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    RxData[RxDataLen] = USART_ReceiveData(USART2);
    USART_SendData(USART2, RxData[RxDataLen]);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}
    if((RxData[RxDataLen] == 0x08)&&(RxDataLen != 0)) //Backspace
    {
      RxDataLen--;
      USART_SendData(USART2, 0x20);
      while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
      {}      
      USART_SendData(USART2, 0x08);
      while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
      {}
    }else if(RxData[RxDataLen] == 0x0D) //Enter
    { 
      EndOfCom = 1;   
    }else
    {
      RxDataLen++;
      if(RxDataLen > 0xf0)
        RxDataLen = 0;
    }
    
  }
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


