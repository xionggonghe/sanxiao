
/********************************************************/
/*
*   功能：  基于STM32单片机与OneNet平台的LED灯控制实验  	
*   日期：  2018.05.18                                 
*   来源：  广东海洋大学																
*   作者：  智能控制团队Allen                           
*   PS：由于某些代码是自己敲的 可能不规范 或者有些瑕疵 大家凑合着看吧  
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

/*   全局变量定义  */
unsigned int Heart_Pack=0;  //用于定时器TIM1自加计数，用于满足设定自加数值时发送EDP心跳包的标志位
u8  AT_Mode=0;     //发AT指令的模式 用于表示程序处于配置Wifi模块的模式  对串口接收数据进行不同的处理
u8  Contral_flag=0;  //用于存贮控制指令  判断进行什么操作
u8  display[1025];
u8  SendData[100];
u8  temperature;  	    
u8  humidity;  
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
    EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};   //定义一个结构体变量   供心跳包装载数据用
		
    System_Init();       //初始化各模块

    Connect_OneNet();    //接入OneNet平台
	PutStr_H16W8(1,"serve_ok",48, 0);  
	ScreenRefresh(); 
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	PutStr_H16W8(1,"                 ",0, 0); 
	PutStr_H16W8(1,"                 ",16, 0);
	PutStr_H16W8(1,"                 ",32, 0);
	PutStr_H16W8(1,"                 ",48, 0);
	PutStr_H16W8(1,"                 ",64, 0);
	ScreenRefresh();
	
		while(1)	             //while循环执行各种功能 
		{  		
			if(1)
			{
				/*  定时发送心跳包  */			
				if(Heart_Pack==1000)              //每隔5秒发一次心跳包   OneNet平台默认四分钟无数据交换就会断开与设备的连接 发心跳包可以保证连接。
				{
					EDP_PacketPing(&edpPacket);     //装载并发送心跳包
					Heart_Pack=0;
				} 
				if(USART_RX_STA==1)
				{
					PEout(5)=!PEout(5);
					
					char show[30]="             ";
					
					PutStr_H16W8(1,"                 ",0, 0); 
					ScreenRefresh();
//					if(x++>=2)
//					{
//							PutStr_H16W8(1,"                 ",0, 0); 
//							PutStr_H16W8(1,"                 ",16, 0);
//							PutStr_H16W8(1,"                 ",32, 0);
//							ScreenRefresh();
//							x=0;
//					}
						USART_RX_STA=0;
						sprintf(show,"%s",Message_Buf );
						PutStr_H16W8(1,show,0,0);
						ScreenRefresh();
						memset(Message_Buf, 0, sizeof(Message_Buf)); 				
				}


				
				/*用strstr函数来判断操作指令是否匹配 对该函数不了解的朋友自行百度吧*/	 
				 if(strstr((const char*)Message_Buf,"open"))    //判断到操作指令为OPEN 
				 {
	//					LED_ON;   //打开LED
						OneNet_SendData(1);  //向平台发数据1
						delay_ms(20);     //延迟保护 防止频繁发数据，容易断开与平台连接
					 GPIO_SetBits(GPIOB,GPIO_Pin_5);
						memset(Message_Buf, 0, sizeof(Message_Buf));    //执行完指令 清空指令存贮空间 防止继续执行该指令
				 }
				
				 if(strstr((const char*)Message_Buf,"close"))   //判断到操作指令为CLOSE 
				 {
	//					LED_OFF; //关闭LED
						OneNet_SendData(0);  //向平台发数据0
						delay_ms(20);   //延迟
					 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
						memset(Message_Buf, 0, sizeof(Message_Buf));    //执行完指令 清空指令存贮空间 防止继续执行该指令
				 }
				  Lrun();
				 DHT11_Read_Data(&temperature,&humidity);
				 //				 temperature = LorafFlag[0];
//				 humidity = LorafFlag[1];
				 EDPFlag[0]='T';
				 OneNet_SendData(temperature);
				 EDPFlag[0]='H';
				 OneNet_SendData(humidity);
				 PutChar_H16W8(1,USART_RX_STA+'0',48, 0);
				ScreenRefresh();
			 }
			 delay_ms(50);     //延迟保护 防止频繁发数据，容易断开与平台连接
		}	
 }


		
