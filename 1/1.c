#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char
sbit LED1=P2^0;
sbit LED2=P2^1;
sbit LED3=P2^2;
sbit LED4=P2^3;
sbit KEY1=P3^2;
sbit KEY2=P3^3;
sbit KEY3=P3^4;
sbit KEY4=P3^5;
uchar flag = 0;
char table[] = {0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0X88,0X83,0XC6,0XA1,0X86,0X8E};
void Delay_xms(int x) {	//延迟x毫秒
	uchar i, j, k;
	for(k=0;k<x;k++){
		i = 2;
		j = 199;
		do {
			while (--j);
		} while (--i);
	}
}
void number(uint num) {	//输入啥数就能显示啥数的函数
//		LED1=0;
		P0=table[num/1000%10];
		Delay_xms(1);
		LED1=1;
		P0=0xff;
		
//		LED2=0;
		P0=table[num/100%10];
		Delay_xms(1);
		LED2=1;
		P0=0xff;
		
		LED3=0;
		P0=table[num/10%10];
		Delay_xms(1);
		LED3=1;
		P0=0xff;
		
		LED4=0;
		P0=table[num%10];
		Delay_xms(1);
		LED4=1;
		P0=0xff;
}
void main(){
	LED1=1;
	LED2=1;	//前两位数字始终灭
	while(1){
		uint i,j;
		for(i=0;i<20;i++){
			for(j=0;j<1000/5;j++){
				number(i);
			}
		}
	}
	
}