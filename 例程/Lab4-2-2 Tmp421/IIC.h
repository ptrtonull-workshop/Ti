/*
 * IIC.h
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */
#include <msp430.h>
#include "type.h"
#ifndef IIC_H_
#define IIC_H_

///////////////////////////////////////
#define IIC_DIR P3DIR
#define IIC_OUT P3OUT
#define IIC_IN  P3IN

#define IIC_SCL BIT1 					//SCL����
#define IIC_SDA BIT0						//SDA����

#define S_SDA	IIC_OUT |= IIC_SDA	//SDA��λ
#define S_SCL 	IIC_OUT |= IIC_SCL	//SCL��λ
#define C_SDA	IIC_OUT &= ~IIC_SDA	//SDA��λ
#define C_SCL  IIC_OUT &= ~IIC_SCL	//SCL��λ

#define SDA_IN   IIC_DIR &= ~IIC_SDA  //����SDAΪ����
#define SDA_OUT  IIC_DIR |= IIC_SDA	  //����SDAΪ���

#define READ_SDA  (IIC_IN&IIC_SDA)	  //��SDA��ƽ

#define FREQUENCY	4000
#define LOOPBODY	8
#define LOOPCNT	(unsigned int)((FREQUENCY/LOOPBODY))

void InitIIC();
uchar W_I2C(uint device,uint adr,uchar data);
uchar R_I2C(uint device,uint adr);
void Clockint();
void DelayMs(uint ms);

#endif /* IIC_H_ */
