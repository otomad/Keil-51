/****************简易信号发生器****************************
  程序功能：输出正弦波、锯齿波、方波、三角波
  涉及芯片：TLC5615
**********************************************************/
#include <regx52.h>
#include "stdio.h"
#include "tlc5615.h"
#include "uart.h"
#include "ChineseGBK.h"

#define uint unsigned int
#define uchar unsigned char
	
waveType type = NONE;
uchar duty = 50;

sbit din = P1 ^ 5;
sbit sclk = P1 ^ 6;
sbit cs = P1 ^ 7;
// sbit w2 = P1 ^ 0;
// #define PI acos(-1)

uchar code sine_tab[256] = {
	//输出电压从0到最大值（正弦波1/4部分）
	0x80, 0x83, 0x86, 0x89, 0x8d, 0x90, 0x93, 0x96, 0x99, 0x9c, 0x9f, 0xa2,
	0xa5, 0xa8, 0xab, 0xae, 0xb1, 0xb4, 0xb7, 0xba, 0xbc, 0xbf, 0xc2, 0xc5,
	0xc7, 0xca, 0xcc, 0xcf, 0xd1, 0xd4, 0xd6, 0xd8, 0xda, 0xdd, 0xdf, 0xe1,
	0xe3, 0xe5, 0xe7, 0xe9, 0xea, 0xec, 0xee, 0xef, 0xf1, 0xf2, 0xf4, 0xf5,
	0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfd, 0xfe, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff,
	//输出电压从最大值到0（正弦波1/4部分）
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfd, 0xfd, 0xfc, 0xfb, 0xfa,
	0xf9, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4, 0xf2, 0xf1, 0xef, 0xee, 0xec, 0xea,
	0xe9, 0xe7, 0xe5, 0xe3, 0xe1, 0xde, 0xdd, 0xda, 0xd8, 0xd6, 0xd4, 0xd1,
	0xcf, 0xcc, 0xca, 0xc7, 0xc5, 0xc2, 0xbf, 0xbc, 0xba, 0xb7, 0xb4, 0xb1,
	0xae, 0xab, 0xa8, 0xa5, 0xa2, 0x9f, 0x9c, 0x99, 0x96, 0x93, 0x90, 0x8d,
	0x89, 0x86, 0x83, 0x80,
	//输出电压从0到最小值（正弦波1/4部分）
	0x80, 0x7c, 0x79, 0x76, 0x72, 0x6f, 0x6c, 0x69, 0x66, 0x63, 0x60, 0x5d,
	0x5a, 0x57, 0x55, 0x51, 0x4e, 0x4c, 0x48, 0x45, 0x43, 0x40, 0x3d, 0x3a,
	0x38, 0x35, 0x33, 0x30, 0x2e, 0x2b, 0x29, 0x27, 0x25, 0x22, 0x20, 0x1e,
	0x1c, 0x1a, 0x18, 0x16, 0x15, 0x13, 0x11, 0x10, 0x0e, 0x0d, 0x0b, 0x0a,
	0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x02, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	//输出电压从最小值到0（正弦波1/4部分）
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0d, 0x0e, 0x10, 0x11, 0x13, 0x15,
	0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x25, 0x27, 0x29, 0x2b, 0x2e,
	0x30, 0x33, 0x35, 0x38, 0x3a, 0x3d, 0x40, 0x43, 0x45, 0x48, 0x4c, 0x4e,
	0x51, 0x55, 0x57, 0x5a, 0x5d, 0x60, 0x63, 0x66, 0x69, 0x6c, 0x6f, 0x72,
	0x76, 0x79, 0x7c, 0x80
};

/**
 * TLC5615 专用延时函数
 */
void _delay(uint t) {
	while (t--);
}

uchar bdata date; //位寻址
sbit date7 = date ^ 7;
sbit date6 = date ^ 6;
sbit date5 = date ^ 5;
sbit date4 = date ^ 4;
sbit date3 = date ^ 3;
sbit date2 = date ^ 2;
sbit date1 = date ^ 1;
sbit date0 = date ^ 0;

void write_sip5615(uchar k) {
	date = k;
	/*for (uchar i = 7; i >= 0; i--) {
		din = date ^ i;
		sclk = 0;
		sclk = 1;
	}*/
	#define writeDate(n) din = date##n; sclk = 0; sclk = 1
	writeDate(7);
	writeDate(6);
	writeDate(5);
	writeDate(4);
	writeDate(3);
	writeDate(2);
	writeDate(1);
	writeDate(0);
}

void write_5615(uint k) {
	k <<= 2;
	cs = 0;
	write_sip5615(k >> 8); //送高八位
	write_sip5615(k);	   //送低八位
	cs = 1;
}

/* void init_5615() {
	w2=0;
	_delay(500);
	w2=1;
	_delay(500);
} */

void showWave_5615(waveType _type, uchar _duty) {
	static uint sine_i = 0,
				saw_i = 0,
				tri_i;
	if (_duty > 100) _duty = 50;
	switch (_type) {
		case SINE: //正弦波
			write_5615(sine_tab[sine_i]); // 127.5 * (sin(PI / 128 * sine_i) + 1) // 如果用后面这个公式运行会变慢
			sine_i++;
			_delay(16);
			sine_i %= 256;
			break;
		case SAW: //锯齿波
			write_5615(saw_i);
			saw_i += 2;
			_delay(55);
			saw_i %= 250;
			break;
		case SQR: //方波（此处用单片机的I/O效果更好）
			write_5615(200);
			_delay(20 * _duty);
			write_5615(10);
			_delay(20 * (100 - _duty));
			break;
		case TRI: //三角波
			#define TRIANGULAR_LENGTH 278
			for (tri_i = 0; tri_i <= TRIANGULAR_LENGTH; tri_i++) {
				_delay(1);
				write_5615(tri_i);
			}
			for (tri_i = TRIANGULAR_LENGTH; tri_i > 0; tri_i--) {
				write_5615(tri_i);
				_delay(1);
			}
			break;
		default:
			break;
	}
}

void showWave() {
	showWave_5615(type, duty);
}

void setWaveType(waveType _type) {
	type = _type;
	if (type == NONE);
	else if (type == SQR) {
		char duty_str[6];
		sprintf(duty_str, "%d", (int)duty);
		PutString(duty_str);
		PutString(str_pwm);
	} else {
		PutString(
			type == SINE ? str_sin :
			type == SAW ? str_saw :
			str_tri
		);
	}
}
