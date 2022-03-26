#include "notes.h"
#include "timer0.h"
#include "bool.h"
#include "buzzer.h"

//note code test[] = {{A4,0.3},{D5,0.15},{E5,0.15},{F_5,0.3},{A5,0.3},{A5,0.45},{B5,0.15},{A5,0.3},{F_5,0.3},{D5,0.3},{D5,0.15},{E5,0.15},{F_5,0.3},{F_5,0.3},{E5,0.3},{E5,0.3},{D5,0.3},{A4,0.3},{A5,0.3},{F_5,0.3},{A5,0.3},{A5,0.45},{B5,0.15},{A5,0.3},{F_5,0.225},{E5,0.075},{D5,0.3},{D5,0.15},{E5,0.15},{F_5,0.3},{F_5,0.3},{E5,0.3},{D5,0.3},{E5,1.2},{F_5,0.3},{A5,0.3},{A5,0.45},{B5,0.15},{A5,0.3},{F_5,0.225},{E5,0.075},{D5,0.3},{D5,0.15},{E5,0.15},{F_5,0.3},{F_5,0.3},{E5,0.3},{E5,0.3},{D5,1.2},{F_5,0.075},{G5,0.525},{G5,0.6},{G5,0.3},{A5,0.075},{B5,0.825},{G5,0.075},{A5,0.525},{G5,0.075},{A5,0.225},{F_5,0.3},{E5,1.2},{F_5,0.3},{A5,0.3},{A5,0.45},{B5,0.15},{A5,0.3},{F_5,0.225},{E5,0.075},{D5,0.3},{D5,0.15},{E5,0.15},{F_5,0.3},{F_5,0.3},{E5,0.3},{E5,0.3},{D5,1.2},{END,0}};

unsigned char isPlaying = false;
unsigned int process = 0, duration = 0;
//note *song = 0;

note *songList[] = {
	&qingTian,
	&qingHuaCi,
	&yeQu
};

void playMusic(unsigned char index) {
	unsigned char quit = false;
	note *song = songList[index];
	/*setBeepHz(1);
	isPlaying = true;
	beepSong();*/
	while (true) {
		note *current = *song + process;
		if (current->pitch == END || quit) {
			process = 0;
			break;
		}
		quit = !beepTime(key2hz(current->pitch), (current->duration * 1000));
		process++;
	}
}

/*void Timer0Routine() interrupt 1 {
	static unsigned int T0Count;
	Timer0InitValue();
	T0Count++;
	if (T0Count >= 1000) {
		T0Count = 0;
		/*if (isPlaying) {
			if (test[process].pitch == END) {
				setBeepHz(0);
				process = 0;
				duration = 0;
				isPlaying = false;
			}
			// beep(key2hz(test[process].pitch), false);
			setBeepHz(key2hz(test[process].pitch));
			duration = (unsigned int)(test[process].duration * 1000);
			process++;
		}*
	}
}*/