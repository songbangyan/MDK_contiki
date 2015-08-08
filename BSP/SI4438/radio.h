/*! @file radio.h
 * @brief This file is contains the public radio interface functions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_H_
#define RADIO_H_
#include "SI4438_Platform_configuration.h"

/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
/*! Maximal packet length definition (FIFO size) */
#define MAX_PACKET_LENGTH     64-2-4
#define RADIO_MAX_PACKET_LENGTH     64
#define RADIO_RX_ALMOST_FULL_THRESHOLD 40u
#define RADIO_TX_ALMOST_EMPTY_THRESHOLD 30u

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    const U8   *Radio_ConfigurationArray;

} tRadioConfiguration;

/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
  extern const  tRadioConfiguration  *pRadioConfiguration;
  
  
  /*! Si446x configuration array */
  //extern const SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE);
  extern const  U8 Radio_Configuration_Data_Array[];


/*! Si446x configuration array */

/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
extern volatile U8 set_pchannel;
void PA_Rxen(void);
void PA_Txen(void);
void  vRadio_Init(void);
void set_txd_mode(void);
void  vRadio_StartRX(U8);
BIT gRadio_CheckReceived_VariablePacket(void);
U8 bRadio_GetCurrentChannel(void);
void  vRadio_StartTx_Variable_Packet(U8, U8*, U8);
U8 set_hop_table(U8 pchannel);
void sync_word_int(void);
void Clear_int_status(void);
unsigned char get_property(void);
unsigned char get_all_property(void);
unsigned char Rf_set(void);
U8 set_property(const U8* pSetPropCmd,U8 length);

#endif /* RADIO_H_ */
