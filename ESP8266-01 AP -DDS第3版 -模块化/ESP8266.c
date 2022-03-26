/**
��Ƭ����߿����õĲ�����Ϊ9600��������ͨ����������ESP8266��Ϊ9600������(AT+CIOBAUD=9600)��
��Ƭ�������õĲ�����ͨ�������жϳ�ʼ���������ʣ�����

**/
#include "public.h"		
#include "UART.h"		

/**
* @brief  ʵ�ַ���ָ���������
  * @param  *cmd
  * @retval 
  **/
void ESP8266_SendCmd(u8 *pbuf)
{
	while(*pbuf!='\n') //�����ո�����ѭ��	
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\n');//����
}



//ESP8266-WIFIģ�鹤��ģʽ��ʼ��AP�����
void ESP8266_ModeInit(void)
{
	

	
	ESP8266_SendCmd("AT+CWMODE=2\r\n");//����·����ģʽ 1 staTIonģʽ 2 AP�� ·����ģʽ 3 station+AP���ģʽ
	delay_ms(1000);
	
	ESP8266_SendCmd("AT+CWSAP=\"ESP8266\",\"12345678\",11,3\r\n"); //����WIFI�ȵ���������
	delay_ms(1000);
	
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"\r\n");//��������wifiģ��
	delay_ms(1000);

//	ESP8266_SendCmd("AT+RST");//��������wifiģ��
	ESP8266_SendCmd("AT+CIPMUX=1\r\n");	//����������ģʽ�����������ͻ��˽���
	delay_ms(1000);

	ESP8266_SendCmd("AT+CIPSERVER=1,8080\r\n");	//����TCP/IP �˿�Ϊ8080 ʵ�ֻ����������	
	delay_ms(1000);

}
