#include "SPI.h"


u8 SPIFLASH_PAGETMP[SPIFLASH_PAGESIZE];
u32 SPIFLASH_LASTWADDR = 0xFFFFFFFF;
u8 SPIFLASH_LASTwrite;
u32 FlashID;

void SPI_Configuration(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  
  SPIFLASH_CS_HIGH();
  SPIMRAM_CS_HIGH();  
  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPIFORFLASH, &SPI_InitStructure);
  /*!< Enable the sFLASH_SPI  */
  SPI_Cmd(SPIFORFLASH, ENABLE);
  SPIFLASH_LASTWADDR = 0xFFFFFFFF;
}


void  BSP_SPI_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  /*!< sFLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  /*!< SPI pins configuration *************************************************/
  /*!< Connect SPI pins to AF */  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  /*!< SPI SCK pin configuration:PA.05 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*!< SPI MOSI pin configuration:PA.07 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< SPI MISO pin configuration:PA.06 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure PC.04 pin: SPI FLASH HOLD pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  GPIO_SetBits(GPIOC, GPIO_Pin_4);
  /*!< Configure PC.05 pin: SPI FLASH WP pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  GPIO_SetBits(GPIOC, GPIO_Pin_5);

  /*!< Configure PA.04 pin: SPI FLASH CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /*!< Configure PA.01 pin: MRAM CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  SPI_Configuration();
  FlashID = SPIFLASH_ReadID();
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t SPIFLASH_SendByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPIFORFLASH, SPI_I2S_FLAG_TXE) == RESET);
  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPIFORFLASH, byte);
  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPIFORFLASH, SPI_I2S_FLAG_RXNE) == RESET);
  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPIFORFLASH);
}
/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t SPIFLASH_ReadByte(void)
{
  return (SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE));
}
/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void SPIFLASH_WriteEnable(void)
{
  /*!< Select the SPIFLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send "Write Enable" instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_WREN);
  /*!< Deselect the FLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void SPIFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send "Read Status Register" instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_RDSR);
  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE);
  }
  while ((flashstatus & SPIFLASH_WIP_FLAG) == SET); /* Write in progress */
  /*!< Deselect the FLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
}

u32 SPIFALSH_GetTotalsize(void)
{
//  u32 flashid;
//  flashid = SPIFLASH_ReadID();
//  if(SPIFLASH_BG25Q16A_ID == (FlashID>>8))
//  {
//    return SPIFLASH_BG25Q16A_MEM;
//  }else if(SPIFLASH_M25PE16_ID == (FlashID>>8))
//  {
    return SPIFLASH_M25PE16_MEM;  
//  }else if(SPIFLASH_M25P16_ID == (FlashID>>8))
//  {
//    return SPIFLASH_M25P16_MEM; 
//  }else
//  {
//    return 0;
//  }
}

void SPIFLASH_EraseChip(void)
{
  /*!< Send write enable instruction */
  SPIFLASH_WriteEnable(); 
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_CE);
  /*!< Deselect the SPIFLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
  /*!< Wait the end of Flash writing */
  SPIFLASH_WaitForWriteEnd();
}
/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void SPIFLASH_EraseSector(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  SPIFLASH_WriteEnable();
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
//  SPIFLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*!< Send SectorAddr high nibble address byte */
  SPIFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  SPIFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  SPIFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the SPIFLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
  /*!< Wait the end of Flash writing */
  SPIFLASH_WaitForWriteEnd();
}
/**
  * @brief  Erases the specified FLASH subsector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void SPIFLASH_EraseSubSector(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  SPIFLASH_WriteEnable();
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_SSE);
  /*!< Send SectorAddr high nibble address byte */
//  SPIFLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*!< Send SectorAddr high nibble address byte */
  SPIFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  SPIFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  SPIFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the SPIFLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
  /*!< Wait the end of Flash writing */
  SPIFLASH_WaitForWriteEnd();
}
u32 SPIFALSH_GetSectorsize(void)
{
  return SPIFLASH_SECTSIZE;
}

/**
  * @brief  Erases the specified FLASH page.
  * @param  SectorAddr: address of the page to erase.
  * @retval None
  */
void SPIFLASH_ErasePage(uint32_t PageAddr)
{
  /*!< Send write enable instruction */
  SPIFLASH_WriteEnable();
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_PE);
  /*!< Send SectorAddr high nibble address byte */
//  SPIFLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*!< Send SectorAddr high nibble address byte */
  SPIFLASH_SendByte((PageAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  SPIFLASH_SendByte((PageAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  SPIFLASH_SendByte(PageAddr & 0xFF);
  /*!< Deselect the SPIFLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
  /*!< Wait the end of Flash writing */
  SPIFLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void SPIFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  SPIFLASH_WriteEnable();
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
//  SPIFLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  /*!< Send WriteAddr high nibble address byte to write to */
  SPIFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  SPIFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  SPIFLASH_SendByte(WriteAddr & 0xFF);
  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    SPIFLASH_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }
  /*!< Deselect the FLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
  /*!< Wait the end of Flash writing */
  SPIFLASH_WaitForWriteEnd();
}

void SPIFLASH_Write_Page(void)
{
  if(SPIFLASH_LASTwrite == 1)
  {
    SPIFLASH_ErasePage((SPIFLASH_LASTWADDR&GET_HEADOFPAGE));
    SPIFLASH_WritePage(SPIFLASH_PAGETMP, (SPIFLASH_LASTWADDR&GET_HEADOFPAGE), SPIFLASH_PAGESIZE);
    SPIFLASH_LASTwrite = 0;
  }
}
u8 SPIFLASH_WriteBuffer_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
  u32 i;
  if(WriteAddr+NumByteToWrite > SPIFLASH_M25PE16_MEM)
    return 0;
  if(((WriteAddr/SPIFLASH_PAGESIZE) != ((WriteAddr+NumByteToWrite)/SPIFLASH_PAGESIZE)))
  {//写入数据不在一个Page
    Addr = WriteAddr % SPIFLASH_PAGESIZE;
    count = SPIFLASH_PAGESIZE - Addr;
    NumByteToWrite -= count;
    NumOfPage =  NumByteToWrite / SPIFLASH_PAGESIZE;
    NumOfSingle = NumByteToWrite % SPIFLASH_PAGESIZE;
    if(((SPIFLASH_LASTWADDR&GET_HEADOFPAGE) != (WriteAddr&GET_HEADOFPAGE)))
    {//当前写入地址不在上一次的Page
      if(SPIFLASH_LASTWADDR != 0xFFFFFFFF)
      {//非首次写入就需要将之前的缓存数据保存
        SPIFLASH_Write_Page();
      }
      SPIFLASH_ReadBuffer(SPIFLASH_PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
      for(i=WriteAddr&GET_ADDRINPAGE;i<((WriteAddr&GET_ADDRINPAGE)+count);i++)
      {
        SPIFLASH_PAGETMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_ErasePage(WriteAddr&GET_HEADOFPAGE);
      SPIFLASH_WritePage(SPIFLASH_PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
      WriteAddr +=  count;
    }else
    {//当前写入数据与上一次写入位于同一Page
      for(i=WriteAddr&GET_ADDRINPAGE;i<((WriteAddr&GET_ADDRINPAGE)+count);i++)
      {
        SPIFLASH_PAGETMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_ErasePage(WriteAddr&GET_HEADOFPAGE);
      SPIFLASH_WritePage(SPIFLASH_PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
      WriteAddr +=  count;
    }
    while (NumOfPage--)
    {//多Page时写入中间Page
      for(i=0;i<SPIFLASH_PAGESIZE;i++)
      {
        SPIFLASH_PAGETMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_ErasePage(WriteAddr&GET_HEADOFPAGE);
      SPIFLASH_WritePage(SPIFLASH_PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
      WriteAddr +=  SPIFLASH_PAGESIZE;
    }
    if(NumOfSingle != 0)
    {//多Page时将后面的单独数据写入
      SPIFLASH_ReadBuffer(SPIFLASH_PAGETMP, WriteAddr, SPIFLASH_PAGESIZE);
      for(i=0;i<NumOfSingle;i++)
      {
        SPIFLASH_PAGETMP[i] = *pBuffer;
        pBuffer++ ;
      }
    }
    SPIFLASH_LASTWADDR = WriteAddr;
    SPIFLASH_LASTwrite = 1;       
  }else
  {//写入数据位于一个Page
    if(((SPIFLASH_LASTWADDR&GET_HEADOFPAGE) != (WriteAddr&GET_HEADOFPAGE)))
    {//当前地址与上一次非一个Page
      if(SPIFLASH_LASTWADDR != 0xFFFFFFFF)
      {//非首次写入就需要将之前的缓存数据保存
        SPIFLASH_Write_Page();
      }
      SPIFLASH_ReadBuffer(SPIFLASH_PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
      for(i=WriteAddr&GET_ADDRINPAGE;i<((WriteAddr&GET_ADDRINPAGE)+NumByteToWrite);i++)
      {
        SPIFLASH_PAGETMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_LASTWADDR = WriteAddr&GET_HEADOFPAGE;
      SPIFLASH_LASTwrite = 1; 
    }else
    {//当前地址与上一次位于一个Page
      for(i=WriteAddr&GET_ADDRINPAGE;i<((WriteAddr&GET_ADDRINPAGE)+NumByteToWrite);i++)
      {
        SPIFLASH_PAGETMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_LASTwrite = 1; 
    }
  }
  return 1;
}

#if 0
u8 SPIFLASH_TMP[SPIFLASH_SECTSIZE];

/**
  * @brief  Writes one secotr to the FLASH with a single WRITE cycle 
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @retval None
  */
void SPIFLASH_Writesecotr(uint8_t* pBuffer, uint32_t WriteAddr)
{
  u32 i;
  for(i=0;i<SPIFLASH_PAGEINSECTORS;i++)
  {
    SPIFLASH_WritePage(pBuffer, WriteAddr, SPIFLASH_PAGESIZE);
    WriteAddr += SPIFLASH_PAGESIZE;
    pBuffer += SPIFLASH_PAGESIZE;
  }
}

void SPIFLASH_Write_Sector(void)
{
  if(SPIFLASH_LASTwrite == 1)
  {
    SPIFLASH_EraseSector((SPIFLASH_LASTWADDR&GET_HEADOFSECTOR));
    SPIFLASH_Writesecotr(SPIFLASH_TMP, (SPIFLASH_LASTWADDR&GET_HEADOFSECTOR));
    SPIFLASH_LASTwrite = 0;
  }
}
/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  *         WriteAddr: FLASH's internal address to write to.
  *         NumByteToWrite: number of bytes to write to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
u8 SPIFLASH_WriteBuffer_Sector(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint32_t NumOfSingle = 0, Addr = 0, count = 0;
  u32 NumOfSector = 0;
  u32 i;
  if(WriteAddr+NumByteToWrite > SPIFLASH_M25P16_MEM)
    return 0;
  if((WriteAddr/SPIFLASH_SECTSIZE) != ((WriteAddr+NumByteToWrite)/SPIFLASH_SECTSIZE)) 
  {//数据不在一个Sector
    Addr = WriteAddr % SPIFLASH_SECTSIZE;
    count = SPIFLASH_SECTSIZE - Addr;
    NumByteToWrite -= count;
    NumOfSector =  NumByteToWrite / SPIFLASH_SECTSIZE;
    NumOfSingle = NumByteToWrite % SPIFLASH_SECTSIZE;
    if(((SPIFLASH_LASTWADDR&GET_HEADOFSECTOR) != (WriteAddr&GET_HEADOFSECTOR)))   
    {//当前写入地址不在上一次的Sector
      if(SPIFLASH_LASTWADDR != 0xFFFFFFFF)
      {
        SPIFLASH_Write_Sector();
      }
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSECTOR),SPIFLASH_SECTSIZE);
      for(i=WriteAddr&GET_ADDRINSECTOR;i<((WriteAddr&GET_ADDRINSECTOR)+count);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_EraseSector(WriteAddr&GET_HEADOFSECTOR);
      SPIFLASH_Writesecotr(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSECTOR));
      WriteAddr +=  count;    
    }else
    {//当前写入数据与上一次写入位于同一Sector
      for(i=WriteAddr&GET_ADDRINSECTOR;i<((WriteAddr&GET_ADDRINSECTOR)+count);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_EraseSector(WriteAddr&GET_HEADOFSECTOR);
      SPIFLASH_Writesecotr(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSECTOR));
      WriteAddr +=  count;        
    }
    while (NumOfSector--)
    {//多Sector时写入中间Sector
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, WriteAddr,SPIFLASH_SECTSIZE);
      for(i=0;i<SPIFLASH_SECTSIZE;i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_EraseSector(WriteAddr&GET_HEADOFSECTOR);
      SPIFLASH_Writesecotr(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSECTOR));
      WriteAddr +=  SPIFLASH_SECTSIZE;
    }
    if (NumOfSingle != 0)
    {//多Page时将后面的单独数据写入
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, WriteAddr,SPIFLASH_SECTSIZE);
      for(i=0;i<NumOfSingle;i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
    }
    SPIFLASH_LASTWADDR = WriteAddr;
    SPIFLASH_LASTwrite = 1;
  }else
  {//数据在一个Sector
    if(((SPIFLASH_LASTWADDR&GET_HEADOFSECTOR)!=(WriteAddr&GET_HEADOFSECTOR))) 
    {//本次地址与上次不在一个Sector
      if(SPIFLASH_LASTWADDR != 0xFFFFFFFF)
      {//非首次写入就需要将之前的缓存数据保存
        SPIFLASH_Write_Sector();
      }
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSECTOR),SPIFLASH_SECTSIZE);
      for(i=WriteAddr&GET_ADDRINSECTOR;i<((WriteAddr&GET_ADDRINSECTOR)+NumByteToWrite);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++;
      }
      SPIFLASH_LASTWADDR = WriteAddr;
      SPIFLASH_LASTwrite = 1;
    }else                                                                 
    {//当前地址与上一次位于一个Sector               
      for(i=WriteAddr&GET_ADDRINSECTOR;i<((WriteAddr&GET_ADDRINSECTOR)+NumByteToWrite);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++;
      }     
      SPIFLASH_LASTwrite = 1;
    }
  }
  return 1;
}
/**
  * @brief  Writes one secotr to the FLASH with a single WRITE cycle 
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @retval None
  */
void SPIFLASH_WriteSubsecotr(uint8_t* pBuffer, uint32_t WriteAddr)
{
  u32 i;
  for(i=0;i<SPIFLASH_PAGEINSUBSECTORS;i++)
  {
    SPIFLASH_WritePage(pBuffer, WriteAddr, SPIFLASH_PAGESIZE);
    WriteAddr += SPIFLASH_PAGESIZE;
    pBuffer += SPIFLASH_PAGESIZE;
  }
}
/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @retval None
  */
void SPIFLASH_Write_SubSector(void)
{
  if(SPIFLASH_LASTwrite == 1)
  {
    SPIFLASH_EraseSubSector((SPIFLASH_LASTWADDR&GET_HEADOFSUBSECTOR));
    SPIFLASH_WriteSubsecotr(SPIFLASH_TMP, (SPIFLASH_LASTWADDR&GET_HEADOFSUBSECTOR));
    SPIFLASH_LASTwrite = 0;
  }
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  *         WriteAddr: FLASH's internal address to write to.
  *         NumByteToWrite: number of bytes to write to the FLASH.
  *         add: 使用追加的时候应当确认该扇区已经被擦出过，并且一直都是往后面写入。
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
u8 SPIFLASH_WriteBuffer_SubSector(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint32_t NumOfSingle = 0, Addr = 0, count = 0;
  u32 NumOfSector = 0;
  u32 i;
  if(WriteAddr+NumByteToWrite > SPIFLASH_M25P16_MEM)
    return 0;
  if((WriteAddr/SPIFLASH_SUBSECTSIZE) != ((WriteAddr+NumByteToWrite)/SPIFLASH_SUBSECTSIZE)) 
  {//数据不在一个Sector
    Addr = WriteAddr % SPIFLASH_SUBSECTSIZE;
    count = SPIFLASH_SUBSECTSIZE - Addr;
    NumByteToWrite -= count;
    NumOfSector =  NumByteToWrite / SPIFLASH_SUBSECTSIZE;
    NumOfSingle = NumByteToWrite % SPIFLASH_SUBSECTSIZE;
    if(((SPIFLASH_LASTWADDR&GET_HEADOFSUBSECTOR) != (WriteAddr&GET_HEADOFSUBSECTOR)))   
    {//当前写入地址不在上一次的Sector
      if(SPIFLASH_LASTWADDR != 0xFFFFFFFF)
      {
        SPIFLASH_Write_SubSector();
      }
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSUBSECTOR),SPIFLASH_SECTSIZE);
      for(i=WriteAddr&GET_ADDRINSECTOR;i<((WriteAddr&GET_ADDRINSECTOR)+count);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_EraseSector(WriteAddr&GET_HEADOFSUBSECTOR);
      SPIFLASH_Writesecotr(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSUBSECTOR));
      WriteAddr +=  count;    
    }else
    {//当前写入数据与上一次写入位于同一Sector
      for(i=WriteAddr&GET_ADDRINSUBSECTOR;i<((WriteAddr&GET_ADDRINSUBSECTOR)+count);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_EraseSector(WriteAddr&GET_HEADOFSUBSECTOR);
      SPIFLASH_Writesecotr(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSUBSECTOR));
      WriteAddr +=  count;        
    }
    while (NumOfSector--)
    {//多Sector时写入中间Sector
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, WriteAddr,SPIFLASH_SECTSIZE);
      for(i=0;i<SPIFLASH_SECTSIZE;i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
      SPIFLASH_EraseSector(WriteAddr&GET_HEADOFSUBSECTOR);
      SPIFLASH_Writesecotr(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSUBSECTOR));
      WriteAddr +=  SPIFLASH_SECTSIZE;
    }
    if (NumOfSingle != 0)
    {//多Page时将后面的单独数据写入
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, WriteAddr,SPIFLASH_SECTSIZE);
      for(i=0;i<NumOfSingle;i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++ ;
      }
    }
    SPIFLASH_LASTWADDR = WriteAddr;
    SPIFLASH_LASTwrite = 1;
  }else
  {//数据在一个Sector
    if(((SPIFLASH_LASTWADDR&GET_HEADOFSUBSECTOR)!=(WriteAddr&GET_HEADOFSUBSECTOR))) 
    {//本次地址与上次不在一个Sector
      if(SPIFLASH_LASTWADDR != 0xFFFFFFFF)
      {//非首次写入就需要将之前的缓存数据保存
        SPIFLASH_Write_SubSector();
      }
      SPIFLASH_ReadBuffer(SPIFLASH_TMP, (WriteAddr&GET_HEADOFSUBSECTOR),SPIFLASH_SECTSIZE);
      for(i=WriteAddr&GET_ADDRINSUBSECTOR;i<((WriteAddr&GET_ADDRINSUBSECTOR)+NumByteToWrite);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++;
      }
      SPIFLASH_LASTWADDR = WriteAddr;
      SPIFLASH_LASTwrite = 1;
    }else                                                                 
    {//当前地址与上一次位于一个Sector               
      for(i=WriteAddr&GET_ADDRINSUBSECTOR;i<((WriteAddr&GET_ADDRINSUBSECTOR)+NumByteToWrite);i++)
      {
        SPIFLASH_TMP[i] = *pBuffer;
        pBuffer++;
      }     
      SPIFLASH_LASTwrite = 1;
    }
  }
  return 1;
  
}


#endif

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void SPIFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send "Read from Memory " instruction */
  SPIFLASH_SendByte(SPIFLASH_CMD_READ);
  /*!< Send ReadAddr high nibble address byte to read from */
//  SPIFLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /*!< Send ReadAddr high nibble address byte to read from */
  SPIFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  SPIFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  SPIFLASH_SendByte(ReadAddr & 0xFF); 
  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }
  /*!< Deselect the FLASH: Chip Select high */
  SPIFLASH_CS_HIGH();
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t SPIFLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0, Temp3 = 0;

  /*!< Select the FLASH: Chip Select low */
  SPIFLASH_CS_LOW();
  /*!< Send "RDID " instruction */
  SPIFLASH_SendByte(0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp3 = SPIFLASH_SendByte(SPIFLASH_DUMMY_BYTE);
  
  /*!< Deselect the FLASH: Chip Select high */
  SPIFLASH_CS_HIGH();

  Temp = (Temp0 << 24) | (Temp1 << 16) | (Temp2 << 8) | Temp3;

  return Temp;
}


u8 SPIFLASH_WriteBuffer(uint32_t WriteAddr,uint32_t NumByteToWrite, uint8_t* pBuffer)
{
//  if(SPIFLASH_BG25Q16A_ID == (FlashID>>8))
//  {
//    return SPIFLASH_WriteBuffer_SubSector(pBuffer,WriteAddr,NumByteToWrite);
//  }else if(SPIFLASH_M25P16_ID == (FlashID>>8))
//  {
//    return SPIFLASH_WriteBuffer_Sector(pBuffer,WriteAddr,NumByteToWrite); 
//  }else if(SPIFLASH_M25PE16_ID == (FlashID>>8))
//  {
    return SPIFLASH_WriteBuffer_Page(pBuffer,WriteAddr,NumByteToWrite); 
//  }else
//  {
//    return 0;
//  } 
}
void SPIFLASH_Write(void)
{
//  if(SPIFLASH_BG25Q16A_ID == (FlashID>>8))
//  {
//    SPIFLASH_Write_SubSector();
//  }else if(SPIFLASH_M25P16_ID == (FlashID>>8))
//  {
//    SPIFLASH_Write_Sector();  
//  }else if(SPIFLASH_M25PE16_ID == (FlashID>>8))
//  {
    SPIFLASH_Write_Page();  
//  } 
}
void SPIFLASH_Read(uint32_t ReadAddr, uint32_t NumByteToRead, uint8_t* pBuffer)
{
  SPIFLASH_ReadBuffer(pBuffer, ReadAddr, NumByteToRead);
}

u8 PAGETMP[SPIFLASH_PAGESIZE];
u8 SPIFLASH_WriteImmed(uint32_t WriteAddr,uint32_t NumByteToWrite, uint8_t* pBuffer)
{
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
  u32 i;
  Addr = WriteAddr % SPIFLASH_PAGESIZE;
  count = SPIFLASH_PAGESIZE - Addr;
  if(((WriteAddr/SPIFLASH_PAGESIZE) != ((WriteAddr+NumByteToWrite)/SPIFLASH_PAGESIZE)))
  {
    NumByteToWrite -= count;
    NumOfPage =  NumByteToWrite / SPIFLASH_PAGESIZE;
    NumOfSingle = NumByteToWrite % SPIFLASH_PAGESIZE;
  }
  if(WriteAddr+NumByteToWrite > SPIFLASH_M25PE16_MEM)
    return 0;
  if(count != 0)
  { //写入page前段单独的字节
    SPIFLASH_Read((WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE,PAGETMP);
    for(i=WriteAddr&GET_ADDRINPAGE;i<((WriteAddr&GET_ADDRINPAGE)+count);i++)
    {
      PAGETMP[i] = *pBuffer;
      pBuffer++ ;
    }
    SPIFLASH_ErasePage(WriteAddr&GET_HEADOFPAGE);
    SPIFLASH_WritePage(PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
    WriteAddr += count;
  }
  while (NumOfPage--)
  {//多Page时写入中间Page
    SPIFLASH_ErasePage(WriteAddr&GET_HEADOFPAGE);
    SPIFLASH_WritePage(pBuffer, WriteAddr,SPIFLASH_PAGESIZE);
    WriteAddr +=  SPIFLASH_PAGESIZE;
    pBuffer += SPIFLASH_PAGESIZE;
  }
  if(NumOfSingle != 0)
  {//多Page时将后面的单独数据写入
    SPIFLASH_Read((WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE,PAGETMP);
    for(i=WriteAddr&GET_ADDRINPAGE;i<(NumOfSingle);i++)
    {
      PAGETMP[i] = *pBuffer;
      pBuffer++ ;
    }
    SPIFLASH_ErasePage(WriteAddr&GET_HEADOFPAGE);
    SPIFLASH_WritePage(PAGETMP, (WriteAddr&GET_HEADOFPAGE),SPIFLASH_PAGESIZE);
  }
  return 1; 
}


/* DRV for SPIMRAM */
u8 SPIMRAM_SendByte(u8 byte)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPIFORMRAM, SPI_I2S_FLAG_TXE) == RESET);
  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPIFORMRAM, byte);
  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPIFORMRAM, SPI_I2S_FLAG_RXNE) == RESET);
  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPIFORMRAM); 
}

void SPIMRAM_WriteEnable(void)
{
  /*!< Select the SPIMRAM: Chip Select low */
  SPIMRAM_CS_LOW();
  /*!< Send "Write Enable" instruction */
  SPIMRAM_SendByte(SPIMRAM_CMD_WREN);
  /*!< Deselect the MRAM: Chip Select high */
  SPIMRAM_CS_HIGH();  
}

void SPIMRAM_WriteDisable(void)
{
  /*!< Select the SPIMRAM: Chip Select low */
  SPIMRAM_CS_LOW();
  /*!< Send "Write Enable" instruction */
  SPIMRAM_SendByte(SPIMRAM_CMD_WRDI);
  /*!< Deselect the MRAM: Chip Select high */
  SPIMRAM_CS_HIGH();    
}

void SPIMRAM_WaitForWriteEnd(void)
{
  u8 flashstatus = 0;
  /*!< Select the FLASH: Chip Select low */
  SPIMRAM_CS_LOW();
  /*!< Send "Read Status Register" instruction */
  SPIMRAM_SendByte(SPIMRAM_CMD_RDSR);
  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = SPIMRAM_SendByte(SPIFLASH_DUMMY_BYTE);
  }
  while ((flashstatus & SPIMRAM_WIP_FLAG) == SET); /* Write in progress */
  /*!< Deselect the FLASH: Chip Select high */
  SPIMRAM_CS_HIGH();  
}

void SPIMRAM_WriteBuffer(unsigned char* pBuffer, unsigned int WriteAddr, unsigned int NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  SPIMRAM_WriteEnable();
  /*!< Select the MRAM: Chip Select low */
  SPIMRAM_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  SPIMRAM_SendByte(SPIMRAM_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
//  SPIMRAM_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  SPIMRAM_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  SPIMRAM_SendByte(WriteAddr & 0xFF);
  /*!< while there is data to be written on the MRAM */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    SPIMRAM_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }
  /*!< Deselect the MRAM: Chip Select high */
  SPIMRAM_CS_HIGH();  
  /*!< Disable the write access to the FLASH */
  SPIMRAM_WriteDisable();
  /*!< Wait the end of MRAM writing */
  SPIMRAM_WaitForWriteEnd();
}
void SPIMRAM_ReadBuffer(unsigned char* pBuffer, unsigned int ReadAddr, unsigned int NumByteToRead)
{
  /*!< Select the MRAM: Chip Select low */
  SPIMRAM_CS_LOW();
  /*!< Send "Read from Memory " instruction */
  SPIMRAM_SendByte(SPIMRAM_CMD_READ);
  /*!< Send ReadAddr high nibble address byte to read from */
//  SPIMRAM_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  SPIMRAM_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  SPIMRAM_SendByte(ReadAddr & 0xFF);  
  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the MRAM */
    *pBuffer = SPIMRAM_SendByte(SPIFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }
  /*!< Deselect the MRAM: Chip Select high */
  SPIMRAM_CS_HIGH();
}

u32 SPIMRAM_GetTotalsize(void)
{
  return SPIMRAM_MR25H256_MEM;  
}

unsigned char SPIMRAM_Write(unsigned int WriteAddr,unsigned int NumByteToWrite, unsigned char* pBuffer)
{
  if(WriteAddr+NumByteToWrite > SPIMRAM_MR25H256_MEM)
    return 0;
  SPIMRAM_WriteBuffer(pBuffer, WriteAddr, NumByteToWrite);
  return 1;
}
void SPIMRAM_Read(unsigned int ReadAddr,unsigned int NumByteToRead, unsigned char* pBuffer)
{
  SPIMRAM_ReadBuffer(pBuffer, ReadAddr, NumByteToRead);
}

