/*
 * Flash.h
 *
 *  Created on: 2014-4-15
 *      Author: dean
 */

#ifndef FLASH_H_
#define FLASH_H_

typedef unsigned int uint;
typedef unsigned char uchar;
#define LEN 6 //�궨�建������С
#define ADR 0x1880//�궨��д��Flash����ʼ��ַ��λ��Segment C
#define BYTE 0//���ֽ�д���־
#define WORD 1//˫�ֽ�д���־
#define DELETE 0//���ֵ


extern void FlashWriteByte(uint address,uchar *byte,uint count);
extern void FlashWriteWord(uint address,uint *word,uint count);
extern void FlashRead(uint address,uint *buf,uint mode,uint length);
extern void FlashErase(uint address,uint mode,uint length);

#endif /* FLASH_H_ */
