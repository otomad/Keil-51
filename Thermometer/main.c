#include <regx52.h>
#include <stdio.h>
#include "ds18b20.h"
#include "lcd1602.h"
#include "buzzer.h"
#include "delay.h"
#include "key.h"
#include "uart.h"
#include "ChineseGBK.h"

bit enabled = 1;

void setState(bit state) {
	LCD_WriteCommand(0x01); //光标复位，清屏
	LCD_ShowString(1, 1, state ? "Press S1 Key to" : "Measurement");
	LCD_ShowString(2, 1, state ? "Measure Temp" : "Over!");
	PutString(state ? str_welcome : str_stopMeasure);
	if (!state) beepTime(440, 500);
	enabled = state;
}

void startMeasureTemp() {
	float temp;
	char xdata str_temp_lcd[10],
		 xdata str_temp_uart[13];
	if (!enabled) return;
	LCD_ShowString(1, 1, "Measuring Temp");
	LCD_ShowString(2, 1, "Please Wait...");
	PutString(str_startMeasure);
	temp = ReadTemp();
	delay(500);
	sprintf(str_temp_lcd, "%.2f %cC", temp, 0xdf); // 0xdf - 温度符号（LCD1602 - Shift-JIS 编码）
	sprintf(str_temp_uart, "%.2f %c%cC\r\n", temp, 0xa1, 0xe3); // 0xa1, 0xe3 - 温度符号（GBK 编码）
	LCD_WriteCommand(0x01); //光标复位，清屏
	LCD_ShowString(1, 1, "Temperature:");
	LCD_ShowString(2, 1, str_temp_lcd);
	PutString(str_completeCaption);
	PutString(str_temp_uart);
	beepTime(880, 220);
	delay(800);
	beepTime(880, 220);
}

bit dealUartCommand(unsigned char command) {
	if (command < 1 || command > 2) return 0;
	if (command == 1) startMeasureTemp();
	else if (command == 2) setState(!enabled);
	return 1;
}

void main() {
	beepTime(880, 500);
	LCD_Init();
	Init_USART();
	
	LCD_ShowString(1, 1, "Thermometer");
	delay(1000);
	LCD_ClearLine(1);
	
	LCD_ShowString(1, 1, "1900100329");
	LCD_ShowString(2, 1, "HuJing");
	delay(1000);
	
	LCD_ShowString(1, 1, "1900100335");
	LCD_ShowString(2, 1, "HeWen ");
	delay(1000);
	
	setState(1);
	while (1) {
		unsigned char keyNum = Key();
		switch (keyNum) {
			case 1:
				startMeasureTemp();
				break;
			default:
				break;
		}
	}
}
