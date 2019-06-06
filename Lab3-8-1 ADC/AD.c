/*
 * AD.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */
#include "ad.h"

void ioinit()
{
	P6DIR |= BIT3 + BIT4;																					//配置GPIO引脚
	P3DIR |= BIT5+BIT7;
	P7DIR |= BIT4;
	P8DIR |= BIT1;
}
void AD_Init()
{
	ADC12CTL0 |= ADC12MSC;																				//自动循环采样转换
	ADC12CTL0 |= ADC12ON;																					//启动ADC12模块
	ADC12CTL1 |= ADC12CONSEQ1 ;																		//选择单通道循环采样转换
	ADC12CTL1 |= ADC12SHP;																				//采样保持模式
	ADC12MCTL0 |= ADC12INCH_5; 																		//选择通道5，连接拨码电位器
	ADC12CTL0 |= ADC12ENC;
}


