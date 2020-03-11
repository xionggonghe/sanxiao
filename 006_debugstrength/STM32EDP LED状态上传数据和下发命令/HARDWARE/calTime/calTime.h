#ifndef __calTime_H
#define __calTime_H
#include "Head.h"


extern unsigned short formtime;
extern unsigned short downtime[4];
extern  char ddtime[9];
extern unsigned short realtime[4];
extern  char Mnum;
extern unsigned char Dtime[9];
extern unsigned char timeArray[4];
extern unsigned char dayArray[7];
extern unsigned char flagArray[7];

void computertime();
void downtimeshow();
#endif







