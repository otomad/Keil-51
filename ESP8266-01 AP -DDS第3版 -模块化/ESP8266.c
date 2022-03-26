/**
单片机最高可设置的波特率为9600，所以先通过下载器把ESP8266改为9600波特率(AT+CIOBAUD=9600)；
单片机可设置的波特率通过串口中断初始化（波特率）设置

**/
#include "public.h"		
#include "UART.h"		

/**
* @brief  实现发送指令，并换新行
  * @param  *cmd
  * @retval 
  **/
void ESP8266_SendCmd(u8 *pbuf)
{
	while(*pbuf!='\n') //遇到空格跳出循环	
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\n');//换行
}



//ESP8266-WIFI模块工作模式初始化AP服务端
void ESP8266_ModeInit(void)
{
	

	
	ESP8266_SendCmd("AT+CWMODE=2\r\n");//设置路由器模式 1 staTIon模式 2 AP点 路由器模式 3 station+AP混合模式
	delay_ms(1000);
	
	ESP8266_SendCmd("AT+CWSAP=\"ESP8266\",\"12345678\",11,3\r\n"); //设置WIFI热点名及密码
	delay_ms(1000);
	
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"\r\n");//重新启动wifi模块
	delay_ms(1000);

//	ESP8266_SendCmd("AT+RST");//重新启动wifi模块
	ESP8266_SendCmd("AT+CIPMUX=1\r\n");	//开启多连接模式，允许多个各客户端接入
	delay_ms(1000);

	ESP8266_SendCmd("AT+CIPSERVER=1,8080\r\n");	//启动TCP/IP 端口为8080 实现基于网络控制	
	delay_ms(1000);

}
