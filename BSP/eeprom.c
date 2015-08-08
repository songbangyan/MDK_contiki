#include "eeprom.h"

#define EEPROM         1
#define FRAM           2
#define SIZE_64K       (64*1024)

#define DEV_TYPE       FRAM        //铁电存储器
#define PAGE_SIZE      64          //页大小为64字节
#define MEM_SIZE      (64*1024)    //当前存储器大小为64k字节

#if(MEM_SIZE > SIZE_64K)
u8 eeprom_num = 1;
#define DEV_ADDR1      0xA0
#define DEV_ADDR2      0xA2
#define SEL_CHIP1      eeprom_num = 1
#define SEL_CHIP2      eeprom_num = 2
#else
#define DEV_ADDR      0xA0
#define SEL_CHIP1     
#define SEL_CHIP2     
#endif

#if(DEV_TYPE == FRAM)
#define DELAY_NUM       2
#else
#define DELAY_NUM       50
#endif



enum ENUM_TWI_REPLY
{
TWI_NACK=0 
,TWI_ACK=1
};
enum ENUM_TWI_BUS_STATE
{
TWI_READY=0
,TWI_BUS_BUSY=1
,TWI_BUS_ERROR=2
};


#define TWI_SCL_0 GPIOB->BRR=GPIO_Pin_10
#define TWI_SCL_1 GPIOB->BSRR=GPIO_Pin_10
#define TWI_SDA_0 GPIOB->BRR=GPIO_Pin_11
#define TWI_SDA_1 GPIOB->BSRR=GPIO_Pin_11
#define TWI_SDA_STATE (GPIOB->IDR&GPIO_Pin_11)



#define TWI_NOP TWI_Delay()

u32 EEPROM_GetTotalsize(void) 
{
  return MEM_SIZE-1;
}
void TWI_Delay(void)
{
 u32 i=DELAY_NUM;
 while(i--); 
}


void TWI_Initialize(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  TWI_SDA_1;
  TWI_SCL_1;
  
  //////DebugPrint("Software TWI Initializing...\n"); 
}

u8 TWI_START(void)
{ 
 TWI_SDA_1; 
 TWI_NOP;
   
 TWI_SCL_1; 
 TWI_NOP;    
 if(!TWI_SDA_STATE)
 {
  ////DebugPrint("TWI_START:BUSY\n");
  return TWI_BUS_BUSY;
 }
 TWI_SDA_0;
 TWI_NOP;
  
 TWI_SCL_0;  
 TWI_NOP; 
 if(TWI_SDA_STATE)
 {
  ////DebugPrint("TWI_START:BUS ERROR\n");
  return TWI_BUS_ERROR;
 } 
 
 return TWI_READY;
}


void TWI_STOP(void)
{
 TWI_SDA_0; 
 TWI_NOP;
   
 TWI_SCL_1; 
 TWI_NOP;    
 TWI_SDA_1;
 TWI_NOP;
  
// TWI_SCL_0;  
// TWI_NOP;  
 //////DebugPrint("TWI_STOP\n");   
}

void TWI_SendACK(void)
{
 TWI_SDA_0;
 TWI_NOP;
 TWI_SCL_1;
 TWI_NOP;
 TWI_SCL_0; 
 TWI_NOP; 
 //////DebugPrint("TWI_SendACK\n");   
}


void TWI_SendNACK(void)
{
 TWI_SDA_1;
 TWI_NOP;
 TWI_SCL_1;
 TWI_NOP;
 TWI_SCL_0; 
 TWI_NOP;
 //////DebugPrint("TWI_SendNACK\n");    
}


u8 TWI_SendByte(u8 Data)
{
 u8 i;
 TWI_SCL_0;
 for(i=0;i<8;i++)
 {  
  //---------数据建立----------
  if(Data&0x80)
  {
   TWI_SDA_1;
  }
  else
  {
   TWI_SDA_0;
  } 
  Data<<=1;
  TWI_NOP;
  //---数据建立保持一定延时----
  
  //----产生一个上升沿[正脉冲] 
  TWI_SCL_1;
  TWI_NOP;
  TWI_SCL_0;
  TWI_NOP;//延时,防止SCL还没变成低时改变SDA,从而产生START/STOP信号
  //---------------------------   
 }
 //接收从机的应答 
 TWI_SDA_1; 
 TWI_NOP;
 TWI_SCL_1;
 TWI_NOP;   
 if(TWI_SDA_STATE)
 {
  TWI_SCL_0;
  //////DebugPrint("TWI_NACK!\n");
  return TWI_NACK;
 }
 else
 {
  TWI_SCL_0;
  //////DebugPrint("TWI_ACK!\n");
  return TWI_ACK;  
 }    
}


u8 TWI_ReceiveByte(void)
{
 u8 i,Dat;
 TWI_SDA_1;
 TWI_SCL_0; 
 Dat=0;
 for(i=0;i<8;i++)
 {
  TWI_SCL_1;//产生时钟上升沿[正脉冲],让从机准备好数据 
  TWI_NOP; 
  Dat<<=1;
  if(TWI_SDA_STATE) //读引脚状态
  {
   Dat|=0x01; 
  }   
  TWI_SCL_0;//准备好再次接收数据  
  TWI_NOP;//等待数据准备好         
 }
 //////DebugPrint("TWI_Dat:%x\n",Dat);
 return Dat;
}



//void delay_5ms(void)
//{
//  my_u32 t = 0x3ffff;
//  while(t--);
//}


/*
description:查询eeprom写周期是否结束
return:0-写周期结束
       1-写周期没有结束
*/
uint8_t is_eeprom_busy(void)
{
  uint8_t result = 0;
  uint8_t temp = 0;

  result = TWI_START();
  if(result != TWI_READY)
    return 1;
#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|0;
  else
    temp = DEV_ADDR2|0;
#else
  temp = DEV_ADDR|0;
#endif
  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;

  return 0;
}



/*
description:向eeprom写一个字节数据
return：0--写数据正常结束
        1--I2C启动错误
        2--无ACK
*/
uint8_t eeprom_write_byte(uint16_t addr,uint8_t data)
{
  uint8_t result = 0;
  uint8_t temp = 0;

  while(is_eeprom_busy());

  result = TWI_START();
  if(result != TWI_READY)
    return 1;

#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|0;
  else
    temp = DEV_ADDR2|0;
#else
  temp = DEV_ADDR|0;
#endif

  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte((uint8_t)(addr>>8));
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte((uint8_t)(addr));
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte(data);
  if(result != TWI_ACK)
    return 2;
           
  TWI_STOP();
//  delay_5ms();
  return 0;
}



/*
description:向eeprom一页内写入部分或者全部数据
return：0--写数据正常结束
        1--I2C启动错误
        2--无ACK
*/
uint8_t eeprom_write_page(uint16_t page_num,uint8_t offshit,uint8_t num,uint8_t *data)
{
  uint8_t result = 0;
  uint8_t temp = 0;
  uint8_t i = 0;
  uint16_t addr = page_num*PAGE_SIZE+offshit;

  while(is_eeprom_busy());

  result = TWI_START();
  if(result != TWI_READY)
    return 1;

#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|0;
  else
    temp = DEV_ADDR2|0;
#else
  temp = DEV_ADDR|0;
#endif
  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte((uint8_t)(addr>>8));
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte((uint8_t)(addr));
  if(result != TWI_ACK)
    return 2;

  for(i = 0; i < num; i++)
  {
    result = TWI_SendByte(data[i]);
    if(result != TWI_ACK)
      return 2;
  }
          
  TWI_STOP();
//  delay_5ms();
  return 0;
}


/*
description:向eeprom读一个字节数据
return：0--写数据正常结束
        1--I2C启动错误
        2--无ACK
*/
uint8_t eeprom_read_byte(uint16_t addr,uint8_t *data)
{
  uint8_t result = 0;
  uint8_t temp = 0;

  while(is_eeprom_busy());

  result = TWI_START();
  if(result != TWI_READY)
    return 1;

#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|0;
  else
    temp = DEV_ADDR2|0;
#else
  temp = DEV_ADDR|0;
#endif
  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte((uint8_t)(addr>>8));
  result = TWI_SendByte((uint8_t)(addr));
  if(result != TWI_ACK)
    return 2;

  result = TWI_START();
#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|1;
  else
    temp = DEV_ADDR2|1;
#else
  temp = DEV_ADDR|1;
#endif
  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;


  *data = TWI_ReceiveByte();

  TWI_SendNACK();            
  TWI_STOP();
  return 0;
}


/*
description:向eeprom读一个页内的部分或者全部数据
return：0--写数据正常结束
        1--I2C启动错误
        2--无ACK
*/
uint8_t eeprom_read_sequential(uint16_t addr,uint16_t  num,uint8_t *data)
{
  uint8_t result = 0;
  uint8_t temp = 0;
  uint16_t i = 0;

  while(is_eeprom_busy());
    
  result = TWI_START();
  if(result != TWI_READY)
    return 1;

#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|0;
  else
    temp = DEV_ADDR2|0;
#else
  temp = DEV_ADDR|0;
#endif
  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;

  result = TWI_SendByte((uint8_t)(addr>>8));
  result = TWI_SendByte((uint8_t)(addr));
  if(result != TWI_ACK)
    return 2;

  result = TWI_START();
#if(MEM_SIZE > SIZE_64K)
  if(eeprom_num == 1)
    temp = DEV_ADDR1|1;
  else
    temp = DEV_ADDR2|1;
#else
  temp = DEV_ADDR|1;
#endif
  result = TWI_SendByte(temp);
  if(result != TWI_ACK)
    return 2;

  for(i = 0; i < num; i++)
  {
    data[i] = TWI_ReceiveByte();
    if(i != num-1)
      TWI_SendACK();
    else
      TWI_SendNACK();
  }
            
  TWI_STOP();
//  delay_5ms();
  return 0;
}



void I2C_EE_Init(void)
{
#ifdef SPIMARM
  BSP_SPI_Init();
#else
  TWI_Initialize();   
#endif  
}




void eeprom_write_bank(uint16_t addr,uint16_t num,uint8_t *p_data)
{
  uint16_t i = 0;
  uint16_t cur_blk = addr/PAGE_SIZE;//写入地址所在页
  uint8_t  blk_shift = addr%PAGE_SIZE;//写入地址在页内的偏移量
  uint8_t first_blk_num = PAGE_SIZE - (addr%PAGE_SIZE);//写入地址所在页的剩余容量
  uint16_t last_blk = (addr+num)/PAGE_SIZE;//写入数据的最后页号
  uint8_t last_blk_num = (addr+num)%PAGE_SIZE;//写入数据的最后页内需要的写的字节数
  uint16_t full_page_num = last_blk - cur_blk; 

  if(num <= first_blk_num)
  {
    while(eeprom_write_page(cur_blk,blk_shift,num,p_data));
    p_data += num; 
    cur_blk++;    
  }else
  {
    while(eeprom_write_page(cur_blk,blk_shift,first_blk_num,p_data));
    p_data += first_blk_num;
    cur_blk++; 
  }


  if(full_page_num > 1)
  {
    for(i = 0; i < full_page_num - 1; i++)
    {
      while(eeprom_write_page(cur_blk,0,PAGE_SIZE,p_data));
      p_data += PAGE_SIZE; 
      cur_blk++;
    }
  }


  if(last_blk == cur_blk)
  {
    while(eeprom_write_page(cur_blk,0,last_blk_num,p_data));
  }
}

volatile u8 wait_eeprom_idle = 0;//1表示存在等待eeprom执行完毕需要立即执行的任务
volatile u8 eeprom_status = 0;//0-空闲，1-忙
void eeprom_write(uint32_t addr,uint16_t num,uint8_t *p_data)
{
#ifdef SPIMARM
  eeprom_status = 1;
  SPIMRAM_WriteBuffer(p_data, addr, num);
#else
  u16 temp_addr;
  u16 temp_num;
  eeprom_status = 1;
  if(addr&0x10000)
  {
    SEL_CHIP2;
    temp_addr = (addr&0xffff);
    temp_num = num;
    eeprom_write_bank(temp_addr,temp_num,p_data);
  }else if((addr + num)&0x10000)
  {
    SEL_CHIP1;
    temp_addr = (addr&0xffff);
    temp_num = 0x10000 - temp_addr;
    eeprom_write_bank(temp_addr,temp_num,p_data);
    SEL_CHIP2;
    eeprom_write_bank(0,num-temp_num,p_data+temp_num);
  }else
  {
    SEL_CHIP1;
    temp_addr = (addr&0xffff);
    temp_num = num;
    eeprom_write_bank(temp_addr,temp_num,p_data);
  }
#endif  
  eeprom_status = 0;
  if(wait_eeprom_idle == 1)
  {
    wait_eeprom_idle = 0;
  }
}

void eeprom_read_bytes(uint16_t addr,uint16_t num,uint8_t * p)
{
  uint16_t i = 0;
//  if(addr+num<MEM_SIZE-0x500)
//  {
//    eeprom_read_sequential(addr,num,p);   
//  }else
  {
    for(i = 0; i < num; i++)
      eeprom_read_byte(addr+i,p+i);       
  }
}

void eeprom_read(uint32_t addr,uint16_t num,uint8_t *p_data)
{
#ifdef SPIMARM
  eeprom_status = 1;
  SPIMRAM_ReadBuffer(p_data, addr, num);
#else
  u16 temp_addr;
  u16 temp_num;
  eeprom_status = 1;
  if(addr&0x10000)
  {
    SEL_CHIP2;
    temp_addr = (addr&0xffff);
    temp_num = num;
    eeprom_read_bytes(temp_addr,temp_num,p_data);
  }else if((addr + num)&0x10000)
  {
    SEL_CHIP1;
    temp_addr = (addr&0xffff);
    temp_num = 0x10000 - temp_addr;
    eeprom_read_bytes(temp_addr,temp_num,p_data);
    SEL_CHIP2;
    eeprom_read_bytes(0,num-temp_num,p_data+temp_num);
  }else
  {
    SEL_CHIP1;
    temp_addr = (addr&0xffff);
    temp_num = num;
    eeprom_read_bytes(temp_addr,temp_num,p_data);
  } 
#endif  
  eeprom_status = 0;
  if(wait_eeprom_idle == 1)
  {
    wait_eeprom_idle = 0;
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/




