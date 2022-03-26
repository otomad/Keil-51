#ifndef __MEASURING_H__
#define __MEASURING_H__

void measuringTrigger();
void measuringInit();
unsigned long int measuring();

sbit trig = P2 ^ 5;      //超声波测距模块Trig
sbit echo = P3 ^ 2;     //超声波测距模块Echo

extern unsigned char count;

#endif
