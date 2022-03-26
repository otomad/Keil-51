#include "reg52.h"
#include "public.h"


unsigned char Key()
{
	unsigned char KeyNumber=0;
	switch(ComBuf[4])
					{
						case 0x03:{P3_5=0;delay_ms(10);P3_5=1;KeyNumber=1;break;}
						case 0x04:{P3_4=0;delay_ms(10);P3_4=1;KeyNumber=1;break;}
						case 0x05:{P3_3=0;delay_ms(10);P3_3=1;KeyNumber=1;break;}
						case 0x06:{P3_2=0;delay_ms(10);P3_2=1;KeyNumber=1;break;}
					}
	return KeyNumber;
}

