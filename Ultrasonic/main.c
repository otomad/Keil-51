#include <intrins.h>
#include <regx52.h>
#include "buzzer.h"
#include "delay.h"
#include "lcd1602.h"
#include "matrixkey.h"
#include "timer0.h"
#include "measuring.h"
#include "stdio.h"
#include "UART.h"
#include "ChineseGBK.h"

unsigned char keyNum, flag = 0; //标志位

#define alertBeep beepTime(880, 500)
#define alertOk {\
	LCD_ShowString(1, 14, "OK ");\
	alertBeep;\
}
#define alertError {\
	LCD_ShowString(1, 14, "ERR");\
	beepTime(440, 500);\
}
#define clearPassword LCD_ShowString(2, 1, "__________")
#define clearSecondLine LCD_ShowString(2, 1, "               ")
#define USER_ID_1 1900100136
#define USER_ID_2 1800020144

void main() {
	unsigned int count;
	unsigned long int password;
	alertBeep;

	LCD_Init(); //液晶初始化
	UART_Init(); //串口初始化
	
	LCD_ShowString(1, 1, "Ultrasonic"); // 超声波
	LCD_ShowString(2, 1, "Range Finder"); // 测距仪
	delay(1000);
	LCD_WriteCommand(0x01);

	LCD_ShowNum(1, 1, USER_ID_1, 10);
	LCD_ShowString(2, 1, "LuoZeFang "); // 罗泽芳
	delay(1000);

	LCD_ShowNum(1, 1, USER_ID_2, 10);
	LCD_ShowString(2, 1, "ShanZhiGao"); // 单志高
	delay(1000);

	LCD_WriteCommand(0x01); //光标复位，清屏

	LCD_ShowString(1, 1, "StudentID:"); //学号就是密码
	clearPassword;
	while (1) {
		keyNum = MatrixKey();
		if (keyNum) {
			LCD_ShowString(1, 14, "   ");
			if (keyNum >= 1 && keyNum <= 10) { //如果薄膜键盘上的0~9键位按下，输入密码
				if (count < 10) { //如果输入次数小于4
					password *= 10; //密码左移一位
					password += keyNum % 10; //获取一位密码
					count++; //计次加一
				}
				LCD_ShowNum(2, 1, password, count); //更新显示
			}
			if (keyNum == 20) { //如果按键A按键按下，确认
				if ((password == USER_ID_1 || password == USER_ID_2) && count == 10) { //如果密码等于正确密码则运行后面程序
					UART_String(str_correct);
					alertOk;
					password = 0; //密码清零
					count = 0; //计次清零
					clearSecondLine;

					delay(1000);
					flag = 1; //解锁成功标志位置一

					if (flag == 1) LCD_WriteCommand(0x01); //光标复位，清屏
				} else { //否则
					UART_String(str_wrong);
					alertError;
					password = 0; //密码清零
					count = 0; //计次清零
					clearPassword; //更新显示
				}
			}
			if (keyNum == 21) { //如果按键B按键按下，取消
				password = 0; //密码清零
				count = 0; //计次清零
				clearPassword; //更新显示
			}
		}

		if (flag == 1) { //进入
			timer0Init(); //定时器0初始化
			measuringInit(); //超声波相应端口初始化
			//为什么把初始化放在这里？因为之前的串口中断会干扰超声波正常运行。
			
			TR0 = 0;
			LCD_ShowString(1, 1, "Press # key to");
			LCD_ShowString(2, 1, "Start Measuring");
			while (1) {
				keyNum = MatrixKey();
				if (keyNum) {
//					LCD_ShowNum(2, 13, keyNum, 2);
					if (keyNum == 18) { //#键按下开始测量
						unsigned long int distance;
						float distance_f;
						char result[10];
						LCD_ShowString(1, 1, "Ranging,       ");
						LCD_ShowString(2, 1, "Please wait... ");
						measuringTrigger(); //触发超声波启动
						while(echo == 0); //等待回声
						distance = measuring(); //进行距离测量
						distance_f = distance / 100.0f;
						delay(500); //这个延迟不是必要的，本身计算就很快，这样前面的“测量中请稍等”的字都来不及显示。
						LCD_WriteCommand(0x01);
						LCD_ShowString(1, 1, "Distance:");
						sprintf(result, "%.2f cm", distance_f); //对测量结果进行显示
						LCD_ShowString(2, 1, result);
//						LCD_ShowNum(2, 1, distance, 4);
						measuringInit(); //超声波相应端口初始化
						beepTime(880, 300);
						delay(500);
						beepTime(880, 300);
						delayt(600); //每次测量间隔 60 ms
					}
				}
			}
		}
	}
}
