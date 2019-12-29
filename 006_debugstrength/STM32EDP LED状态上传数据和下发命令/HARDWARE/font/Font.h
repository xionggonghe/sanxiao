#ifndef __FONT_H
#define __FONT_H	 

#include "stm32f10x.h"
#include "esp8266.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "sys.h"
#include "led.h"
#include "string.h" 


//字库文件

/*=================== 6*8像素点阵声明（ASC字符库） ===================*/

extern unsigned char W6_H8[][6];


/*=================== 8*16像素点阵声明（ASC字符库） ===================*/

extern unsigned char W8_H16[][16];


/*=================== 16*16像素点阵声明（汉字/自定义图形库） ===================*/

extern unsigned char W16_H16[][32];


#endif
