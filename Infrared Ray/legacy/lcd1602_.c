#include <reg52.h>
#include "lcd1602.h"

void Read_Busy()
{
    uchar busy;
    P0 = 0XFF;      //将P0复位
    RS = 0;
    RW = 1;
    do
    {
        EN = 1;
        busy = P0;
        EN = 0;      //以便下一次产生上升沿
    }while(busy & 0x80);
}
void LCD_Write_cmd(uchar cmd)   //写入操控lcd的指令
{
    Read_Busy();
    RS = 0;
    RW = 0;
    P0 = cmd;
    EN = 1;
    EN = 0;
}
void LCD_Write_dat(uchar dat)
{
    Read_Busy();
    RS = 1;
    RW = 0;
    P0 = dat;
    EN = 1;
    EN = 0;
}
void LCD_Init()
{
    LCD_Write_cmd(0x38);
    LCD_Write_cmd(0x0c);
    LCD_Write_cmd(0x06);
    LCD_Write_cmd(0x01);
}

void LCD1602_Display()
{
	uchar i;
	LCD_Write_cmd(0x80+0x04);		//第一行第五个
	for(i=0;i<4;i++)
	{
		if(cord[i]/16<10)
		{
			LCD_Write_dat(cord[i]/16 + 0x30);
		}
		else
		{
			LCD_Write_dat(cord[i]/16 + 0x37);
		}
		if(cord[i]%16<10)
		{
			LCD_Write_dat(cord[i]%16 + 0x30);
		}
		else
		{
			LCD_Write_dat(cord[i]%16 + 0x37);
		}
	}
}