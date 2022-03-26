#ifndef __ISR_H__
#define __ISR_H__

extern unsigned char irok;
extern unsigned char irnum[];
extern unsigned char ir_changed;

void init0_init();
void ir_process();
unsigned char ir_getKey();

#endif
