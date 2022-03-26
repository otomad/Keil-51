#include <reg52.h>
//#include<STCY1Delay.h>
#include <lcd1602.h>
#include <intrins.h>
#ifndef udata
	#define udata
	typedef unsigned char uchar;
	typedef unsigned int uint;
	typedef unsigned long ulong;
#endif

sbit Echo=P1^0;//接受端
sbit Trig=P1^1;//控制端
uchar flag;//中断溢出标志

void Delay20us()
	{
		unsigned char i;
		_nop_();
		i = 7;
		while (--i);
	}
	
void Delay100ms() 
	{
		uchar i, j;
		i = 195;
		j = 138;
		do
		{
			while (--j);
		} while (--i);
	}

/*****************CSBofCM.h**************/
/*****************可用函数***************/
/************编写于2018/08/18************/
/**************作者：Belous**************/
/*******测试应用与STC89C516——12MHZ*******/
void CSB_Init(void);//初始化CSB（初始化定时器T0）
float CSB_GetOnce(void);//得到一次超声波测距模块的距离
void CSB_UseLcd1602Show(float distance);//显示距离到Lcd1602的第二行
/****************************************/

void CSB_UseLcd1602Show(float distance)
{
	uchar i,f1,f2,f3,f4;
	code uchar array1[]=" Overflow Show  ";
	code uchar array2[]=" CM             ";
	//Lcd1602_Init();//是否需要初始化？
	Lcd1602_Secondrow();//将在第二行显示
	if((distance>=700)||flag==1) //超出测量范围显示"Overflow Show"
	{	 
		flag=0;//清空中断溢出标志
		for(i=0;array1[i];i++)
			Lcd1602_WriteDC(array1[i],1);
	}
	else
	{
		f1=(uint)distance/100;//百位
		f2=(uint)distance/10%10;//十位
		f3=(uint)distance%10;//个位
		f4=(uint)(distance*10)%10;
		Lcd1602_WriteDC(f1+0x30,1);
		Lcd1602_WriteDC(f2+0x30,1);
		Lcd1602_WriteDC(f3+0x30,1);
		Lcd1602_WriteDC('.',1);
		Lcd1602_WriteDC(f4+0x30,1);
		for(i=0;array2[i];i++)
			Lcd1602_WriteDC(array2[i],1);
	}
}
float CSB_GetDistance(void)
{
	uint time=TH0*256+TL0;//得到总时间，单位us
	float distance=(float)(time*0.017);
	TH0=0;
	TL0=0;//清空定时器
	return distance;
}
void CSB_Time0() interrupt 1 //T0中断用来计数器溢出,超过测距范围
{
    flag=1;
}
void CSB_Rstart()//启动模块
{
	Trig=1;
	Delay20us();
	Trig=0;
	return;
}
void CSB_Init(void)
{
	TMOD=0x01;
	TH0=0;
	TL0=0;
	EA=1;          
	ET0=1;
}
float CSB_GetOnce(void)//得到一次超声波测距模块的距离
{
	CSB_Rstart();
	while(!Echo);//当Echo为零时等待
	TR0=1;//开启计数
	while(Echo);//当Echo为1计数并等待
	TR0=0;//关闭计数
	return CSB_GetDistance();//计算
}