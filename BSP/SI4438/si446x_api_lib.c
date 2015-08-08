/*!
 * File:
 *  si446x_api_lib.c
 *
 * Description:
 *  This file contains the Si446x API library.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */
#include "radio_comm.h"
#include "si446x_api_lib.h"
#include "si446x_defs.h"
#include "si446x_patch.h"
#include "radio_hal.h"
#include "SI4438_config.h"
//#include "systick.h"
#include "common.h"
#include "wireless_phy.h"

#include <stdarg.h>

//SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union, SEG_XDATA );
union si446x_cmd_reply_union Si446xCmd;

//SEGMENT_VARIABLE( Pro2Cmd[16], U8, SEG_XDATA );
U8 Pro2Cmd[16];

#ifdef SI446X_PATCH_CMDS
//SEGMENT_VARIABLE( Si446xPatchCommands[][8] = { SI446X_PATCH_CMDS }, U8, SEG_CODE);
const U8 Si446xPatchCommands[][8] = { SI446X_PATCH_CMDS };

#endif


/*!
 * This functions is used to reset the si446x radio by applying shutdown and
 * releasing it.  After this function @ref si446x_boot should be called.  You
 * can check if POR has completed by waiting 4 ms or by polling GPIO 0, 2, or 3.
 * When these GPIOs are high, it is safe to call @ref si446x_boot.
 */
void si446x_reset(void)
{
  volatile  unsigned int tick_tmp;
 
  tick_tmp=GET_SYS_TICK;
  while(GET_SYS_TICK<(tick_tmp+TICK_10MS));

    /* Put radio in shutdown, wait then release */
    GPIO_WriteBit(SDN_PORT, SDN_PIN, Bit_SET);
    tick_tmp=GET_SYS_TICK;
  while(GET_SYS_TICK<(tick_tmp+TICK_10MS));
    GPIO_WriteBit(SDN_PORT, SDN_PIN, Bit_RESET);
  SI4438_SDN_NVIC_Configuration();
  EXTI_ClearITPendingBit(SDN_EXTI);
    tick_tmp=GET_SYS_TICK;
  while(GET_SYS_TICK<(tick_tmp+TICK_10MS));
    radio_comm_ClearCTS();
}

/*!
 * This function is used to initialize after power-up the radio chip.
 * Before this function @si446x_reset should be called.
 */
void si446x_power_up(U8 BOOT_OPTIONS, U8 XTAL_OPTIONS, U32 XO_FREQ)
{
    Pro2Cmd[0] = SI446X_CMD_ID_POWER_UP;
    Pro2Cmd[1] = BOOT_OPTIONS;
    Pro2Cmd[2] = XTAL_OPTIONS;
    Pro2Cmd[3] = (U8)(XO_FREQ >> 24);
    Pro2Cmd[4] = (U8)(XO_FREQ >> 16);
    Pro2Cmd[5] = (U8)(XO_FREQ >> 8);
    Pro2Cmd[6] = (U8)(XO_FREQ);

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_POWER_UP, Pro2Cmd,0,0 );
}

/*!
 * This function is used to load all properties and commands with a list of NULL terminated commands.
 * Before this function @si446x_reset should be called.
 */
U8 si446x_configuration_init(const U8* pSetPropCmd)
{
//  SEGMENT_VARIABLE(col, U8, SEG_DATA);
  U8 col;

 // SEGMENT_VARIABLE(numOfBytes, U8, SEG_DATA);
 U8 numOfBytes;

  /* While cycle as far as the pointer points to a command */
  while (*pSetPropCmd != 0x00)
  {
    /* Commands structure in the array:
     * --------------------------------
     * LEN | <LEN length of data>
     */

    numOfBytes = *pSetPropCmd++;

    if (numOfBytes > 16u)
    {
      /* Number of command bytes exceeds maximal allowable length */
      return SI446X_COMMAND_ERROR;
    }

    for (col = 0u; col < numOfBytes; col++)
    {
      Pro2Cmd[col] = *pSetPropCmd;
      pSetPropCmd++;
    }

    if (radio_comm_SendCmdGetResp(numOfBytes, Pro2Cmd, 0, 0) != 0xFF)
    {
      /* Timeout occured */
      return SI446X_CTS_TIMEOUT;
    }

    if (radio_hal_NirqLevel() == 0)
    {
      /* Get and clear all interrupts.  An error has occured... */
      si446x_get_int_status(0, 0, 0);
    
      if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
      {
        return SI446X_COMMAND_ERROR;
      }
    }
  }

  return SI446X_SUCCESS;
}

void si446x_part_info(void)
{
    Pro2Cmd[0] = SI446X_CMD_ID_PART_INFO;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PART_INFO,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_PART_INFO,
                              Pro2Cmd );

    Si446xCmd.PART_INFO.CHIPREV         = Pro2Cmd[0];
    Si446xCmd.PART_INFO.PART.U8[MSB]    = Pro2Cmd[1];
    Si446xCmd.PART_INFO.PART.U8[LSB]    = Pro2Cmd[2];
    Si446xCmd.PART_INFO.PBUILD          = Pro2Cmd[3];
    Si446xCmd.PART_INFO.ID.U8[MSB]      = Pro2Cmd[4];
    Si446xCmd.PART_INFO.ID.U8[LSB]      = Pro2Cmd[5];
    Si446xCmd.PART_INFO.CUSTOMER        = Pro2Cmd[6];
    Si446xCmd.PART_INFO.ROMID           = Pro2Cmd[7];
}
/*!
 * This function is used to apply a firmware patch to the si446x radio.  This
 * patch is stored in code using the si446x_patch.h file.
 *
 * @return  SI446X_CTS_TIMEOUT If a CTS error occurs.
 *          SI446X_PATCH_FAIL If the patch fails.
 *          SI446X_SUCCESS If the patch is successful.
 *          SI446X_NO_PATCH If there is no patch in the Flash to load.
 */
U8 si446x_apply_patch(void)
{
#ifdef SI446X_PATCH_CMDS
    U16 line;
    U8 row;

    /* Check if patch is needed. */
  
    si446x_part_info();

    if ((Si446xCmd.PART_INFO.ROMID == SI446X_PATCH_ROMID) && (Si446xCmd.PART_INFO.ID.U8[MSB] < SI446X_PATCH_ID))
    {
      for (line = 0; line < (sizeof(Si446xPatchCommands) / 8u); line++)
      {
        for (row=0; row<8; row++)
        {
          Pro2Cmd[row] = Si446xPatchCommands[line][row];
        }
        if (radio_comm_SendCmdGetResp(8, Pro2Cmd, 0, 0) != 0xFF)
        {
          // Timeout occured
          return SI446X_CTS_TIMEOUT;
        }
        if (radio_hal_NirqLevel() == 0)
        {
          /* Get and clear all interrupts.  An error has occured... */
          si446x_get_int_status(0, 0, 0);
          return SI446X_PATCH_FAIL;
        }
      }
    }
//  DBUG("SI446X_SUCCESS\r\n");

    return SI446X_SUCCESS;
#else
    return SI446X_NO_PATCH;
#endif
}

void si446x_set_power(U8 level)
{
    unsigned char i=0;
    if(level>0x7f)
    {
      level=0x7f;
    }
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP);
    si4438_phy.RF_status=RF_IDLE_FLAG;
    Pro2Cmd[i++] = 0x11;
    Pro2Cmd[i++] = 0x22;
    Pro2Cmd[i++] = 0x05;
    Pro2Cmd[i++] = 0x00;
    Pro2Cmd[i++] = 0x88;
    Pro2Cmd[i++] = level;
    Pro2Cmd[i++] = 0x00;
    Pro2Cmd[i++] = 0x3F;
    Pro2Cmd[i++] = 0xFF;
    radio_comm_SendCmdGetResp(0x09, Pro2Cmd, 0, 0);
}


/*! Sends START_TX command to the radio.
 *
 * @param CHANNEL   Channel number.
 * @param CONDITION Start TX condition.
 * @param TX_LEN    Payload length (exclude the PH generated CRC).
 */
void si446x_start_tx(U8 CHANNEL, U8 CONDITION, U16 TX_LEN)
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_TX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (U8)(TX_LEN >> 8);
    Pro2Cmd[4] = (U8)(TX_LEN);
    Pro2Cmd[5] = 0x00;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_TX, Pro2Cmd );
}

/*!
 * Sends START_RX command to the radio.
 *
 * @param CHANNEL     Channel number.
 * @param CONDITION   Start RX condition.
 * @param RX_LEN      Payload length (exclude the PH generated CRC).
 * @param NEXT_STATE1 Next state when Preamble Timeout occurs.
 * @param NEXT_STATE2 Next state when a valid packet received.
 * @param NEXT_STATE3 Next state when invalid packet received (e.g. CRC error).
 */
void si446x_start_rx(U8 CHANNEL, U8 CONDITION, U16 RX_LEN, U8 NEXT_STATE1, U8 NEXT_STATE2, U8 NEXT_STATE3)
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_RX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (U8)(RX_LEN >> 8);
    Pro2Cmd[4] = (U8)(RX_LEN);
    Pro2Cmd[5] = NEXT_STATE1;
    Pro2Cmd[6] = NEXT_STATE2;
    Pro2Cmd[7] = NEXT_STATE3;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_RX, Pro2Cmd );
}

/*!
 * Get the Interrupt status/pending flags form the radio and clear flags if requested.
 *
 * @param PH_CLR_PEND     Packet Handler pending flags clear.
 * @param MODEM_CLR_PEND  Modem Status pending flags clear.
 * @param CHIP_CLR_PEND   Chip State pending flags clear.
 */
U8 si446x_get_int_status(U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND)
{

    Pro2Cmd[0] = SI446X_CMD_ID_GET_INT_STATUS;
    Pro2Cmd[1] = PH_CLR_PEND;
    Pro2Cmd[2] = MODEM_CLR_PEND;
    Pro2Cmd[3] = CHIP_CLR_PEND;
  if(radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_INT_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_INT_STATUS,
                              Pro2Cmd )==0xff)
  {

    Si446xCmd.GET_INT_STATUS.INT_PEND       = Pro2Cmd[0];
    Si446xCmd.GET_INT_STATUS.INT_STATUS     = Pro2Cmd[1];
    Si446xCmd.GET_INT_STATUS.PH_PEND        = Pro2Cmd[2];
    Si446xCmd.GET_INT_STATUS.PH_STATUS      = Pro2Cmd[3];
    Si446xCmd.GET_INT_STATUS.MODEM_PEND     = Pro2Cmd[4];
    Si446xCmd.GET_INT_STATUS.MODEM_STATUS   = Pro2Cmd[5];
    Si446xCmd.GET_INT_STATUS.CHIP_PEND      = Pro2Cmd[6];
    Si446xCmd.GET_INT_STATUS.CHIP_STATUS    = Pro2Cmd[7];

  //for(i=0;i<8;i++)
  //{
  //  DBUG("%02x ",Pro2Cmd[i]);
  //}
    return TRUE;
  }
  else{
    return FALSE;
  }
}


/*!
 * Send SET_PROPERTY command to the radio.
 *
 * @param GROUP       Property group.
 * @param NUM_PROPS   Number of property to be set. The properties must be in ascending order
 *                    in their sub-property aspect. Max. 12 properties can be set in one command.
 * @param START_PROP  Start sub-property address.
 */

/*!
 * Issue a change state command to the radio.
 *
 * @param NEXT_STATE1 Next state.
 */
void si446x_change_state(U8 NEXT_STATE1)
{
    Pro2Cmd[0] = SI446X_CMD_ID_CHANGE_STATE;
    Pro2Cmd[1] = NEXT_STATE1;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_CHANGE_STATE, Pro2Cmd );
}


/*!
 * Requests the current state of the device and lists pending TX and RX requests
 */
void si446x_request_device_state(void)
{
    Pro2Cmd[0] = SI446X_CMD_ID_REQUEST_DEVICE_STATE;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_REQUEST_DEVICE_STATE,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE,
                              Pro2Cmd );

    Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE       = Pro2Cmd[0];
    Si446xCmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL  = Pro2Cmd[1];
}

void si446x_get_modem_status( U8 MODEM_CLR_PEND )
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_MODEM_STATUS;
    Pro2Cmd[1] = MODEM_CLR_PEND;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_MODEM_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_MODEM_STATUS,
                              Pro2Cmd );

    Si446xCmd.GET_MODEM_STATUS.MODEM_PEND   = Pro2Cmd[0];
    Si446xCmd.GET_MODEM_STATUS.MODEM_STATUS = Pro2Cmd[1];
    Si446xCmd.GET_MODEM_STATUS.CURR_RSSI    = Pro2Cmd[2];
    Si446xCmd.GET_MODEM_STATUS.LATCH_RSSI   = Pro2Cmd[3];
   // Si446xCmd.GET_MODEM_STATUS.ANT1_RSSI    = Pro2Cmd[4];
   // Si446xCmd.GET_MODEM_STATUS.ANT2_RSSI    = Pro2Cmd[5];
   // Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET.U8[MSB]  = Pro2Cmd[6];
   // Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET.U8[LSB]  = Pro2Cmd[7];
}


void si446x_fifo_info(U8 FIFO)
{
    Pro2Cmd[0] = SI446X_CMD_ID_FIFO_INFO;
    Pro2Cmd[1] = FIFO;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_FIFO_INFO,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_FIFO_INFO,
                              Pro2Cmd );

    Si446xCmd.FIFO_INFO.RX_FIFO_COUNT   = Pro2Cmd[0];
    Si446xCmd.FIFO_INFO.TX_FIFO_SPACE   = Pro2Cmd[1];
}

/*!
 * The function can be used to load data into TX FIFO.
 *
 * @param numBytes  Data length to be load.
 * @param pTxData   Pointer to the data (U8*).
 */
void si446x_write_tx_fifo(U8 numBytes, U8* pTxData)
{
  radio_comm_WriteData( SI446X_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
}

/*!
 * Reads the RX FIFO content from the radio.
 *
 * @param numBytes  Data length to be read.
 * @param pRxData   Pointer to the buffer location.
 */
void si446x_read_rx_fifo(U8 numBytes, U8* pRxData)
{
  radio_comm_ReadData( SI446X_CMD_ID_READ_RX_FIFO, 0, numBytes, pRxData );
}


