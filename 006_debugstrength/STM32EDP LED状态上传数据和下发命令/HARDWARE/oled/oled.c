#include<oled.h>


#ifdef IIC
void IIC_start		(void);					//IIC 开总线
void IIC_stop		(void);					//IIC 关总线
void IIC_write		(unsigned char Byt);			//IIC 字节写
#endif
void OLED_dat		(unsigned char Dat);			//OLED 命令写
void OLED_cmd		(unsigned char Cmd);			//OLED 数据写
void OLED_locat		(unsigned char X,unsigned char Yr);	//OLED 屏幕指针定位


/************************************************************************/

/*=================== IIC时序 ===================*/

void IIC_start(void)
{
	OLED_SCL(1);
	OLED_SDA(1);
	OLED_SDA(0);
	OLED_SCL(0);
}


void IIC_stop(void)
{
	OLED_SCL(0);
	OLED_SDA(0);
	OLED_SCL(1);
	OLED_SDA(1);
}


void IIC_write(unsigned char Byt)
{
unsigned char i,Bdat=0;
for(i=8;i;i--)
{
	Bdat=(Byt&&0x80);
    Byt <<= 1;
	OLED_SCL(Bdat);
	OLED_SCL(1);
	OLED_SCL(0);
}
	OLED_SDA(1);
	OLED_SCL(1);
	OLED_SCL(0);
}


/************************************************************************/


/*=================== OLED协议2（IIC模式） ===================*/

void OLED_dat(unsigned char Dat)
{
IIC_start();
IIC_write(0x78);
IIC_write(0x40);
IIC_write(Dat);
IIC_stop();
}


void OLED_cmd(unsigned char Cmd)
{
IIC_start();
IIC_write(0x78);
IIC_write(0x00);
IIC_write(Cmd);
IIC_stop();
}

/*=================== OLED工具函数 ===================*/

void OLED_locat(unsigned char X,unsigned char Yr)
{ 
OLED_cmd(0xB0+Yr);
OLED_cmd((X >> 4) | 0x10);
OLED_cmd((X & 0x0F )| 0x01);
}


/*=================== OLED应用函数 ===================*/

void OLED_Fill(unsigned char Dat)
{
unsigned char x,y;
for(y=0xB0;y<0xB8;y++)
{
OLED_cmd(y);
OLED_cmd(0x01);
OLED_cmd(0x10);
for(x=128;x;x--)
OLED_dat(Dat);
}
}


void OLED_Init(void)
{

OLED_cmd(0xAE);		//--turn off oled panel
OLED_cmd(0x00);		//---set 0 column address
OLED_cmd(0x10);		//---set 1 column address
OLED_cmd(0x40);		//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
OLED_cmd(0x81);		//--set contrast control register
OLED_cmd(0xCF);		// Set SEG Output Current Brightness
OLED_cmd(0xA1);		//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
OLED_cmd(0xC8);		//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
OLED_cmd(0xA6);		//--set normal display
OLED_cmd(0xA8);		//--set multiplex ratio(1 to 64)
OLED_cmd(0x3F);		//--1/64 duty
OLED_cmd(0xD3);		//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
OLED_cmd(0x00);		//-not offset
OLED_cmd(0xD5);		//--set display clock divide ratio/oscillator frequency
OLED_cmd(0x80);		//--set divide ratio, Set Clock as 100 Frames/Sec
OLED_cmd(0xD9);		//--set pre-charge period
OLED_cmd(0xF1);		//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
OLED_cmd(0xDA);		//--set com pins hardware configuration
OLED_cmd(0x12);		//
OLED_cmd(0xDB);		//--set vcomh
OLED_cmd(0x40);		//Set VCOM Deselect Level
OLED_cmd(0x20);		//-Set Page Addressing Mode (0x00/0x01/0x02)
OLED_cmd(0x02);		//
OLED_cmd(0x8D);		//--set Charge Pump enable/disable
OLED_cmd(0x14);		//--set(0x10) disable
OLED_cmd(0xA4);		// Disable Entire Display On (0xa4/0xa5)
OLED_cmd(0xA6);		// Disable Inverse Display On (0xa6/a7) 
OLED_cmd(0xAF);		//--turn on oled panel
OLED_Fill(0x00);
//OLED_locat(0,0);
} 


void OLED_Printf_W6_H8(unsigned char X,unsigned char Yr,unsigned char Str[])
{
unsigned char x,y;
while(*Str)
{
y=*Str++-32;
if(X>126)
{
X=0;
Yr++;
}
OLED_locat(X,Yr);
X+=6;
for(x=0;x<6;x++)
//if(R)
OLED_dat(W6_H8[y][x]);
//else
//OLED_dat(~W6_H8[y][x]);
}
}


void OLED_Printf_W8_H16(unsigned char X,unsigned char Yr,unsigned char Str[])
{
unsigned char x,y;
while(*Str)
{
y=*Str++-32;
if(X>120)
{
X=0;
Yr++;
}
OLED_locat(X,Yr);
for(x=0;x<8;x++)
//if(R)
OLED_dat(W8_H16[y][x]);
//else
//OLED_dat(~W8_H16[y][x]);
OLED_locat(X,Yr+1);
X+=8;
for(x=8;x<16;x++)
//if(R)
OLED_dat(W8_H16[y][x]);
//else
//OLED_dat(~W8_H16[y][x]);
}
}


void OLED_W16_H16(unsigned char X,unsigned char Yr,unsigned char Num)
{
unsigned char x;
OLED_locat(X,Yr);
for(x=0;x<16;x++)
//if(R)
OLED_dat(W16_H16[Num][x]);
//else
//OLED_dat(~W16_H16[Num][x]);
OLED_locat(X,Yr+1);
for(x=16;x<32;x++)
//if(R)
OLED_dat(W16_H16[Num][x]);
//else
//OLED_dat(~W16_H16[Num][x]);
}


void OLED_Picture(unsigned char Pic[])
{
unsigned char x,y;
for(y=0;y<8;y++)
{
OLED_locat(0,y);
for(x=0;x<128;x++,Pic++)
//if(R)
OLED_dat(*Pic);
//else
//OLED_dat(~*Pic);
}
}