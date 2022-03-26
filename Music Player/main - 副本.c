#include "public.h"

unsigned char keyNum, flag = 0; //标志位

int keyAbs[] = { -10, -9, -7, -5, -4, -2, 0, 2, 3, 5 };
unsigned int correctPassword = 1111;

#define alertBeep beepTime(880, 250)
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
#define testBeep false

#if testBeep
void main() {
//	int i;
	Timer1Init();
//	while (true) {
//		for (i = 1; i <= 8; i++)
//			beepTime(key2hz(keyAbs[i]), 1000);
		setBeepHz(100);
//	}
}
#else
void main() {
	unsigned int count;
	unsigned long int password;
	Timer0Init();
	// Buzzer_Time(500); //响铃0.5s
	alertBeep;

	LCD_Init();

	LCD_ShowString(1, 1, "1900100103");
	LCD_ShowString(2, 1, "SuShangKan"); // 苏尚戡
	delay(1000);

	LCD_ShowString(1, 1, "1900100108");
	LCD_ShowString(2, 1, "LuoHao    "); // 罗皓
	delay(1000);

	LCD_WriteCommand(0x01); //光标复位，清屏

	LCD_ShowString(1, 1, "StudentID:");
	clearPassword;
	while (true) {
		keyNum = MatrixKey();
		if (keyNum) {
			LCD_ShowString(1, 14, "   ");
			if (keyNum >= 1 && keyNum <= 10) { //如果薄膜键盘上的0~9键位按下，输入密码
				if (count < 4) { //如果输入次数小于4
					password *= 10; //密码左移一位
					password += keyNum % 10; //获取一位密码
					count++; //计次加一
				}
				LCD_ShowNum(2, 1, password, count); //更新显示
			}
			if (keyNum == 20) { //如果按键A按键按下，确认
				if (password == correctPassword && count == 4) { //如果密码等于正确密码则运行后面程序
					alertOk;
					password = 0; //密码清零
					count = 0; //计次清零
					clearSecondLine;

					delay(1000);
					flag = 1; //解锁成功标志位置一

					if (flag == 1) LCD_WriteCommand(0x01); //光标复位，清屏
				} else { //否则
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

		if (flag == 1) { //进入电子琴模式
			TR0 = 0;
			LCD_ShowString(1, 1, "<Keyboard Mode>");
			while (true) {
				keyNum = MatrixKeyPress();
				if (keyNum) {
					LCD_ShowNum(2, 13, keyNum, 2);
					if (keyNum <= 10)
						beep(key2hz(keyAbs[keyNum % 10]));
					if (keyNum == 18) { //#键按下退出电子琴模式
						while (!MatrixKey());
						flag = 2;
						LCD_WriteCommand(0x01); //光标复位，清屏
						break;
					}
				}
			}
			if (flag == 2) { //音乐模式
				TR0 = 1;
				while (true) {
					LCD_ShowString(1, 1, "<Music Mode>             ");
					keyNum = MatrixKey();
					if (keyNum) {
						char songName[][13] = {
							"MiXueIceCity",
							"GG Bond     ",
							"QiJiZaiXian "
						};
						LCD_ShowNum(2, 13, keyNum, 2); //标题最多12个字母
						if (keyNum >= 1 && keyNum <= 3) {
							unsigned char k = keyNum - 1;
							LCD_ShowString(2, 1, songName[k]);
							playMusic(k);
						}
						/*if (keyNum == 1) { //播放歌曲 蜜雪冰城
							LCD_ShowString(2, 1, "MiXueIceCity");
							playMusic(1);
						}
						if (keyNum == 2) { //播放歌曲 猪猪侠
							LCD_ShowString(2, 1, "GG Bond");
							
						}
						if (keyNum == 3) { //播放歌曲 奇迹再现
							LCD_ShowString(2, 1, "QiJiZaiXian");
							
						}*/
						if (keyNum == 18) { //#键按下退出电子琴模式
							flag = 1;
							LCD_WriteCommand(0x01); //光标复位，清屏
							break;
						}
						if (keyNum == 17) { //*键开启或关闭定时器中断，你自由决定吧
							TR0 = !TR0;
							LCD_ShowString(1, 1, TR0 ? "Interrupt On " : "Interrupt Off");
							alertBeep;
							delay(500);
							continue;
						}
					}
					clearSecondLine;
				}
			}
		}
	}
}
#endif
