#include "calTime.h"
#include "OLED_Screen.h"
#include "DS1302.h"

unsigned short formtime;
unsigned short downtime[4]={0};
 char ddtime[9]={0};

unsigned short realtime[4];

unsigned char Dtime[9]={0,0,0,0,0,0,0,0};
unsigned char timeArray[4]={0,0,0,0};  //����ʱ���־
/*****************************************************
timeArray[0]:   ʱ
timeArray[1]:   ��
timeArray[2]:   ��
timeArray[3]:   ����ʱ���־λ  0������ 1������
******************************************************/
char Mnum=0; //������ȡ�ݴ�λ
/******************ÿ�춨ʱ��־λ***********************************/
 unsigned char dayArray[7]={0,0,0,0,0,0,0};  
//dayArray[0]:   ʱ
//dayArray[1]:   ��
//dayArray[2]:   ��
//dayArray[3]:   0����  1����
//dayArray[4]:   ������־λ  0:������ 1:����
//dayArray[5]:   ִ�б�־λ  0����ִ�� 1��ִ��
 unsigned char flagArray[7]={0,0,0,0,0,0,0};   //��־λ����
/******************���ⶨʱ��־λ***********************************

flagArray[0]:���ر�־λ  0���� 1����
flagArray[1]:��ʱ��־λ  0��δ��ʱ 1��������ʱ
flagArray[2]:  Сʱ��
flagArray[3]:  ������
flagArray[4]:  ����
flagArray[5]:  ��ʱ״̬��־λ  0����ʱ���� 1����ʱ��
flagArray[6]:  ִ�б�־λ 0����ִ�� 1��ִ��

*****************************************************/


void computertime()
{
	DS1302_GetTime();
	flagArray[1]=0; //����Ҫ����ֹһֱ�ظ���ֵ
	if(downtime[2]>0)
	{
		if(formtime!=realtime[2]&&flagArray[5]==1)
		{
			formtime=realtime[2];
			downtime[2]--;	
		}
	}
	else
	{
		if(downtime[2]<=0)
		{
			if(downtime[1]>0)
			{
				downtime[1]--;
				downtime[2]=59;
				
			}
			else if(downtime[1]==0&&downtime[0]>0)
			{
				downtime[2]=59;
				downtime[1]=59;
				downtime[0]--;
			}
			else if(downtime[0]==0&&downtime[1]==0&&downtime[2]==0)  
			{
				flagArray[5]=0;
				flagArray[6]=1;
	//            relayexe();
			}
		}
	}
}


void downtimeshow()
{
	ddtime[0]=(downtime[0]/10)+48;//ʱH 0
	ddtime[1]=(downtime[0]%10)+48;//ʱL 1
	ddtime[2]=':';
	ddtime[3]=(downtime[1]/10)+48;//��H 3
	ddtime[4]=(downtime[1]%10)+48;//��L 4
	ddtime[5]=':';
	ddtime[6]=(downtime[2]/10)+48;//��H 6
	ddtime[7]=(downtime[2]%10)+48;//��L 7
	DS1302_GetTime();	
	PutStr_H16W8(1,t,0,0);
	ScreenRefresh(); //ˢ��oled��
	delay_ms(50);
	PutStr_H16W8(1,ddtime,48, 0);
	ScreenRefresh(); //ˢ��oled��
}

