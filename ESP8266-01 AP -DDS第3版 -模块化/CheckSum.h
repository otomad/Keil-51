///*unsigned int word; //ռ��2���ֽ�
//unsigned char byte[2];//ռ��2��1�ֽ�,Ҳ��2�ֽ�
//union��ʾword��byte[0],byte[1]������ͬ��ַ��2�ֽ��ڴ���
//1¥�Ĳ�Ҫ�󵼱���Ŷ
//��typedef ������һ������������,��������ΪunWord16*/

//  union
// {
//	unsigned char uc_Byte[2];
//	unsigned int  ui_Word;//У����
// }un_CRCResult;

//�÷�:
//un_CRCResult.ui_Word = CrcCheckSum(&ComBuf[2], RecLen-6);	
//		ComBuf[RecLen-4]=un_CRCResult.uc_Byte[1];
//     ComBuf[RecLen-3]=un_CRCResult.uc_Byte[0];

#ifndef _CheckSum_H
#define _CheckSum_H

unsigned int CrcCheckSum(unsigned char *puchMsg, unsigned char usDataLen);//CRC16/XMODEMУ����

#endif