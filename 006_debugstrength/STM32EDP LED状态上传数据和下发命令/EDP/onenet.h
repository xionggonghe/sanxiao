#ifndef _ONENET_H_
#define _ONENET_H_

extern char EDPFlag[];

_Bool OneNet_DevLink(void);

void OneNet_SendData(int data);

void OneNet_SendStringData(char * StringData);

void OneNet_RevPro(unsigned char *cmd);

_Bool OneNet_PushData(const char* dst_devid, const char* data, unsigned int data_len);
	
#endif
