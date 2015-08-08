#ifndef __TOOL_H_
#define __TOOL_H_

#include "stm32f10x.h" 
#include "string.h"


//#define FALSE 0
//#define TRUE  !FALSE
#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE !FALSE
#endif

unsigned char Char2Hex(unsigned char b);
unsigned char Hex2Char(unsigned char b);
unsigned char Hex2Bcd(unsigned char b);
unsigned char Bcd2Hex(unsigned char b);
unsigned char iHex2iBcd(unsigned short n, unsigned char *b);
unsigned char Hex64toBcd(double n, unsigned char *b);
unsigned char Mem_Reverse(unsigned char *in, unsigned char len, unsigned char *out);
unsigned short iBcd2iHex(unsigned char *b);

unsigned char mem_comp(unsigned char *pdata_a,unsigned char *pdata_b,unsigned int len);
void mem_copy(unsigned char *pdst,unsigned char *psrc,unsigned int len);
void mem_clear(unsigned char *pdata,unsigned int len);
void mem_set(unsigned char *pdata,unsigned int len);

void one_word_crc32(unsigned int data, unsigned int *result);
void set_crc32_reset(unsigned int *result);
void get_crc32(unsigned char *pdata, unsigned short len, unsigned int *result);

#undef  DEF_CHECKSUM_CALC
#define DEF_CHECKSUM_CALC     0   //����У���
#undef  DEF_CHECKSUM_CHECK
#define DEF_CHECKSUM_CHECK      1   //У��У���
unsigned char Cs_Check(unsigned char bDir, unsigned char *pbCheckbuf, unsigned int uiStartPos, unsigned int uiCheckLen);

////////////////////////////////////////////////////////////////////////////
#define DEF_PACK_UPCOM_TRANSPROT_00 0 //����ȫ͸����ʽת��
#define DEF_PACK_UPCOM_TRANSPROT_97 1 //��97��ʽת��
#define DEF_PACK_UPCOM_TRANSPROT_07 2 //��07��ʽת��
#define DEF_PACK_UPCOM_TRANSPROT_DB 3 //������ݴ���

#define DEF_PACK_UPCOM_DATA_LEN   150   //֡�����ݳ���


#define DEF_PACK_UPCOM_ADDR_LEN   6   //֡��ַ����


#define DEF_PACK_UPCOM_TRANSPROT    3   //��֡Ϊת��֡
#define DEF_PACK_UPCOM_645_RECV_OK  2   //��֡Ϊ��ȷ645ͨѶ֡
#define DEF_PACK_UPCOM_RECV_OK    1   //��֡Ϊ��ȷ�ɼ���ͨѶ֡
#define DEF_PACK_UPCOM_RECV_TIMEOUT -1    //��֡Ϊ���ճ�ʱ

#define DEF_PACK_UPCOM_NO_DATA    -3    //δ�յ�645Э��������
#define DEF_PACK_UPCOM_NO_PACK    -4    //δ�յ�645Э������֡
#define DEF_PACK_UPCOM_PHEAD_ERR    -5    //��֡Ϊ֡ͷ����
#define DEF_PACK_UPCOM_PEND_ERR   -6    //��֡Ϊ֡β����
#define DEF_PACK_UPCOM_PCS_ERR    -7    //��֡Ϊ֡CS����
#define DEF_PACK_UPCOM_ADDR_ERR   -8    //��֡��ַ����
typedef struct
{
  
  unsigned char pcmd ;
  unsigned char isDbFrame; //�Ƿ�Ϊ���֡

}SPACK_UPCOM;

//�ӳ�ʱ��
void DelayTime(unsigned int delay);

//�������
//bytDir��У�鷽��=0��ʾ��У�飬=1��ʾУ����
//abytCommOrder����Ҫ��У���У���������
//nStartPos��У�鿪ʼλ��
//nCheckLen����ҪУ����ֽڳ���
//���ز���
//��bytDir=0���򷵻ؼ������У��
//��bytDir=1����У����ȷ����1�����򷵻�0
unsigned short Crc_Check(unsigned int wInitCrc,unsigned char bytDir, unsigned char *abytCommOrder , unsigned short nStartPos, unsigned short nCheckLen);

//���ز���
//DEF_PACK_UPCOM_RECV_OK    ���յ���ȷ֡
//DEF_PACK_UPCOM_645_RECV_OK      ���յ�645֡
//DEF_PACK_UPCOM_PHEAD_ERR  ��δ�յ�֡ͷ
//DEF_PACK_UPCOM_PEND_ERR   ��δ�յ�֡β
//DEF_PACK_UPCOM_PCS_ERR    ��CSУ�����
signed short PactUpCom_Analysis(unsigned char *bCheckBuf, unsigned char bChecklen, SPACK_UPCOM *pack_UpCom);


//����һ֡����ͨ��Э���Ƿ�Ϊת��֡
//�������
//bCheckBuf ����Ҫ����������
//bChecklen ����Ҫ���������ݳ���
//DEF_PACK_UPCOM_RECV_OK    ���յ���ȷ֡
//DEF_PACK_UPCOM_645_RECV_OK      ���յ�645֡
//DEF_PACK_UPCOM_PHEAD_ERR  ��δ�յ�֡ͷ
//DEF_PACK_UPCOM_PEND_ERR   ��δ�յ�֡β
//DEF_PACK_UPCOM_PCS_ERR    ��CSУ�����
//DEF_PACK_UPCOM_TRANSPROT    : ���͸��ת��
signed short PactUpCom_Analysis_TransPort(unsigned char *bCheckBuf, unsigned char bChecklen,signed short uPackUpComType, SPACK_UPCOM *pack_UpCom);


#define DEF_CMD_645_GET_DATA    0x01    //645Э�����������
#define DEF_CMD_645_SET_DATA    0x04    //645Э�����д����
#define DEF_CMD_645_MODIFY_PWD    0x0F    //645Э������ĵ������
#define DEF_CMD_645_BROAD_SETTIME 0x08    //645Э������㲥Уʱ
#define DEF_CMD_645_BROAD_GETADDR 0x0D    //645Э������㲥������ַ
#define DEF_CMD_645_BROAD_FREEZE  0x12    //645Э������㲥�������


/************************07******************************************/
#define DEF_CMD_645_07_GET_DATA     0x11    //645_07Э�����������
#define DEF_CMD_645_07_SET_DATA     0x14    //645_07Э�����д����
#define DEF_CMD_645_07_MODIFY_PWD   0x18    //645_07Э������ĵ������
#define DEF_CMD_645_07_BROAD_SETTIME  0x08    //645_07Э������㲥Уʱ
#define DEF_CMD_645_07_BROAD_GETADDR  0x13    //645_07Э������㲥������ַ
#define DEF_CMD_645_07_BROAD_SWITCH     0x1c      //645_07Э�������բ����բ������
#define DEF_CMD_645_07_BROAD_CONTROL     0x1d     //645_07Э�������������
#define DEF_CMD_645_07_SAFEATTH_DATA    0x03  //645_07Э�������ȫ��֤

//////////////����Լ
#define DEF_DB_GY_645   1   //645Э��
#define DEF_DB_GY_07    30 //07��Լ 
#define DEF_DB_GY_UNKNOW  255 //δ֪

#define DEF_DB_GY_BROADCAST_SYC_TIME 0XFF //�㲥ͬ��ʱ��

//�õ����Ĺ�Լ����
unsigned char GetGyTypeByCtr(unsigned char bCtrCode);


#endif
