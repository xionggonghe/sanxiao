//2018.12.8

#ifndef __OLED_Screen_para_h_
#define __OLED_Screen_para_h_


#define OLED_BUS_TYPE  (IIC) //�������ͣ�SPI/IIC��
#define OLED_BUS_DELAY (4)   //������ʱ������ͨ�����ʣ�һ��IIC��Ӧ�ٶȵ���SPI����Ҫ��������ʱ��


#define OLED_SCLK  PA:0 //SPIʱ�����Ŷ��壨D0������������ѡΪSPIʱ��Ч��
#define OLED_MOSI  PA:1 //SPI�������Ŷ��壨D1������������ѡΪSPIʱ��Ч��
#define OLED_RST   PA:2 //SPI��λ���Ŷ��壨����������ѡΪSPIʱ��Ч��
#define OLED_DC    PA:3 //SPI����/�������Ŷ��壨����������ѡΪSPIʱ��Ч��
#define OLED_CS    PA:4 //SPIƬѡ���Ŷ��壨����������ѡΪSPIʱ��Ч��


#define OLED_SCL  PB:0 //IICʱ�����Ŷ��壨����������ѡΪIICʱ��Ч��
#define OLED_SDA  PB:1 //IIC�������Ŷ��壨����������ѡΪIICʱ��Ч��


#define SCREEN_H (64)  //��Ļ�߶ȣ����أ�
#define SCREEN_W (128) //��Ļ��ȣ����أ�


#endif
