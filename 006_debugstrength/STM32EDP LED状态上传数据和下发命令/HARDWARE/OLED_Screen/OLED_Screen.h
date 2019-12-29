//2018.12.8

#ifndef __OLED_Screen_h_
#define __OLED_Screen_h_

#include "OLED_Screen_rule.h"
//#include "OLED_Screen_para.h"


extern void ScreenInit      (void * buf);                                                                                   //��Ļ��ʼ�������ⲿ�ṩһ��ͼ�񻺴���buf�����С����Ϊ��Ļ��(��*(��+7)/8)���ֽڣ�
extern void FlipHorizontal  (bool k);                                                                                       //ˮƽ������ʾ��k�����Ƿ���0/1������/���񣬽��Ժ���������Ч��
extern void FlipVertical    (bool k);                                                                                       //��ֱ������ʾ��k�����Ƿ���0/1������/����������Ч��
extern void ColorReversal   (bool k);                                                                                       //��ɫ��ʾ��k�����Ƿ�ɫ��0/1������/��ɫ��������Ч��
extern void VerticalOffset  (unsigned char H);                                                                              //��ֱƫ�ƣ�H����ͼ����ֱѭ������������ȡ0~63��������Ч��
extern void DisplayStateSet (unsigned char mode);                                                                           //��ʾ״̬���ã�mode������ʾ״̬��0/1/2/3������/����/����/���ߣ�������Ч��
extern void BrightnessSet   (unsigned int rank);                                                                            //�������ã�rankԽ������Խ�ߣ�ȡ0~511����512����ע�ⲻͬ����0�����Ȳ��ϴ󣬽����������ò�Ҫ��̫�ӽ�0��ֵ��ʼ��������Ч��
extern void DrawDot         (bool pix, unsigned int H0, unsigned W0);                                                       //�����㣨�����㻭��������H0,W0λ�ã�pix��������ɫ��
extern void DrawLine        (bool pix, unsigned int H0, unsigned int W0, unsigned int H1, unsigned int W1);                 //���߶Σ����߶λ���������H0,W0��H1,W1����֮�䣬pix��������ɫ��
extern void DrawRectangle   (bool pix, unsigned int H, unsigned int W, unsigned int H0, unsigned int W0);                   //�����Σ�����H��W�ľ��λ���������H0,W0λ�ã��󶥵㶨λ����pix��������ɫ��
extern void DrawPicture     (bool pix, void * pic, unsigned int H, unsigned int W, unsigned int H0, unsigned W0);           //��ͼ�񣨽���H��W��ͼƬpic����������H0,W0λ�ã��󶥵㶨λ����pix��������ɫ��
extern void PutChar_H8W6    (bool pix, char CH, unsigned int H0, unsigned int W0);                                          //д�ַ���6*8 ASCII�ַ��������������Ӧλ�ã�pix��������ɫ��
extern void PutChar_H16W8   (bool pix, char CH, unsigned int H0, unsigned int W0);                                          //д�ַ���8*16 ASCII�ַ��������������Ӧλ�ã�pix��������ɫ��
extern void PutStr_H8W6     (bool pix, char * str, unsigned int H0, unsigned int W0);                                       //д�ַ�����6*8 ASCII�ַ����������������Ӧλ�ã�pix��������ɫ��
extern void PutStr_H16W8    (bool pix, char * str, unsigned int H0, unsigned int W0);                                       //д�ַ�����8*16 ASCII�ַ����������������Ӧλ�ã�pix��������ɫ��
extern void PutNum_H8W6     (bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);            //д���֣�6*8 ʵ���������������Ӧλ�ã�pix��������ɫ��ע��������Чλ�������п��ܵ����쳣��
extern void PutNum_H16W8    (bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);            //д���֣�8*16 ʵ���������������Ӧλ�ã�pix��������ɫ��ע��������Чλ�������п��ܵ����쳣��
extern void ScreenRefresh   (void);                                                                                         //��Ļˢ�£�����ǰ�������ڵ�����ͼ������������ʾ����
extern void DisplayPicture  (bool pix, void * pic, unsigned char Hr, unsigned char W, unsigned char Hr0, unsigned char W0); //��ʾͼ�񣨽���Hr��W��ͼƬpic��ʾ����Ļ��ֱ����ʾ����������������Hr0,W0λ�ã��󶥵㶨λ����pix��������ɫ��ע��Hr��Hr0�ĵ�λ�ǡ��С���1��=8���أ�


#endif
