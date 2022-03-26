unsigned int CrcCheckSum(unsigned char *puchMsg, unsigned char usDataLen)//CRC16/XMODEMÐ£ÑéÂë
{
unsigned int wCRCin = 0x0000;
unsigned int wCPoly = 0x1021;
unsigned char wChar = 0;
unsigned char i;
while (usDataLen--) 
{
	wChar = *(puchMsg++);
	wCRCin ^= (wChar << 8);
	for( i = 0; i < 8; i++) 
	{
		if (wCRCin & 0x8000)
			wCRCin = (wCRCin << 1) ^ wCPoly;
		else
			wCRCin = wCRCin << 1;
	}
}
return (wCRCin);
}