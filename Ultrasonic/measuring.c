#include <regx52.h>
#include <intrins.h>
#include "measuring.h"
#include "delay.h"

#define uint unsigned int
#define uchar unsigned char

uchar count;           //中断累加变量

/*------------------------------------------------
    超声波模块触发信号
------------------------------------------------*/
void measuringTrigger() {
	unsigned char i;
    trig = 0;
	for (i = 0; i < 42; i++) _nop_();
	trig = 1;       
}


/*------------------------------------------------
   超声波模块相应端口初始化函数
------------------------------------------------*/
void measuringInit() {
	trig = 1;
	echo = 1;
	count = 0;
}


/*------------------------------------------------
     超声波模块距离测试函数
------------------------------------------------*/
unsigned long int measuring() {
	unsigned long int distance;
	uchar l;
	uint h, y;
	TR0 = 1;
	while (echo == 1);    
	TR0 = 0;
	l = TL0;
	h = TH0;
	y = (h << 8) + l;
	y = y - 0xfc66;//us部分
	distance = y + 1000 * count;//计算总时间,单位是微秒
	TL0 = 0x66;
	TH0 = 0xfc;
	delayt(30);
	distance = 34000 * distance / 20000;//原始为: (0.34毫米 / us) * 时间 / 2
	return distance;
}
