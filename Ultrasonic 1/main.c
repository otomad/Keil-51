#include <lcd1602.h>
#include <CSBofCM.h>
//#include<STCY1Delay.h>
#ifndef udata
	typedef unsigned char uchar;
	typedef unsigned int uint;
	typedef unsigned long ulong;
#endif
void main(void)
{
	uchar i;
	code uchar array[]="CSB of CM Test";
	Lcd1602_Init();
	CSB_Init();
	for(i=0;array[i];i++)
		Lcd1602_WriteDC(array[i],1);
	while(1)
	{
		CSB_UseLcd1602Show(CSB_GetOnce());
		Delay100ms();
	}
	return;
}