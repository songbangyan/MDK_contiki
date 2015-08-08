/*!
 * File:
 *  radio_comm.h
 *
 * Description:
 *  This file contains the RADIO communication layer.
 *
 * Silicon Laboratories Confidential
 * Copyright 2012 Silicon Laboratories, Inc.
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */
#include "radio_hal.h"
#include "common.h"
#include "spi_SI4438.h"
#include "radio_comm.h"
//#include "systick.h"
#include "wireless_phy.h"


#define RADIO_USER_CFG_USE_GPIO3_FOR_CTS
                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

 BIT ctsWentHigh = 0;


                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

                /* ======================================= *
                 *     P U B L I C   F U N C T I O N S     *
                 * ======================================= */


/*!
 * Gets a command response from the radio chip
 *
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 *
 * @return CTS value
 */
U8 radio_comm_GetResp(U8 byteCount, U8* pData)
{
 U8 ctsVal = 0u;

 U32 errCnt=RADIO_CTS_TIMEOUT;

  while (errCnt != 0)      //wait until radio IC is ready with the data
  {
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(0x44);    //read CMD buffer
    radio_hal_SpiReadByte(&ctsVal);
  //DBUG("ctsVal=%x\n",ctsVal);
    if (ctsVal == 0xFF)
    {
      if (byteCount)
      {
        vSpi_ReadDataSpi0(byteCount, pData);
      }
      radio_hal_SetNsel();
      break;
    }
    radio_hal_SetNsel();
  
    errCnt--;
  }

  if (errCnt == 0)
  {
    ctsWentHigh = 0;
  
  }

  if (ctsVal == 0xFF)
  {
    ctsWentHigh = 1;
  }
  return ctsVal;
}

/*!
 * Sends a command to the radio chip
 *
 * @param byteCount     Number of bytes in the command to send to the radio device
 * @param pData         Pointer to the command to send.
 */
void radio_comm_SendCmd(U8 byteCount, U8* pData)
{
  unsigned int tick_tmp;
    /* There was a bug in A1 hardware that will not handle 1 byte commands. 
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
    if (byteCount == 1)
        byteCount++;
    tick_tmp=GET_SYS_TICK;
    while ((!radio_hal_Gpio3Level())&&(GET_SYS_TICK<(tick_tmp+TICK_100MS)));
   
    radio_hal_ClearNsel();
    vSpi_WriteDataSpi0(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip.
 * @param pData         Pointer to where to put the data.
 */
void radio_comm_ReadData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
  unsigned int tick_tmp;

    if(pollCts)
    {
       tick_tmp=GET_SYS_TICK;
      while ((!radio_hal_Gpio3Level())&&(GET_SYS_TICK<(tick_tmp+TICK_100MS)));
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    vSpi_ReadDataSpi0(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}


/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 */
void radio_comm_WriteData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
  unsigned int tick_tmp;
    if(pollCts)
    {
        tick_tmp=GET_SYS_TICK;
      while ((!radio_hal_Gpio3Level())&&(GET_SYS_TICK<(tick_tmp+TICK_100MS)));
     
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    vSpi_WriteDataSpi0(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

///*!
// * Waits for CTS to be high
// *
// * @return CTS value
// */
//U8 radio_comm_PollCTS(void)
//{
//  unsigned int tick_tmp;

//#ifdef RADIO_USER_CFG_USE_GPIO3_FOR_CTS
//  tick_tmp=GET_SYS_TICK;

//    while(!radio_hal_Gpio3Level()&&(GET_SYS_TICK<(tick_tmp+10)))
//    {
//        /* Wait...*/
//    }
//  
//    ctsWentHigh = 1;
//    return 0xFF;
//#else
//    return radio_comm_GetResp(0, 0);
//#endif
//}

/**
 * Clears the CTS state variable.
 */
void radio_comm_ClearCTS()
{
  ctsWentHigh = 0;
}


/*!
 * Sends a command to the radio chip and gets a response
 *
 * @param cmdByteCount  Number of bytes in the command to send to the radio device
 * @param pCmdData      Pointer to the command data
 * @param respByteCount Number of bytes in the response to fetch
 * @param pRespData     Pointer to where to put the response data
 *
 * @return CTS value
 */
U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData)
{
    radio_comm_SendCmd(cmdByteCount, pCmdData);
    return radio_comm_GetResp(respByteCount, pRespData);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  U8 bApi_GetFastResponseRegister(U8 bStartReg, U8 bNmbrOfRegs, U8 * pbRegValues)
  +
  + DESCRIPTION:    get fast response registers
  +
  + INPUT:          bStartReg - one of the fast response registers
  +                 bNmbrOfRegs - number of registers (1 ... 4)
  +                 pbRegValues - value of the registers
  +
  + RETURN:         0 - operation successful
  +                 1 - error
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
U8 bApi_GetFastResponseRegister(U8 bStartReg, U8 bNmbrOfRegs, U8 * pbRegValues)
{
/*
  if((bNmbrOfRegs == 0) || (bNmbrOfRegs > 4))
  {
    return 1;
  }
*/
  radio_hal_ClearNsel();
  radio_hal_SpiWriteByte(bStartReg);
  vSpi_ReadDataSpi0(bNmbrOfRegs, pbRegValues);
  radio_hal_SetNsel();
  return 0;
}


