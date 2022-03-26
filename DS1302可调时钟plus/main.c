#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"
#include "stdio.h"
#include "Delay.h"

#define YEAR DS1302_Time[0]
#define MONTH DS1302_Time[1]
#define DATE DS1302_Time[2]
#define HOUR DS1302_Time[3]
#define MINUTE DS1302_Time[4]
#define SECOND DS1302_Time[5]
#define DAY DS1302_Time[6]
#define ALARM_HOUR DS1302_Time[7]
#define ALARM_MINUTE DS1302_Time[8]

unsigned char KeyNum,MODE,TimeSetSelect,TimeSetFlashFlag;
sbit Beep=P3^6;
unsigned char code position[] = {
	1, 3, //年
	1, 6, //月
	1, 9, //日
	2, 1, //时
	2, 4, //分
	2, 7, //秒
	1, 12, //星期
	2, 10, //闹钟时
	2, 13 //闹钟分
};
char code dayName [][4] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

void beep() {
	unsigned char i;
	for (i = 0; i < 200; i++) {
		unsigned int j = 247;
		Beep = ~Beep;
		while (--j);
	}
}

void TimeShow(void) {//时间显示功能
	char line[2][15];
	DS1302_ReadTime();//读取时间
	sprintf(&line[0], "20%02d/%02d/%02d %s", (int)YEAR, (int)MONTH, (int)DATE, dayName[DAY % 7]);
	sprintf(&line[1], "%02d:%02d:%02d %02d:%02d", (int)HOUR, (int)MINUTE, (int)SECOND, (int)ALARM_HOUR, (int)ALARM_MINUTE);
	LCD_ShowString(1, 1, line[0]);
	LCD_ShowString(2, 1, line[1]);
	LCD_ShowNum(position[16], position[17], ALARM_MINUTE, 2); //未知不可名状bug，只能单独显示
}

char mod(char *a, char b) { //C++ 才可以写成 int &a
	return *a = (*a + b) % b;
}

char mod1(char *a, char b) { //越界归1，而不是归0
	return *a = (*a + b - 1) % b + 1;
}

void checkTimeLegal() {
	static unsigned int code BIG_MONTH = 0xad5; //0b101011010101
	mod(&YEAR, 100);//YEAR %= 100;//年越界判断
	mod1(&MONTH, 12);//月越界判断
	if (MONTH == 2) mod1(&DATE, YEAR % 4 ? 28 : 29);
	else mod1(&DATE, (BIG_MONTH & 1 << (MONTH - 1)) ? 31 : 30);//日越界判断
	mod(&HOUR, 24);//时越界判断
	mod(&MINUTE, 60);//分越界判断
	mod(&SECOND, 60);//秒越界判断
	mod1(&DAY, 7);//星期越界判断
	mod(&ALARM_HOUR, 24);//时越界判断
	mod(&ALARM_MINUTE, 60);//分越界判断
}

void TimeSet(void) {//时间设置功能
	unsigned char i;
	if(KeyNum==2) {//按键2按下
		TimeSetSelect++;//设置选择位加1
		TimeSetSelect %= 9;//越界清零
	}
	if(KeyNum==3) {//按键3按下
		DS1302_Time[TimeSetSelect]++;//时间设置位数值加1
		checkTimeLegal();
	}
	if(KeyNum==4) {//按键4按下
		DS1302_Time[TimeSetSelect]--;//时间设置位数值减1
		checkTimeLegal();
	}
	//更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
	#define DAY_INDEX 6
	for (i = 0; i < 9; i++)
		if (TimeSetSelect == i && TimeSetFlashFlag) LCD_ShowString(position[2 * i], position[2 * i + 1], i != DAY_INDEX ? "  " : "   ");
		else if (i != DAY_INDEX) LCD_ShowNum(position[2 * i], position[2 * i + 1], DS1302_Time[i], 2);
		else LCD_ShowString(position[2 * i], position[2 * i + 1], dayName[DAY % 7]);
}

void main() {
	beep();
	
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	
	LCD_ShowString(1, 1, "Clock");
	Delay(1000);
	
	LCD_ShowString(1, 1, "1900100313");
	LCD_ShowString(2, 1, "ZhangLin");
	Delay(1000);
	
	LCD_ShowString(1, 1, "1900100310");
	LCD_ShowString(2, 1, "TuMin   ");
	Delay(1000);
	
//	LCD_ShowString(1,1,"  -  -  ");//静态字符初始化显示
//	LCD_ShowString(2,1,"  :  :  ");
	
//	LCD_ShowString(1,10,"week:");
//	LCD_ShowString(2,12,":");
	
	DS1302_SetTime();//设置时间
	
	while(1) {
		KeyNum = Key();//读取键码
		if (KeyNum == 1) {//按键1按下
			MODE ? DS1302_SetTime() : (TimeSetSelect = 0);//功能切换
			MODE = !MODE;
		}
		MODE ? TimeSet() : TimeShow();//根据不同的功能执行不同的函数
		
		if (HOUR == ALARM_HOUR && MINUTE == ALARM_MINUTE)
			beep();
		//Beep=~Beep;
	}
}

void Timer0_Routine() interrupt 1 {
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count >= 500) {//每500ms进入一次
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag; //闪烁标志位取反
	}
}
