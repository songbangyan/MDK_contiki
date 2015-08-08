

#include "tool.h"



unsigned char Char2Hex(unsigned char b)
{
  if(b>='0' && b<='9')
    return b-'0';
  if(b>='A' && b<='F')
    return b-'A'+10 ;
  if(b>='a' && b<='f')
    return b-'a'+10 ;
  return '?' ;  //63='?'
}

unsigned char Hex2Char(unsigned char b)
{
  if(b <= 9)
    return b + '0';
  else if(b <= 0x0f)
    return b - 10 + 'A';
  return '?' ;  //63='?'
}

unsigned char Hex2Bcd(unsigned char b)
{
  unsigned char bcd ;

  bcd = ((b/10)<<4) | (b%10) ;

  return bcd ;
}

unsigned char Bcd2Hex(unsigned char b)
{
  unsigned char hex ;

  hex = (((b&0xf0)>>4)*10) + (b&0x0f) ;

  return hex ;
}

unsigned char iHex2iBcd(unsigned short n, unsigned char *b)
{
  unsigned char hex1, hex2 ;

  hex1 = (n%10000)/100 ;
  hex2 = n%100 ;
  b[1] = ((hex1/10) << 4) | hex1%10 ;
  b[0] = ((hex2/10) << 4) | hex2%10 ;

  return 1 ;
}



//unsigned char Hex64toBcd(double n, unsigned char *b)
//{
//  unsigned char i=0;
//  double hex1, hex2 ;
//  while(n >0)
//  {
//    hex1 = (n%10000)/100 ;
//    hex2 = n%100 ;
//    b[i+1] = (unsigned char)((hex1/10) << 4) | hex1%10 ;
//    b[i] = (unsigned char)((hex2/10) << 4) | hex2%10 ;
//    n = n/10000;
//    i = i+2;
//  }

//  return 0;

//}



//�ڴ淴ת ���ת6�ֽ�
unsigned char Mem_Reverse(unsigned char *in, unsigned char len, unsigned char *out)
{
  signed short i ;
  unsigned char tmp[6] ;

  memcpy(tmp,in,len) ;
  for(i=0;i<len;i++)
  {
    out[i] = tmp[len-1-i] ;
  }
  return 1 ;
}

unsigned short iBcd2iHex(unsigned char *b)
{
  unsigned short hex ;

  hex = Bcd2Hex(b[1])*100 + Bcd2Hex(b[0]) ;

  return hex ;
}


//����˵��    ��У��ͼ��
//�������
//bDir      ��У�鷽��=0��ʾ��У�飬=1��ʾУ����
//bCheckbuf   ����ҪУ�������
//uiStartPos  ��У�鿪ʼλ��
//uiCheckLen  ����ҪУ����ֽڳ���
//���ز���
//��bDir=0���򷵻ؼ������У���
//��bDir=1����У����ȷ����1�����򷵻�0
unsigned char Cs_Check(unsigned char bDir, unsigned char *pbCheckbuf, unsigned int uiStartPos, unsigned int uiCheckLen)
{
  unsigned char cs ;
  unsigned int index ;

  cs = 0 ;
  for(index=uiStartPos; index<uiStartPos+uiCheckLen; index++)
  {
    cs = cs + pbCheckbuf[index] ;
  }
  if(bDir == 0)
  {
    return cs ;
  }
  if(pbCheckbuf[index] == cs) return 1 ;
  return 0 ;
}


//�������
//bytDir��У�鷽��=0��ʾ��У�飬=1��ʾУ����
//abytCommOrder����Ҫ��У���У���������
//nStartPos��У�鿪ʼλ��
//nCheckLen����ҪУ����ֽڳ���
//���ز���
//��bytDir=0���򷵻ؼ������У��
//��bytDir=1����У����ȷ����1�����򷵻�0
unsigned short Crc_Check(unsigned int wInitCrc,unsigned char bytDir, unsigned char *abytCommOrder , unsigned short nStartPos, unsigned short nCheckLen)
{
  signed short MSBInfo ;
  unsigned short wCrcData ;
  unsigned short nIndexI,nIndexJ ;

  wCrcData=wInitCrc;
  for(nIndexI = nStartPos; nIndexI < (nCheckLen+nStartPos); nIndexI ++)
  {
    wCrcData = wCrcData ^ abytCommOrder[nIndexI];
    for(nIndexJ = 0; nIndexJ < 8; nIndexJ ++)
    {
      MSBInfo = wCrcData & 0x0001;
      wCrcData = wCrcData  >> 1;
      if(MSBInfo != 0 )
      {
        wCrcData = wCrcData ^ 0xa001;
      }
    }
  }
  if (bytDir== 0)
  {
    //abytCommOrder[nIndexI] = wCrcData % 0x100;
    //abytCommOrder[nIndexI + 1] = wCrcData / 0x100;
    //abytCommOrder[nIndexI + 2] = 0x16;
    return wCrcData;
  }
  if (abytCommOrder[nIndexI] != (wCrcData % 0x100)) return 0;
  if (abytCommOrder[nIndexI + 1] != (wCrcData / 0x100)) return 0;
  return 1;
}

signed short PactUpCom_Analysis(unsigned char *bCheckBuf, unsigned char bChecklen, SPACK_UPCOM *pack_UpCom)
{
  unsigned char bit, dlen, flg ;
  pack_UpCom->isDbFrame = FALSE;
  flg = 0 ;
  for(bit=0; bit+12<=bChecklen; bit++)
  {
    if(  ( (bCheckBuf[bit] == 0x68) && (bCheckBuf[bit+7] == 0x68) ) /* ||  ( (bCheckBuf[bit] == 0xA8) && (bCheckBuf[bit+7] == 0xA8) ) */ )
    {
      dlen = bCheckBuf[bit+9] ;               //�õ������򳤶�
    

      if(bit+dlen+12 <= bChecklen)              //�ж��Ƿ��յ�ָ�������ֽ���
      {
        flg++ ;
        if(bCheckBuf[bit+9+dlen+2] == 0x16)       //�жϽ�����
        {
          if(Cs_Check(DEF_CHECKSUM_CHECK, bCheckBuf, bit, dlen+10) == 1)  //CSУ����ȷ
          {

#ifdef VIRTUAL_PC
//              printf("DEF_PACK_UPCOM_645_RECV_OK\n");
#endif        
              pack_UpCom->isDbFrame =TRUE;
              pack_UpCom->pcmd = bCheckBuf[bit+8];
              return DEF_PACK_UPCOM_645_RECV_OK ;     //�յ���ȷ645֡
    
    
          }
          else
          {
#ifdef VIRTUAL_PC
//            printf("DEF_PACK_UPCOM_PCS_ERR\n");
#endif
            return DEF_PACK_UPCOM_PCS_ERR ;     //CSУ�����
          
          }
        }
      }
    }
  }


  if(flg == 0) 
  {
#ifdef VIRTUAL_PC
//    printf("DEF_PACK_UPCOM_PHEAD_ERR\n");
#endif
    return  DEF_PACK_UPCOM_PHEAD_ERR ;        //δ�յ�֡ͷ
    
  }
  return DEF_PACK_UPCOM_PEND_ERR ;                //δ�յ�֡β
}



unsigned char GetGyTypeByCtr(unsigned char bCtrCode)
{
  unsigned char i=0;
  const unsigned char ctr07[] = {DEF_CMD_645_07_GET_DATA,DEF_CMD_645_07_SET_DATA,DEF_CMD_645_07_MODIFY_PWD,DEF_CMD_645_07_BROAD_GETADDR,DEF_CMD_645_07_BROAD_SWITCH,DEF_CMD_645_07_BROAD_CONTROL,DEF_CMD_645_07_SAFEATTH_DATA};
  const unsigned char ctr97[] = {DEF_CMD_645_GET_DATA,DEF_CMD_645_SET_DATA,DEF_CMD_645_MODIFY_PWD,DEF_CMD_645_BROAD_GETADDR,DEF_CMD_645_BROAD_FREEZE};

  if( bCtrCode == DEF_CMD_645_07_BROAD_SETTIME)
  {
    return DEF_DB_GY_BROADCAST_SYC_TIME;
  }

  for( i=0; i<sizeof(ctr07)/sizeof(unsigned char); i++)
  {
    if( bCtrCode == ctr07[i])
    {
      return DEF_DB_GY_07;
    }
  }

  for( i=0; i<sizeof(ctr97)/sizeof(unsigned char); i++)
  {
    if( bCtrCode == ctr97[i])
    {
      return DEF_DB_GY_645;
    }
  }

  //���򶼿�����δ֪��Լ
  return DEF_DB_GY_UNKNOW;
}


/*�����ڴ�Ƚ�
in:pdata_a--���Ƚ�����A
   pdata_b--���Ƚ�����B
   len--�Ƚ����ݳ���
out:��
return:0--�в��죬1--��ȫһ��
*/
unsigned char mem_comp(unsigned char *pdata_a,unsigned char *pdata_b,unsigned int len)
{
  unsigned int i;
  for(i = 0; i < len; i++)
  {
    if(pdata_a[i] != pdata_b[i])
      return 0;
  }
  return 1;
}
/*
���ݿ鿽��
*/
void mem_copy(unsigned char *pdst,unsigned char *psrc,unsigned int len)
{
  unsigned int i;
  for(i = 0; i < len; i++)
    pdst[i] = psrc[i];
}
/*
���ݿ�����
*/
void mem_clear(unsigned char *pdata,unsigned int len)
{
  unsigned int i;
  for(i = 0; i < len; i++)
    pdata[i] = 0;
}
/*
���ݿ�ȫ�����0xff
*/
void mem_set(unsigned char *pdata,unsigned int len)
{
  unsigned int i;
  for(i = 0; i < len; i++)
    pdata[i] = 0xff;
}

//#define CRC32_POLYNOMIAL 0x04c11db7
#define CRC32_POLYNOMIAL 0x04C11DB7

void set_crc32_reset(unsigned int *result)
{
  *result = 0xFFFFFFFF;    // init
}

void one_word_crc32(unsigned int data, unsigned int *result)
{
  unsigned int  xbit = ((unsigned int)0x80000000);
  unsigned int bits = 0;

  for (bits = 0; bits < 32; bits++) 
  {
    if (*result & 0x80000000) 
    {
      *result <<= 1;
      *result ^= CRC32_POLYNOMIAL;
    }
    else
    {
      *result <<= 1;
    }   
    if (data & xbit)
    {
      *result ^= CRC32_POLYNOMIAL;
    }
    xbit >>= 1;
  }
}

void get_crc32(unsigned char *pdata, unsigned short len, unsigned int *result)
{
  unsigned int last_len;
  unsigned int temp_len = 0;
  unsigned int data = 0;
  unsigned char last_data;
  while((temp_len+4) <= len)
  {
    data = *(unsigned int *)(pdata+temp_len);
    temp_len += 4;
    one_word_crc32(data, result);
  }
  last_len = len - temp_len;
  if(last_len == 0)
  {
    last_data = 0;    
  }else if(last_len == 1)
  {
    last_data = pdata[temp_len];
    one_word_crc32(last_data, result);
  }else if(last_len == 2)
  {
    last_data = (pdata[temp_len]<<8) + pdata[temp_len+1];
    one_word_crc32(last_data, result);
  }else if(last_len == 3)
  {
    last_data = (pdata[temp_len]<<16)+(pdata[temp_len+1]<<8)+pdata[temp_len+2];
    one_word_crc32(last_data, result);
  }else
  {
    last_len = 0;
    last_data = 0;
  }
}

