
#ifndef __ADS1118_H__
#define __ADS1118_H__

#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;



#define CONFIG_VALUE    0X038B       //AIN0-AIN1  4.096  128sps  pull on DOUT


sbit SCLK = P1^1;
sbit MOSI = P1^2;
sbit MISO = P1^3;
sbit CS   = P1^0;

#define SCLK_H     SCLK = 1
#define SCLK_L     SCLK = 0

#define MOSI_H     MOSI = 1
#define MOSI_L     MOSI = 0

#define MISO_H     MISO = 1
#define MISO_L     MISO = 0

#define CS_H       CS = 1
#define CS_L       CS = 0

#define READ_MISO  P1 & 0X8  

void ADS1118_Init(void);
uint Write_ADS1118(uint dat,uchar mode);
void delay_ms(uint z);
int ADS_SEL_Read(uchar road,uchar Ref);

#endif
