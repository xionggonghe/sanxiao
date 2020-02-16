#include "findFun.h"
#include "math.h"
#include "string.h"
extern unsigned char Mnum;

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
	for(i=0;!(lnum-i>=0&&a[lnum-i]<='9'&&a[lnum-i]>='0');i++);    //Ìø¹ıÎŞ¹Ø×Ö·û
	for(j=0;lnum-i>=0&&a[lnum-i]<='9'&&a[lnum-i]>='0';j++,i++)
	{
		re=re+(a[lnum-i]-'0')*pow(10,j); 
//		a[lnum-i]=1;			
	}
	return re;
}
