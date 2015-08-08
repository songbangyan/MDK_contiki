#ifndef PLATFORM_CONFIGURATION_H
#define PLATFORM_CONFIGURATION_H

#include "STM32F10x.h"


#define HIGH  1
#define LOW   0

// used with UU16
#define LSB 0
#define MSB 1


typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;
typedef unsigned char bit;
typedef bit BIT;


#endif //PLATFORM_CONFIGURATION_H
