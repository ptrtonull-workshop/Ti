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
#define LEN 6 //宏定义缓冲区大小
#define ADR 0x1880//宏定义写入Flash的起始地址，位于Segment C
#define BYTE 0//单字节写入标志
#define WORD 1//双字节写入标志
#define DELETE 0//清除值


extern void FlashWriteByte(uint address,uchar *byte,uint count);
extern void FlashWriteWord(uint address,uint *word,uint count);
extern void FlashRead(uint address,uint *buf,uint mode,uint length);
extern void FlashErase(uint address,uint mode,uint length);

#endif /* FLASH_H_ */
