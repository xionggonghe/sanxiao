//2018.12.20

#include "sys.h"
#include "delay.h"
#include "Font.h"
#include "OLED_Screen_rule.h" //编译规则文件
#include "OLED_Screen_para.h" //编译参数文件


//函数声明
void OLED_IO_Init(void);                                                                                           //驱动IO初始化
void SPI_start(void);                                                                                              //SPI总线启动
void SPI_stop(void);                                                                                               //SPI总线停止
void SPI_write(unsigned char Byt);                                                                                 //SPI字节写
void OLED_dat(unsigned char Dat);                                                                                  //显示器数据写
void OLED_cmd(unsigned char Cmd);                                                                                  //显示器命令写
void OLED_locat(unsigned char Hr0, unsigned char W0);                                                              //屏幕指针定位
void OLED_Fill(unsigned char Dat);                                                                                 //全屏填充
void OLED_Init(void);                                                                                              //显示器配置初始化
void FlipHorizontal(bool k);                                                                                       //水平镜像显示
void FlipVertical(bool k);                                                                                         //竖直镜像显示
void ColorReversal(bool k);                                                                                        //反色显示
void VerticalOffset(unsigned char H);                                                                              //竖直偏移
void DisplayStateSet(unsigned char mode);                                                                          //显示状态设置
void BrightnessSet(unsigned int rank);                                                                             //亮度设置
void DisplayPicture(bool pix, void * pic, unsigned char Hr, unsigned char W, unsigned char Hr0, unsigned char W0); //显示图像
void ScreenInit(void * buf);                                                                                       //屏幕初始化
bool pix_r(void * buf, unsigned int h, unsigned int w);                                                            //像素读（列行式扫描）
void pix_w(bool pix, unsigned int h, unsigned int w);                                                              //像素写（列行式扫描）
void DrawDot(bool pix, unsigned int H0, unsigned W0);                                                              //画单点
void DrawLine(bool pix, unsigned int H0, unsigned int W0, unsigned int H1, unsigned int W1);                       //画线段
void DrawRectangle(bool pix, unsigned int H, unsigned int W, unsigned int H0, unsigned int W0);                    //画矩形
void DrawPicture(bool pix, void * pic, unsigned int H, unsigned int W, unsigned int H0, unsigned W0);              //画图像
void PutChar_H8W6(bool pix, char CH, unsigned int H0, unsigned int W0);                                            //写字符
void PutChar_H16W8(bool pix, char CH, unsigned int H0, unsigned int W0);                                           //写字符
void PutStr_H8W6(bool pix, char * str, unsigned int H0, unsigned int W0);                                          //写字符串
void PutStr_H16W8(bool pix, char * str, unsigned int H0, unsigned int W0);                                         //写字符串
void A_B_exc(unsigned char *A, unsigned char *B);                                                                  //交换AB两字符的值
void str_inv(unsigned char str[]);                                                                                 //字符串倒置（结束符'\0'除外）
void decimal_str_0(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n);                //十进制小数转字符串（低位在前）
void decimal_str(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n);                  //十进制小数转字符串（高位在前）
long long pow1(unsigned int a, unsigned int b);                                                                    //幂运算
void PutNum_H8W6(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);               //写数字
void PutNum_H16W8(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);              //写数字
void ScreenRefresh(void);                                                                                          //屏幕刷新


#if (OLED_BUS_DELAY == 0)
#define BUS_delay()
#elif (OLED_BUS_DELAY > 0)
void BUS_delay(void)
{
	unsigned int i;
	for (i = OLED_BUS_DELAY - 1; i > 0; i--);
}
#endif


#define SPI 'A'
#define IIC 'B'


//端口号
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7


#if(OLED_BUS_TYPE == SPI)
/*==================================== SPI总线 ====================================*/

//端口定义与时钟使能
#if ((1 ? OLED_SCLK) == PA)
#define ENRCC_A
#define OLED_GPIO_SCLK GPIOA
#elif ((1 ? OLED_SCLK) == PB)
#define ENRCC_B
#define OLED_GPIO_SCLK GPIOB
#elif ((1 ? OLED_SCLK) == PC)
#define ENRCC_C
#define OLED_GPIO_SCLK GPIOC
#elif ((1 ? OLED_SCLK) == PD)
#define ENRCC_D
#define OLED_GPIO_SCLK GPIOD
#elif ((1 ? OLED_SCLK) == PE)
#define ENRCC_E
#define OLED_GPIO_SCLK GPIOE
#elif ((1 ? OLED_SCLK) == PF)
#define ENRCC_F
#define OLED_GPIO_SCLK GPIOF
#elif ((1 ? OLED_SCLK) == PG)
#define ENRCC_G
#define OLED_GPIO_SCLK GPIOG
#endif

#if ((1 ? OLED_MOSI) == PA)
#define ENRCC_A
#define OLED_GPIO_MOSI GPIOA
#elif ((1 ? OLED_MOSI) == PB)
#define ENRCC_B
#define OLED_GPIO_MOSI GPIOB
#elif ((1 ? OLED_MOSI) == PC)
#define ENRCC_C
#define OLED_GPIO_MOSI GPIOC
#elif ((1 ? OLED_MOSI) == PD)
#define ENRCC_D
#define OLED_GPIO_MOSI GPIOD
#elif ((1 ? OLED_MOSI) == PE)
#define ENRCC_E
#define OLED_GPIO_MOSI GPIOE
#elif ((1 ? OLED_MOSI) == PF)
#define ENRCC_F
#define OLED_GPIO_MOSI GPIOF
#elif ((1 ? OLED_MOSI) == PG)
#define ENRCC_G
#define OLED_GPIO_MOSI GPIOG
#endif

#if ((1 ? OLED_RST) == PA)
#define ENRCC_A
#define OLED_GPIO_RST GPIOA
#elif ((1 ? OLED_RST) == PB)
#define ENRCC_B
#define OLED_GPIO_RST GPIOB
#elif ((1 ? OLED_RST) == PC)
#define ENRCC_C
#define OLED_GPIO_RST GPIOC
#elif ((1 ? OLED_RST) == PD)
#define ENRCC_D
#define OLED_GPIO_RST GPIOD
#elif ((1 ? OLED_RST) == PE)
#define ENRCC_E
#define OLED_GPIO_RST GPIOE
#elif ((1 ? OLED_RST) == PF)
#define ENRCC_F
#define OLED_GPIO_RST GPIOF
#elif ((1 ? OLED_RST) == PG)
#define ENRCC_G
#define OLED_GPIO_RST GPIOG
#endif

#if ((1 ? OLED_DC) == PA)
#define ENRCC_A
#define OLED_GPIO_DC GPIOA
#elif ((1 ? OLED_DC) == PB)
#define ENRCC_B
#define OLED_GPIO_DC GPIOB
#elif ((1 ? OLED_DC) == PC)
#define ENRCC_C
#define OLED_GPIO_DC GPIOC
#elif ((1 ? OLED_DC) == PD)
#define ENRCC_D
#define OLED_GPIO_DC GPIOD
#elif ((1 ? OLED_DC) == PE)
#define ENRCC_E
#define OLED_GPIO_DC GPIOE
#elif ((1 ? OLED_DC) == PF)
#define ENRCC_F
#define OLED_GPIO_DC GPIOF
#elif ((1 ? OLED_DC) == PG)
#define ENRCC_G
#define OLED_GPIO_DC GPIOG
#endif

#if ((1 ? OLED_CS) == PA)
#define ENRCC_A
#define OLED_GPIO_CS GPIOA
#elif ((1 ? OLED_CS) == PB)
#define ENRCC_B
#define OLED_GPIO_CS GPIOB
#elif ((1 ? OLED_CS) == PC)
#define ENRCC_C
#define OLED_GPIO_CS GPIOC
#elif ((1 ? OLED_CS) == PD)
#define ENRCC_D
#define OLED_GPIO_CS GPIOD
#elif ((1 ? OLED_CS) == PE)
#define ENRCC_E
#define OLED_GPIO_CS GPIOE
#elif ((1 ? OLED_CS) == PF)
#define ENRCC_F
#define OLED_GPIO_CS GPIOF
#elif ((1 ? OLED_CS) == PG)
#define ENRCC_G
#define OLED_GPIO_CS GPIOG
#endif


//引脚定义
#define OLED_PIN_SCLK (1 << (0 ? OLED_SCLK))
#define OLED_PIN_MOSI (1 << (0 ? OLED_MOSI))
#define OLED_PIN_RST  (1 << (0 ? OLED_RST))
#define OLED_PIN_DC   (1 << (0 ? OLED_DC))
#define OLED_PIN_CS   (1 << (0 ? OLED_CS))


//IO操作
#define OLED_SCLK_H() (OLED_GPIO_SCLK->BSRR=OLED_PIN_SCLK) //时钟 D0，置位（1）
#define OLED_SCLK_L() (OLED_GPIO_SCLK->BRR=OLED_PIN_SCLK)  //时钟 D0，复位（0）
#define OLED_MOSI_H() (OLED_GPIO_MOSI->BSRR=OLED_PIN_MOSI) //数据 D1，置位（1）
#define OLED_MOSI_L() (OLED_GPIO_MOSI->BRR=OLED_PIN_MOSI)  //数据 D1，复位（0）
#define OLED_RST_H()  (OLED_GPIO_RST->BSRR=OLED_PIN_RST)   //复位，置位（1）
#define OLED_RST_L()  (OLED_GPIO_RST->BRR=OLED_PIN_RST)    //复位，复位（0）
#define OLED_DC_H()   (OLED_GPIO_DC->BSRR=OLED_PIN_DC)     //数据/命令，置位（1）
#define OLED_DC_L()   (OLED_GPIO_DC->BRR=OLED_PIN_DC)      //数据/命令，复位（0）
#define OLED_CS_H()   (OLED_GPIO_CS->BSRR=OLED_PIN_CS)     //片选，置位（1）
#define OLED_CS_L()   (OLED_GPIO_CS->BRR=OLED_PIN_CS)      //片选，复位（0）


void OLED_IO_Init(void) //驱动IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

#ifdef ENRCC_A
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PA) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PA) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PA) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PA) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PA) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PA;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_B
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PB) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PB) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PB) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PB) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PB) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PB;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_C
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PC) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PC) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PC) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PC) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PC) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PC;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_D
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PD) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PD) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PD) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PD) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PD) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PD;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_E
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PE) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PE) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PE) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PE) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PE) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PE;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_F
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PF) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PF) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PF) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PF) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PF) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PF;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_G
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCLK) == PG) ? (OLED_PIN_SCLK) : (0))
		|
		(((1 ? OLED_MOSI) == PG) ? (OLED_PIN_MOSI) : (0))
		|
		(((1 ? OLED_RST) == PG) ? (OLED_PIN_RST) : (0))
		|
		(((1 ? OLED_DC) == PG) ? (OLED_PIN_DC) : (0))
		|
		(((1 ? OLED_CS) == PG) ? (OLED_PIN_CS) : (0))
		);
	RCC->APB2ENR |= 2 << PG;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_InitStructure.GPIO_Pin);
#endif
}


void SPI_start(void) //SPI总线启动
{
	OLED_SCLK_L(); BUS_delay();
	OLED_CS_L(); BUS_delay();
}


void SPI_stop(void) //SPI总线停止
{
	OLED_CS_H(); BUS_delay();
	OLED_DC_H(); BUS_delay();
	OLED_MOSI_H(); BUS_delay();
	OLED_SCLK_H(); BUS_delay();
}


void SPI_write(unsigned char Byt) //SPI字节写
{
	(Byt & 0x80) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x40) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x20) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x10) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x08) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x04) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x02) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
	(Byt & 0x01) ? (OLED_MOSI_H()) : (OLED_MOSI_L()); BUS_delay(); OLED_SCLK_H(); BUS_delay(); OLED_SCLK_L(); BUS_delay();
}


void OLED_dat(unsigned char Dat) //显示器数据写
{
	SPI_start();
	OLED_DC_H();
	SPI_write(Dat);
	SPI_stop();
}


void OLED_cmd(unsigned char Cmd) //显示器命令写
{
	SPI_start();
	OLED_DC_L();
	SPI_write(Cmd);
	SPI_stop();
}


#elif(OLED_BUS_TYPE == IIC)
/*==================================== IIC总线 ====================================*/

//端口定义与时钟使能
#if ((1 ? OLED_SCL) == PA)
#define ENRCC_A
#define OLED_GPIO_SCL GPIOA
#elif ((1 ? OLED_SCL) == PB)
#define ENRCC_B
#define OLED_GPIO_SCL GPIOB
#elif ((1 ? OLED_SCL) == PC)
#define ENRCC_C
#define OLED_GPIO_SCL GPIOC
#elif ((1 ? OLED_SCL) == PD)
#define ENRCC_D
#define OLED_GPIO_SCL GPIOD
#elif ((1 ? OLED_SCL) == PE)
#define ENRCC_E
#define OLED_GPIO_SCL GPIOE
#elif ((1 ? OLED_SCL) == PF)
#define ENRCC_F
#define OLED_GPIO_SCL GPIOF
#elif ((1 ? OLED_SCL) == PG)
#define ENRCC_G
#define OLED_GPIO_SCL GPIOG
#endif

#if ((1 ? OLED_SDA) == PA)
#define ENRCC_A
#define OLED_GPIO_SDA GPIOA
#elif ((1 ? OLED_SDA) == PB)
#define ENRCC_B
#define OLED_GPIO_SDA GPIOB
#elif ((1 ? OLED_SDA) == PC)
#define ENRCC_C
#define OLED_GPIO_SDA GPIOC
#elif ((1 ? OLED_SDA) == PD)
#define ENRCC_D
#define OLED_GPIO_SDA GPIOD
#elif ((1 ? OLED_SDA) == PE)
#define ENRCC_E
#define OLED_GPIO_SDA GPIOE
#elif ((1 ? OLED_SDA) == PF)
#define ENRCC_F
#define OLED_GPIO_SDA GPIOF
#elif ((1 ? OLED_SDA) == PG)
#define ENRCC_G
#define OLED_GPIO_SDA GPIOG
#endif


//引脚定义
#define OLED_PIN_SCL (1 << (0 ? OLED_SCL))
#define OLED_PIN_SDA (1 << (0 ? OLED_SDA))


#define OLED_SCL_H() (OLED_GPIO_SCL->BSRR=OLED_PIN_SCL) //时钟，置位（1）
#define OLED_SCL_L() (OLED_GPIO_SCL->BRR=OLED_PIN_SCL)  //时钟，复位（0）
#define OLED_SDA_H() (OLED_GPIO_SDA->BSRR=OLED_PIN_SDA) //数据，置位（1）
#define OLED_SDA_L() (OLED_GPIO_SDA->BRR=OLED_PIN_SDA)  //数据，复位（0）


void OLED_IO_Init(void) //驱动IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

#ifdef ENRCC_A
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PA) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PA) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PA;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_B
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PB) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PB) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PB;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_C
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PC) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PC) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PC;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_D
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PD) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PD) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PD;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_E
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PE) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PE) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PE;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_F
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PF) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PF) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PF;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_InitStructure.GPIO_Pin);
#endif

#ifdef ENRCC_G
	GPIO_InitStructure.GPIO_Pin = (
		(((1 ? OLED_SCL) == PG) ? (OLED_PIN_SCL) : (0))
		|
		(((1 ? OLED_SDA) == PG) ? (OLED_PIN_SDA) : (0))
		);
	RCC->APB2ENR |= 2 << PG;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_InitStructure.GPIO_Pin);
#endif
}


void IIC_start(void) //IIC总线启动
{
	OLED_SCL_H(); BUS_delay();
	OLED_SDA_H(); BUS_delay();
	OLED_SDA_L(); BUS_delay();
	OLED_SCL_L(); BUS_delay();
}


void IIC_stop(void) //IIC总线停止
{
	OLED_SCL_L(); BUS_delay();
	OLED_SDA_L(); BUS_delay();
	OLED_SCL_H(); BUS_delay();
	OLED_SDA_H(); BUS_delay();
}


void IIC_write(unsigned char Byt) //IIC字节写
{
	(Byt & 0x80) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x40) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x20) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x10) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x08) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x04) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x02) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	(Byt & 0x01) ? (OLED_SDA_H()) : (OLED_SDA_L()); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
	OLED_SDA_H(); BUS_delay(); OLED_SCL_H(); BUS_delay(); OLED_SCL_L(); BUS_delay();
}


void OLED_dat(unsigned char Dat) //显示器数据写
{
	IIC_start();
	IIC_write(0x78);
	IIC_write(0x40);
	IIC_write(Dat);
	IIC_stop();
}


void OLED_cmd(unsigned char Cmd) //显示器命令写
{
	IIC_start();
	IIC_write(0x78);
	IIC_write(0x00);
	IIC_write(Cmd);
	IIC_stop();
}


#endif


/*==================================== 屏幕控制 ====================================*/

void OLED_locat(unsigned char Hr0, unsigned char W0) //屏幕指针定位
{
	OLED_cmd(0xB0 + Hr0);
	OLED_cmd((W0 >> 4) | 0x10);
	OLED_cmd(W0 & 0x0F); //后面这个算法是原本例程里的，但可能有误：OLED_cmd((W0 & 0x0F) | 0x01);
}


void OLED_Fill(unsigned char Dat) //全屏填充
{
	unsigned char Hr, W;
	for (Hr = 0; Hr < ((SCREEN_H + 7) >> 3); Hr++)
	{
		OLED_cmd(0xB0 + Hr);
		OLED_cmd(0x10);
		OLED_cmd(0x00);
		for (W = 0; W < SCREEN_W; W++)
			OLED_dat(Dat);
	}
}


void OLED_Init(void) //显示器配置初始化
{
	OLED_IO_Init();
	delay_ms(5);

#if(OLED_BUS_TYPE == SPI)
	OLED_RST_L(); BUS_delay();
	OLED_RST_H(); BUS_delay();
	OLED_RST_L(); BUS_delay();
	OLED_RST_H(); BUS_delay();
	delay_ms(5);
#endif

	OLED_cmd(0x8D); //电荷泵开关（下一条指令为设定值）
	OLED_cmd(0x10); //电荷泵开关（开/关：0x14/0x10，电荷泵关闭相当于屏显电源切断，屏幕全黑）

	OLED_cmd(0x00); //设置0列地址
	OLED_cmd(0x10); //设置1列地址
	OLED_cmd(0x40); //设置起始地址

	OLED_cmd(0x81); //亮度设置（下一条指令为设定值）
	OLED_cmd(0x7F); //亮度设置（控制段选电流：0~255）

	OLED_cmd(0xA1); //水平显示（非镜像/镜像：0xA1/0xA0，仅对后续数据有效）

	OLED_cmd(0xC8); //竖直显示（非镜像/镜像：0xC8/0xC0，立即生效）

	OLED_cmd(0xA6); //正反色（正/反：0xA6/0xA7，立即生效）

	OLED_cmd(0xA8); //设置缓存区行绝对上移（下一条指令为设定值）
	OLED_cmd(0x3F); //设置缓存区行绝对上移（向上偏移行数：0~63，移出缓存区的行不显示，建议此值固定为63，此时所有数据都在缓存区内，减小此值会造成扫描电流过于集中在有效行，导致这部分区域过亮，缩短寿命）

	OLED_cmd(0xD3); //设置显示域行循环下移（下一条指令为设定值）
	OLED_cmd(0x00); //设置显示域行循环下移（向下偏移行数：0~63，由于是循环下移，移出下边界的行会从上边界移入，立即生效）

	OLED_cmd(0xD5); //设置时钟频率及扫描信号分频比（下一条指令为设定值）
	OLED_cmd(0xF0); //设置时钟频率及扫描信号分频比（上一条指令的设定值，低4位：分频比，数值越大分频比越高；高4位时钟频率，数值越大频率越高）

	OLED_cmd(0xD9); //亮度设置（下一条指令为设定值）
	OLED_cmd(0x78); //亮度设置（控制位选供电占空比，低4位：关断时钟数1~15；高4位：开通时钟数1~15）

	OLED_cmd(0xDA); //COM引脚硬件配置（下一条指令为设定值）
	OLED_cmd(0x12); //COM引脚硬件配置（用于匹配内部点阵与控制器之间的不同接线方式，无需修改）

	OLED_cmd(0xDB); //亮度设置（下一条指令为设定值）
	OLED_cmd(0x20); //亮度设置（控制位选供电电压0.65Vcc/0.77Vcc/0.83Vcc：0x00/0x20/0x30）

	OLED_cmd(0x20); //设置缓存区寻址方式（下一条指令为设定值）
	OLED_cmd(0x02); //设置缓存区寻址方式（水平寻址/竖直寻址/页面寻址：0x00/0x01/0x02）

	OLED_cmd(0xA4); //白屏状态控制（是/否：0xA5/0xA4，优先级低于黑屏状态）

	OLED_cmd(0xAF); //黑屏状态控制（是/否：0xAE/OxAF）

	OLED_Fill(0x00); //清0显示缓存区

	OLED_cmd(0x8D); //电荷泵开关（下一条指令为设定值）
	OLED_cmd(0x14); //电荷泵开关（开/关：0x14/0x10，电荷泵关闭相当于屏显电源切断，屏幕全黑）

	delay_ms(5);
}


void FlipHorizontal(bool k) //水平镜像显示
{
	OLED_cmd((k) ? (0xA0) : (0xA1));
}


void FlipVertical(bool k) //竖直镜像显示
{
	OLED_cmd((k) ? (0xC0) : (0xC8));
}


void ColorReversal(bool k) //反色显示
{
	OLED_cmd((k) ? (0xA7) : (0xA6));
}


void VerticalOffset(unsigned char H) //竖直偏移
{
	OLED_cmd(0xD3);
	OLED_cmd(H & 0x3F);
}


void DisplayStateSet(unsigned char mode) //显示状态设置
{
	switch (mode)
	{
	case 0: //正常
		OLED_cmd(0xA4);
		OLED_cmd(0xAF);
		OLED_cmd(0x8D); OLED_cmd(0x14);
		break;
	case 1: //白屏
		OLED_cmd(0xA5);
		OLED_cmd(0xAF);
		OLED_cmd(0x8D); OLED_cmd(0x14);
		break;
	case 2: //黑屏
		OLED_cmd(0xAE);
		OLED_cmd(0x8D); OLED_cmd(0x14);
		break;
	case 3: //休眠
		OLED_cmd(0x8D); OLED_cmd(0x10);
		break;
	}
}


void BrightnessSet(unsigned int rank) //亮度设置
{
	OLED_cmd(0x81); OLED_cmd(rank & 0xFF);		                  //亮度设置1（256级）
	OLED_cmd(0xD9); OLED_cmd((rank & 0x100) ? (0xE1) : (0x1E)); //亮度设置2（仅PWM时有15级）
	OLED_cmd(0xDB); OLED_cmd((rank & 0x100) ? (0x30) : (0x00)); //亮度设置3（3级）
//注：实际测试中“设置1最亮，设置2某亮度X，设置3最暗”与“设置1最暗，设置2某亮度Y，设置3最亮”两种情况的亮度基本相同，
//为保证调节的线性度，故使用设置1的全256级与设置2、3同步调整为“亮度X/亮度Y、最暗/最亮”的仅2级共同控制亮度，共512级。
}


void DisplayPicture(bool pix, void * pic, unsigned char Hr, unsigned char W, unsigned char Hr0, unsigned char W0) //显示图像
{
	unsigned char M;
	Hr += Hr0;
	if (Hr > ((SCREEN_H + 7) >> 3) || (W0 + W) > SCREEN_W)
		return;
	for (Hr += Hr0; Hr0 < Hr; Hr0++)
	{
		OLED_locat(Hr0, W0);
		for (M = 0; M < W; M++, pic = (unsigned char *)pic + 1)
			OLED_dat((pix) ? (*(unsigned char *)pic) : (~*(unsigned char *)pic));
	}
}


unsigned char(*DATABUF)[SCREEN_W]; //屏幕缓存区定位指针


void ScreenInit(void * buf) //屏幕初始化
{
	DATABUF = buf;
	DrawRectangle(0, SCREEN_H, SCREEN_W, 0, 0);
	OLED_Init();
}


bool pix_r(void * buf, unsigned int h, unsigned int w) //像素读（列行式扫描）
{
	return ((((unsigned char(*)[SCREEN_W])buf)[h >> 3][w] & (1 << (h & 0x00000007))) ? (1) : (0));
}


void pix_w(bool pix, unsigned int h, unsigned int w) //像素写（列行式扫描）
{
	if (pix)
		DATABUF[h >> 3][w] |= (1 << (h & 0x00000007));
	else
		DATABUF[h >> 3][w] &= ~(1 << (h & 0x00000007));
}


void DrawDot(bool pix, unsigned int H0, unsigned W0) //画单点
{
	if (H0 >= SCREEN_H || W0 >= SCREEN_W)
		return;
	pix_w(pix, H0, W0);
}


void DrawLine(bool pix, unsigned int H0, unsigned int W0, unsigned int H1, unsigned int W1) //画线段
{
	double h, w;
	double d;
	if (H0 >= SCREEN_H || W0 >= SCREEN_W || H1 >= SCREEN_H || W1 >= SCREEN_W)
		return;
	if (((W1 < W0) ? (W0 - W1) : (W1 - W0)) >((H1 < H0) ? (H0 - H1) : (H1 - H0)))
	{
		d = (double)((int)H1 - (int)H0) / ((int)W1 - (int)W0);
		if (W1 > W0)
		{
			for (w = W0, h = H0; w <= W1; w++, h += d)
				pix_w(pix, (unsigned int)h, (unsigned int)w);
		}
		else
		{
			for (w = W0, h = H0; w >= W1; w--, h -= d)
				pix_w(pix, (unsigned int)h, (unsigned int)w);
		}
	}
	else
	{
		d = (double)((int)W1 - (int)W0) / ((int)H1 - (int)H0);
		if (H1 > H0)
		{
			for (w = W0, h = H0; h <= H1; h++, w += d)
				pix_w(pix, (unsigned int)h, (unsigned int)w);
		}
		else
		{
			for (w = W0, h = H0; h >= H1; h--, w -= d)
				pix_w(pix, (unsigned int)h, (unsigned int)w);
		}
	}
	pix_w(pix, H1, W1);
}


void DrawRectangle(bool pix, unsigned int H, unsigned int W, unsigned int H0, unsigned int W0) //画矩形
{
	unsigned int h, w;
	H += H0;
	W += W0;
	if (H > SCREEN_H || W > SCREEN_W)
		return;
	for (h = H0; h < H; h++)
		for (w = W0; w < W; w++)
			pix_w(pix, h, w);
}


void DrawPicture(bool pix, void * pic, unsigned int H, unsigned int W, unsigned int H0, unsigned W0) //画图像
{
	unsigned int h, w;
	H += H0;
	W += W0;
	if (H > SCREEN_H || W > SCREEN_W)
		return;
	for (h = H0; h < H; h++)
		for (w = W0; w < W; w++)
		{
			pix_w((pix_r(pic, h, w) == pix), h, w);
		}
}


void PutChar_H8W6(bool pix, char CH, unsigned int H0, unsigned int W0) //写字符
{
	unsigned char h, w;
	if (CH < 32 || CH>127)
		CH = 127;
	for (h = 0; h < 8; h++)
		for (w = 0; w < 6; w++)
		{
			if ((ASCII_H8W6[CH][w] >> h) & 0x01)
				pix_w(pix, H0 + h, W0 + w);
			else pix_w(!pix, H0 + h, W0 + w);
		}
}


void PutChar_H16W8(bool pix, char CH, unsigned int H0, unsigned int W0) //写字符
{
	unsigned char h, w;
	if (CH < 32 || CH > 127)
		CH = 127;
	for (h = 0; h < 8; h++)
		for (w = 0; w < 8; w++)
		{
			if ((ASCII_H16W8[CH][w] >> h) & 0x01)
				pix_w(pix, H0 + h, W0 + w);
			else pix_w(!pix, H0 + h, W0 + w);
			if ((ASCII_H16W8[CH][w + 8] >> h) & 0x01)
				pix_w(pix, H0 + h + 8, W0 + w);
			else pix_w(!pix, H0 + h + 8, W0 + w);
		}
}


void PutStr_H8W6(bool pix, char * str, unsigned int H0, unsigned int W0) //写字符串
{
	while (*str != '\0')
	{
		PutChar_H8W6(pix, *str++, H0, W0);
		W0 += 6;
	}
}


void PutStr_H16W8(bool pix, char * str, unsigned int H0, unsigned int W0) //写字符串
{
	while (*str != '\0')
	{
		PutChar_H16W8(pix, *str++, H0, W0);
		W0 += 8;
	}
}


void A_B_exc(unsigned char *A, unsigned char *B) //交换AB两字符的值
{
	unsigned char dat = *A;
	*A = *B;
	*B = dat;
}


void str_inv(unsigned char str[]) //字符串倒置（结束符'\0'除外）
{
	unsigned char m = 0, n = 0;
	for (; str[n]; n++);
	while (n > m)
		A_B_exc(str + (m++), str + (--n));
}


//注：为保证效率，十进制小数以特殊形式输入，而不采用浮点形式。
//buf[] :输出缓存地址
//num   :整型基础值
//sign  :是否显示正号
//dp    :小数点左移位数（* 10^(-dp)）
//n     :保留小数的位数（取舍原则为“四舍五入”）
//输出结果即:num * 10^(-dp)，精度:n位小数
void decimal_str_0(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n) //十进制小数转字符串（低位在前）
{
	bool s = (num < 0) ? (num = -num, true) : (false); //正负号（false/true 正/负）
	if (dp < n)
	{
		n -= dp;
		while (n--)
			*buf++ = '0';
		while (dp--)
		{
			*buf++ = num % 10 + '0';
			num /= 10;
		}
		*buf++ = '.';
	}
	else
	{
		dp -= n;
		if (dp)
		{
			while (dp-- > 1)
				num /= 10;
			num = (num + 5) / 10;
		}
		if (n)
		{
			while (n--)
			{
				*buf++ = num % 10 + '0';
				num /= 10;
			}
			*buf++ = '.';
		}
	}
	do
	{
		*buf++ = num % 10 + '0';
		num /= 10;
	} while (num);
	if (s == true)
		*buf++ = '-';
	else if (sign == true)
		*buf++ = '+';
	*buf = 0;
}


void decimal_str(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n) //十进制小数转字符串（高位在前）
{
	decimal_str_0(buf, num, sign, dp, n);
	str_inv(buf);
}


long long pow1(unsigned int a, unsigned int b) //幂运算
{
	long long n = 1;
	while (b--)
		n *= a;
	return n;
}


void PutNum_H8W6(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0) //写数字
{
	unsigned char buf[32];
	decimal_str(buf, (long long)(num * pow1(10, n + 1)), sign, n + 1, n);
	PutStr_H8W6(pix, (void*)buf, H0, W0);
}


void PutNum_H16W8(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0) //写数字
{
	unsigned char buf[32];
	decimal_str(buf, (long long)(num * pow1(10, n + 1)), sign, n + 1, n);
	PutStr_H16W8(pix, (void*)buf, H0, W0);
}


void ScreenRefresh(void) //屏幕刷新
{
	unsigned char Hr, W;
	for (Hr = 0; Hr < ((SCREEN_H + 7) >> 3); Hr++)
	{
		OLED_cmd(0xB0 + Hr);
		OLED_cmd(0x10);
		OLED_cmd(0x00);
		for (W = 0; W < SCREEN_W; W++)
			OLED_dat(DATABUF[Hr][W]);
	}
}

