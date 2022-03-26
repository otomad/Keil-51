#ifndef __TIMER0_H__
#define __TIMER0_H__

void Timer0Init(void);
void Timer0InitValue(void);
void setTimer0Start(unsigned char start);
void setTimeout(unsigned int t);

extern unsigned char timeout;

//void Timer1Init(void);
//void Timer1InitValue(void);

#endif
