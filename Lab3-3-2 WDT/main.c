#include <msp430.h> 
#include "key.h"
#define NewBoard
#define uint unsigned int
/*
 * main.c
 */
#define WDT_ADLY_1000 (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)			// 时钟为ACLK, 模式为内部时钟。
int main(void)
{
	WDTCTL = WDT_ADLY_1000;
	IO_Init();
	SFRIE1 |= WDTIE;      												// 开启看门狗中断

	__enable_interrupt();
	while(1)
	{
		ScanKey();														//按键扫描
		if(KeyFlag.S1==1)
		{
			KeyFlag.S1=0;
			WDTCTL = WDT_ADLY_1000;										//喂狗
		}

	}
}
/************************看门狗中断函数********************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	P8OUT ^= BIT1;														//LED灯闪烁
}

