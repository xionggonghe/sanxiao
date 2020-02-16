
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
//#include "codeGraph.h"
#include "ds1302.h"
#include "findFun.h"
#include "calTime.h"


#define uchar unsigned char
#define uint unsigned int

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
char store_receiver[100];

extern unsigned short downtime[4];
extern char Mnum;
extern char x2_16;
extern unsigned char Dtime[9];
extern unsigned char timeArray[4];
extern unsigned char dayArray[7];
extern unsigned char flagArray[7];


 char t1[]={0xe5,0xbc,0x80,0}; //开
 char t2[]={0xe5,0x85,0xb3,0}; //关
 char t3[]={0xe6,0x97,0xb6,0}; //时
 char t4[]={0xe5,0x88,0x86,0}; //分
 char t5[]={0xe7,0xa7,0x92,0}; //秒

 char t6[]={0xe5,0xbd,0x93,0}; //当
 char t7[]={0xe5,0x89,0x8d,0}; //前

 char t8[]={0xe7,0x8e,0xb0,0}; //现
 char t9[]={0xe5,0x9c,0xa8,0}; //在

 char t10[]={0xe6,0x97,0xb6,0xe9,0x97,0xb4,0}; //时间

 char t11[]={0xe6,0xaf,0x8f,0}; //每
 char t12[]={0xe5,0xa4,0xa9,0}; //天
 char ZW_linkOk[] = {0xEF,0xBB,0xBF,0xE8,0xAE,0xBE,0xE5,0xA4,0x87,0xE8,0xBF,0x9E,0xE6,0xAD,0xA3,0xE5,0xB8,0xB8,0};
 char ZW_commandReceive[] = {0xEF,0xBB,0xBF,0xE6,0x94,0xB6,0xE5,0x88,0xB0,0xE5,0x91,0xBD,0xE4,0xBB,0xA4,0x32,0xEF,0xBC,0x9A,0};


void find(char* receive);//函数声明  

char receiver[120]="hello";
 
char EDPFlag[3];
/*****标志位数组************************
0：数据流选择位    'T':温度   'H':湿度

****************************************/
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
	
		delay_init();  //延时初始化
		DS1302_Init(); //时钟模块初始化
		DS1302_WriteTime(); //时钟模块时间设定
		uart_init(115200);  //打开串口1，与Wifi模块通信
		LED_Init();  //LED灯初始化		
	    Linit(0,23); //lora模块初始化
		ScreenInit( (void*)display); //oled模块初始化
		PutStr_H16W8(1,"hello ^-^ **",0, 0);  
		PutStr_H16W8(1,"lora init ok",16, 0);
		ScreenRefresh(); //刷新oled屏
		DHT11_Init();  //温湿度DHT11初始化 
		PutStr_H16W8(1,"                 ",0, 0); 
		PutStr_H16W8(1,"                 ",16, 0); 
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

/*======================================================================
//函数功能：在接收的命令中提取有效信息
//入口参数：无
//返回参数：	无
//说明：无
//======================================================================*/
void findCmd(void)
{
	if(USART_RX_STA==1)
	{
		strcat(store_receiver,Message_Buf);
		memset(Message_Buf, 0, sizeof(Message_Buf)); 

		find(store_receiver);
		 if(strstr((const char*)store_receiver,"open"))    //判断到操作指令为OPEN 
		 {
				LED_ON;   //打开LED
				OneNet_SendData(1);  //向平台发数据1
				delay_ms(20);     //延迟保护 防止频繁发数据，容易断开与平台连接
		 }
		 if(strstr((const char*)store_receiver,"close"))   //判断到操作指令为CLOSE 
		 {
				LED_OFF; //关闭LED
				OneNet_SendData(0);  //向平台发数据0
				delay_ms(20);   //延迟
		 }
		 if(strstr((const char*)store_receiver,"test"))   //测试通信用
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
		 memset(store_receiver, 0, sizeof(store_receiver)); 
		 USART_RX_STA=0;
		 
	}
}


/*======================================================================
//函数功能：向发送平台发送信息
//入口参数：在函数内设定
//返回参数：	无
//说明：添加数据时，首先将EDPFlag[0]信息标志位的设定唯一标识值，然后通过OneNet_SendData函数进入OneNet_FillBuf函数
//	 在其中的switch语句中添加对应分支。
//		注：如果是int型数据，使用OneNet_SendData函数；
//		  如果是字符串型数据，使用OneNet_SendStringData函数；
//======================================================================*/
void sendMessage()
{
		EDPFlag[0]='T';         		//设置信息标志位
		OneNet_SendData(temperature);   //发送信息，参数为int型 数据
		EDPFlag[0]='H';
		OneNet_SendData(humidity);
		EDPFlag[0]='M';
		if(showflag>0) showflag--;	
		else	 sprintf(receiver,"%s",ZW_linkOk);    //选择上传文字信息
		OneNet_SendStringData(receiver);
}

void find(char* receive)  
{
	/*******************************调整时间***********************************/
	if( ((found(receive,t6,3)&&found(receive,t7,3)) || (found(receive,t8,3)&&found(receive,t9,3)))&& found(receive,t10,6))
	{
		timeArray[3]=1;
			
		if(found(receive,t3,3))           //时
		{
			timeArray[0]=foundnumber(receive,Mnum);		
		}	
		if(found(receive,t4,3))           //分
		{
			timeArray[1]=foundnumber(receive,Mnum);	
		}
		
		if(found(receive,t5,3))           //秒
		{
			timeArray[2]=foundnumber(receive,Mnum);		
		}	
	}
	/******************************每天定时*************************************/
	if(found(receive,t11,3)&&found(receive,t12,3))
	{
		
	    if(found(receive,t1,3))           //开
		{
			dayArray[3]=1;
			dayArray[4]=1;	
		}
		if(found(receive,t2,3))           //关
		{
			dayArray[3]=0;
            dayArray[4]=1;	
		}	
		if(found(receive,t3,3))           //时
		{
			dayArray[0]=foundnumber(receive,Mnum);	
		}	
		if(found(receive,t4,3))           //分
		{
			dayArray[1]=foundnumber(receive,Mnum);
		}
		if(found(receive,t5,3))           //秒
		{
			dayArray[2]=foundnumber(receive,Mnum);		
		}	
	}
	/*****************************任意定时***************************************/
//	if(timeArray[3]!=1)
//	{
		if(strstr(receive,t1))           //开
		{
			flagArray[0]=1;	
            flagArray[6]=1;
//			PutStr_H16W8(1,"open",16, 0);
//			ScreenRefresh();
		}
		if(strstr(receive,t2))           //关
		{
			flagArray[0]=0;
            flagArray[6]=1;
//			PutStr_H16W8(1,"close",16, 0);
//			ScreenRefresh();
		}	
		
		if(found(receive,t3,3))           //时     
		{
			flagArray[1]=1;
			flagArray[5]=1;
			flagArray[2]=foundnumber(receive,Mnum);	//
		PutNum_H16W8(1,Mnum,0,0,32,0);
		ScreenRefresh();
		}	
		if(found(receive,t4,3))           //分
		{
			flagArray[1]=1;	
			flagArray[5]=1;		
			flagArray[3]= foundnumber(receive,Mnum);
		PutNum_H16W8(1,Mnum,0,0,32,32);
		ScreenRefresh();
		}
		
		if(found(receive,t5,3))           //秒
		{
			flagArray[1]=1;	
			flagArray[5]=1;
			flagArray[4]=foundnumber(receive,Mnum);	
		PutNum_H16W8(1,Mnum,0,0,32,64);
		ScreenRefresh();			
		}	      		
//	}
	/*****************************************/
	if(flagArray[1]==1)
	{
		Dtime[0]=(flagArray[2]/10)+'0';//时H 0
		Dtime[1]=(flagArray[2]%10)+'0';//时L 1
		Dtime[2]=':';
		Dtime[3]=(flagArray[3]/10)+'0';//分H 3
		Dtime[4]=(flagArray[3]%10)+'0';//分L 4
		Dtime[5]=':';
		Dtime[6]=(flagArray[4]/10)+'0';//秒H 6
		Dtime[7]=(flagArray[4]%10)+'0';//秒L 7
		
		PutStr_H16W8(1,Dtime,16, 0);
		ScreenRefresh();
		
		downtime[0]=flagArray[2];
		downtime[1]=flagArray[3];
		downtime[2]=flagArray[4];
		flagArray[5]=1;
	}	
}

void timeexecute()
{
	unsigned char i;
	DS1302_GetTime();
//	OLED_Printf_W8_H16(48,4,t,1);
	if(flagArray[5]==1)
	{
		computertime();
	}
	if(flagArray[1]==0&&flagArray[6]==1&&flagArray[5]==0)
	{
//		relayexe();
		for(i=0;i<7;i++)
		{
			flagArray[i]=0;
		}
	}
	/**********************************/
	if(timeArray[3]==1)
	{
//		write_ds1302(timeArray);
		timeArray[3]=0;
	}
	if(dayArray[4]==1)
	{	
		DS1302_GetTime();
		if(realtime[0]==dayArray[0]&&realtime[1]==dayArray[1]&&realtime[2]==dayArray[2])  //
		{
			dayArray[5]=1;
//			relayexe();
		}
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
		findCmd();
		Lrun();      //lora模块测试函数
		DHT11_Read_Data(&temperature,&humidity);
			temperature = LorafFlag[0];
			humidity = LorafFlag[1];
		sendMessage();
		timeexecute();
		downtimeshow();
		delay_ms(200);
	}	
 }


		
