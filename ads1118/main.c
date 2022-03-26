#include "ads1118.h"
#include "lcd1602.h"
#include "buzzer.h"
#include "key.h"
#include "delay.h"
#include "uart.h"
#include "ChineseGBK.h"

#define testVolt 0

bit enabled = 1;

void setState(bit state) {
	LCD_WriteCommand(0x01); //光标复位，清屏
	LCD_ShowString(1, 1, state ? "Press S1 Key to" : "Measurement");
	LCD_ShowString(2, 1, state ? "Measure Voltage" : "Over!");
	PutString(state ? str_welcome : str_stopMeasure);
	if (!state) beepTime(440, 500);
	enabled = state;
}

void startMeasureVolt() {
	float volt;
	char str_volt[10];
	if (!enabled) return;
	LCD_ShowString(1, 1, "Measuring Volt ");
	LCD_ShowString(2, 1, "Please Wait... ");
	PutString(str_startMeasure);
	volt = getVolt();
	sprintf(str_volt, "%.2f V", volt);
	delay(500);
	LCD_WriteCommand(0x01); //光标复位，清屏
	LCD_ShowString(1, 1, "Voltage:");
	LCD_ShowString(2, 1, str_volt);
	PutString(str_completeCaption);
	PutString(str_volt);
	PutString("\r\n");
	beepTime(880, 400);
	delay(1000);
	beepTime(880, 400);
}

void testMeasureVolt() {
	float volt = getVolt();
	char str_volt[10];
	sprintf(str_volt, "%.2f V", volt);
	LCD_ShowString(1, 1, "Voltage:");
	LCD_ShowString(2, 1, str_volt);
}

bit dealUartCommand(unsigned char command) {
	if (command < 1 || command > 2) return 0;
	if (command == 1) startMeasureVolt();
	else if (command == 2) setState(!enabled);
	return 1;
}

void main(void) {
	beepTime(880, 500);
	LCD_Init();
	ADS1118_Init();
	Init_USART();
	
	LCD_ShowString(1, 1, "Voltmeter");
	delay(1000);
	
	LCD_ShowString(1, 1, "1900100305");
	LCD_ShowString(2, 1, "TaoAnHang");
	delay(1000);
	
	LCD_ShowString(1, 1, "1900100336");
	LCD_ShowString(2, 1, "HeAnQi   ");
	delay(1000);
	
	LCD_ShowString(1, 1, "1900100316");
	LCD_ShowString(2, 1, "WangJing ");
	delay(1000);

	setState(1);
	while (1) {
		unsigned char keyNum = Key();
		switch (keyNum) {
			case 1:
				#if !testVolt
				startMeasureVolt();
				#else
				LCD_WriteCommand(0x01); //光标复位，清屏
				while (1) {
					testMeasureVolt();
				}
				#endif
				break;
			default:
				break;
		}
	}
}
