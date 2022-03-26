/**
 * 按键说明：
 * ┌─┬─┬─┬─┐   ┌─┬─┬─┬─┐
 * │1│2│3│A│ → │1│2│3│+│
 * ├─┼─┼─┼─┤   ├─┼─┼─┼─┤
 * │4│5│6│B│ → │4│5│6│-│
 * ├─┼─┼─┼─┤   ├─┼─┼─┼─┤
 * │7│8│9│C│ → │7│8│9│×│
 * ├─┼─┼─┼─┤   ├─┼─┼─┼─┤
 * │*│0│#│D│ → │←│0│=│÷│
 * └─┴─┴─┴─┘   └─┴─┴─┴─┘
 * 先按 # 再按 0 表示 AC
 */

#include <regx52.h>
#include <intrins.h>
#include <stdio.h>
#include "buzzer.h"
#include "delay.h"
#include "lcd1602.h"
#include "matrixkey.h"
#include "str.h"
#include "timer0.h"
#include "calc.h"

#define alertBeep beepTime(880, 500)
#define alertBeepShort beepTime(880, 250)
#define alertBeepError beepTime(440, 1000)

//unsigned char cursor = 1;
char symbols[] = " 1234567890+-*/";
char input[17];

void showCursor(bit show) {
	unsigned char pos = str_getLength(input) + 1;
	LCD_ShowChar(1, pos, show ? '_' : ' ');
}

void main() {
	unsigned char keyNum;
	alertBeep;
	LCD_Init();
	
	LCD_ShowString(1, 1, "Calculator");
	LCD_ShowString(2, 1, "   + - * / =    ");
	delay(1000);

	LCD_ClearLine(2);
	LCD_ShowString(1, 1, "1900100324");
	LCD_ShowString(2, 1, "WangChenYi"); // 王晨懿
	delay(1000);

	LCD_ShowString(1, 1, "1900100327");
	LCD_ShowString(2, 1, "XueHan    "); // 薛涵
	delay(1000);
	
	LCD_WriteCommand(0x01);
	timer0Init(); // 之后初始化定时器 0，要不然一启动就能显示光标了
	showCursor(1);
	
	while (1) {
		unsigned char len = str_getLength(input);
		bit ac = 0; // 归零指示标志位
		keyNum = MatrixKey();
		if (keyNum) {
			if (!startTimer0 && keyNum <= 10) {
				input[0] = '\0';
				len = 0;
				if (keyNum == 10) ac = 1;
			}
			startTimer0 = 1;
			if (keyNum <= 14) { // 除 * # 键外
				#define syntaxError {\
					alertBeepError;\
					continue;\
				}
				if (len >= 16) syntaxError // 超过屏幕最大 16 位限度
				else {
					if (keyNum >= 11) { // + - * /
						if (symbols[keyNum] == '+' || symbols[keyNum] == '*' || symbols[keyNum] == '/') {
							if (
								len == 0 // 在开头输入除负号外的符号
								|| len == 1 && input[len - 1] == '-' // 在负号之后输入符号（开头）
								|| len > 1 && input[len - 1] == '-' && (input[len - 2] == '+' || input[len - 2] == '*' || input[len - 2] == '/') // 在负号之后输入符号（中间）
							) syntaxError
							if (input[len - 1] == '+' || input[len - 1] == '-' || input[len - 1] == '*' || input[len - 1] == '/') str_removeLast(input); // 可以更改符号
						} else if (symbols[keyNum] == '-') { // 减号亦可用作负号
							if (len == 1 && (input[len - 1] == '+' || input[len - 1] == '-' || input[len - 1] == '*' || input[len - 1] == '/')) syntaxError // 在负号之后输入符号
							if (len > 1 && (input[len - 1] == '+' || input[len - 1] == '-')) str_removeLast(input); // 可以更改符号
						}
					}
					if (!ac) str_appendChar(input, symbols[keyNum]);
				}
			}
			if (keyNum == 15) { // ←
				str_removeLast(input); // 清除末位
			}
			if (keyNum == 16) { // =
				double result;
				char result_str[17];
				if (input[0] == '\0') continue;
				while (input[str_getLength(input) - 1] < '0') str_removeLast(input); // 清除末尾多余的符号
				result = calc(input);
				sprintf(result_str, "%g", result); // 为什么不用 %16g？因为这样写当其为科学计数法时会出现位数的 bug
				str_rightAlign(result_str, 16);
				LCD_ShowString(2, 1, result_str);
				startTimer0 = 0;
			}
			LCD_ClearLine(1);
			LCD_ShowString(1, 1, input); // 更新显示屏，概率性会错位
			if (keyNum == 10) { // 0
				if (len > 0 && input[len - 1] == '/') { // 如果除以 0
					startTimer0 = 0;
					LCD_ShowString(2, 1, "Error ");
					alertBeepError;
					str_removeLast(input);
					LCD_ClearLine(1);
					LCD_ShowString(1, 1, input);
					LCD_ClearLine(2);
					startTimer0 = 1;
					showCursor(1);
				}
			}
			showCursor(1);
			alertBeepShort;
		}
	}
}

void timer0Routine() interrupt 1 {
	static unsigned int T0Count;
	static bit timeSetFlashFlag = 1; // 显示光标指示器
	TL0 = 0x18; // 设置定时初值
	TH0 = 0xFC; // 设置定时初值
	T0Count++;
	if (T0Count >= 500) { // 每 500ms 进入一次
		T0Count = 0;
		timeSetFlashFlag = !timeSetFlashFlag; // 光标指示器反转
		showCursor(timeSetFlashFlag);
	}
}
