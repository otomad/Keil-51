#include <reg52.h>

unsigned char irtime = 0;
unsigned char irdate[33] = 0;
unsigned char bitnum = 0;
unsigned char irok = 0;
unsigned char irnum[4];
unsigned char ir_changed = 0;
bit start_flag = 0;
// sbit buzzer = P3 ^ 6;

void init0_init() {
	EA = 1;
	IT0 |= 1; //设置中断为下降沿触发
	EX0 = 1; //打开外部中断
}

void timer0_isr() interrupt 1 {
	irtime++;
}

void init0_isr() interrupt 0 {
	if(start_flag) {
		if((irtime > 45) && (irtime < 60))
			bitnum = 0;
        irdate[bitnum] = irtime;
        bitnum++;
        irtime = 0;
        if(33 == bitnum) {
			bitnum = 0;
			irok = 1;
			ir_changed = 1;
        }                
    } else {
		irtime = 0;
		start_flag = 1;
    }
}

void ir_process() {
	unsigned char i = 0, j = 0, k = 1, temp;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 8; j++) {
			temp >>= 1;
			if(irdate[k] > 6)
				temp +=0x80;
			k++;
		}
		irnum[i] = temp;
	}
}

unsigned char return_choose(unsigned char num) {
	unsigned char i, code remoteCode[] = {0x45,0x46,0x47,0x44,0x40,0x43,0x07,0x15,0x09,0x16,0x19,0x0D,0x0C,0x18,0x5E,0x08,0x1C,0x5A,0x42,0x52,0x4A};
	for (i = 0; i < 21; i++)
		if (remoteCode[i] == num)
			return i + 1;
	return 0;
}

unsigned char ir_getKey() {
	unsigned char choose;
	if (irok) {
		ir_process();
        if ((irnum[0] + irnum[1] == 0xff) && (irnum[2] + irnum[3] == 0xff)) {                            
			choose = return_choose(irnum[2]);
		}
	} else choose = 0;
	return choose;
}

/*unsigned char chose = 0;
unsigned char ir_changed() {
	unsigned char choose = ir_getKey();
	unsigned char isChanged = chose != choose;
	chose = choose;
	return isChanged;
}*/
