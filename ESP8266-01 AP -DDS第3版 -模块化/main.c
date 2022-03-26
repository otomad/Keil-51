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

 #define CommLenMax		    50     //通信缓冲区长度

void ServiceCOMM();

sbit Buzzer=P3^6;

bit mode=0;




unsigned char TrLen=0, TrCount=0, RecCount=0, RecChar=0, RecLen=0;  //
bit  RecOK;
unsigned char idata ComBuf[CommLenMax];//要执行（底层）指令的储存数组
u8 RecBuf[50];//指令的储存数组

unsigned char  AT[17]={'A','T','+','C','I','P','S','E','N','D','=','0',',','0','9','\r','\n'};


/*unsigned int word; //占用2个字节
unsigned char byte[2];//占用2个1字节,也是2字节
union表示word和byte[0],byte[1]共用相同地址的2字节内存区
1楼的不要误导别人哦
用typedef 来声明一个共用体类型,类型名称为unWord16*/

  union
 {
	unsigned char uc_Byte[2];
	unsigned int  ui_Word;//校验码
 }un_CRCResult;
 



//主函数***********************************************
void main()
{	
	UART_Init(); //串口初始化
	ESP8266_ModeInit();
	ES=1; //允许串口中断
	
	while(1)
	{		
		if(RecOK)
		{
			EA=0;
			ComBuf[RecLen]='\r';//\n变为数据是否发送完标志
			ComBuf[RecLen+1]='\n';
			ComBuf[RecLen+2]='\0';
			ServiceCOMM();
			ESP8266_SendCmd(AT);
				delay_ms(3);
			ESP8266_SendCmd(ComBuf);
			memset(ComBuf,'\0',50);
			memset(RecBuf,'\0',50);//很重要的步骤
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
		RecBuf[i++]=SBUF;//要注意最后i的取值，实际为字符串字符长度。
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
 
	 		 
	 un_CRCResult.ui_Word = CrcCheckSum(&ComBuf[2], RecLen-6);  //实际是从长度字开始计算，数据长度为RecLen-6，减去检验码，命令头，尾，就是减去6
	 if((un_CRCResult.uc_Byte[1] != ComBuf[RecLen-4])||(un_CRCResult.uc_Byte[0] != ComBuf[RecLen-3]))//检验码比较
	 {	
		 Buzzer_Time(1000);	
	 }

    
	switch(ComBuf[3]) //命令分析。第4个字节为命令字
	{
	  case 0x20:  //流水灯
	     { 	
					char i;
					i=ComBuf[4];
				  P1=~i;//ComBuf[4]为数据域
					ComBuf[0]=0x55;//返回值
					ComBuf[1]=0xAA;
					ComBuf[2]=0x03;
					ComBuf[3]=0x20;
				ComBuf[4]=0x00;//当为0时，是空字符，SBUF会出现数据错误。
	 
	       break;
		  }
	  case 0x21:  //数码管
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
	  case 0x22:  //蜂鸣器
	     { 	 
				 //*******************
						unsigned int ms;
						unsigned int t;
						unsigned int  f;
						unsigned int idata i;
					 f=ComBuf[4]*256+ComBuf[5];
						t=1*100000/f;//以10us为单位
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
						ms=500*100/t;//蜂鸣器响的时间，以500ms为例
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
	  case 0x23:  //按键
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
					ComBuf[0]=0x55;//返回值
					ComBuf[1]=0xAA;
					ComBuf[2]=0x03;
					ComBuf[3]=0x23;
	       break;
		  }
	  case 0x90:  //读取参数值
	     { 	 
					ComBuf[0]=0x55;//返回值
					ComBuf[1]=0xAA;
					ComBuf[2]=0x03;
					ComBuf[3]=0x20;
				 
					
	       break;
		  }
		 
	  default:
		{			//其他
       break;
		}
	} 
	un_CRCResult.ui_Word = CrcCheckSum(&ComBuf[2], RecLen-6);	//因为ComBuf中的数据改变了，所以要重新检验
		ComBuf[RecLen-4]=un_CRCResult.uc_Byte[1];
     ComBuf[RecLen-3]=un_CRCResult.uc_Byte[0];
     ComBuf[RecLen-2]=0xAA;
     ComBuf[RecLen-1]=0x55;
}	
	
	
	
	
	
	
