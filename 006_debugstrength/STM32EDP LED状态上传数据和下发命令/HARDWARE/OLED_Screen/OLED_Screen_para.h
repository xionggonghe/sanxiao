//2018.12.8

#ifndef __OLED_Screen_para_h_
#define __OLED_Screen_para_h_


#define OLED_BUS_TYPE  (IIC) //总线类型（SPI/IIC）
#define OLED_BUS_DELAY (4)   //总线延时（控制通信速率；一般IIC响应速度低于SPI，需要更长的延时）


#define OLED_SCLK  PA:0 //SPI时钟引脚定义（D0，仅总线类型选为SPI时有效）
#define OLED_MOSI  PA:1 //SPI数据引脚定义（D1，仅总线类型选为SPI时有效）
#define OLED_RST   PA:2 //SPI复位引脚定义（仅总线类型选为SPI时有效）
#define OLED_DC    PA:3 //SPI数据/命令引脚定义（仅总线类型选为SPI时有效）
#define OLED_CS    PA:4 //SPI片选引脚定义（仅总线类型选为SPI时有效）


#define OLED_SCL  PB:0 //IIC时钟引脚定义（仅总线类型选为IIC时有效）
#define OLED_SDA  PB:1 //IIC数据引脚定义（仅总线类型选为IIC时有效）


#define SCREEN_H (64)  //屏幕高度（像素）
#define SCREEN_W (128) //屏幕宽度（像素）


#endif
