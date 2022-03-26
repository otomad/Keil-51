#include "public.h"
#include <math.h>

typedef unsigned int uint;
typedef unsigned char uchar;

sbit buzzer = P3 ^ 6; //Buzzer port
uint beepCms = 0;

/**
 * @brief  Buzzer private delay function, delay for 500 microseconds.
 * @param  None
 * @retval None
 */
void buzzerDelay500Us() { //@12.000MHz
	uchar i;
	_nop_();
	i = 247;
	while (--i);
}

/**
 * @brief  Buzzer private delay function, used for change pitch.
 * @param  {unsigned int} i - Delay time which is for pitch.
 * If i is equal to 10, the delay time is about 100μs, and different sound effects can be achieved by modifying the delay time.
 * @retval None
 */
void buzzerDelay(uint i) {
	while (--i);
}

/**
 * @brief  Transform Hz to cms.
 * @param  {unsigned int} hz - Hz.
 * @retval {unsigned int} cms.
 */
unsigned int hz2cms(uint hz) {
	if (!hz) return 0;
	return 1e5 / hz;
}

/**
 * @brief  Buzzer singing!
 * @param  {unsigned int} ms - Singing time, range: 0~32767
 * @retval None
 */
uchar beepTime(uint hz, uint ms) {
	if (!TR0) {
		uint i,
			cms = hz2cms(hz),
			len = ms / cms * 100;
		if (!hz) return false;
		for (i = 0; i < len; i++) {
			buzzer = ~buzzer;
			buzzerDelay(cms);
			if (MatrixKey()) return false;
		}
	} else {
		uint cms = hz2cms(hz);
		if (!hz) return false;
		setTimeout(ms);
		while (true) {
			buzzer = ~buzzer;
			buzzerDelay(cms);
			if (MatrixKey()) return false;
			if (timeout) break;
		}
	}
	return true;
}

void beep(uint hz) {
	uint cms = hz2cms(hz);
	while (hz) {
		buzzer = ~buzzer;
		buzzerDelay(cms);
		if (!MatrixKeyPress()) break;
	}
//	beepCms = hz2cms(hz);
	//if (!MatrixKeyPress()) beepCms = 0;
}

uint key2hz(int key) {
	static uint CENTER_A = 880;
	return CENTER_A * pow(2, key / 12.0);
}

/*void setBeepHz(uint hz) {
	beepCms = hz2cms(hz);
}

void beepSong(void) {
	while (beepCms) {
		buzzer = ~buzzer;
		buzzerDelay(beepCms);
	}
}

void Timer1Routine() interrupt 3 {
	static unsigned int T1Count;
	Timer1InitValue();
	T1Count++;
	if (T1Count >= beepCms/* && beepCms != 0*) {
		T1Count = 0;
		buzzer = ~buzzer;
	}
}*/
