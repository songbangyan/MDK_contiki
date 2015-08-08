#ifndef __SPIFLASH_H__
#define __SPIFLASH_H__

#include "stm32f10x.h"

/*
MRAM_CS     ~ PA1
SPIFLASH_CS ~ PA4
CLK         ~ PA5
MISO        ~ PA6
MOSI        ~ PA7
HOLD        ~ PC4
WP          ~ PC5
*/
#define SPIFORFLASH SPI1
#define SPIFORMRAM  SPI1

#define SPIFLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPIFLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4) 
#define SPIMRAM_CS_LOW()        GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define SPIMRAM_CS_HIGH()       GPIO_SetBits(GPIOA, GPIO_Pin_1) 

/**
  * @brief  BG25Q16A SPI Flash supported commands
  */  
#define SPIFLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define SPIFLASH_CMD_WRITEPAGE      0x0A  /*!< Write to Memory instruction */
#define SPIFLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define SPIFLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define SPIFLASH_CMD_WRDI           0x04  /*!< Write Disable instruction */
#define SPIFLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define SPIFLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define SPIFLASH_CMD_RDID           0x9F  /*!< Read identification */
#define SPIFLASH_CMD_PE             0xDB  /*!< Page Erase instruction 256Byte*/
#define SPIFLASH_CMD_SE             0xD8  /*!< Sector Erase instruction 64K*/
#define SPIFLASH_CMD_SSE            0x20  /*!< SubSector Erase instruction 4K*/
#define SPIFLASH_CMD_CE             0xC7  /*!< Chip Erase instruction */
#define SPIFLASH_CMD_4BYTE          0xB7  /*!< Enter 4 byte Addr mode */

#define SPIFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define SPIFLASH_DUMMY_BYTE         0xA5

#define SPIFLASH_PAGESIZE          0x100  
#define GET_HEADOFPAGE             0xFFFFFF00
#define GET_ADDRINPAGE             0x000000FF
#define SPIFLASH_SECTSIZE          0x10000
#define GET_HEADOFSECTOR           0xFFFF0000
#define GET_ADDRINSECTOR           0x0000FFFF
#define SPIFLASH_PAGEINSECTORS     (SPIFLASH_SECTSIZE/SPIFLASH_PAGESIZE)
#define SPIFLASH_SUBSECTSIZE       0x1000
#define GET_HEADOFSUBSECTOR          0xFFFFF000
#define GET_ADDRINSUBSECTOR          0x00000FFF
#define SPIFLASH_PAGEINSUBSECTORS    (SPIFLASH_SUBSECTSIZE/SPIFLASH_PAGESIZE)
//#define SPIFLASH_MAXSECTORSNO      0x200  

#define SPIFLASH_BG25Q16A_ID         0xE04015
#define SPIFLASH_BG25Q16A_MEM        (2*1024*1024)

#define SPIFLASH_M25PE16_ID          0x208015
#define SPIFLASH_M25PE16_MEM         (2*1024*1024)

#define SPIFLASH_M25P16_ID           0x202015
#define SPIFLASH_M25P16_MEM          (2*1024*1024)

#define SPIMRAM_CMD_WREN           0x06  /*!< Write enable instruction */
#define SPIMRAM_CMD_WRDI           0x04  /*!< Write Disable instruction */
#define SPIMRAM_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define SPIMRAM_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define SPIMRAM_CMD_READ           0x03  /*!< Read from Memory instruction */
#define SPIMRAM_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define SPIMRAM_CMD_SLEEP          0xB9  /*!< Enter Sleep Mode instruction */
#define SPIMRAM_CMD_WAKIE          0xAB  /*!< Exit Sleep Mode instruction */

#define SPIMRAM_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */


//#define SPIMRAM_MR25H10_MEM        0x20000
#define SPIMRAM_MR25H256_MEM       0x8000

void BSP_SPI_Init(void);


void SPIFLASH_EraseChip(void);
u32 SPIFALSH_GetTotalsize(void);
uint32_t SPIFLASH_ReadID(void);
void SPIFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);

void SPIFLASH_ErasePage(uint32_t PageAddr);
void SPIFLASH_Write_Page(void);
u8 SPIFLASH_WriteBuffer_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

void SPIFLASH_EraseSubSector(uint32_t SectorAddr);
void SPIFLASH_Write_SubSector(void);
u8 SPIFLASH_WriteBuffer_SubSector(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

void SPIFLASH_EraseSector(uint32_t SectorAddr);
void SPIFLASH_Write_Sector(void);
u8 SPIFLASH_WriteBuffer_Sector(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);


void SPIFLASH_Read(uint32_t ReadAddr, uint32_t NumByteToRead, uint8_t* pBuffer);
u8 SPIFLASH_WriteBuffer(uint32_t WriteAddr,uint32_t NumByteToWrite, uint8_t* pBuffer );
void SPIFLASH_Write(void);
u8 SPIFLASH_WriteImmed(uint32_t WriteAddr,uint32_t NumByteToWrite, uint8_t* pBuffer);


void SPIMRAM_ReadBuffer(unsigned char* pBuffer, unsigned int ReadAddr, unsigned int NumByteToRead);
void SPIMRAM_WriteBuffer(unsigned char* pBuffer, unsigned int WriteAddr, unsigned int NumByteToWrite);
unsigned int SPIMRAM_GetTotalsize(void);
void SPIMRAM_Read(unsigned int ReadAddr,unsigned int NumByteToRead, unsigned char* pBuffer);
unsigned char SPIMRAM_Write(unsigned int WriteAddr,unsigned int NumByteToWrite, unsigned char* pBuffer);

#endif
