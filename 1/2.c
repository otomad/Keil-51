#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char
sbit LED1=P2^0;
sbit LED2=P2^1;
sbit LED3=P2^2;
sbit LED4=P2^3;
sbit KEY4=P3^2;
sbit KEY3=P3^3;
sbit KEY2=P3^4;
sbit KEY1=P3^5;
uchar flag = 0;
char table[] = {0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0X88,0X83,0XC6,0XA1,0X86,0X8E};
char spread[] = {0X18,0X24,0X42,0X81};
uint len = sizeof(spread)/sizeof(spread[0]);
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
void number(uint num){	//输入啥数就能显示啥数的函数
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
	uint i,j,k,mode,timer,num,flag_bulb,flag_timer;
	reset:
	i=0;j=0;k=0;mode=0;timer=0;num=0;flag_bulb=0;flag_timer=0;
	P1=0xff;LED1=1;LED2=1;
	while(1){
//		Delay_xms(10);
		if(!KEY1){
			P1=0xff;
			flag_bulb=1;
		}
		else if(flag_bulb){
			mode++;
			if(mode>1) mode=-1;
			flag_bulb=0;
		}
		if(!KEY2)
			flag_timer=1;
		else if(flag_timer){
			timer=!timer;
			flag_timer=0;
		}
		if(!KEY3) goto reset;
		if(mode && KEY1){
			P1 = ~spread[mode==1?i:len-1-i];
			i++;
			if(i==len) i=0;
		}
		for(j=0;j<50;j++)
			number(num);
		if(timer && KEY2){
			k++;
			if(k==10){
				k=0;
				num++;
				if(num>=20){
					num=0;
					mode=-mode;
				}
			}
		}
	}
}