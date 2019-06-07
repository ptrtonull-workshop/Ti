/*
 * key.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */
#include "key.h"
#include <msp430.h>
#define NewBoard

StrKeyFlag KeyFlag;
void delay(uint16 t)
{
	uint16 i,j,k;

	for(j=t;j>0;j--)
		for(i=250;i>0;i--)
			k=0;
}

/************************IO口初始化********************************/

void IO_Init(void)
{
	P8DIR |= BIT1;              			// 设置P8.1口为输出模式  控制LED灯
	P8OUT |= BIT1;							// 选中P8.1为输出方式
	P8REN |= BIT1;							// P8.1使能
	P3DIR |= BIT6;              			// 设置P3.6口为输出模式  控制蜂鸣器
	P3OUT |= BIT6;							// 选中P3.6为输出方式
	P3REN |= BIT6;							// P3.6使能
	P2DIR |= BIT2;
	P2OUT &=~ BIT2;

#ifdef NewBoard
	P2DIR &= ~(BIT3+BIT6);              	// 设置P8.1口为输出模式
	P2OUT |= BIT3+BIT6;						// 选中P8.1为输出方式
	P2REN |= BIT3+BIT6;						// P8.1使能
	P1DIR &=~( BIT3+BIT2);              	// 设置P8.1口为输出模式
	P1OUT |= BIT3+BIT2;						// 选中P8.1为输出方式
	P1REN = BIT3+BIT2;						// P8.1使能
	#else
	P1DIR &=~( BIT3+BIT2+BIT4+BIT5);        // 设置P8.1口为输出模式
	P1OUT |= BIT3+BIT2+BIT4+BIT5;			// 选中P8.1为输出方式
	P1REN = BIT3+BIT2+BIT4+BIT5;			// P8.1使能
	#endif
}

void ScanKey(void)
{
	#ifndef NewBoard
	if((P1IN&0x3c)!=0x3c)					//通过IO口值得出按键按下信息
	{
		delay(50);							//延时去抖
		if((P1IN&0x3c)!=0x3c)				//通过IO口值得出按键按下信息
		{
			switch(P1IN&0x3c)
			{
				case 0x38 : KeyFlag.S1=1; ;break;//S1按键按下
				case 0x1c : KeyFlag.S2=1; ;break;//S1按键按下
				case 0x2c : KeyFlag.S3=1; ;break;//S1按键按下
				case 0x34 : KeyFlag.S4=1 ;break;//S4按键按下
				default : break;
			}
		}
	}
	#else
	{
		if((P1IN&0x04)!=0x04)				//通过IO口值得出按键按下信息
		{
			delay(50);						//延时去抖
			if((P1IN&0x04)!=0x04)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S1=1;
			}
		}
		if((P1IN&0x08)!=0x08)				//通过IO口值得出按键按下信息
		{
			delay(50);						//延时去抖
			if((P1IN&0x08)!=0x08)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S2=1;
			}
		}
		if((P2IN&0x08)!=0x08)				//通过IO口值得出按键按下信息
		{
			delay(50);						//延时去抖
			if((P2IN&0x08)!=0x08)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S3=1;
			}
		}
		if((P2IN&0x40)!=0x40)				//通过IO口值得出按键按下信息
		{
			delay(50);						//延时去抖
			if((P2IN&0x40)!=0x40)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S4=1;
			}
		}
	}
	#endif
}
