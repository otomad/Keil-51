#include "public.h"		



//数码管段码表
//unsigned char shumaguan[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0X7F,0x6F};//共阴数码管显示数字的数组
unsigned char shumaguan[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};//共阳数码管显示数字的数组
void Nixie(unsigned char Lacation,Num)
{
	switch(Lacation)
	{
		case 1:P2_0=0;P2_1=1;P2_2=1;P2_3=1;break;
		case 2:P2_0=1;P2_1=0;P2_2=1;P2_3=1;break;
		case 3:P2_0=1;P2_1=1;P2_2=0;P2_3=1;break;
		case 4:P2_0=1;P2_1=1;P2_2=1;P2_3=0;break;
	}
	P0=shumaguan[Num];
	delay_ms(1);
	P0=0xff;
}
