#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f10x.h"
//#include "Low_Rate_Check.h"
#include "bsp.h"
/* IO */
/*
IRQ ~ PE2
SDN ~ PE6
RE ~ PD3
TE ~ PA15
GDO0 ~ PB9
GDO1 ~ PB8
GDO2 ~ PB7
GOD3 ~ PB8
CS ~ PG13
CLK ~PB3
MISO ~ PB4
MOSI ~ PB5
*/
#define TE_PORT GPIOA
#define TE_PIN  GPIO_Pin_15

#define RE_PORT GPIOD
#define RE_PIN  GPIO_Pin_3

#define GDIO0_PORT GPIOB
#define GDIO0_PIN  GPIO_Pin_9
#define GDIO0_EXTI  EXTI_Line9
#define GDIO0_EXTI_IRQn EXTI9_5_IRQn

#define GDIO1_PORT GPIOB
#define GDIO1_PIN  GPIO_Pin_8
#define GDIO1_EXTI  EXTI_Line8
#define GDIO1_EXTI_IRQn EXTI9_5_IRQn

//#define IRQ_PORT GPIOA
//#define IRQ_PIN  GPIO_Pin_0
//#define IRQ_EXTI  EXTI_Line0

#define IRQ_PORT GPIOE
#define IRQ_PIN  GPIO_Pin_2
#define IRQ_EXTI  EXTI_Line2

//#define SDN_PORT GPIOA
//#define SDN_PIN  GPIO_Pin_1
//#define SDN_EXTI  EXTI_Line1

#define SDN_PORT GPIOE
#define SDN_PIN  GPIO_Pin_6
#define SDN_EXTI  EXTI_Line6


#define GDIO2_PORT GPIOB
#define GDIO2_PIN  GPIO_Pin_7

#define GDIO3_PORT GPIOB
#define GDIO3_PIN  GPIO_Pin_6




#define RF_SDI_PORT GPIOB
#define RF_SDI_PIN  GPIO_Pin_5

#define RF_SDO_PORT GPIOB
#define RF_SDO_PIN  GPIO_Pin_4

#define RF_SCLK_PORT GPIOB
#define RF_SCLK_PIN  GPIO_Pin_3

#define RF_SEL_PORT GPIOG
#define RF_SEL_PIN  GPIO_Pin_13



#define LED_TXD_PORT  GPIOG
#define LED_TXD_PIN   GPIO_Pin_10 //green led 

#define LED_RXD_PORT  GPIOG
#define LED_RXD_PIN   GPIO_Pin_11 //red led

#define TXD_LED(x) x==1 ? (LED_TXD_PORT->BSRR=LED_TXD_PIN) :(LED_TXD_PORT->BRR = LED_TXD_PIN) 
#define RXD_LED(x) x==1 ? (LED_RXD_PORT->BSRR=LED_RXD_PIN) : (LED_RXD_PORT->BRR = LED_RXD_PIN)

#define DISABLE_IRQ_GPIO_NVIC() NVIC_DisableIRQ(EXTI9_5_IRQn);NVIC_DisableIRQ(EXTI2_IRQn)
#define ENABLE_IRQ_GPIO_NVIC() NVIC_EnableIRQ(EXTI9_5_IRQn);NVIC_EnableIRQ(EXTI2_IRQn)
#define DISABLE_SDN_NVIC()  SDN_IRQ_CONFIG = 0;


void SI4438_SDN_NVIC_Configuration(void);

void SI4438_Config(void);


#endif


