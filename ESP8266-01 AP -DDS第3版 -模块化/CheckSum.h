///*unsigned int word; //占用2个字节
//unsigned char byte[2];//占用2个1字节,也是2字节
//union表示word和byte[0],byte[1]共用相同地址的2字节内存区
//1楼的不要误导别人哦
//用typedef 来声明一个共用体类型,类型名称为unWord16*/

//  union
// {
//	unsigned char uc_Byte[2];
//	unsigned int  ui_Word;//校验码
// }un_CRCResult;

//用法:
//un_CRCResult.ui_Word = CrcCheckSum(&ComBuf[2], RecLen-6);	
//		ComBuf[RecLen-4]=un_CRCResult.uc_Byte[1];
//     ComBuf[RecLen-3]=un_CRCResult.uc_Byte[0];

#ifndef _CheckSum_H
#define _CheckSum_H

unsigned int CrcCheckSum(unsigned char *puchMsg, unsigned char usDataLen);//CRC16/XMODEM校验码

#endif