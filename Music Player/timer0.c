#include <regx52.h>

#define TL 0x18
#define TH 0xfc

void Timer0Init(void) {
	TMOD &= 0xF0; //设置定时器模式
	TMOD |= 0x01; //设置定时器模式
	TL0 = TL; //设置定时初值
	TH0 = TH; //设置定时初值
	TF0 = 0; //清除TF0标志
	TR0 = 0; //定时器0开始计时
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

void Timer0InitValue(void) {
	TL0 = TL; //设置定时初值
	TH0 = TH; //设置定时初值
}

void setTimer0Start(unsigned char start) {
	TR0 = !!start;
}

unsigned int time;
unsigned char timeout = 1;

void setTimeout(unsigned int t) {
	time = t;
	timeout = 0;
	TR0 = 1;
}

void Timer0Routine() interrupt 1 {
	static unsigned int T0Count;
	Timer0InitValue();
	T0Count++;
	if (T0Count >= time) {
		T0Count = 0;
		timeout = 1;
		//TR0 = 0;
	}
}

/*定时器中断函数模板
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
}
*/
