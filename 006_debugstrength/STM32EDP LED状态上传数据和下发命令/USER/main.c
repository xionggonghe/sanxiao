
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
//#include "codeGraph.h"
#include "ds1302.h"
#include "findFun.h"
#include "calTime.h"


#define uchar unsigned char
#define uint unsigned int

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
char store_receiver[100];

extern unsigned short downtime[4];
extern char Mnum;
extern char x2_16;
extern unsigned char Dtime[9];
extern unsigned char timeArray[4];
extern unsigned char dayArray[7];
extern unsigned char flagArray[7];


 char t1[]={0xe5,0xbc,0x80,0}; //��
 char t2[]={0xe5,0x85,0xb3,0}; //��
 char t3[]={0xe6,0x97,0xb6,0}; //ʱ
 char t4[]={0xe5,0x88,0x86,0}; //��
 char t5[]={0xe7,0xa7,0x92,0}; //��

 char t6[]={0xe5,0xbd,0x93,0}; //��
 char t7[]={0xe5,0x89,0x8d,0}; //ǰ

 char t8[]={0xe7,0x8e,0xb0,0}; //��
 char t9[]={0xe5,0x9c,0xa8,0}; //��

 char t10[]={0xe6,0x97,0xb6,0xe9,0x97,0xb4,0}; //ʱ��

 char t11[]={0xe6,0xaf,0x8f,0}; //ÿ
 char t12[]={0xe5,0xa4,0xa9,0}; //��
 char ZW_linkOk[] = {0xEF,0xBB,0xBF,0xE8,0xAE,0xBE,0xE5,0xA4,0x87,0xE8,0xBF,0x9E,0xE6,0xAD,0xA3,0xE5,0xB8,0xB8,0};
 char ZW_commandReceive[] = {0xEF,0xBB,0xBF,0xE6,0x94,0xB6,0xE5,0x88,0xB0,0xE5,0x91,0xBD,0xE4,0xBB,0xA4,0x32,0xEF,0xBC,0x9A,0};


void find(char* receive);//��������  

char receiver[120]="hello";
 
char EDPFlag[3];
/*****��־λ����************************
0��������ѡ��λ    'T':�¶�   'H':ʪ��

****************************************/
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
	
		delay_init();  //��ʱ��ʼ��
		DS1302_Init(); //ʱ��ģ���ʼ��
		DS1302_WriteTime(); //ʱ��ģ��ʱ���趨
		uart_init(115200);  //�򿪴���1����Wifiģ��ͨ��
		LED_Init();  //LED�Ƴ�ʼ��		
	    Linit(0,23); //loraģ���ʼ��
		ScreenInit( (void*)display); //oledģ���ʼ��
		PutStr_H16W8(1,"hello ^-^ **",0, 0);  
		PutStr_H16W8(1,"lora init ok",16, 0);
		ScreenRefresh(); //ˢ��oled��
		DHT11_Init();  //��ʪ��DHT11��ʼ�� 
		PutStr_H16W8(1,"                 ",0, 0); 
		PutStr_H16W8(1,"                 ",16, 0); 
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

/*======================================================================
//�������ܣ��ڽ��յ���������ȡ��Ч��Ϣ
//��ڲ�������
//���ز�����	��
//˵������
//======================================================================*/
void findCmd(void)
{
	if(USART_RX_STA==1)
	{
		strcat(store_receiver,Message_Buf);
		memset(Message_Buf, 0, sizeof(Message_Buf)); 

		find(store_receiver);
		 if(strstr((const char*)store_receiver,"open"))    //�жϵ�����ָ��ΪOPEN 
		 {
				LED_ON;   //��LED
				OneNet_SendData(1);  //��ƽ̨������1
				delay_ms(20);     //�ӳٱ��� ��ֹƵ�������ݣ����׶Ͽ���ƽ̨����
		 }
		 if(strstr((const char*)store_receiver,"close"))   //�жϵ�����ָ��ΪCLOSE 
		 {
				LED_OFF; //�ر�LED
				OneNet_SendData(0);  //��ƽ̨������0
				delay_ms(20);   //�ӳ�
		 }
		 if(strstr((const char*)store_receiver,"test"))   //����ͨ����
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
		 memset(store_receiver, 0, sizeof(store_receiver)); 
		 USART_RX_STA=0;
		 
	}
}


/*======================================================================
//�������ܣ�����ƽ̨������Ϣ
//��ڲ������ں������趨
//���ز�����	��
//˵�����������ʱ�����Ƚ�EDPFlag[0]��Ϣ��־λ���趨Ψһ��ʶֵ��Ȼ��ͨ��OneNet_SendData��������OneNet_FillBuf����
//	 �����е�switch�������Ӷ�Ӧ��֧��
//		ע�������int�����ݣ�ʹ��OneNet_SendData������
//		  ������ַ��������ݣ�ʹ��OneNet_SendStringData������
//======================================================================*/
void sendMessage()
{
		EDPFlag[0]='T';         		//������Ϣ��־λ
		OneNet_SendData(temperature);   //������Ϣ������Ϊint�� ����
		EDPFlag[0]='H';
		OneNet_SendData(humidity);
		EDPFlag[0]='M';
		if(showflag>0) showflag--;	
		else	 sprintf(receiver,"%s",ZW_linkOk);    //ѡ���ϴ�������Ϣ
		OneNet_SendStringData(receiver);
}

void find(char* receive)  
{
	/*******************************����ʱ��***********************************/
	if( ((found(receive,t6,3)&&found(receive,t7,3)) || (found(receive,t8,3)&&found(receive,t9,3)))&& found(receive,t10,6))
	{
		timeArray[3]=1;
			
		if(found(receive,t3,3))           //ʱ
		{
			timeArray[0]=foundnumber(receive,Mnum);		
		}	
		if(found(receive,t4,3))           //��
		{
			timeArray[1]=foundnumber(receive,Mnum);	
		}
		
		if(found(receive,t5,3))           //��
		{
			timeArray[2]=foundnumber(receive,Mnum);		
		}	
	}
	/******************************ÿ�춨ʱ*************************************/
	if(found(receive,t11,3)&&found(receive,t12,3))
	{
		
	    if(found(receive,t1,3))           //��
		{
			dayArray[3]=1;
			dayArray[4]=1;	
		}
		if(found(receive,t2,3))           //��
		{
			dayArray[3]=0;
            dayArray[4]=1;	
		}	
		if(found(receive,t3,3))           //ʱ
		{
			dayArray[0]=foundnumber(receive,Mnum);	
		}	
		if(found(receive,t4,3))           //��
		{
			dayArray[1]=foundnumber(receive,Mnum);
		}
		if(found(receive,t5,3))           //��
		{
			dayArray[2]=foundnumber(receive,Mnum);		
		}	
	}
	/*****************************���ⶨʱ***************************************/
//	if(timeArray[3]!=1)
//	{
		if(strstr(receive,t1))           //��
		{
			flagArray[0]=1;	
            flagArray[6]=1;
//			PutStr_H16W8(1,"open",16, 0);
//			ScreenRefresh();
		}
		if(strstr(receive,t2))           //��
		{
			flagArray[0]=0;
            flagArray[6]=1;
//			PutStr_H16W8(1,"close",16, 0);
//			ScreenRefresh();
		}	
		
		if(found(receive,t3,3))           //ʱ     
		{
			flagArray[1]=1;
			flagArray[5]=1;
			flagArray[2]=foundnumber(receive,Mnum);	//
		PutNum_H16W8(1,Mnum,0,0,32,0);
		ScreenRefresh();
		}	
		if(found(receive,t4,3))           //��
		{
			flagArray[1]=1;	
			flagArray[5]=1;		
			flagArray[3]= foundnumber(receive,Mnum);
		PutNum_H16W8(1,Mnum,0,0,32,32);
		ScreenRefresh();
		}
		
		if(found(receive,t5,3))           //��
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
		Dtime[0]=(flagArray[2]/10)+'0';//ʱH 0
		Dtime[1]=(flagArray[2]%10)+'0';//ʱL 1
		Dtime[2]=':';
		Dtime[3]=(flagArray[3]/10)+'0';//��H 3
		Dtime[4]=(flagArray[3]%10)+'0';//��L 4
		Dtime[5]=':';
		Dtime[6]=(flagArray[4]/10)+'0';//��H 6
		Dtime[7]=(flagArray[4]%10)+'0';//��L 7
		
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
		findCmd();
		Lrun();      //loraģ����Ժ���
		DHT11_Read_Data(&temperature,&humidity);
			temperature = LorafFlag[0];
			humidity = LorafFlag[1];
		sendMessage();
		timeexecute();
		downtimeshow();
		delay_ms(200);
	}	
 }


		
