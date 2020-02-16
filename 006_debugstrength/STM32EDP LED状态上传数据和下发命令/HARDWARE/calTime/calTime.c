#include "calTime.h"
#include "OLED_Screen.h"
#include "DS1302.h"

unsigned short formtime;
unsigned short downtime[4]={0};
 char ddtime[9]={0};

unsigned short realtime[4];

unsigned char Dtime[9]={0,0,0,0,0,0,0,0};
unsigned char timeArray[4]={0,0,0,0};  //更改时间标志
/*****************************************************
timeArray[0]:   时
timeArray[1]:   分
timeArray[2]:   秒
timeArray[3]:   更新时间标志位  0：更改 1：不变
******************************************************/
char Mnum=0; //数字提取暂存位
/******************每天定时标志位***********************************/
 unsigned char dayArray[7]={0,0,0,0,0,0,0};  
//dayArray[0]:   时
//dayArray[1]:   分
//dayArray[2]:   秒
//dayArray[3]:   0：关  1：开
//dayArray[4]:   开启标志位  0:不开启 1:开启
//dayArray[5]:   执行标志位  0：不执行 1：执行
 unsigned char flagArray[7]={0,0,0,0,0,0,0};   //标志位数组
/******************任意定时标志位***********************************

flagArray[0]:开关标志位  0：关 1：开
flagArray[1]:定时标志位  0：未定时 1：开启定时
flagArray[2]:  小时数
flagArray[3]:  分钟数
flagArray[4]:  秒数
flagArray[5]:  定时状态标志位  0：定时结束 1：定时中
flagArray[6]:  执行标志位 0：不执行 1：执行

*****************************************************/


void computertime()
{
	DS1302_GetTime();
	flagArray[1]=0; //很重要，防止一直重复赋值
	if(downtime[2]>0)
	{
		if(formtime!=realtime[2]&&flagArray[5]==1)
		{
			formtime=realtime[2];
			downtime[2]--;	
		}
	}
	else
	{
		if(downtime[2]<=0)
		{
			if(downtime[1]>0)
			{
				downtime[1]--;
				downtime[2]=59;
				
			}
			else if(downtime[1]==0&&downtime[0]>0)
			{
				downtime[2]=59;
				downtime[1]=59;
				downtime[0]--;
			}
			else if(downtime[0]==0&&downtime[1]==0&&downtime[2]==0)  
			{
				flagArray[5]=0;
				flagArray[6]=1;
	//            relayexe();
			}
		}
	}
}


void downtimeshow()
{
	ddtime[0]=(downtime[0]/10)+48;//时H 0
	ddtime[1]=(downtime[0]%10)+48;//时L 1
	ddtime[2]=':';
	ddtime[3]=(downtime[1]/10)+48;//分H 3
	ddtime[4]=(downtime[1]%10)+48;//分L 4
	ddtime[5]=':';
	ddtime[6]=(downtime[2]/10)+48;//秒H 6
	ddtime[7]=(downtime[2]%10)+48;//秒L 7
	DS1302_GetTime();	
	PutStr_H16W8(1,t,0,0);
	ScreenRefresh(); //刷新oled屏
	delay_ms(50);
	PutStr_H16W8(1,ddtime,48, 0);
	ScreenRefresh(); //刷新oled屏
}

