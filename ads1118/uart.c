#include <regx52.h>
#include "ChineseGBK.h"

//------------------串口通信的数据包协议-----------------//
/*
	此程序的串口字符串通信使用到下面的一个自定义协议，每次通信都是发送或接收一个数据包，数据包格式解释如下(长度恒为15):
	例如:A01_fmq_01Off___#
	A--------数据包的开始标记(可以为A到Z,意味着数据包可以有26种)
	01-----设备代号
	fmq_01Off___--------指令(长度恒为10)，指令的前4个人字符是指令头部，指令的后6个字符是指令尾部
	#---------数据包的结束标记

	例如:A02_SenT010250#
	A--------数据包的开始标记(可以为A到Z,意味着数据包可以有26种)
	02-----设备代号
	SenT010250--------指令(长度恒为10)，指令的前4个人字符是指令头部，指令的后6个字符是指令尾部
	#---------数据包的结束标记
*/
#define LENGTH 20
char xdata RecvString_buf[LENGTH]; //定义数据包长度为19个字符

//这个是曾经用来控制温度传感模块（DS18B20）的数据包
//我随便定义了一个数据包用来做"心跳包"，比如单片机系统向电脑每2秒发送一次该数据包，如果电脑没有按时接收到，就认为单片机死掉了
//----------------------------------------------//
/*******************************
			串口通信
	MCU:89C52RC        11.0592MHz

//11.0592MHz 0xd0 1200bps
//12MHz 0xcc 1200bps
//11.0592MHz 0xfa 9600bps
//0xf4 11.0592MHz  0xf3 12MHz 4800bps
//均在SMOD=1的情况下（波特率倍增模式）
*******************************/
//串口发送函数
void PutString(unsigned char *TXStr) {
	ES = 0;
	while (*TXStr != 0) {
		SBUF = *TXStr;
		while (TI == 0);
		TI = 0;
		TXStr++;
	}
	ES = 1;
}
//串口接收函数
bit ReceiveString() {
	char *RecStr = RecvString_buf;
	char num = 0;
	unsigned char count = 0;
	while (1) {
		bit flag = 0;
		*RecStr = SBUF;
		count = 0;
		RI = 0;
		num++;
		RecStr++;
		while (!RI) {
			count++;
			if (count > 130) { //接收数据等待延迟，等待时间太久会导致CPU运算闲置，太短会出现"数据包被分割",默认count=130
				flag = 1;
				break;
			}
		}
		if (num >= LENGTH - 1 || flag) break; //数据包长度为19个字符,尝试连续接收19个字符
	}
	*RecStr = '\0';
	return 1;
}
//定时器1用作波特率发生器
void Init_USART() {
	EA = 1;
	
	SCON = 0x50; //串口方式1，使能接收
	TMOD |= 0x20; //定时器1工作方式2（8位自动重装初值）
	TMOD &= ~0x10;
	TH1 = 0xfa; // 9600bps
	TL1 = 0xfa;
	PCON |= 0x80; // SMOD=1
	TR1 = 1;
	TI = 0;
	RI = 0;
	// PS = 1; //提高串口中断优先级
	ES = 1; //开启串口中断使能
}
//比较指令头部
bit CompareCMD_head(char CMD_head[]) {
	unsigned char CharNum, quality;
	for (quality = 1; ; quality++)
		if (CMD_head[quality] == '\0')
			break;
	for (CharNum = 0; CharNum < quality; CharNum++) { //指令长度为10个字符
		if (RecvString_buf[CharNum] != CMD_head[CharNum]) {
			return 0; //指令头部匹配失败
		}
	}
	return 1; //指令头部匹配成功
}
//比较指令尾部(start:从哪里开始比较，quality:比较多少个字符，CMD_tail[]：要比较的字符串)
bit CompareCMD_tail(unsigned char start, unsigned char quality, char CMD_tail[]) {
	unsigned char CharNum;
	for (CharNum = 0; CharNum < quality; CharNum++) {
		if (!(RecvString_buf[start + CharNum] == CMD_tail[CharNum])) {
			return 0;
		}
	}
	return 1;
}
//统一转换为小写
void buff_strlwr() {
	unsigned char i;
	for (i = 0; i < LENGTH; i++)
		if (RecvString_buf[i] >= 'A' && RecvString_buf[i] <= 'Z')
			RecvString_buf[i] += 0x20;
}
bit dealUartCommand(unsigned char command); // 声明，定义在主函数
bit Deal_UART_RecData() { //处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）
	buff_strlwr(); //将输入的内容全部转换为小写，这样用户输入大小写都是可以的了
//	PutString(RecvString_buf);
	if (CompareCMD_head(str_commandStartMeasure)) return dealUartCommand(1);
	else if (CompareCMD_head(str_commandStopMeasure)) return dealUartCommand(2);
	else return 0;
//	return 1;
}
/************************
		中断函数
************************/
//串口中断服务函数-----------
void USART() interrupt 4 { //标志位TI和RI需要手动复位，TI和RI置位共用一个中断入口
	if (ReceiveString()) {
		//数据包长度正确则执行以下代码
		Deal_UART_RecData();
	} else {
		//数据包长度错误则执行以下代码
		// LED1=~LED1;
	}
	RI = 0; //接收并处理一次数据后把接收中断标志清除一下，拒绝响应在中断接收忙的时候发来的请求
}
/***************************
		主函数
***************************/
/* void main() {
	EA = 1;
	Init_USART(); //初始化串口中断通信，当串口接受完数据包后，如果检测到数据包包含有效指令，则自动执行对应的代码，执行完自动返回到主函数，为了尽可能不影响主函数的时序，串口中断函数的执行代码不要过复杂
	while (1) {
		//下面可以放要经常运行的用户代码，使用PutString()函数来发送数据包，如PutString(HeartBeat);
		//注:空格的ASCII码是:0x20,回车是:0x0D
	}
} */
