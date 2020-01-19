
/********************************************************/
/*
*   功能：  基于STM32单片机与OneNet平台的控制系统实验  	
*   日期：                              
*   来源： 													
*   作者：                          
*  说明：该程序存在一个BUG 配置完AT指令后 延迟函数delay_ms 就有点失灵了大概延迟时间会缩小10倍 我找不到原因。
*/
/********************************************************/
#include "stm32f10x.h"
#include "esp8266.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "sys.h"
#include "led.h"
#include "onenet.h"
#include "string.h" 
#include "Tim.h"
#include "OLED_Screen.h"
#include "dht11.h"
#include "lora_app.h"
#include "stdio.h"
#include "codeGraph.h"
#include "ds1302.h"

/*   全局变量定义  */
unsigned int Heart_Pack=0;  //用于定时器TIM1自加计数，用于满足设定自加数值时发送EDP心跳包的标志位
u8  AT_Mode=0;     //发AT指令的模式 用于表示程序处于配置Wifi模块的模式  对串口接收数据进行不同的处理
u8  Contral_flag=0;  //用于存贮控制指令  判断进行什么操作
u8  display[1025];
u8  SendData[100];
u8  temperature;  	    
u8  humidity; 
u8  teststring=0; 
u8  showflag=0;
char receiver[120]="hello";
char EDPFlag[3];

/*  标志位数组
0：数据流选择位    'T':温度   'H':湿度

*/
u8 LorafFlag[3];
/*
0:温度存放位
1:湿度存放位
2:
3:
*/

void System_Init()
{
/*  基本功能初始化   */
	
		delay_init();					//延时初始化
		DS1302_Init();
		DS1302_WriteTime();
		uart_init(115200);    //打开串口1，与Wifi模块通信
		LED_Init();           //LED灯初始化
        ScreenInit( (void*)display);
		PutStr_H16W8(1,"hello ^-^ **",0, 0); 		
	    Linit(0,23);
		PutStr_H16W8(1,"lora init ok",16, 0);
		ScreenRefresh();
		DHT11_Init();   //温湿度DHT11初始化 
		DrawRectangle(0, 0,0, 48, 100);
		ScreenRefresh();
	
/*  配置wifi模块     */
		ESP8266_GPIO_Init();  //初始化ESP8266的CH_DP引脚，拉高即可
		ESP8266Mode_init();   //初始化ESP8266的AT指令

/*  打开定时器       */
		Tim1_Init(5000,72);   //定时器周期配置为5MS
}

void Connect_OneNet(void)
{
     while(!OneNet_Connect)     //判断是否接入平台成功
	   {	
	    	while(OneNet_DevLink())			//接入OneNET
			delay_ms(200);
			
	   }
}


int main(void)
{	 
	char x=0;
	char nowtime[3]={10,20,23};
    EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};   //定义一个结构体变量   供心跳包装载数据用
    System_Init();       //初始化各模块
    Connect_OneNet();    //接入OneNet平台
	PutStr_H16W8(1,"serve_ok",48, 0);  
	ScreenRefresh(); 
	delay_ms(1000);
	PutStr_H16W8(1,"                 ",0, 0); 
	PutStr_H16W8(1,"                 ",16, 0);
	PutStr_H16W8(1,"                 ",32, 0);
	PutStr_H16W8(1,"                 ",48, 0);
	PutStr_H16W8(1,"                 ",64, 0);
	ScreenRefresh();
	
	while(1)	             //while循环执行各种功能 
	{  		
		/*  定时发送心跳包  */			
		if(Heart_Pack==1000)              //每隔5秒发一次心跳包   OneNet平台默认四分钟无数据交换就会断开与设备的连接 发心跳包可以保证连接。
		{
			EDP_PacketPing(&edpPacket);     //装载并发送心跳包
			Heart_Pack=0;
		} 
		if(USART_RX_STA==1)
		{
			 if(strstr((const char*)Message_Buf,"open"))    //判断到操作指令为OPEN 
			 {
					LED_ON;   //打开LED
					OneNet_SendData(1);  //向平台发数据1
					delay_ms(20);     //延迟保护 防止频繁发数据，容易断开与平台连接
			 }
			 if(strstr((const char*)Message_Buf,"close"))   //判断到操作指令为CLOSE 
			 {
					LED_OFF; //关闭LED
					OneNet_SendData(0);  //向平台发数据0
					delay_ms(20);   //延迟
			 }
			 if(strstr((const char*)Message_Buf,"test"))   //测试通信用
			 {
				 LED_ON;
				 sprintf(receiver,"%s",ZW_commandReceive);
				 strcat(receiver,Message_Buf); 					
				 OneNet_SendData(0);  //向平台发数据0
				 OneNet_SendStringData(receiver);	
				 delay_ms(20);   //延迟
				 showflag=6;
				 LED_OFF; 
			 }
			 memset(Message_Buf, 0, sizeof(Message_Buf)); 
			 USART_RX_STA=0;
		}
		DS1302_ReadTime();
		DS1302_GetTime();
		PutStr_H16W8(1,t,1,1);
		ScreenRefresh();
		Lrun();
		DHT11_Read_Data(&temperature,&humidity);
			temperature = LorafFlag[0];
			humidity = LorafFlag[1];
		EDPFlag[0]='T';
		OneNet_SendData(temperature);
		EDPFlag[0]='H';
		OneNet_SendData(humidity);
		EDPFlag[0]='M';
		
		if(showflag>0) showflag--;	
		else	 sprintf(receiver,"%s",ZW_linkOk);    //选择上传文字信息
		
		OneNet_SendStringData(receiver);
		delay_ms(50);     //延迟保护 防止频繁发数据，容易断开与平台连接
	}	
 }


		
