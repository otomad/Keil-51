#include <reg52.h>
#include "lcd1602.h"

void Init_INT0()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
}
void Init_timer0()
{
	EA = 1;
	TR0 = 1;
	TMOD = 0X02;		//八位自动重装
	ET0 = 1;
	TH0 = 0;
	TL0 = 0;
}

void INT_0() interrupt 0
{
	extern_num++;
	
	if(extern_num == 1)
	{
		time_num = 0;
	}
	else
	{
		if(time_num > 32)		//起始码判断
		{
			count = 0;
		}
		timerecord[count] = time_num;		//第一个是起始码，不需要
		time_num = 0;
		count++;
		if(count == 33)
		{
			extern_num = 0;
			flag_ok = 1;
		}
	}
}

void timer0() interrupt 1
{
	time_num++;		//256us
}
/*	1码的脉冲宽度为2.25ms
	0码的脉冲宽度为1.12ms	
	起始码的脉冲宽度为9ms	*/