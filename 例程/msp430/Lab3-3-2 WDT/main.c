#include <msp430.h> 
#include "key.h"
#define NewBoard
#define uint unsigned int
/*
 * main.c
 */
#define WDT_ADLY_1000 (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)			// ʱ��ΪACLK, ģʽΪ�ڲ�ʱ�ӡ�
int main(void)
{
	WDTCTL = WDT_ADLY_1000;
	IO_Init();
	SFRIE1 |= WDTIE;      												// �������Ź��ж�

	__enable_interrupt();
	while(1)
	{
		ScanKey();														//����ɨ��
		if(KeyFlag.S1==1)
		{
			KeyFlag.S1=0;
			WDTCTL = WDT_ADLY_1000;										//ι��
		}

	}
}
/************************���Ź��жϺ���********************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	P8OUT ^= BIT1;														//LED����˸
}

