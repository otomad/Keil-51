#include <regx52.h>

#define TL 0xf7
#define TH 0xff

void Timer1Init(void) {
	TMOD &= 0x0F; //设置定时器模式
	TMOD |= 0x10; //设置定时器模式
	TL1 = TL; //设置定时初值
	TH1 = TH; //设置定时初值
	TF1 = 0; //清除TF1标志
	TR1 = 1; //定时器1开始计时
	ET1 = 1;
	EA = 1;
	PT0 = 0;
}

void Timer1InitValue(void) {
	TL0 = TL; //设置定时初值
	TH0 = TH; //设置定时初值
}
