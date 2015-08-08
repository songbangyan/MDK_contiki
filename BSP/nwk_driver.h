#ifndef NWK_DRIVER_H__
#define NWK_DRIVER_H__
#include "infobase.h"
//保存永久性数据
//入口参数：wrmode：保存数据模式，
//立刻写入：0x11
//先保存在内存中，12小时内写入: others
//return 1: 成功写入
//return 0：写入失败
#define IMMER   0x11
#define OTHERS  0xaa

extern unsigned char Module_Write_Flash(unsigned int *p, unsigned char length);
BYTE SetUartBrandRate(BYTE com, DWORD BaudRate);
BYTE SetUartState(BYTE com, DWORD BaudRate,WORD Parity,WORD StopBits);
extern void Module_Readinfo_fromFlash(void);
extern unsigned char Module_Saveinfo(unsigned char wrmode);
BYTE SendUartData(BYTE com,VOID *pBuff, BYTE length);
extern void ClrRecvBuf(unsigned char temp);
void RestDev(void);
#endif


