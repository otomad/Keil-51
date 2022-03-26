#include <regx52.h>
#include "measuring.h"

/*------------------------------------------------
定时器0初始化，用于计算响应信号时间
------------------------------------------------*/
void timer0Init() {
	TMOD = 0x01;
	TL0 = 0x66;
	TH0 = 0xfc; //1ms
	ET0 = 1;
	EA = 1;
}

//……………………………………………中断服务函数…………………………………………………//
void timer0Routine() interrupt 1 {
	TF0 = 0;
	TL0 = 0x66;
	TH0 = 0xfc;
	count++;
	if (count == 18) {
		TR0 = 0;
		TL0 = 0x66;
		TH0 = 0xfc;
		count = 0;
	}
}