
/********************************************************/
/*
*   ���ܣ�  ����STM32��Ƭ����OneNetƽ̨�Ŀ���ϵͳʵ��  	
*   ���ڣ�                              
*   ��Դ�� 													
*   ���ߣ�                          
*  ˵�����ó������һ��BUG ������ATָ��� �ӳٺ���delay_ms ���е�ʧ���˴���ӳ�ʱ�����С10�� ���Ҳ���ԭ��
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

/*   ȫ�ֱ�������  */
unsigned int Heart_Pack=0;  //���ڶ�ʱ��TIM1�ԼӼ��������������趨�Լ���ֵʱ����EDP�������ı�־λ
u8  AT_Mode=0;     //��ATָ���ģʽ ���ڱ�ʾ����������Wifiģ���ģʽ  �Դ��ڽ������ݽ��в�ͬ�Ĵ���
u8  Contral_flag=0;  //���ڴ�������ָ��  �жϽ���ʲô����
u8  display[1025];
u8  SendData[100];
u8  temperature;  	    
u8  humidity; 
u8  teststring=0; 
u8  showflag=0;
char receiver[120]="hello";
char EDPFlag[3];

/*  ��־λ����
0��������ѡ��λ    'T':�¶�   'H':ʪ��

*/
u8 LorafFlag[3];
/*
0:�¶ȴ��λ
1:ʪ�ȴ��λ
2:
3:
*/

void System_Init()
{
/*  �������ܳ�ʼ��   */
	
		delay_init();					//��ʱ��ʼ��
		DS1302_Init();
		DS1302_WriteTime();
		uart_init(115200);    //�򿪴���1����Wifiģ��ͨ��
		LED_Init();           //LED�Ƴ�ʼ��
        ScreenInit( (void*)display);
		PutStr_H16W8(1,"hello ^-^ **",0, 0); 		
	    Linit(0,23);
		PutStr_H16W8(1,"lora init ok",16, 0);
		ScreenRefresh();
		DHT11_Init();   //��ʪ��DHT11��ʼ�� 
		DrawRectangle(0, 0,0, 48, 100);
		ScreenRefresh();
	
/*  ����wifiģ��     */
		ESP8266_GPIO_Init();  //��ʼ��ESP8266��CH_DP���ţ����߼���
		ESP8266Mode_init();   //��ʼ��ESP8266��ATָ��

/*  �򿪶�ʱ��       */
		Tim1_Init(5000,72);   //��ʱ����������Ϊ5MS
}

void Connect_OneNet(void)
{
     while(!OneNet_Connect)     //�ж��Ƿ����ƽ̨�ɹ�
	   {	
	    	while(OneNet_DevLink())			//����OneNET
			delay_ms(200);
			
	   }
}


int main(void)
{	 
	char x=0;
	char nowtime[3]={10,20,23};
    EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};   //����һ���ṹ�����   ��������װ��������
    System_Init();       //��ʼ����ģ��
    Connect_OneNet();    //����OneNetƽ̨
	PutStr_H16W8(1,"serve_ok",48, 0);  
	ScreenRefresh(); 
	delay_ms(1000);
	PutStr_H16W8(1,"                 ",0, 0); 
	PutStr_H16W8(1,"                 ",16, 0);
	PutStr_H16W8(1,"                 ",32, 0);
	PutStr_H16W8(1,"                 ",48, 0);
	PutStr_H16W8(1,"                 ",64, 0);
	ScreenRefresh();
	
	while(1)	             //whileѭ��ִ�и��ֹ��� 
	{  		
		/*  ��ʱ����������  */			
		if(Heart_Pack==1000)              //ÿ��5�뷢һ��������   OneNetƽ̨Ĭ���ķ��������ݽ����ͻ�Ͽ����豸������ �����������Ա�֤���ӡ�
		{
			EDP_PacketPing(&edpPacket);     //װ�ز�����������
			Heart_Pack=0;
		} 
		if(USART_RX_STA==1)
		{
			 if(strstr((const char*)Message_Buf,"open"))    //�жϵ�����ָ��ΪOPEN 
			 {
					LED_ON;   //��LED
					OneNet_SendData(1);  //��ƽ̨������1
					delay_ms(20);     //�ӳٱ��� ��ֹƵ�������ݣ����׶Ͽ���ƽ̨����
			 }
			 if(strstr((const char*)Message_Buf,"close"))   //�жϵ�����ָ��ΪCLOSE 
			 {
					LED_OFF; //�ر�LED
					OneNet_SendData(0);  //��ƽ̨������0
					delay_ms(20);   //�ӳ�
			 }
			 if(strstr((const char*)Message_Buf,"test"))   //����ͨ����
			 {
				 LED_ON;
				 sprintf(receiver,"%s",ZW_commandReceive);
				 strcat(receiver,Message_Buf); 					
				 OneNet_SendData(0);  //��ƽ̨������0
				 OneNet_SendStringData(receiver);	
				 delay_ms(20);   //�ӳ�
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
		else	 sprintf(receiver,"%s",ZW_linkOk);    //ѡ���ϴ�������Ϣ
		
		OneNet_SendStringData(receiver);
		delay_ms(50);     //�ӳٱ��� ��ֹƵ�������ݣ����׶Ͽ���ƽ̨����
	}	
 }


		
