#include "reg52.h"

void timer0_init() {
	EA = 1;
	TMOD |= 0x02;//设定定时器工作在方式二，8位定时器，12M，256us
	TH0 = 0;
	ET0 = 1;//打开定时器中断
	TR0 = 1;//打开定时器
}