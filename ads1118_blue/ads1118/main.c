
#include "ads1118.h"





void Init(void)    //�˺������㴮�ڵ���
{
   TMOD = 0X20;
   TH1 = 0XFD;
   TR1 = 1;
   SM1 = 1;SM0 = 0;
   REN = 1;
   TI = 1;
}
float Volt_Value;
void main(void)
{
	uint value;
	Init();
	EA = 1;
	ADS1118_Init();
	Write_ADS1118(CONFIG_VALUE,0);
	while(1)
	{
		delay_ms(1000);
		value  =  ADS_SEL_Read(5,0);  //6.144
		printf("value = %d\n\r",value);
		Volt_Value = value*6.144/32768;   //�ڶ���ת��ʱ��������ǰһ�εĵ�ѹ
		printf("Volt_Value %f\n\r\n\r",Volt_Value);
  }
}

