/*
 * key.h
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#ifndef KEY_H_
#define KEY_H_
#define uint16	unsigned int
typedef struct{
	unsigned char S1;					//S1�������±�־
	unsigned char S2;					//S1�������±�־
	unsigned char S3;					//S1�������±�־
	unsigned char S4;					//S4�������±�־
}StrKeyFlag;
extern StrKeyFlag KeyFlag;
void IO_Init(void);
void ScanKey(void);

#endif /* KEY_H_ */
