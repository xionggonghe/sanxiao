//2018.12.20

#include "sys.h"
#include "delay.h"
#include "Font.h"
#include "OLED_Screen_rule.h" //��������ļ�
#include "OLED_Screen_para.h" //��������ļ�


//��������
void OLED_IO_Init(void);                                                                                           //����IO��ʼ��
void SPI_start(void);                                                                                              //SPI��������
void SPI_stop(void);                                                                                               //SPI����ֹͣ
void SPI_write(unsigned char Byt);                                                                                 //SPI�ֽ�д
void OLED_dat(unsigned char Dat);                                                                                  //��ʾ������д
void OLED_cmd(unsigned char Cmd);                                                                                  //��ʾ������д
void OLED_locat(unsigned char Hr0, unsigned char W0);                                                              //��Ļָ�붨λ
void OLED_Fill(unsigned char Dat);                                                                                 //ȫ�����
void OLED_Init(void);                                                                                              //��ʾ�����ó�ʼ��
void FlipHorizontal(bool k);                                                                                       //ˮƽ������ʾ
void FlipVertical(bool k);                                                                                         //��ֱ������ʾ
void ColorReversal(bool k);                                                                                        //��ɫ��ʾ
void VerticalOffset(unsigned char H);                                                                              //��ֱƫ��
void DisplayStateSet(unsigned char mode);                                                                          //��ʾ״̬����
void BrightnessSet(unsigned int rank);                                                                             //��������
void DisplayPicture(bool pix, void * pic, unsigned char Hr, unsigned char W, unsigned char Hr0, unsigned char W0); //��ʾͼ��
void ScreenInit(void * buf);                                                                                       //��Ļ��ʼ��
bool pix_r(void * buf, unsigned int h, unsigned int w);                                                            //���ض�������ʽɨ�裩
void pix_w(bool pix, unsigned int h, unsigned int w);                                                              //����д������ʽɨ�裩
void DrawDot(bool pix, unsigned int H0, unsigned W0);                                                              //������
void DrawLine(bool pix, unsigned int H0, unsigned int W0, unsigned int H1, unsigned int W1);                       //���߶�
void DrawRectangle(bool pix, unsigned int H, unsigned int W, unsigned int H0, unsigned int W0);                    //������
void DrawPicture(bool pix, void * pic, unsigned int H, unsigned int W, unsigned int H0, unsigned W0);              //��ͼ��
void PutChar_H8W6(bool pix, char CH, unsigned int H0, unsigned int W0);                                            //д�ַ�
void PutChar_H16W8(bool pix, char CH, unsigned int H0, unsigned int W0);                                           //д�ַ�
void PutStr_H8W6(bool pix, char * str, unsigned int H0, unsigned int W0);                                          //д�ַ���
void PutStr_H16W8(bool pix, char * str, unsigned int H0, unsigned int W0);                                         //д�ַ���
void A_B_exc(unsigned char *A, unsigned char *B);                                                                  //����AB���ַ���ֵ
void str_inv(unsigned char str[]);                                                                                 //�ַ������ã�������'\0'���⣩
void decimal_str_0(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n);                //ʮ����С��ת�ַ�������λ��ǰ��
void decimal_str(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n);                  //ʮ����С��ת�ַ�������λ��ǰ��
long long pow1(unsigned int a, unsigned int b);                                                                    //������
void PutNum_H8W6(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);               //д����
void PutNum_H16W8(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0);              //д����
void ScreenRefresh(void);                                                                                          //��Ļˢ��


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


//�˿ں�
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7


#if(OLED_BUS_TYPE == SPI)
/*==================================== SPI���� ====================================*/

//�˿ڶ�����ʱ��ʹ��
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


//���Ŷ���
#define OLED_PIN_SCLK (1 << (0 ? OLED_SCLK))
#define OLED_PIN_MOSI (1 << (0 ? OLED_MOSI))
#define OLED_PIN_RST  (1 << (0 ? OLED_RST))
#define OLED_PIN_DC   (1 << (0 ? OLED_DC))
#define OLED_PIN_CS   (1 << (0 ? OLED_CS))


//IO����
#define OLED_SCLK_H() (OLED_GPIO_SCLK->BSRR=OLED_PIN_SCLK) //ʱ�� D0����λ��1��
#define OLED_SCLK_L() (OLED_GPIO_SCLK->BRR=OLED_PIN_SCLK)  //ʱ�� D0����λ��0��
#define OLED_MOSI_H() (OLED_GPIO_MOSI->BSRR=OLED_PIN_MOSI) //���� D1����λ��1��
#define OLED_MOSI_L() (OLED_GPIO_MOSI->BRR=OLED_PIN_MOSI)  //���� D1����λ��0��
#define OLED_RST_H()  (OLED_GPIO_RST->BSRR=OLED_PIN_RST)   //��λ����λ��1��
#define OLED_RST_L()  (OLED_GPIO_RST->BRR=OLED_PIN_RST)    //��λ����λ��0��
#define OLED_DC_H()   (OLED_GPIO_DC->BSRR=OLED_PIN_DC)     //����/�����λ��1��
#define OLED_DC_L()   (OLED_GPIO_DC->BRR=OLED_PIN_DC)      //����/�����λ��0��
#define OLED_CS_H()   (OLED_GPIO_CS->BSRR=OLED_PIN_CS)     //Ƭѡ����λ��1��
#define OLED_CS_L()   (OLED_GPIO_CS->BRR=OLED_PIN_CS)      //Ƭѡ����λ��0��


void OLED_IO_Init(void) //����IO��ʼ��
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


void SPI_start(void) //SPI��������
{
	OLED_SCLK_L(); BUS_delay();
	OLED_CS_L(); BUS_delay();
}


void SPI_stop(void) //SPI����ֹͣ
{
	OLED_CS_H(); BUS_delay();
	OLED_DC_H(); BUS_delay();
	OLED_MOSI_H(); BUS_delay();
	OLED_SCLK_H(); BUS_delay();
}


void SPI_write(unsigned char Byt) //SPI�ֽ�д
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


void OLED_dat(unsigned char Dat) //��ʾ������д
{
	SPI_start();
	OLED_DC_H();
	SPI_write(Dat);
	SPI_stop();
}


void OLED_cmd(unsigned char Cmd) //��ʾ������д
{
	SPI_start();
	OLED_DC_L();
	SPI_write(Cmd);
	SPI_stop();
}


#elif(OLED_BUS_TYPE == IIC)
/*==================================== IIC���� ====================================*/

//�˿ڶ�����ʱ��ʹ��
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


//���Ŷ���
#define OLED_PIN_SCL (1 << (0 ? OLED_SCL))
#define OLED_PIN_SDA (1 << (0 ? OLED_SDA))


#define OLED_SCL_H() (OLED_GPIO_SCL->BSRR=OLED_PIN_SCL) //ʱ�ӣ���λ��1��
#define OLED_SCL_L() (OLED_GPIO_SCL->BRR=OLED_PIN_SCL)  //ʱ�ӣ���λ��0��
#define OLED_SDA_H() (OLED_GPIO_SDA->BSRR=OLED_PIN_SDA) //���ݣ���λ��1��
#define OLED_SDA_L() (OLED_GPIO_SDA->BRR=OLED_PIN_SDA)  //���ݣ���λ��0��


void OLED_IO_Init(void) //����IO��ʼ��
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


void IIC_start(void) //IIC��������
{
	OLED_SCL_H(); BUS_delay();
	OLED_SDA_H(); BUS_delay();
	OLED_SDA_L(); BUS_delay();
	OLED_SCL_L(); BUS_delay();
}


void IIC_stop(void) //IIC����ֹͣ
{
	OLED_SCL_L(); BUS_delay();
	OLED_SDA_L(); BUS_delay();
	OLED_SCL_H(); BUS_delay();
	OLED_SDA_H(); BUS_delay();
}


void IIC_write(unsigned char Byt) //IIC�ֽ�д
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


void OLED_dat(unsigned char Dat) //��ʾ������д
{
	IIC_start();
	IIC_write(0x78);
	IIC_write(0x40);
	IIC_write(Dat);
	IIC_stop();
}


void OLED_cmd(unsigned char Cmd) //��ʾ������д
{
	IIC_start();
	IIC_write(0x78);
	IIC_write(0x00);
	IIC_write(Cmd);
	IIC_stop();
}


#endif


/*==================================== ��Ļ���� ====================================*/

void OLED_locat(unsigned char Hr0, unsigned char W0) //��Ļָ�붨λ
{
	OLED_cmd(0xB0 + Hr0);
	OLED_cmd((W0 >> 4) | 0x10);
	OLED_cmd(W0 & 0x0F); //��������㷨��ԭ��������ģ�����������OLED_cmd((W0 & 0x0F) | 0x01);
}


void OLED_Fill(unsigned char Dat) //ȫ�����
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


void OLED_Init(void) //��ʾ�����ó�ʼ��
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

	OLED_cmd(0x8D); //��ɱÿ��أ���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x10); //��ɱÿ��أ���/�أ�0x14/0x10����ɱùر��൱�����Ե�Դ�жϣ���Ļȫ�ڣ�

	OLED_cmd(0x00); //����0�е�ַ
	OLED_cmd(0x10); //����1�е�ַ
	OLED_cmd(0x40); //������ʼ��ַ

	OLED_cmd(0x81); //�������ã���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x7F); //�������ã����ƶ�ѡ������0~255��

	OLED_cmd(0xA1); //ˮƽ��ʾ���Ǿ���/����0xA1/0xA0�����Ժ���������Ч��

	OLED_cmd(0xC8); //��ֱ��ʾ���Ǿ���/����0xC8/0xC0��������Ч��

	OLED_cmd(0xA6); //����ɫ����/����0xA6/0xA7��������Ч��

	OLED_cmd(0xA8); //���û������о������ƣ���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x3F); //���û������о������ƣ�����ƫ��������0~63���Ƴ����������в���ʾ�������ֵ�̶�Ϊ63����ʱ�������ݶ��ڻ������ڣ���С��ֵ�����ɨ��������ڼ�������Ч�У������ⲿ���������������������

	OLED_cmd(0xD3); //������ʾ����ѭ�����ƣ���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x00); //������ʾ����ѭ�����ƣ�����ƫ��������0~63��������ѭ�����ƣ��Ƴ��±߽���л���ϱ߽����룬������Ч��

	OLED_cmd(0xD5); //����ʱ��Ƶ�ʼ�ɨ���źŷ�Ƶ�ȣ���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0xF0); //����ʱ��Ƶ�ʼ�ɨ���źŷ�Ƶ�ȣ���һ��ָ����趨ֵ����4λ����Ƶ�ȣ���ֵԽ���Ƶ��Խ�ߣ���4λʱ��Ƶ�ʣ���ֵԽ��Ƶ��Խ�ߣ�

	OLED_cmd(0xD9); //�������ã���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x78); //�������ã�����λѡ����ռ�ձȣ���4λ���ض�ʱ����1~15����4λ����ͨʱ����1~15��

	OLED_cmd(0xDA); //COM����Ӳ�����ã���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x12); //COM����Ӳ�����ã�����ƥ���ڲ������������֮��Ĳ�ͬ���߷�ʽ�������޸ģ�

	OLED_cmd(0xDB); //�������ã���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x20); //�������ã�����λѡ�����ѹ0.65Vcc/0.77Vcc/0.83Vcc��0x00/0x20/0x30��

	OLED_cmd(0x20); //���û�����Ѱַ��ʽ����һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x02); //���û�����Ѱַ��ʽ��ˮƽѰַ/��ֱѰַ/ҳ��Ѱַ��0x00/0x01/0x02��

	OLED_cmd(0xA4); //����״̬���ƣ���/��0xA5/0xA4�����ȼ����ں���״̬��

	OLED_cmd(0xAF); //����״̬���ƣ���/��0xAE/OxAF��

	OLED_Fill(0x00); //��0��ʾ������

	OLED_cmd(0x8D); //��ɱÿ��أ���һ��ָ��Ϊ�趨ֵ��
	OLED_cmd(0x14); //��ɱÿ��أ���/�أ�0x14/0x10����ɱùر��൱�����Ե�Դ�жϣ���Ļȫ�ڣ�

	delay_ms(5);
}


void FlipHorizontal(bool k) //ˮƽ������ʾ
{
	OLED_cmd((k) ? (0xA0) : (0xA1));
}


void FlipVertical(bool k) //��ֱ������ʾ
{
	OLED_cmd((k) ? (0xC0) : (0xC8));
}


void ColorReversal(bool k) //��ɫ��ʾ
{
	OLED_cmd((k) ? (0xA7) : (0xA6));
}


void VerticalOffset(unsigned char H) //��ֱƫ��
{
	OLED_cmd(0xD3);
	OLED_cmd(H & 0x3F);
}


void DisplayStateSet(unsigned char mode) //��ʾ״̬����
{
	switch (mode)
	{
	case 0: //����
		OLED_cmd(0xA4);
		OLED_cmd(0xAF);
		OLED_cmd(0x8D); OLED_cmd(0x14);
		break;
	case 1: //����
		OLED_cmd(0xA5);
		OLED_cmd(0xAF);
		OLED_cmd(0x8D); OLED_cmd(0x14);
		break;
	case 2: //����
		OLED_cmd(0xAE);
		OLED_cmd(0x8D); OLED_cmd(0x14);
		break;
	case 3: //����
		OLED_cmd(0x8D); OLED_cmd(0x10);
		break;
	}
}


void BrightnessSet(unsigned int rank) //��������
{
	OLED_cmd(0x81); OLED_cmd(rank & 0xFF);		                  //��������1��256����
	OLED_cmd(0xD9); OLED_cmd((rank & 0x100) ? (0xE1) : (0x1E)); //��������2����PWMʱ��15����
	OLED_cmd(0xDB); OLED_cmd((rank & 0x100) ? (0x30) : (0x00)); //��������3��3����
//ע��ʵ�ʲ����С�����1����������2ĳ����X������3����롰����1�������2ĳ����Y������3������������������Ȼ�����ͬ��
//Ϊ��֤���ڵ����Զȣ���ʹ������1��ȫ256��������2��3ͬ������Ϊ������X/����Y���/�������Ľ�2����ͬ�������ȣ���512����
}


void DisplayPicture(bool pix, void * pic, unsigned char Hr, unsigned char W, unsigned char Hr0, unsigned char W0) //��ʾͼ��
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


unsigned char(*DATABUF)[SCREEN_W]; //��Ļ��������λָ��


void ScreenInit(void * buf) //��Ļ��ʼ��
{
	DATABUF = buf;
	DrawRectangle(0, SCREEN_H, SCREEN_W, 0, 0);
	OLED_Init();
}


bool pix_r(void * buf, unsigned int h, unsigned int w) //���ض�������ʽɨ�裩
{
	return ((((unsigned char(*)[SCREEN_W])buf)[h >> 3][w] & (1 << (h & 0x00000007))) ? (1) : (0));
}


void pix_w(bool pix, unsigned int h, unsigned int w) //����д������ʽɨ�裩
{
	if (pix)
		DATABUF[h >> 3][w] |= (1 << (h & 0x00000007));
	else
		DATABUF[h >> 3][w] &= ~(1 << (h & 0x00000007));
}


void DrawDot(bool pix, unsigned int H0, unsigned W0) //������
{
	if (H0 >= SCREEN_H || W0 >= SCREEN_W)
		return;
	pix_w(pix, H0, W0);
}


void DrawLine(bool pix, unsigned int H0, unsigned int W0, unsigned int H1, unsigned int W1) //���߶�
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


void DrawRectangle(bool pix, unsigned int H, unsigned int W, unsigned int H0, unsigned int W0) //������
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


void DrawPicture(bool pix, void * pic, unsigned int H, unsigned int W, unsigned int H0, unsigned W0) //��ͼ��
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


void PutChar_H8W6(bool pix, char CH, unsigned int H0, unsigned int W0) //д�ַ�
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


void PutChar_H16W8(bool pix, char CH, unsigned int H0, unsigned int W0) //д�ַ�
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


void PutStr_H8W6(bool pix, char * str, unsigned int H0, unsigned int W0) //д�ַ���
{
	while (*str != '\0')
	{
		PutChar_H8W6(pix, *str++, H0, W0);
		W0 += 6;
	}
}


void PutStr_H16W8(bool pix, char * str, unsigned int H0, unsigned int W0) //д�ַ���
{
	while (*str != '\0')
	{
		PutChar_H16W8(pix, *str++, H0, W0);
		W0 += 8;
	}
}


void A_B_exc(unsigned char *A, unsigned char *B) //����AB���ַ���ֵ
{
	unsigned char dat = *A;
	*A = *B;
	*B = dat;
}


void str_inv(unsigned char str[]) //�ַ������ã�������'\0'���⣩
{
	unsigned char m = 0, n = 0;
	for (; str[n]; n++);
	while (n > m)
		A_B_exc(str + (m++), str + (--n));
}


//ע��Ϊ��֤Ч�ʣ�ʮ����С����������ʽ���룬�������ø�����ʽ��
//buf[] :��������ַ
//num   :���ͻ���ֵ
//sign  :�Ƿ���ʾ����
//dp    :С��������λ����* 10^(-dp)��
//n     :����С����λ����ȡ��ԭ��Ϊ���������롱��
//��������:num * 10^(-dp)������:nλС��
void decimal_str_0(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n) //ʮ����С��ת�ַ�������λ��ǰ��
{
	bool s = (num < 0) ? (num = -num, true) : (false); //�����ţ�false/true ��/����
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


void decimal_str(unsigned char buf[], long long num, bool sign, unsigned int dp, unsigned int n) //ʮ����С��ת�ַ�������λ��ǰ��
{
	decimal_str_0(buf, num, sign, dp, n);
	str_inv(buf);
}


long long pow1(unsigned int a, unsigned int b) //������
{
	long long n = 1;
	while (b--)
		n *= a;
	return n;
}


void PutNum_H8W6(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0) //д����
{
	unsigned char buf[32];
	decimal_str(buf, (long long)(num * pow1(10, n + 1)), sign, n + 1, n);
	PutStr_H8W6(pix, (void*)buf, H0, W0);
}


void PutNum_H16W8(bool pix, double num, bool sign, unsigned int n, unsigned int H0, unsigned int W0) //д����
{
	unsigned char buf[32];
	decimal_str(buf, (long long)(num * pow1(10, n + 1)), sign, n + 1, n);
	PutStr_H16W8(pix, (void*)buf, H0, W0);
}


void ScreenRefresh(void) //��Ļˢ��
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

