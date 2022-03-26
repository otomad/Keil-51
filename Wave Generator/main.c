#include <regx52.h>
#include "tlc5615.h"
#include "key.h"
#include "buzzer.h"
#include "delay.h"
#include "lcd1602.h"
#include "uart.h"
#include "ChineseGBK.h"

void main() {	
	LCD_Init();
	Init_USART();
	
	beepTime(880, 500);
	
	LCD_ShowString(1, 1, str_id1);
	LCD_ShowString(2, 1, str_enname1);
	PutString(str_id1);
	PutString(str_name1);
	delay(1000);
	
	LCD_ShowString(1, 1, str_id2);
	LCD_ShowString(2, 1, str_enname2);
	PutString(str_id2);
	PutString(str_name2);
	delay(1000);
	
	LCD_ShowString(1, 1, "Waveform  ");
	LCD_ShowString(2, 1, "Generator");
	PutString(str_title);
	
	while (1) {
		unsigned char k = Key();
		if (k) setWaveType((waveType)k);
		showWave();
	}
}