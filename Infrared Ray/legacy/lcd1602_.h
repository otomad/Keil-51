#ifndef __LCD1602_H__
#define __LCD1602_H__

#define uchar unsigned char
#define uint  unsigned int

sbit RS = P3^5;
sbit RW = P3^6;
sbit EN = P3^4;
sbit dula = P2^6;
sbit wela = P2^7;

extern uchar time_num,extern_num;
extern uchar timerecord[33];
extern uchar cord[4];
extern uchar flag_ok;
extern uchar count;

void LCD_Init();
void LCD1602_Display();

#endif
