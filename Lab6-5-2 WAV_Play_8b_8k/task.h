/*
 * task.h
 *
 *  Created on: 2014-7-16
 *      Author: Administrator
 */

#ifndef TASK_H_
#define TASK_H_
#include "dr_sdcard/integer.h"
#define NewBoard

typedef unsigned char   uint8_t;
typedef struct{
	unsigned char S1;					//S1按键按下标志
	unsigned char S2;					//S1按键按下标志
	unsigned char S3;					//S1按键按下标志
	unsigned char S4;					//S4按键按下标志
}StrKeyFlag;
extern StrKeyFlag KeyFlag;				//按键按下标志
void delay1(UINT t);
void IOInit();
void ScanKey(void);
void TimeInit();

#endif /* TASK_H_ */
