#include "bsp.h"





unsigned char Module_Write_Flash(unsigned int *p, unsigned char length)
{
  return 0;
}




 BYTE SetUartBrandRate(BYTE com, DWORD BaudRate)
 {
  return 1;
 }

 BYTE SetUartState(BYTE com, DWORD BaudRate,WORD Parity,WORD StopBits)
 {
  return 1;
 }


void Module_Readinfo_fromFlash(void)
{



}




unsigned char Module_Saveinfo(unsigned char wrmode)
{


  return 0;

}




 BYTE SendUartData(BYTE com,VOID *pBuff, BYTE length)
 {
  return 1; 
 }




void ClrRecvBuf(unsigned char temp)
{

}



void RestDev(void)
{

}



