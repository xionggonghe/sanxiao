//2018.12.8

#ifndef __OLED_Screen_h_
#define __OLED_Screen_h_

#include "OLED_Screen_rule.h"
//#include "OLED_Screen_para.h"


extern void ScreenInit      (void * buf);                                                                                   //屏幕初始化（需外部提供一个图像缓存区buf，其大小至少为屏幕的(宽*(高+7)/8)个字节）
extern void FlipHorizontal  (bool k);                                                                                       //水平镜像显示（k控制是否镜像，0/1：正常/镜像，仅对后续数据有效）
extern void FlipVertical    (bool k);                                                                                       //竖直镜像显示（k控制是否镜像，0/1：正常/镜像，立即生效）
extern void ColorReversal   (bool k);                                                                                       //反色显示（k控制是否反色，0/1：正常/反色，立即生效）
extern void VerticalOffset  (unsigned char H);                                                                              //竖直偏移（H控制图像竖直循环下移行数，取0~63，立即生效）
extern void DisplayStateSet (unsigned char mode);                                                                           //显示状态设置（mode控制显示状态，0/1/2/3：正常/白屏/黑屏/休眠，立即生效）
extern void BrightnessSet   (unsigned int rank);                                                                            //亮度设置（rank越大亮度越高，取0~511，共512级，注意不同屏的0点亮度差别较大，建议亮度设置不要从太接近0的值开始，立即生效）
extern void DrawDot         (bool pix, unsigned int H0, unsigned W0);                                                       //画单点（将单点画到缓存区H0,W0位置，pix控制正反色）
extern void DrawLine        (bool pix, unsigned int H0, unsigned int W0, unsigned int H1, unsigned int W1);                 //画线段（将线段画到缓存区H0,W0与H1,W1两点之间，pix控制正反色）
extern void DrawRectangle   (bool pix, unsigned int H, unsigned int W, unsigned int H0, unsigned int W0);                   //画矩形（将高H宽W的矩形画到缓存区H0,W0位置（左顶点定位），pix控制正反色）
extern void DrawPicture     (bool pix, void * pic, unsigned int H, unsigned int W, unsigned int H0, unsigned W0);           //画图像（将高H宽W的图片pic画到缓存区H0,W0位置（左顶点定位），pix控制正反色）
extern void PutChar_H8W6    (bool pix, char CH, unsigned int H0, unsigned int W0);                                          //写字符（6*8 ASCII字符输出到缓存区相应位置，pix控制正反色）
extern void PutChar_H16W8   (bool pix, char CH, unsigned int H0, unsigned int W0);                                          //写字符（8*16 ASCII字符输出到缓存区相应位置，pix控制正反色）
extern void PutStr_H8W6     (bool pix, char * str, unsigned int H0, unsigned int W0);                                       //写字符串（6*8 ASCII字符串输出到缓存区相应位置，pix控制正反色）
extern void PutStr_H16W8    (bool pix, char * str, unsigned int H0, unsigned int W0);                                       //写字符串（8*16 ASCII字符串输出到缓存区相应位置，pix控制正反色）
extern void PutNum_H8W6     (bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);            //写数字（6*8 实数输出到缓存区相应位置，pix控制正反色；注：数字有效位数过长有可能导致异常）
extern void PutNum_H16W8    (bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);            //写数字（8*16 实数输出到缓存区相应位置，pix控制正反色；注：数字有效位数过长有可能导致异常）
extern void ScreenRefresh   (void);                                                                                         //屏幕刷新（将当前缓存区内的整副图像数据送入显示器）
extern void DisplayPicture  (bool pix, void * pic, unsigned char Hr, unsigned char W, unsigned char Hr0, unsigned char W0); //显示图像（将高Hr宽W的图片pic显示在屏幕（直接显示，不经过缓存区）Hr0,W0位置（左顶点定位），pix控制正反色；注：Hr与Hr0的单位是“行”，1行=8像素）


#endif
