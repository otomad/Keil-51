void delay(unsigned int ms) { // @11.0592MHz
	unsigned char i, j;
	while (ms--) {
		i = 2;
		j = 199; // 239
		do {
			while (--j);
		} while (--i);
	}
}

/*------------------------------------------------
             超声波专用延时函数
------------------------------------------------*/
void delayt(unsigned int x) {
    unsigned char j;
    while (x-- > 0) {
		for (j = 0; j < 125; j++);
    }
}
