#ifndef __BUZZER_H__
#define __BUZZER_H__

void beep(unsigned int hz);
unsigned char beepTime(unsigned int hz, unsigned int ms);
unsigned int key2hz(int key);
//void setBeepHz(unsigned int hz);
//void beepSong(void);

void playMusic(unsigned char index);

#endif
