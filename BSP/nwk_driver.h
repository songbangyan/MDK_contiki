#ifndef NWK_DRIVER_H__
#define NWK_DRIVER_H__
#include "infobase.h"
//��������������
//��ڲ�����wrmode����������ģʽ��
//����д�룺0x11
//�ȱ������ڴ��У�12Сʱ��д��: others
//return 1: �ɹ�д��
//return 0��д��ʧ��
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


