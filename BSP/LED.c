
#include "LED.h"




#define NUM_LEDS 4//(sizeof(Pin_LED)/sizeof(GPIO_PIN_ID))


/*-----------------------------------------------------------------------------
 *      LED_Initialize:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void BSP_LED_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

  /* Configure PD0 and PD2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  LED_Off(LED_POW);
  LED_Off(LED_RXD);
  LED_Off(LED_TXD);
  LED_Off(LED_NET); 
}


/*-----------------------------------------------------------------------------
 *      LED_Uninitialize:  Uninitialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Uninitialize (void) 
{ 
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) 
{  
  switch (num)
  {
    case LED_POW:
      GPIO_ResetBits(GPIOG, GPIO_Pin_9);
      break;
    case LED_RXD:
      GPIO_ResetBits(GPIOG, GPIO_Pin_10);
      break;
    case LED_TXD:
      GPIO_ResetBits(GPIOG, GPIO_Pin_11);
      break;    
    case LED_NET:
      GPIO_ResetBits(GPIOG, GPIO_Pin_12);
      break;    
    default:
      break;
  }  
}

/*-----------------------------------------------------------------------------
 *      LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) 
{
  switch (num)
  {
    case LED_POW:
      GPIO_SetBits(GPIOG, GPIO_Pin_9);
      break;
    case LED_RXD:
      GPIO_SetBits(GPIOG, GPIO_Pin_10);
      break;
    case LED_TXD:
      GPIO_SetBits(GPIOG, GPIO_Pin_11);
      break;    
    case LED_NET:
      GPIO_SetBits(GPIOG, GPIO_Pin_12);
      break;    
    default:
      break;
  }
}

void LED_Toggle (uint32_t num) 
{
  switch (num)
  {
    case LED_POW:
      GPIO_WriteBit(GPIOG, GPIO_Pin_9, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_9)));
      break;
    case LED_RXD:
      GPIO_WriteBit(GPIOG, GPIO_Pin_10, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_10)));
      break;
    case LED_TXD:
      GPIO_WriteBit(GPIOG, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_11)));
      break;    
    case LED_NET:
      GPIO_WriteBit(GPIOG, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_12)));
      break;    
    default:
      break;
  }
}
/*-----------------------------------------------------------------------------
 *      LED_Out: Write value to LEDs
 *
 * Parameters:  val - value to be displayed on LEDs
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Out (uint32_t val) 
{
}

/*-----------------------------------------------------------------------------
 *      LED_Num: Get number of available LEDs
 *
 * Parameters: (none)
 * Return:      number of available LEDs
 *----------------------------------------------------------------------------*/
uint32_t LED_Num (void) {
  return (NUM_LEDS);
}
