#include "public.h"	


void UART_Init()  //9600bps@11.0592MHz
{
	EA=0;
 
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

 ES=0;         //串行中断允许
 TR1=1;        //启动定时器1
 EA=1;         //允许中断
}


void UART_SendByte(u8 dat)
{
	ES=0; //关闭串口中断
	TI=0; //清发送完毕中断请求标志位
	SBUF=dat; //发送
	while(TI==0); //等待发送完毕
	TI=0; //清发送完毕中断请求标志位
	ES=1; //允许串口中断
}
