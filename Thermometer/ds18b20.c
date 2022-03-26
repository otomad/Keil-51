// 温度初始化设定是 85°C，再次测量即可

#include <regx52.h>
#include <intrins.h>
#include "ds18b20.h"

//管脚定义
sbit DQ = P1 ^ 0; // 默认 P3 ^ 0 貌似影响串口正常工作

//uchar TempBuffer[8];

/*************************************
函数名：delaydq
函数功能：延时
参数：i(秒数)
返回：无
备注：无
*************************************/
void delaydq(uchar i) {
	while (i--);
}
/*************************************
函数名：dqinit
函数功能：初始化
参数：无
返回：无
备注：无
*************************************/
void dqinit(void) {
	uchar x = 0;
	DQ = 1;		 // DQ复位
	delaydq(8);	 //稍做延时
	DQ = 0;		 //单片机将DQ拉低
	delaydq(80); //精确延时 大于 480us
	DQ = 1;		 //拉高总线
	delaydq(14);
	x = DQ; //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	delaydq(20);
}
/*************************************
函数名：re1820b
函数功能：读数据
参数：无
返回：dat(数据)
备注：无
*************************************/
uchar re1820b(void) {
	uchar i = 0;
	uchar dat = 0;
	for (i = 8; i > 0; i--) {
		DQ = 0; // 给脉冲信号
		dat >>= 1;
		DQ = 1; // 给脉冲信号
		if (DQ)
			dat |= 0x80;
		delaydq(4);
	}
	return (dat);
}
/*************************************
函数名：wr1820b
函数功能：写数据
参数：dat(数据)
返回：无
备注：无
*************************************/
void wr1820b(uchar dat) {
	unsigned char i = 0;
	for (i = 8; i > 0; i--) {
		DQ = 0;
		DQ = dat & 0x01;
		delaydq(5);
		DQ = 1;
		dat >>= 1;
	}
}
/*************************************
函数名：ReadTemp
函数功能：温度转换
参数：无
返回：无
备注：无
*************************************/
float ReadTemp(void) {
	uchar temp_value, temp2, xs, fflag = 0;
	
	unsigned char a = 0;
	unsigned char b = 0;
	unsigned char t = 0;
	
	float result;

	dqinit();
	wr1820b(0xCC); // 跳过读序号列号的操作
	wr1820b(0x44); // 启动温度转换

	delaydq(100);
	dqinit();
	wr1820b(0xCC); //跳过读序号列号的操作
	wr1820b(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度

	delaydq(100);

	a = re1820b(); //读取温度值低位
	b = re1820b(); //读取温度值高位
	if (b > 127) {
		b = (256 - b);
		a = (256 - a);
		fflag = 1; //负温度求补码
	}
	temp_value = b << 4;
	temp_value += (a & 0xf0) >> 4;
	temp2 = a & 0x0f; //小数的值

	xs = temp2 * 0.0625 * 100; //小数位,若为0.5则算为5来显示   xs小数
//	temp_to_str();
//	return TempBuffer;
	result = temp_value + xs * 0.01;
	if (fflag) result *= -1;
	return result;
}

/*************************************
函数名：temp_to_str
函数功能：温度显示
参数：无
返回：无
备注：无
*************************************/
/*void temp_to_str() //温度数据转换成液晶字符显示
{
	if (fflag == 1) {
		TempBuffer[0] = '-';
		TempBuffer[1] = temp_value / 10 + '0'; //十位
		TempBuffer[2] = temp_value % 10 + '0'; //个位
		TempBuffer[3] = '.';
		TempBuffer[4] = xs + '0';
		TempBuffer[5] = 0xdf; //温度符号
		TempBuffer[6] = 'C';
		TempBuffer[7] = '\0';
	} else {
		TempBuffer[0] = '+';
		TempBuffer[1] = temp_value / 10 + '0'; //十位
		TempBuffer[2] = temp_value % 10 + '0'; //个位
		TempBuffer[3] = '.';
		TempBuffer[4] = xs + '0';
		TempBuffer[5] = 0xdf; //温度符号
		TempBuffer[6] = 'C';
		TempBuffer[7] = '\0';
	}
}*/