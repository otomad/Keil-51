#include <regx52.h>

sbit buzzer = P3 ^ 6; //Buzzer port

/**
 * @brief  Buzzer private delay function, used for change pitch.
 * @param  {unsigned int} i - Delay time which is for pitch.
 * If i is equal to 10, the delay time is about 100μs, and different sound effects can be achieved by modifying the delay time.
 * @retval None
 */
void buzzerDelay(unsigned int i) {
	while (--i);
}

/**
 * @brief  Buzzer singing!
 * @param  {unsigned int} ms - Singing time, range: 0~32767
 * @retval None
 */
void beepTime(unsigned int hz, unsigned int ms) {
	unsigned int i, cms = 1e5 / hz, len = ms / cms * 100;
	for (i = 0; i < len; i++) {
		buzzer = !buzzer;
		buzzerDelay(cms);
	}
}
