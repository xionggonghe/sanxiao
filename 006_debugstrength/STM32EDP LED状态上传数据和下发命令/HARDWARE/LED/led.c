#include "led.h"
   
//LED IO��ʼ��
void LED_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1);						 //Ĭ�ϲ���
	
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5);						 //Ĭ�ϲ���
	    
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOE,GPIO_Pin_5);						 //Ĭ�ϲ���
}

 
