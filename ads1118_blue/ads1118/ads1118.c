
#include "ads1118.h"


void delay_ms(uint z)
{
	uint x;
	for(;z>0;z--)
	for(x=110;x>0;x--);
}
void ADS1118_Init(void)
{
	CS_H;
	SCLK_H;
	MOSI_L;
}
union aa{
           uint dat1[2];
		       ulong dat2;
					};
uint Write_ADS1118(uint dat,uchar mode)
{
//	uint temp;
	union aa temp;
	union aa temp1;
//	ulong Value;
	uchar i;		
  if(mode == 1)dat |= 0x8000;	
	temp.dat1[1] = dat;
  temp.dat1[0] = dat;
	SCLK_L;
	CS_L;
	delay_ms(1);
	for(i=0;i<32;i++)
	{
		if(temp.dat2 & 0x80000000)MOSI_H;
		else MOSI_L;
		temp.dat2 <<= 1;
		_nop_();
		SCLK_H;
		_nop_();
		SCLK_L;
		_nop_();
		
		temp1.dat2 <<= 1;
		if(READ_MISO)temp1.dat2 |= 0x01;
		_nop_();
	}
  CS_H;
	SCLK_L;
	MOSI_L;
	MISO_L;
	printf("temp %d\n\r",temp1.dat1[0]);
	printf("temp %#X\n\r",temp1.dat1[1]);
	return temp1.dat1[0];
}

/*******************************************************************************
//函数名称：ADS_SEL_Read（）
//函数功能：读取各路电压，通过两个switch选择读取不同的通道
//输    入：road:增益放大器两端的电压选择，并选择测几路电压
//          Ref: 选择参考电压，有6种选择
//输    出：dat：16位ad转换数据
//备    注：这一次读出的转换数据是上一次的转换数据，不要混淆.这里选择的是单次
            转换电压值，当然，也可以选择多次转换,通过寄存器的第8位可以设置
//日    期：2013.6.8
*******************************************************************************/
int ADS_SEL_Read(uchar road,uchar Ref)         //测几路电压
{
    int dat = 0;
    uint Config_Value = 0x008b;                     //默认低8位，DOUT带上拉电阻
    switch(road)
    {
    case 0:  Config_Value += 0x0000;break;    //AINP = AIN0 and AINN = AIN1 (default)
    case 1:  Config_Value += 0x1000;break;    //AINP = AIN0 and AINN = AIN3
    case 2:  Config_Value += 0x2000;break;    //AINP = AIN1 and AINN = AIN3
    case 3:  Config_Value += 0x3000;break;    //AINP = AIN2 and AINN = AIN3
    case 4:  Config_Value += 0x4000;break;    //AINP = AIN0 and AINN = GND
    case 5:  Config_Value += 0x5000;break;    //AINP = AIN1 and AINN = GND
    case 6:  Config_Value += 0x6000;break;    //AINP = AIN2 and AINN = GND
    case 7:  Config_Value += 0x7000;break;    //AINP = AIN3 and AINN = GND
    default : break;
    }
    switch(Ref)
    {
    case 0:  Config_Value += 0x0000;break;    //000 : FS = ±6.144V(1)
    case 1:  Config_Value += 0x0200;break;    //001 : FS = ±4.096V(1)
    case 2:  Config_Value += 0x0400;break;    //002 : FS = ±2.048V(1)
    case 3:  Config_Value += 0x0600;break;    //003 : FS = ±1.024V(1)
    case 4:  Config_Value += 0x0800;break;    //004 : FS = ±0.512V(1)
    case 5: case 6: case 7: Config_Value += 0x0a00;break;    //005 : FS = ±0.256V(1)
    default : break;
    }
    CS_L;
    dat = Write_ADS1118(Config_Value,1);
    _nop_();
    CS_H;
    return dat;
}

