#include "lora_app.h"
#include "usart3.h"
#include "string.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "lora_ui.h"
#include "OLED_Screen.h"

#define u16 unsigned short
#define u8 unsigned char

//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板 
//ATK-LORA-01模块功能驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/4/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//******************************************************************************** 

//设备参数初始化(具体设备参数见lora_cfg.h定义)
_LoRa_CFG LoRa_CFG=
{
	.addr = 0,       //设备地址  LORA_ADDR
	.power = LORA_POWER,     //发射功率
	.chn = 23,         //信道    LORA_CHN
	.wlrate = LORA_RATE,     //空中速率
	.wltime = LORA_WLTIME,   //睡眠时间
	.mode = LORA_MODE,       //工作模式
	.mode_sta = LORA_STA,    //发送状态
	.bps = LORA_TTLBPS_115200 ,     //波特率设置
	.parity = LORA_TTLPAR    //校验位设置
};

//全局参数
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

//设备工作模式(用于记录设备状态)
u8 Lora_mode=0;//0:配置模式 1:接收模式 2:发送模式
//记录中断状态
static u8 Int_mode=0;//0:关闭 1:上升沿 2:下降沿

//AUX中断设置
//mode:配置的模式 0:关闭 1:上升沿 2:下降沿
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//关闭中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿
	    else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//记录中断模式
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
}

//LORA_AUX中断服务函数
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//上升沿(发送:开始发送数据 接收:数据开始输出)     
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART3_RX_STA=0;//数据计数清0
		  }
		  Int_mode=2;//设置下降沿触发
//		  LED0=0;//DS0亮
	   }
       else if(Int_mode==2)//下降沿(发送:数据已发送完 接收:数据输出结束)	
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART3_RX_STA|=1<<15;//数据计数标记完成
		  }else if(Lora_mode==2)//发送模式(串口数据发送完毕)
		  {
			 Lora_mode=1;//进入接收模式
		  }
		  Int_mode=1;//设置上升沿触发
//          LED0=1;//DS0灭		   
	   }
       Aux_Int(Int_mode);//重新设置中断边沿
	   EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位  
	}	
}

//LoRa模块初始化
//返回值: 0,检测成功
//        1,检测失败
u8 LoRa_Init(void)
{
	 u8 retry=0;
	 u8 temp=1;
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PA端口时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //下拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //中断线关闭(先关闭后面再打开)
  	 EXTI_Init(&EXTI_InitStructure);//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2， 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//子优先级3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //关闭外部中断通道（后面再打开）
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//确保LORA模块在空闲状态下(LORA_AUX=0)
	 {
         delay_ms(100);		 
	 }
	 usart3_init(115200);//初始化串口3
	 LORA_MD0=1;//进入AT模式
	 delay_ms(40);
	 retry=5;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//检测成功
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//检测失败
	 return temp;
}

//Lora模块参数配置
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//进入配置模式前设置通信波特率和校验位(115200 8位数据 1位停止 无数据校验）
	usart3_rx(1);//开启串口3接收
	
	while(LORA_AUX);//等待模块空闲
	LORA_MD0=1; //进入配置模式
	delay_ms(40);
	Lora_mode=0;//标记"配置模式"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//设置设备地址
	while(lora_send_cmd(sendbuf,"OK",100))delay_ms(10) ;
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//设置信道和空中速率
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//设置发射功率
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//设置工作模式
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//设置发送状态
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//设置睡眠时间
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//设置串口波特率、数据校验位
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);

	LORA_MD0=0;//退出配置,进入通信
	delay_ms(40);
	while(LORA_AUX);//判断是否空闲(模块会重新配置参数)
	USART3_RX_STA=0;
	Lora_mode=1;//标记"接收模式"
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//返回通信,更新通信串口配置(波特率、数据校验位)
	Aux_Int(1);//设置LORA_AUX上升沿中断	
}

u8 Dire_Date[]={0x17,0x04,0x31,0x25,0x03,0x24};//定向传输数据
u8 date[30]={0};//定向数组
u8 Tran_Data[30]={0};//透传数组

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
 u32 obj_addr=2;//记录用户输入目标地址
 u8 obj_chn=23;//记录用户输入目标信道

//Lora模块发送数据
void LoRa_SendData(void)
{      
	static u8 num=0;
    u16 addr;
	u8 chn;
	u16 i=0; 
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//透明传输
	{
		sprintf((char*)Tran_Data,"17043125");
		u3_printf("%s\r\n",Tran_Data);
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//定向传输
	{
		
		addr = (u16)obj_addr;//目标地址
		chn = obj_chn;//目标信道
		
		date[i++] =(addr>>8)&0xff;//高位地址
		date[i++] = addr&0xff;//低位地址
		date[i] = chn;//无线信道
		
		for(i=0;i<Dire_DateLen;i++)//数据写到发送BUFF
		{
			date[3+i] = Dire_Date[i];
		}	
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
			USART_SendData(USART3,date[i]); 
		}	
	    Dire_Date[4]++;//Dire_Date[4]数据更新
	}	
}

//Lora模块接收数据
void LoRa_ReceData(void)
{
    u16 i=0;
    u16 len=5;
	
//	PutStr_H16W8(1,"buf ",16, 0);
//	
	ScreenRefresh();
	PutStr_H16W8(1,"sta ",16, 0);
	PutNum_H16W8(1,(USART3_RX_STA&0x8000)>>15,false,1,16,36);
//	PutNum_H16W8(1,(USART3_RX_STA&0x7fff),false,0,32,0);
//	PutNum_H16W8(1,(USART3_RX_STA&0x7fff)/100%10,false,0,32,0);
//	PutNum_H16W8(1,(USART3_RX_STA&0x7fff)/10%10,false,0,32,36);
//	PutNum_H16W8(1,(USART3_RX_STA&0x7fff)%10,false,2,32,72);
	ScreenRefresh();
	delay_ms(300);
	//有数据来了
	if( USART3_RX_STA&0x8000 )
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//添加结束符
		USART3_RX_STA=0;
		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
			USART_SendData(USART3,USART3_RX_BUF[i]); 
		}
		if(strstr(USART3_RX_BUF,"on"))
		{
			PBout(5)=0;
		}
		if(strstr(USART3_RX_BUF,"off"))
		{
			PBout(5)=1;
		}
		delay_ms(50);
		memset((char*)USART3_RX_BUF,0x00,len);//串口接收缓冲区清0
	}
	
}

//发送和接收处理
void LoRa_Process(void)
{
	u8 key=0;
	u8 t=0;
		
	while(1)
	{
		  if(!LORA_AUX&&(LoRa_CFG.mode!=LORA_MODE_SLEEP))//空闲且非省电模式
		  {
			  Lora_mode=2;//标记"发送状态"		  
			  LoRa_SendData();//发送数据
			  delay_ms(200);
//			  PutStr_H16W8(1,"hi ",4, 0);
			  delay_ms(300);
		  }
		//数据接收
		Lora_mode=1;  
		LoRa_ReceData();		
   }	
}

//主测试函数
void Lora_Test(void)
{	
	u8 t=0;
	u8 key=0;
	u8 netpro=0;
	while(LoRa_Init())//初始化ATK-LORA-01模块
	{
		delay_ms(100);
	}
	LoRa_Set();
    delay_ms(100); 	
//	PBout(5)=0;
	while(1)
	{
		LoRa_Process();//开始数据测试
		t++;
		if(t==30)
		{
			t=0;
		}
		delay_ms(10);
	}	
}

/***********************************整体工程使用函数***********************************/

void Linit(u16 add,u8 chn)
{
	while(LoRa_Init())//初始化ATK-LORA-01模块
	{
		delay_ms(100);
	}
	LoRa_CFG.addr=add;
	LoRa_CFG.chn=chn;
	LoRa_Set();
}


#define Lstrsize sizeof(Lsendbuf)/sizeof(Lsendbuf[0])
	
void Lsend(char* Lsendbuf)
{
	static u8 num=0;
	u8 Ldate[30]={0};//定向数组
    u16 addr;
	u8 chn;
	u16 i=0; 
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//透明传输
	{
		u3_printf("%s\r\n",Lsendbuf);
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//定向传输
	{
		
		addr = (u16)obj_addr;//目标地址
		chn = obj_chn;//目标信道
		
		Ldate[i++] =(addr>>8)&0xff;//高位地址
		Ldate[i++] = addr&0xff;//低位地址
		Ldate[i] = chn;//无线信道
		
		for(i=0;i<Lstrsize;i++)//数据写到发送BUFF
		{
			Ldate[3+i] = Lsendbuf[i];
		}	
		for(i=0;i<(Lstrsize+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
			USART_SendData(USART3,Ldate[i]); 
		}	
//	    Dire_Date[4]++;//Dire_Date[4]数据更新
	}
}


extern u8 LorafFlag[3];
void Lrend()
{
	u16 i=0;
    u16 len=0;
	//有数据来了
	u8* findstring;
	if( USART3_RX_STA&0x8000 )
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//添加结束符
		USART3_RX_STA=0;
//		for(i=0;i<len;i++)
//		{
//			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
//			USART_SendData(USART3,USART3_RX_BUF[i]); 
//		}
		Lsend("OK,Receive");
		findstring=strstr(USART3_RX_BUF,"F");
		LorafFlag[0]=findstring[1]*10;
		LorafFlag[1]=findstring[2]*10;
		
		if(strstr(USART3_RX_BUF,"on"))
		{
			PBout(5)=0;
		}
		if(strstr(USART3_RX_BUF,"off"))
		{
			PBout(5)=1;
		}
		delay_ms(50);
		memset((char*)USART3_RX_BUF,0x00,len);//串口接收缓冲区清0
	}
}

void Lrun()
{
	  if(!LORA_AUX&&(LoRa_CFG.mode!=LORA_MODE_SLEEP))//空闲且非省电模式
	  {
		  Lora_mode=2;//标记"发送状态"		  
		  Lsend("170431");//发送数据
		  delay_ms(200);
		  delay_ms(300);
	  }
	//数据接收
	Lora_mode=1;  
	Lrend();	
}







