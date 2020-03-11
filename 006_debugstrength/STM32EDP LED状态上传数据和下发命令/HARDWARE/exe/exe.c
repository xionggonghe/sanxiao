#include "Exe.h"
#include "Head.h"

extern unsigned short downtime[4];
extern char Mnum;
extern unsigned char Dtime[9];
extern unsigned char timeArray[4];
extern unsigned char dayArray[7];
extern unsigned char flagArray[7];
extern unsigned short realtime[4];

void timeexecute()
{
	unsigned char i;
	DS1302_GetTime();
	if(flagArray[5]==1)
	{
		computertime();
	}
	if(flagArray[1]==0&&flagArray[6]==1&&flagArray[5]==0)
	{
		relayexe();
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
			relayexe();
		}
	}
}










