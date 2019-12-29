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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������ 
//ATK-LORA-01ģ�鹦������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/4/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//******************************************************************************** 

//�豸������ʼ��(�����豸������lora_cfg.h����)
_LoRa_CFG LoRa_CFG=
{
	.addr = 0,       //�豸��ַ  LORA_ADDR
	.power = LORA_POWER,     //���书��
	.chn = 23,         //�ŵ�    LORA_CHN
	.wlrate = LORA_RATE,     //��������
	.wltime = LORA_WLTIME,   //˯��ʱ��
	.mode = LORA_MODE,       //����ģʽ
	.mode_sta = LORA_STA,    //����״̬
	.bps = LORA_TTLBPS_115200 ,     //����������
	.parity = LORA_TTLPAR    //У��λ����
};

//ȫ�ֲ���
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

//�豸����ģʽ(���ڼ�¼�豸״̬)
u8 Lora_mode=0;//0:����ģʽ 1:����ģʽ 2:����ģʽ
//��¼�ж�״̬
static u8 Int_mode=0;//0:�ر� 1:������ 2:�½���

//AUX�ж�����
//mode:���õ�ģʽ 0:�ر� 1:������ 2:�½���
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//�ر��ж�
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //������
	    else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//��¼�ж�ģʽ
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
}

//LORA_AUX�жϷ�����
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//������(����:��ʼ�������� ����:���ݿ�ʼ���)     
	   {
		  if(Lora_mode==1)//����ģʽ
		  {
			 USART3_RX_STA=0;//���ݼ�����0
		  }
		  Int_mode=2;//�����½��ش���
//		  LED0=0;//DS0��
	   }
       else if(Int_mode==2)//�½���(����:�����ѷ����� ����:�����������)	
	   {
		  if(Lora_mode==1)//����ģʽ
		  {
			 USART3_RX_STA|=1<<15;//���ݼ���������
		  }else if(Lora_mode==2)//����ģʽ(�������ݷ������)
		  {
			 Lora_mode=1;//�������ģʽ
		  }
		  Int_mode=1;//���������ش���
//          LED0=1;//DS0��		   
	   }
       Aux_Int(Int_mode);//���������жϱ���
	   EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ  
	}	
}

//LoRaģ���ʼ��
//����ֵ: 0,���ɹ�
//        1,���ʧ��
u8 LoRa_Init(void)
{
	 u8 retry=0;
	 u8 temp=1;
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��PA�˿�ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//��ֹJTAG,�Ӷ�PA15��������ͨIOʹ��,����PA15��������ͨIO!!!	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //�ж��߹ر�(�ȹرպ����ٴ�)
  	 EXTI_Init(&EXTI_InitStructure);//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2�� 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//�����ȼ�3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //�ر��ⲿ�ж�ͨ���������ٴ򿪣�
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//ȷ��LORAģ���ڿ���״̬��(LORA_AUX=0)
	 {
         delay_ms(100);		 
	 }
	 usart3_init(115200);//��ʼ������3
	 LORA_MD0=1;//����ATģʽ
	 delay_ms(40);
	 retry=5;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//���ɹ�
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//���ʧ��
	 return temp;
}

//Loraģ���������
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//��������ģʽǰ����ͨ�Ų����ʺ�У��λ(115200 8λ���� 1λֹͣ ������У�飩
	usart3_rx(1);//��������3����
	
	while(LORA_AUX);//�ȴ�ģ�����
	LORA_MD0=1; //��������ģʽ
	delay_ms(40);
	Lora_mode=0;//���"����ģʽ"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//�����豸��ַ
	while(lora_send_cmd(sendbuf,"OK",100))delay_ms(10) ;
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//�����ŵ��Ϳ�������
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//���÷��书��
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//���ù���ģʽ
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//���÷���״̬
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//����˯��ʱ��
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//���ô��ڲ����ʡ�����У��λ
	while(lora_send_cmd(sendbuf,"OK",50)) delay_ms(10);

	LORA_MD0=0;//�˳�����,����ͨ��
	delay_ms(40);
	while(LORA_AUX);//�ж��Ƿ����(ģ����������ò���)
	USART3_RX_STA=0;
	Lora_mode=1;//���"����ģʽ"
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//����ͨ��,����ͨ�Ŵ�������(�����ʡ�����У��λ)
	Aux_Int(1);//����LORA_AUX�������ж�	
}

u8 Dire_Date[]={0x17,0x04,0x31,0x25,0x03,0x24};//����������
u8 date[30]={0};//��������
u8 Tran_Data[30]={0};//͸������

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
 u32 obj_addr=2;//��¼�û�����Ŀ���ַ
 u8 obj_chn=23;//��¼�û�����Ŀ���ŵ�

//Loraģ�鷢������
void LoRa_SendData(void)
{      
	static u8 num=0;
    u16 addr;
	u8 chn;
	u16 i=0; 
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//͸������
	{
		sprintf((char*)Tran_Data,"17043125");
		u3_printf("%s\r\n",Tran_Data);
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//������
	{
		
		addr = (u16)obj_addr;//Ŀ���ַ
		chn = obj_chn;//Ŀ���ŵ�
		
		date[i++] =(addr>>8)&0xff;//��λ��ַ
		date[i++] = addr&0xff;//��λ��ַ
		date[i] = chn;//�����ŵ�
		
		for(i=0;i<Dire_DateLen;i++)//����д������BUFF
		{
			date[3+i] = Dire_Date[i];
		}	
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
			USART_SendData(USART3,date[i]); 
		}	
	    Dire_Date[4]++;//Dire_Date[4]���ݸ���
	}	
}

//Loraģ���������
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
	//����������
	if( USART3_RX_STA&0x8000 )
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//��ӽ�����
		USART3_RX_STA=0;
		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
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
		memset((char*)USART3_RX_BUF,0x00,len);//���ڽ��ջ�������0
	}
	
}

//���ͺͽ��մ���
void LoRa_Process(void)
{
	u8 key=0;
	u8 t=0;
		
	while(1)
	{
		  if(!LORA_AUX&&(LoRa_CFG.mode!=LORA_MODE_SLEEP))//�����ҷ�ʡ��ģʽ
		  {
			  Lora_mode=2;//���"����״̬"		  
			  LoRa_SendData();//��������
			  delay_ms(200);
//			  PutStr_H16W8(1,"hi ",4, 0);
			  delay_ms(300);
		  }
		//���ݽ���
		Lora_mode=1;  
		LoRa_ReceData();		
   }	
}

//�����Ժ���
void Lora_Test(void)
{	
	u8 t=0;
	u8 key=0;
	u8 netpro=0;
	while(LoRa_Init())//��ʼ��ATK-LORA-01ģ��
	{
		delay_ms(100);
	}
	LoRa_Set();
    delay_ms(100); 	
//	PBout(5)=0;
	while(1)
	{
		LoRa_Process();//��ʼ���ݲ���
		t++;
		if(t==30)
		{
			t=0;
		}
		delay_ms(10);
	}	
}

/***********************************���幤��ʹ�ú���***********************************/

void Linit(u16 add,u8 chn)
{
	while(LoRa_Init())//��ʼ��ATK-LORA-01ģ��
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
	u8 Ldate[30]={0};//��������
    u16 addr;
	u8 chn;
	u16 i=0; 
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//͸������
	{
		u3_printf("%s\r\n",Lsendbuf);
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//������
	{
		
		addr = (u16)obj_addr;//Ŀ���ַ
		chn = obj_chn;//Ŀ���ŵ�
		
		Ldate[i++] =(addr>>8)&0xff;//��λ��ַ
		Ldate[i++] = addr&0xff;//��λ��ַ
		Ldate[i] = chn;//�����ŵ�
		
		for(i=0;i<Lstrsize;i++)//����д������BUFF
		{
			Ldate[3+i] = Lsendbuf[i];
		}	
		for(i=0;i<(Lstrsize+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
			USART_SendData(USART3,Ldate[i]); 
		}	
//	    Dire_Date[4]++;//Dire_Date[4]���ݸ���
	}
}


extern u8 LorafFlag[3];
void Lrend()
{
	u16 i=0;
    u16 len=0;
	//����������
	u8* findstring;
	if( USART3_RX_STA&0x8000 )
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//��ӽ�����
		USART3_RX_STA=0;
//		for(i=0;i<len;i++)
//		{
//			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
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
		memset((char*)USART3_RX_BUF,0x00,len);//���ڽ��ջ�������0
	}
}

void Lrun()
{
	  if(!LORA_AUX&&(LoRa_CFG.mode!=LORA_MODE_SLEEP))//�����ҷ�ʡ��ģʽ
	  {
		  Lora_mode=2;//���"����״̬"		  
		  Lsend("170431");//��������
		  delay_ms(200);
		  delay_ms(300);
	  }
	//���ݽ���
	Lora_mode=1;  
	Lrend();	
}







