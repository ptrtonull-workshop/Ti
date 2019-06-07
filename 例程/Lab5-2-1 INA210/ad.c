/*
 * ad.c
 *
 *  Created on: 2014-7-16
 *      Author: Administrator
 */
#include "ad.h"
void ioinit()
{
	P1DIR |= BIT5;						//设置P1.5为输出模式
	P8DIR |= BIT1;						//设置P8.1为输出模式
	P1REN |= BIT2+BIT3;					//使能S1，S2的上下拉电阻
	P1OUT |= BIT2+BIT3;					//设置S1，S2为上拉状态
	P2DIR |= BIT2+BIT4+BIT5;            //设置P2.4和P2.5为输出模式
	P2OUT |= BIT2;
}
void ADInit(void)
{
	ADC12CTL0 |= ADC12MSC;									//自动循环采样转换
	ADC12CTL0 |= ADC12ON;									//启动ADC12模块
	ADC12CTL1 |= ADC12CONSEQ1 ;								//选择单通道循环采样转换
	ADC12CTL1 |= ADC12SHP;									//采样保持模式
	ADC12MCTL0 |= ADC12INCH_6; 								//选择通道5，连接拨码电位器
	ADC12CTL0 |= ADC12ENC;
	ADC12CTL0 |= ADC12SC;
}


