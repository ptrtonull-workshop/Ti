/*
 * IIC.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */
#include "IIC.h"
void ioinit()
{
   P3OUT |= 0x03;
   P3DIR |= 0x03;
   P3REN |= 0x03;
   P8OUT &=~ 0x04;
   P8DIR |= 0x04;
   P8REN |= 0x04;
   P2DIR |= BIT1+BIT7;              			// 设置P8.1口为输出模式
	P2OUT |= BIT1+BIT7;							// 选中P8.1为输出方式
	P2REN |= BIT1+BIT7;							// P8.1使能
}
void delay1(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=1000;y>0;y--)
			P8OUT&=~0x04;
}
void start()
{
	SCK_H;
	delay1(100);
	SDA_H;
	delay1(100);
	SDA_L;
}
void stop()
{
	SCK_H;
	SDA_L;
	delay1(100);
	SDA_H;
}
void waitack()
{
	 P3DIR &=~ 0x01;
	 SCK_H;
	 delay1(50);
	 SCK_L;
//	 while(P3IN&0x01==0x01);
	 delay1(50);
	 P3DIR |= 0x01;
}
void senddata(unsigned char data)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SCK_L;
		delay1(50);
		if((data&0x80)==0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		data<<=1;
		delay1(50);
		SCK_H;
		delay1(50);
	}
	SCK_L;
}

