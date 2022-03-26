#include "public.h"

sbit buzzer = P3 ^ 6; //蜂鸣器端口

/**
  * @brief  Buzzer private delay function, delay for 500 μs.蜂鸣器私有延时函数，延时500us
  * @param  无
  * @retval 无
  */
void _buzzerDelay500Us() { //@12.000MHz
	unsigned char i; 
	_nop_();
	i = 247;
	while (--i);
}

/**
  * @brief  蜂鸣器私有延时函数
  * @param  无
  * @retval 无
  */
void _buzzerDelay(unsigned int i) {
	while (--i);
}

/**
  * @brief  蜂鸣器发声
  * @param  ms 发声的时长，范围：0~32767
  * @retval 无
  */
void Buzzer_Time(unsigned int ms) {
	unsigned int i;
	for (i = 0; i < ms * 2; i++) {
		buzzer = ~buzzer;
		_buzzerDelay(100);
	}
}
