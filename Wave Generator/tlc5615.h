#ifndef __TLC5615_H__
#define __TLC5615_H__

typedef enum {
	NONE = 0,
	SINE,
	SAW,
	SQR,
	TRI
} waveType;

void write_5615(unsigned int k);
void showWave_5615(unsigned char type, unsigned char duty);
void showWave();
void setWaveType(waveType type);

extern waveType type;
extern unsigned char duty;

#endif
