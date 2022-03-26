#ifndef __LCD1602_H__
#define __LCD1602_H__

void Lcd1602_WriteDC(unsigned char com,unsigned char control);
void Lcd1602_Init(void);
void Lcd1602_Secondrow(void);

#endif
