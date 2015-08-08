/*!
 * File:
 *  si446x_api_lib.h
 *
 * Description:
 *  This file contains the Si446x API library.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

#ifndef _SI446X_API_LIB_H_
#define _SI446X_API_LIB_H_
#include "SI4438_Platform_configuration.h"

//extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union, SEG_XDATA );
extern union si446x_cmd_reply_union Si446xCmd;
extern  U8  Pro2Cmd[16];

//extern SEGMENT_VARIABLE( Pro2Cmd[16], U8, SEG_XDATA );


#define SI466X_FIFO_SIZE 64

enum
{
    SI446X_SUCCESS,
    SI446X_NO_PATCH,
    SI446X_CTS_TIMEOUT,
    SI446X_PATCH_FAIL,
    SI446X_COMMAND_ERROR
};

/* Minimal driver support functions */
void si446x_reset(void);
void si446x_power_up(U8 BOOT_OPTIONS, U8 XTAL_OPTIONS, U32 XO_FREQ);

U8 si446x_configuration_init(const U8* pSetPropCmd);
U8 si446x_apply_patch(void);
void si446x_part_info(void);
/*函数功能：设置SI4438发射功率
入口参数：level:设置发送功率参数，0~0x7f*/
void si446x_set_power(U8 level);
void si446x_start_tx(U8 CHANNEL, U8 CONDITION, U16 TX_LEN);
void si446x_start_rx(U8 CHANNEL, U8 CONDITION, U16 RX_LEN, U8 NEXT_STATE1, U8 NEXT_STATE2, U8 NEXT_STATE3);

U8 si446x_get_int_status(U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND);

void si446x_change_state(U8 NEXT_STATE1);


  void si446x_fifo_info(U8 FIFO);
  void si446x_write_tx_fifo( U8 numBytes, U8* pData );
  void si446x_read_rx_fifo( U8 numBytes, U8* pRxData );

  void si446x_get_modem_status( U8 MODEM_CLR_PEND );


    void si446x_request_device_state(void);

    void si446x_rx_hop(U8 INTE, U8 FRAC2, U8 FRAC1, U8 FRAC0, U8 VCO_CNT1, U8 VCO_CNT0);
    void si446x_agc_override(U8 AGC_OVERRIDE);



#endif //_SI446X_API_LIB_H_
