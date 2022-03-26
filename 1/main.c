#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

// uchar led[17]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0X88,0X83,0XC6,0XA1,0X86,0X8E};

void delay(uint ms) { //@11.0592MHz
	uchar i, j;
	uint k;
	for(k = 0; k < ms; k++){
		_nop_();
		i = 2;
		j = 199;
		do {
			while (--j);
		} while (--i);
	}
}


void delay10us(uint i) { //@11.0592MHz
	while(i--);	
}

void main() {
	uint i, direct = 1;
	P2 = 0xfe;
	while(1) {
		for (i = 0; i < 8; i++) {
			P2 = ~(direct ? 1 << i : 0x80 >> i);
			delay(1000); // 等 1000 毫秒
		}
		direct = !direct;
	}
}
