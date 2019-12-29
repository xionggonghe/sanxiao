#ifndef __OLED_H
#define __OLED_H	 

#include "stm32f10x.h"
#include "esp8266.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "sys.h"
#include "led.h"
#include "string.h" 
#include "font.h"

#define OLED_SCL(x) ((x)?GPIO_SetBits(GPIOB,GPIO_Pin_0):GPIO_ResetBits(GPIOB,GPIO_Pin_0))

#define OLED_SDA(x) ((x)?GPIO_SetBits(GPIOB,GPIO_Pin_1):GPIO_ResetBits(GPIOB,GPIO_Pin_1))


#endif