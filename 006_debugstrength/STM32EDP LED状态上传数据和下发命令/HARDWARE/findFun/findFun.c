#include "findFun.h"
#include "Head.h"
extern  char Mnum;


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

 
 
extern unsigned short downtime[4];
extern unsigned char Dtime[9];
extern unsigned char timeArray[4];
extern unsigned char dayArray[7];
extern unsigned char flagArray[7];





char found(char*a,char*b,unsigned char e)
{
	int len,s,n;
	Mnum=0;
	for(len=0;len<strlen(a);(len++))
	{
		for(s=0,n=0;s<e;s++)
		{
			if(a[len+s]==b[s])
			  n++;		
		}
		if(n==e)
		{
			Mnum=len;
//			for(s=0;s<e;s++)
//			  a[len+s]=1;
			return 1;
		}
	}
	return 0;
}


char foundnumber(char *a,unsigned char lnum)
{
	char i=0,j=0,re=0;
	for(i=0;!(lnum-i>=0&&a[lnum-i]<='9'&&a[lnum-i]>='0');i++);    //�����޹��ַ�
	for(j=0;lnum-i>=0&&a[lnum-i]<='9'&&a[lnum-i]>='0';j++,i++)
	{
		re=re+(a[lnum-i]-'0')*pow(10,j); 
//		a[lnum-i]=1;			
	}
	return re;
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



