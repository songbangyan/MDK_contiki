#ifndef __COMMON_H__
#define __COMMON_H__

#include "STM32F10x.h"
#include "SI4438_Platform_configuration.h"
//#include "config.h"
#include "stdio.h"

#if 0
  #define DPUTC(ch)    dputc(ch)
  #define DBUG(format,arg...) printf(format, ##arg)
#else
  #define DPUTC(ch)
  #define DBUG(format, arg...)
#endif

#define DEF_SYS_STICK_COLOCK_1MS

#ifdef DEF_SYS_STICK_COLOCK_1MS

#define TICK_10MS 10
#define TICK_20MS 20
#define TICK_50MS 50
#define TICK_100MS 100
#define TICK_500MS 500
#define TICK_1S 1000
#define TICK_2S 2000
#define TICK_4S 4000
#endif

#ifdef DEF_SYS_STICK_COLOCK_10MS
#define TICK_10MS 1
#define TICK_20MS 2
#define TICK_50MS 5
#define TICK_100MS 10
#define TICK_500MS 50
#define TICK_1S 100
#define TICK_2S 200

#endif


#define GET_SYS_TICK    get_systick()
//-------------------------------------------------------------------------------------------------------

// Data
typedef uint8_t       BYTE;
typedef uint16_t      WORD;
typedef uint32_t       DWORD;

// Unsigned numbers
typedef uint8_t       UINT8;
typedef uint16_t      UINT16;
typedef uint32_t       UINT32;

// Signed numbers
typedef int8_t         INT8;
typedef int16_t        INT16;
typedef int32_t         INT32;
//-------------------------------------------------------------------------------------------------------
#ifndef ENABLE_BIT_DEFINITIONS
#define ENABLE_BIT_DEFINITIONS
#endif


//type define:
#define    BYTE   unsigned char
#define    UINT8  unsigned char
typedef unsigned char       BOOL;




/*
#define P_SI1           {DDRC |= 0x20; PORTC |= 0x20;}
#define P_SI0           {DDRC |= 0x20; PORTC &= ~0x20;}
#define P_SCLK1   {DDRC |= 0x10; PORTC |= 0x10;}
#define P_SCLK0   {DDRC |= 0x10; PORTC &= ~0x10;}
#define P_SO            (PINC & 0x08)
#define P_GDO2    (PINC & 0x04)
#define P_GDO0    (PINC & 0x02)
#define P_CSn1    {DDRC |= 0x01; PORTC |= 0x01;}
#define P_CSn0    {DDRC |= 0x01; PORTC &= ~0x01;}
*/


//-------------------------------------------------------------------------------------------------------
// Common values
#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef NULL
    #define NULL 0
#endif
#ifndef SERIAL_CLK_TIMER_OUT
    #define SERIAL_CLK_TIMER_OUT 2
#endif


//-------------------------------------------------------------------------------------------------------




#endif// COMMON_H
