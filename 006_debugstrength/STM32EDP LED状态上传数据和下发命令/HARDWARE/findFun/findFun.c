#include "findFun.h"
#include "Head.h"
extern  char Mnum;


 char t1[]={0xe5,0xbc,0x80,0}; //开
 char t2[]={0xe5,0x85,0xb3,0}; //关
 char t3[]={0xe6,0x97,0xb6,0}; //时
 char t4[]={0xe5,0x88,0x86,0}; //分
 char t5[]={0xe7,0xa7,0x92,0}; //秒

 char t6[]={0xe5,0xbd,0x93,0}; //当
 char t7[]={0xe5,0x89,0x8d,0}; //前

 char t8[]={0xe7,0x8e,0xb0,0}; //现
 char t9[]={0xe5,0x9c,0xa8,0}; //在

 char t10[]={0xe6,0x97,0xb6,0xe9,0x97,0xb4,0}; //时间

 char t11[]={0xe6,0xaf,0x8f,0}; //每
 char t12[]={0xe5,0xa4,0xa9,0}; //天

 
 
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
	for(i=0;!(lnum-i>=0&&a[lnum-i]<='9'&&a[lnum-i]>='0');i++);    //跳过无关字符
	for(j=0;lnum-i>=0&&a[lnum-i]<='9'&&a[lnum-i]>='0';j++,i++)
	{
		re=re+(a[lnum-i]-'0')*pow(10,j); 
//		a[lnum-i]=1;			
	}
	return re;
}


void find(char* receive)  
{
	/*******************************调整时间***********************************/
	if( ((found(receive,t6,3)&&found(receive,t7,3)) || (found(receive,t8,3)&&found(receive,t9,3)))&& found(receive,t10,6))
	{
		timeArray[3]=1;
			
		if(found(receive,t3,3))           //时
		{
			timeArray[0]=foundnumber(receive,Mnum);		
		}	
		if(found(receive,t4,3))           //分
		{
			timeArray[1]=foundnumber(receive,Mnum);	
		}
		
		if(found(receive,t5,3))           //秒
		{
			timeArray[2]=foundnumber(receive,Mnum);		
		}	
	}
	/******************************每天定时*************************************/
	if(found(receive,t11,3)&&found(receive,t12,3))
	{
		
	    if(found(receive,t1,3))           //开
		{
			dayArray[3]=1;
			dayArray[4]=1;	
		}
		if(found(receive,t2,3))           //关
		{
			dayArray[3]=0;
            dayArray[4]=1;	
		}	
		if(found(receive,t3,3))           //时
		{
			dayArray[0]=foundnumber(receive,Mnum);	
		}	
		if(found(receive,t4,3))           //分
		{
			dayArray[1]=foundnumber(receive,Mnum);
		}
		if(found(receive,t5,3))           //秒
		{
			dayArray[2]=foundnumber(receive,Mnum);		
		}	
	}
	/*****************************任意定时***************************************/
//	if(timeArray[3]!=1)
//	{
		if(strstr(receive,t1))           //开
		{
			flagArray[0]=1;	
            flagArray[6]=1;
//			PutStr_H16W8(1,"open",16, 0);
//			ScreenRefresh();
		}
		if(strstr(receive,t2))           //关
		{
			flagArray[0]=0;
            flagArray[6]=1;
//			PutStr_H16W8(1,"close",16, 0);
//			ScreenRefresh();
		}	
		
		if(found(receive,t3,3))           //时     
		{
			flagArray[1]=1;
			flagArray[5]=1;
			flagArray[2]=foundnumber(receive,Mnum);	//
		PutNum_H16W8(1,Mnum,0,0,32,0);
		ScreenRefresh();
		}	
		if(found(receive,t4,3))           //分
		{
			flagArray[1]=1;	
			flagArray[5]=1;		
			flagArray[3]= foundnumber(receive,Mnum);
		PutNum_H16W8(1,Mnum,0,0,32,32);
		ScreenRefresh();
		}
		
		if(found(receive,t5,3))           //秒
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
		Dtime[0]=(flagArray[2]/10)+'0';//时H 0
		Dtime[1]=(flagArray[2]%10)+'0';//时L 1
		Dtime[2]=':';
		Dtime[3]=(flagArray[3]/10)+'0';//分H 3
		Dtime[4]=(flagArray[3]%10)+'0';//分L 4
		Dtime[5]=':';
		Dtime[6]=(flagArray[4]/10)+'0';//秒H 6
		Dtime[7]=(flagArray[4]%10)+'0';//秒L 7
		
		PutStr_H16W8(1,Dtime,16, 0);
		ScreenRefresh();
		
		downtime[0]=flagArray[2];
		downtime[1]=flagArray[3];
		downtime[2]=flagArray[4];
		flagArray[5]=1;
	}	
}



