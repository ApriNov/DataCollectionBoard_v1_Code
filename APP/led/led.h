#ifndef _led_H
#define _led_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define LED_PORT 			GPIOA   
#define LED_PIN 			(GPIO_Pin_9|GPIO_Pin_10)
#define LED_PORT_RCC		RCC_APB2Periph_GPIOA


#define led1 PAout(9)
#define led2 PAout(10)


void LED_Init(void);


#endif
