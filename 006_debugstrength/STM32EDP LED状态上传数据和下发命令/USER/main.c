
/********************************************************/
/*
*   功能：  基于STM32单片机与OneNet平台的控制系统实验  	
*   日期：                              
*   来源： 													
*   作者：                          
*  说明：该程序存在一个BUG 配置完AT指令后 延迟函数delay_ms 就有点失灵了大概延迟时间会缩小10倍 我找不到原因。
*/
/********************************************************/
#include "onenet.h"
#include "Head.h"


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



 char ZW_linkOk[] = {0xEF,0xBB,0xBF,0xE8,0xAE,0xBE,0xE5,0xA4,0x87,0xE8,0xBF,0x9E,0xE6,0xAD,0xA3,0xE5,0xB8,0xB8,0};
 char ZW_commandReceive[] = {0xEF,0xBB,0xBF,0xE6,0x94,0xB6,0xE5,0x88,0xB0,0xE5,0x91,0xBD,0xE4,0xBB,0xA4,0x32,0xEF,0xBC,0x9A,0};


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


		
