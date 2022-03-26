#include <REGX52.H>

// 特别注意：波特率是 9600

/**
  * @brief  串口初始化
  * @param  无
  * @retval 无
  */
void UART_Init() {
//	SCON = 0x40;
//	PCON |= 0x80;
//	TMOD &= 0x0F;		//设置定时器模式
//	TMOD |= 0x20;		//设置定时器模式
//	TL1 = 0xF3;		//设定定时初值
//	TH1 = 0xF3;		//设定定时器重装值
//	ET1 = 0;		//禁止定时器1中断
//	TR1 = 1;		//启动定时器1
	EA=1;//中断模式开
	TMOD=0x20;//设置定时器1为工作方式2
	TH1=0xfd;//波特率是9600
	TL1=0xfd;
	TR1=1;
	SCON=0x40;
	PCON&=0x7F;
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART_SendByte(unsigned char Byte) {
	SBUF = Byte;
	while(TI == 0);
	TI = 0;
}

/**
  * @brief  串口发送一个字符串
  * @param  Byte 要发送的字符串数组
  * @retval 无
  */
void UART_String(unsigned char *str) {
	while (*str != '\0') {
		UART_SendByte(*str++);
	}
}
