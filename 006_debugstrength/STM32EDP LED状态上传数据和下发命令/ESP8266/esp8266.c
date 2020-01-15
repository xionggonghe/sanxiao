
//单片机头文件
#include "stm32f10x.h"

//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "Led.h"
//C库
#include <string.h>
#include <stdio.h>
#include "OLED_Screen.h"

/** 串口发送一组数据的函数
  *
  *
  *
  */ 
void USART_Write(unsigned char *cmd, int len)
{  
	  int i;
	   USART_ClearFlag(USART1,USART_FLAG_TC);    //发送之前清空发送标志  没有这一句 很容易丢包 第一个数据容易丢失
	   for(i=0;i<len;i++)
	   {
		   USART_SendData(USART1,*cmd);   //发送当前数据
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  //发送完成;
			 cmd++;      //指针自加 准备发送下一个数据
		   
		 }
 
}

/**
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
void ESP8266_GPIO_Init(void)
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;


		/* 配置 CH_PD 引脚*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //设置推挽输出模式
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;                      //初始化CH-DP引脚 拉高
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //设置时钟
		GPIO_Init(GPIOG,&GPIO_InitStructure);   
		GPIO_SetBits(GPIOG,GPIO_Pin_13); 

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //设置推挽输出模式
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;                      //初始化RST引脚 拉高
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //设置时钟
		GPIO_Init(GPIOG,&GPIO_InitStructure);   
		GPIO_SetBits(GPIOG,GPIO_Pin_12); 
  
}

void ESP8266Mode_init(void)
{
	  int Delay = 100;
	
	  AT_Mode = 1;  //进入发AT指令模式
	  
/****wifi模块与OneNet平台数据对接配置 */
	GPIO_ResetBits(GPIOG,GPIO_Pin_12);
	delay_ms(80);
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	
	
	PutStr_H16W8(1,"                 ",0, 0); 
	PutStr_H16W8(1,"                 ",16, 0); 
	ScreenRefresh();
	
    SendCmd(AT,"OK", Delay);		//模块有效性检查
	PutStr_H16W8(1,"AT ok",0, 0);  
	ScreenRefresh();
    SendCmd(CWMODE, "OK", Delay);	//模块工作模式

    SendCmd(wifi_RST, "OK", Delay);		//模块重置
	SendCmd(wifi_RST, "OK", Delay);		//模块重置
//	PutStr_H16W8(1,"mode ok",20, 0);  
//	ScreenRefresh();  
	  delay_ms(100);								//等待模块复位完成
	
    SendCmd(CWJAP, "OK", 300);		//配置需要连接的WIFI热点SSID和密码  CIFSR
	PutStr_H16W8(1,"link ok",16, 0);  
	ScreenRefresh();  
    SendCmd(CIPSTART, "OK", Delay);	//TCP连接
    SendCmd(CIPMODE1, "OK", Delay);	//配置透传模式
	SendCmd(CIPSEND, ">", Delay);	//进入透传模式
	
	PutStr_H16W8(1,"init_ok",32, 0);  
	ScreenRefresh();  	
		AT_Mode = 0;   //AT指令发送完毕 退出该模式
}


	
/** 
	* 函数功能： 发送Cmd命令的函数
  * CMD： 		需要发送的AT指令
  * result ： 发送成功时返回的数值与result期望结果对比
  * timeOut ：延迟时间
  *	
*/	
	
void SendCmd(char* cmd, char* result, int timeOut)
{
    while(1)
    {
		  memset(Rx_Buff, 0, sizeof(Rx_Buff)); //发送数据之前，先情况接收数组，数据在串口中接收。
		  Rx_count=0;
		USART_Write((unsigned char *)cmd,strlen((const char *)cmd));   //用串口把cmd命令写给ESP8266
		delay_ms(timeOut);                                          //延迟等待
			  LED=~LED;        //闪灯提醒
        if(ok_flag==1)	//比较两个指针里面的数据是否一样，判断是否有预期的结果  和预期结果相同，表示指令设置成功，跳出
        {  
			ok_flag=0;   //清空标志
            break;
        }
        else
        {
            delay_ms(200);
        }
    }
}


/**   
 * 函数功能：ESP8266写数据的函数

 *   

*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
	 
  	USART_Write(data,len);   //发送数据

}


