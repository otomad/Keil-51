#include "public.h"			 
#include "String.h"
#include "Buzzer.h"
#include "string.h"
#include "Nixie.h"
#include "Key.h"
#include <intrins.h>
#include "UART.h"
#include "ESP8266.h"
#include "CheckSum.h"

 #define CommLenMax		    50     //ͨ�Ż���������

void ServiceCOMM();

sbit Buzzer=P3^6;

bit mode=0;




unsigned char TrLen=0, TrCount=0, RecCount=0, RecChar=0, RecLen=0;  //
bit  RecOK;
unsigned char idata ComBuf[CommLenMax];//Ҫִ�У��ײ㣩ָ��Ĵ�������
u8 RecBuf[50];//ָ��Ĵ�������

unsigned char  AT[17]={'A','T','+','C','I','P','S','E','N','D','=','0',',','0','9','\r','\n'};


/*unsigned int word; //ռ��2���ֽ�
unsigned char byte[2];//ռ��2��1�ֽ�,Ҳ��2�ֽ�
union��ʾword��byte[0],byte[1]������ͬ��ַ��2�ֽ��ڴ���
1¥�Ĳ�Ҫ�󵼱���Ŷ
��typedef ������һ������������,��������ΪunWord16*/

  union
 {
	unsigned char uc_Byte[2];
	unsigned int  ui_Word;//У����
 }un_CRCResult;
 



//������***********************************************
void main()
{	
	UART_Init(); //���ڳ�ʼ��
	ESP8266_ModeInit();
	ES=1; //�������ж�
	
	while(1)
	{		
		if(RecOK)
		{
			EA=0;
			ComBuf[RecLen]='\r';//\n��Ϊ�����Ƿ������־
			ComBuf[RecLen+1]='\n';
			ComBuf[RecLen+2]='\0';
			ServiceCOMM();
			ESP8266_SendCmd(AT);
				delay_ms(3);
			ESP8266_SendCmd(ComBuf);
			memset(ComBuf,'\0',50);
			memset(RecBuf,'\0',50);//����Ҫ�Ĳ���
			RecOK=0;
			EA=1;
		
		}

	}		
}

#if 1
void Usart() interrupt 4
{
	static u8 i=0;
	static u8 j=0;
	
	if(RI)
	{
		RI=0;	
		RecBuf[i++]=SBUF;//Ҫע�����i��ȡֵ��ʵ��Ϊ�ַ����ַ����ȡ�
		if(RecBuf[i-1]=='\n')
		{
			i=0;
			memset(RecBuf,'\0',50);
		}	
		if(RecBuf[9]==0x55)
			{
				ComBuf[j]=RecBuf[9+j];				
				j++;
			
		if(i>13)
			{
				RecLen=RecBuf[11]+6;
				if(j>RecLen-1)
				{
					j=0;
					RecOK=1;
					if(RecLen>9)
					{
						AT[13]=RecLen/10 +'0';
						AT[14]=RecLen%10 +'0';
					}
					else
					{
						AT[13]='0';
						AT[14]=RecLen +'0';
					}
				}
			}
		}
			if(RecBuf[10]==0x55)
			{
				ComBuf[j]=RecBuf[10+j];				
				j++;
			
		if(i>13)
			{
				RecLen=RecBuf[12]+6;
				if(j>RecLen-1)
				{
					j=0;
					RecOK=1;
					if(RecLen>9)
					{
						AT[13]=RecLen/10 +'0';
						AT[14]=RecLen%10 +'0';
					}
					else
					{
						AT[13]='0';
						AT[14]=RecLen +'0';
					}
				}
			}
		}
			
			
		
			
	}
	
}
	
				
#endif
	
void ServiceCOMM()
{
  unsigned char n=0;
 
	 		 
	 un_CRCResult.ui_Word = CrcCheckSum(&ComBuf[2], RecLen-6);  //ʵ���Ǵӳ����ֿ�ʼ���㣬���ݳ���ΪRecLen-6����ȥ�����룬����ͷ��β�����Ǽ�ȥ6
	 if((un_CRCResult.uc_Byte[1] != ComBuf[RecLen-4])||(un_CRCResult.uc_Byte[0] != ComBuf[RecLen-3]))//������Ƚ�
	 {	
		 Buzzer_Time(1000);	
	 }

    
	switch(ComBuf[3]) //�����������4���ֽ�Ϊ������
	{
	  case 0x20:  //��ˮ��
	     { 	
					char i;
					i=ComBuf[4];
				  P1=~i;//ComBuf[4]Ϊ������
					ComBuf[0]=0x55;//����ֵ
					ComBuf[1]=0xAA;
					ComBuf[2]=0x03;
					ComBuf[3]=0x20;
				ComBuf[4]=0x00;//��Ϊ0ʱ���ǿ��ַ���SBUF��������ݴ���
	 
	       break;
		  }
	  case 0x21:  //�����
	     { 	 
//						if( ComBuf[2]==0x06)				 
//           {
//						//*******************
////							Nixie(1,ComBuf[4]);
////						  Nixie(2,ComBuf[5]);
////						  Nixie(3,ComBuf[6]);
////						  Nixie(4,ComBuf[7]);
//						
//						//*******************
//						 ComBuf[0]=0x55;
//						 ComBuf[1]=0xAA;
//						 ComBuf[2]=0x06;
//						 ComBuf[3]=0x21;
//					 }
				 if ( ComBuf[2]==0x04)
					{
						 //*******************
				 
						//*******************
						ComBuf[0]=0x55;
						ComBuf[1]=0xAA;
						ComBuf[2]=0x04;
						ComBuf[3]=0x21;
					}
	       break;
		  }	
	  case 0x22:  //������
	     { 	 
				 //*******************
						unsigned int ms;
						unsigned int t;
						unsigned int  f;
						unsigned int idata i;
					 f=ComBuf[4]*256+ComBuf[5];
						t=1*100000/f;//��10usΪ��λ
         if( ComBuf[2]==0x06)				 
           {
						ms=(ComBuf[6]*256+ComBuf[7])*100/t;
						 for(i=0;i<ms;i++)
						{
							Buzzer=!Buzzer;
							delay_10us(t/2);						
						}
						//*******************
						 ComBuf[0]=0x55;
						 ComBuf[1]=0xAA;
						 ComBuf[2]=0x06;
						 ComBuf[3]=0x22;
					 }
					 if ( ComBuf[2]==0x04)
					{
						ms=500*100/t;//���������ʱ�䣬��500msΪ��
						for(i=0;i<ms;i++)
						{
							Buzzer=!Buzzer;
							delay_10us(t/2);						
						}
//						 Buzzer_Time(500);
						ComBuf[0]=0x55;
						ComBuf[1]=0xAA;
						ComBuf[2]=0x04;
						ComBuf[3]=0x22;
					}
	       break;
		}
	  case 0x23:  //����
	     { 	 
				 //*******************
				unsigned char KeyNumber;
				 switch(ComBuf[4])
					{
						case 0x03:{P3_5=0;delay_ms(10);P3_5=1;KeyNumber=1;break;}
						case 0x04:{P3_4=0;delay_ms(10);P3_4=1;KeyNumber=2;break;}
						case 0x05:{P3_3=0;delay_ms(10);P3_3=1;KeyNumber=3;break;}
						case 0x06:{P3_2=0;delay_ms(10);P3_2=1;KeyNumber=4;break;}
					}
					delay_ms(1);
					if(KeyNumber==1)
					{
						P1=0;
						KeyNumber=0;
						Buzzer_Time(500);
					}
				//*******************
					ComBuf[0]=0x55;//����ֵ
					ComBuf[1]=0xAA;
					ComBuf[2]=0x03;
					ComBuf[3]=0x23;
	       break;
		  }
	  case 0x90:  //��ȡ����ֵ
	     { 	 
					ComBuf[0]=0x55;//����ֵ
					ComBuf[1]=0xAA;
					ComBuf[2]=0x03;
					ComBuf[3]=0x20;
				 
					
	       break;
		  }
		 
	  default:
		{			//����
       break;
		}
	} 
	un_CRCResult.ui_Word = CrcCheckSum(&ComBuf[2], RecLen-6);	//��ΪComBuf�е����ݸı��ˣ�����Ҫ���¼���
		ComBuf[RecLen-4]=un_CRCResult.uc_Byte[1];
     ComBuf[RecLen-3]=un_CRCResult.uc_Byte[0];
     ComBuf[RecLen-2]=0xAA;
     ComBuf[RecLen-1]=0x55;
}	
	
	
	
	
	
	
