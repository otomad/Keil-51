#include <reg52.h>
#include <intrins.h>
#include "timer0.h"
#include "isr.h"

#define LED P1
sbit LED1 = LED ^ 0;
// sbit buzzer = P3 ^ 6;

unsigned char Delay100ms() { //@11.0592MHz
	unsigned char i, j;
	i = 180;
	j = 73;
	do {
		while (--j) if (ir_changed) {
			ir_changed = 0;
			return 1;
		}
	} while (--i);
	return 0;
}

void main() {
	unsigned char choose;
	timer0_init();
	init0_init();
	while (1) {
		choose = ir_getKey();
		switch (choose) { // case 13~17，这是因为这是遥控器键盘上的第 13~17 个键
			case 13: // 1-间隔发亮
				LED = 0x55;
				while (1) {
					if (Delay100ms()) break;
					LED = _crol_(LED, 1);
				}
				break;
			case 14: // 2-全亮
				LED = 0x00;
				break;
			case 15: // 3-三盏灯流水闪烁
				LED = 0x1f;
				while (1) {
					if (Delay100ms()) break;
					LED = _crol_(LED, 1);
				}
				break;
			case 16: // 4-一盏灯延时闪烁
				LED = 0xfe;
				while (1) {
					if (Delay100ms()) break;
					LED1 = !LED1;
				}
				break;
			case 17: // 5-点亮一盏 led 灯
				LED = 0xfe;
				break;
			default: // 其它按键均为不亮灯
				LED = 0xff;
				break;
		}
	}
}
