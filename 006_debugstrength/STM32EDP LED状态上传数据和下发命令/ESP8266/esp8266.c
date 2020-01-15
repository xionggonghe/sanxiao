
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "delay.h"
#include "usart.h"
#include "Led.h"
//C��
#include <string.h>
#include <stdio.h>
#include "OLED_Screen.h"

/** ���ڷ���һ�����ݵĺ���
  *
  *
  *
  */ 
void USART_Write(unsigned char *cmd, int len)
{  
	  int i;
	   USART_ClearFlag(USART1,USART_FLAG_TC);    //����֮ǰ��շ��ͱ�־  û����һ�� �����׶��� ��һ���������׶�ʧ
	   for(i=0;i<len;i++)
	   {
		   USART_SendData(USART1,*cmd);   //���͵�ǰ����
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  //�������;
			 cmd++;      //ָ���Լ� ׼��������һ������
		   
		 }
 
}

/**
  * @brief  ��ʼ��ESP8266�õ���GPIO����
  * @param  ��
  * @retval ��
  */
void ESP8266_GPIO_Init(void)
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;


		/* ���� CH_PD ����*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //�����������ģʽ
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;                      //��ʼ��CH-DP���� ����
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //����ʱ��
		GPIO_Init(GPIOG,&GPIO_InitStructure);   
		GPIO_SetBits(GPIOG,GPIO_Pin_13); 

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //�����������ģʽ
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;                      //��ʼ��RST���� ����
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //����ʱ��
		GPIO_Init(GPIOG,&GPIO_InitStructure);   
		GPIO_SetBits(GPIOG,GPIO_Pin_12); 
  
}

void ESP8266Mode_init(void)
{
	  int Delay = 100;
	
	  AT_Mode = 1;  //���뷢ATָ��ģʽ
	  
/****wifiģ����OneNetƽ̨���ݶԽ����� */
	GPIO_ResetBits(GPIOG,GPIO_Pin_12);
	delay_ms(80);
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	
	
	PutStr_H16W8(1,"                 ",0, 0); 
	PutStr_H16W8(1,"                 ",16, 0); 
	ScreenRefresh();
	
    SendCmd(AT,"OK", Delay);		//ģ����Ч�Լ��
	PutStr_H16W8(1,"AT ok",0, 0);  
	ScreenRefresh();
    SendCmd(CWMODE, "OK", Delay);	//ģ�鹤��ģʽ

    SendCmd(wifi_RST, "OK", Delay);		//ģ������
	SendCmd(wifi_RST, "OK", Delay);		//ģ������
//	PutStr_H16W8(1,"mode ok",20, 0);  
//	ScreenRefresh();  
	  delay_ms(100);								//�ȴ�ģ�鸴λ���
	
    SendCmd(CWJAP, "OK", 300);		//������Ҫ���ӵ�WIFI�ȵ�SSID������  CIFSR
	PutStr_H16W8(1,"link ok",16, 0);  
	ScreenRefresh();  
    SendCmd(CIPSTART, "OK", Delay);	//TCP����
    SendCmd(CIPMODE1, "OK", Delay);	//����͸��ģʽ
	SendCmd(CIPSEND, ">", Delay);	//����͸��ģʽ
	
	PutStr_H16W8(1,"init_ok",32, 0);  
	ScreenRefresh();  	
		AT_Mode = 0;   //ATָ������ �˳���ģʽ
}


	
/** 
	* �������ܣ� ����Cmd����ĺ���
  * CMD�� 		��Ҫ���͵�ATָ��
  * result �� ���ͳɹ�ʱ���ص���ֵ��result��������Ա�
  * timeOut ���ӳ�ʱ��
  *	
*/	
	
void SendCmd(char* cmd, char* result, int timeOut)
{
    while(1)
    {
		  memset(Rx_Buff, 0, sizeof(Rx_Buff)); //��������֮ǰ��������������飬�����ڴ����н��ա�
		  Rx_count=0;
		USART_Write((unsigned char *)cmd,strlen((const char *)cmd));   //�ô��ڰ�cmd����д��ESP8266
		delay_ms(timeOut);                                          //�ӳٵȴ�
			  LED=~LED;        //��������
        if(ok_flag==1)	//�Ƚ�����ָ������������Ƿ�һ�����ж��Ƿ���Ԥ�ڵĽ��  ��Ԥ�ڽ����ͬ����ʾָ�����óɹ�������
        {  
			ok_flag=0;   //��ձ�־
            break;
        }
        else
        {
            delay_ms(200);
        }
    }
}


/**   
 * �������ܣ�ESP8266д���ݵĺ���

 *   

*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
	 
  	USART_Write(data,len);   //��������

}


