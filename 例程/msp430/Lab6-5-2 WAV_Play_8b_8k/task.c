/*
 * task.c
 *
 *  Created on: 2014-7-16
 *      Author: Administrator
 */
#include "task.h"
#include <msp430.h>
StrKeyFlag KeyFlag;						//按键按下标志

void delay1(UINT t)
{
	UINT i,j;

	for(j=t;j>0;j--)
		for(i=1;i>0;i--)
			__delay_cycles(1);
}

void TimeInit()
{
	TA0CTL |= MC_1 + TASSEL_1 + TACLR;     	//时钟为SMCLK,比较模式，开始时清零计数器
	TA0CCTL0 = CCIE;						//比较器中断使能
	TA0CCR0  =10000;//32768;						//比较值设为50000，相当于50ms的时间间隔
}

void IOInit()
{
	P3OUT |= 0x03;
	P3DIR |= 0x03;
	P3REN |= 0x03;                            // Assign I2C pins to USCI_B0
	P8OUT |= 0x04;
	P8DIR |= 0x04;
	P8REN |= 0x04;
	P8OUT&=~0x04;
	P1DIR |= BIT4;              			// 设置P8.1口为输出模式
	P1OUT |= BIT4;
	P2DIR |= BIT0+BIT1+BIT7;              			// 设置P8.1口为输出模式
	P2OUT |= BIT0+BIT1+BIT7;							// 选中P8.1为输出方式
	P2REN |= BIT1+BIT7;							// P8.1使能
	P3DIR |= BIT2+BIT3+BIT4;              			// 设置P3.6口为输出模式
	P3OUT |= BIT2+BIT3+BIT4;						// 选中P3.6为输出方式
	P3REN |= BIT2+BIT3+BIT4;					// P3.6使能
#ifdef NewBoard
	P2DIR &= ~(BIT3+BIT6);              			// 设置P8.1口为输出模式
	P2OUT |= BIT3+BIT6;							// 选中P8.1为输出方式
	P2REN |= BIT3+BIT6;							// P8.1使能
	P1DIR &=~( BIT3+BIT2);              			// 设置P8.1口为输出模式
	P1OUT |= BIT3+BIT2;							// 选中P8.1为输出方式
	P1REN = BIT3+BIT2;							// P8.1使能
	#else
	P1DIR &=~( BIT3+BIT2+BIT4+BIT5);              			// 设置P8.1口为输出模式
	P1OUT |= BIT3+BIT2+BIT4+BIT5;							// 选中P8.1为输出方式
	P1REN = BIT3+BIT2+BIT4+BIT5;							// P8.1使能
	#endif
}
void ScanKey(void)
{
	#ifndef NewBoard
	if((P1IN&0x3c)!=0x3c)				//通过IO口值得出按键按下信息
	{
		delay1(50);						//延时去抖
		if((P1IN&0x3c)!=0x3c)			//通过IO口值得出按键按下信息
		{
			switch(P1IN&0x3c)
			{
				case 0x38 : KeyFlag.S1+=1; ;break;//S1按键按下
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
			delay1(5);						//延时去抖
			if((P1IN&0x04)!=0x04)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S1^=1;
			}
		}
		if((P1IN&0x08)!=0x08)				//通过IO口值得出按键按下信息
		{
			delay1(5);						//延时去抖
			if((P1IN&0x08)!=0x08)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S2=1;
			}
		}
		if((P2IN&0x08)!=0x08)				//通过IO口值得出按键按下信息
		{
			delay1(5);						//延时去抖
			if((P2IN&0x08)!=0x08)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S3=1;
			}
		}
		if((P2IN&0x40)!=0x40)				//通过IO口值得出按键按下信息
		{
			delay1(5);						//延时去抖
			if((P2IN&0x40)!=0x40)			//通过IO口值得出按键按下信息
			{
				KeyFlag.S4=1;
			}
		}
	}
	#endif
}
