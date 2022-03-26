#include <reg51.h>
#include <intrins.h> 
#include "intrins.h"
#include "stdio.h"

#define     u8  	   unsigned char
#define     u16      unsigned int
#define     uchar   unsigned char
#define     uint      unsigned int

//TLC5615端口定义
sbit tlc5615_clk=P1^0;
sbit tlc5615_cs=P1^1;
sbit tlc5615_di=P1^2;
sbit tlc5615_dout=P1 ^3;
//蜂鸣器端口定义
sbit BEEP=P3^6;
//显示屏端口定义
sbit LCDRS = P0^7;
sbit LCDRW = P0^6;
sbit LCDEN = P0^5;
sbit D0		 = P2^0;
sbit D1		 = P2^1;
sbit D2		 = P2^2;
sbit D3		 = P2^3;
sbit D4		 = P2^4;
sbit D5		 = P2^5;
sbit D6		 = P2^6;
sbit D7		 = P2^7;

unsigned char code zhx[]={64,67,70,73,76,79,82,85,88,91,94,96,99,102,104,106,109,111,113,115,117,118,120,121,123,124,125,126,126,127,127,127,127,127,127,127,126,126,125,124,123,121,120,118,117,115,113,111,109,106,104,102,99,96,94,91,88,85,82,79,76,73,70,67,64,60,57,54,51,48,45,42,39,36,33,31,28,25,23,21,18,16,14,12,10,9,7,6,4,3,2,1,1,0,0,0,0,0,0,0,1,1,2,3,4,6,7,9,10,12,14,16,18,21,23,25,28,31,33,36,39,42,45,48,51,54,57,60};

uchar strk[50];
	
//短暂延时函数
void delay_ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
	for(j=0;j<110;j++);
}
//延时函数
void delay(unsigned int us)
{
    while(us--)
	{
	   _nop_();
	}
}
//TLC5615数模转化函数
void tlc5615_dac(unsigned int dat)
{
    unsigned char i;
	dat<<=2;
	tlc5615_clk=0;
	tlc5615_cs=0;
	for(i=0;i<16;i++)
	{
	    tlc5615_di=(bit)(dat & 0x8000);
		tlc5615_clk=0;
		dat<<=1;
		tlc5615_clk=1;
	}
	tlc5615_cs=1;
	tlc5615_clk=0;
	delay(10);
}
//显示屏延时
void LCDdelay(uint z)		  //该延时大约100us（不精确，液晶操作的延时不要求很精确）
{
  uint x,y;
  for(x=z;x>0;x--)
  for(y=10;y>0;y--);
}
//显示屏写数据
void LCD_WriteData(u8 dat)	  
{
	if(dat&0x01)D0=1;else D0=0;
	if(dat&0x02)D1=1;else D1=0;
	if(dat&0x04)D2=1;else D2=0;
	if(dat&0x08)D3=1;else D3=0;
	if(dat&0x10)D4=1;else D4=0;
	if(dat&0x20)D5=1;else D5=0;
	if(dat&0x40)D6=1;else D6=0;
	if(dat&0x80)D7=1;else D7=0;
}
//显示屏写指令
void write_com(uchar com)
{
  LCDRS=0;				  
  LCD_WriteData(com);
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
//显示屏写数据
void write_data(uchar date)
{
  LCDRS=1;
  LCD_WriteData(date);
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
//在指定位置写入指定数据
void SelectPosition(unsigned char x,unsigned char y) 
{     
	if (x == 0) 
	{     
		write_com(0x80 + y);     //表示第一行
	}
	else 
	{      
		write_com(0xC0 + y);      //表示第二行
	}        
}
//写入字符串函数
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
	SelectPosition(x,y) ;
	while (*s) 
	{     
		write_data( *s);     
		s ++;     
	}
}
//写入字节函数
void LCD_Write_Char(u8 x,u8 y,u16 s,u8 l) 
{     
	SelectPosition(x,y) ;
	if(l>=5)
		write_data(0x30+s/10000%10);	//万位
	if(l>=4)
		write_data(0x30+s/1000%10);		//千位
	if(l>=3)
		write_data(0x30+s/100%10);		//百位
	if(l>=2)
		write_data(0x30+s/10%10);			//十位
	if(l>=1)
		write_data(0x30+s%10);		//个位
}
//初始化显示屏
void Init1602()
{
  uchar i=0;
  write_com(0x38);//屏幕初始化
  write_com(0x0c);//打开显示 无光标 无光标闪烁
  write_com(0x06);//当读或写一个字符是指针后一一位
  write_com(0x01);//清屏	
}
//串口初始化
void Usart_Init()
{ 
  TMOD=0X20;
  PCON=0X80;
  SCON=0X50;
  TH1=0XF3;
  TL1=0XF3;
  TR1=1;
  ES=1;
  EA=1;
}
//串口发送字节
void sendbyte(unsigned char byt)
{
 SBUF=byt;
 while(!TI);
 TI=0;
}  
//接收函数
void  receivebyte()       
{
  uchar shiftdata;
  static uint i=0;                             
  if(RI)
  {
  RI=0;
  shiftdata=SBUF;
  strk[i]=shiftdata; i++;
  if(shiftdata=='\0'||shiftdata=='\n'){i=0;}
   }                   
}
//串口发送字符串
void sendstring(unsigned char *string)
{       
 uint k;
 while(*string!='\0')
 {
 sendbyte(*string);
 delay(100);string++;
 }
 for(k=0;k<50;k++)
 strk[k]=0;
}

//主函数
void main(void)
{
	
	unsigned char i;
	LCDRW=0;
	Init1602();
	Usart_Init();
	BEEP=1;
	delay_ms(500);
    BEEP=0;	
	LCD_Write_String(0,0,"Yin Jian "); 
	LCD_Write_String(1,0,"1900100319");  
	delay_ms(1000);
	delay_ms(500);
	Init1602();
	LCD_Write_String(0,0,"TLC5615"); 
	LCD_Write_String(1,0,"Waveform generation"); 
    while(1)
	{
	if(strk[2]=='o'&&strk[3]=='u'&&strk[4]=='t'&&strk[5]=='p'&&strk[6]=='u'&&strk[7]=='t'
	&&strk[8]==' '&&strk[9]=='p'&&strk[10]=='w'&&strk[11]=='m'&&strk[12]==' '&&strk[13]=='5'
    &&strk[14]=='0'&&strk[15]=='\r'&&strk[16]=='\n'		
	)
	{
    for(i=255;i>0;i--)
    tlc5615_dac(250);		
    for(i=0;i<255;i++)
    tlc5615_dac(0);
	printf("50%占空比的PWM波已经输出...\r\n");
    }
    if(strk[2]=='o'&&strk[3]=='u'&&strk[4]=='t'&&strk[5]=='p'&&strk[6]=='u'&&strk[7]=='t'
	&&strk[8]==' '&&strk[9]=='p'&&strk[10]=='w'&&strk[11]=='m'&&strk[12]==' '&&strk[13]=='3'
    &&strk[14]=='0'&&strk[15]=='\r'&&strk[16]=='\n'		
	)
   {
    for(i=0;i<255;i++)
    tlc5615_dac(0);
    for(i=255;i>0;i--)
    tlc5615_dac(250);
	printf("30%占空比的PWM波已经输出...\r\n");
    }
   if(strk[2]=='o'&&strk[3]=='u'&&strk[4]=='t'&&strk[5]=='p'&&strk[6]=='u'&&strk[7]=='t'
	&&strk[8]==' '&&strk[9]=='s'&&strk[10]=='i'&&strk[11]=='n'&&strk[12]==' '&&strk[13]=='w'
    &&strk[14]=='a'&&strk[15]=='v'&&strk[16]=='e'&&strk[17]==' '&&strk[18]=='\r'&&strk[19]=='\n'				
	)
   {
	for(i=0;i<128;i++)
    {
    tlc5615_dac(zhx[i]);
	}
	printf("正弦波已经输出...\r\n");
    }
    if(RI)
    {
    sendstring(strk);
	}	
	}
}
//串口中断函数
void Usart_0() interrupt 4
{
  receivebyte();     
}






