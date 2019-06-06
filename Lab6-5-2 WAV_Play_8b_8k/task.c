/*
 * task.c
 *
 *  Created on: 2014-7-16
 *      Author: Administrator
 */
#include "task.h"
#include <msp430.h>
StrKeyFlag KeyFlag;						//�������±�־

void delay1(UINT t)
{
	UINT i,j;

	for(j=t;j>0;j--)
		for(i=1;i>0;i--)
			__delay_cycles(1);
}

void TimeInit()
{
	TA0CTL |= MC_1 + TASSEL_1 + TACLR;     	//ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
	TA0CCTL0 = CCIE;						//�Ƚ����ж�ʹ��
	TA0CCR0  =10000;//32768;						//�Ƚ�ֵ��Ϊ50000���൱��50ms��ʱ����
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
	P1DIR |= BIT4;              			// ����P8.1��Ϊ���ģʽ
	P1OUT |= BIT4;
	P2DIR |= BIT0+BIT1+BIT7;              			// ����P8.1��Ϊ���ģʽ
	P2OUT |= BIT0+BIT1+BIT7;							// ѡ��P8.1Ϊ�����ʽ
	P2REN |= BIT1+BIT7;							// P8.1ʹ��
	P3DIR |= BIT2+BIT3+BIT4;              			// ����P3.6��Ϊ���ģʽ
	P3OUT |= BIT2+BIT3+BIT4;						// ѡ��P3.6Ϊ�����ʽ
	P3REN |= BIT2+BIT3+BIT4;					// P3.6ʹ��
#ifdef NewBoard
	P2DIR &= ~(BIT3+BIT6);              			// ����P8.1��Ϊ���ģʽ
	P2OUT |= BIT3+BIT6;							// ѡ��P8.1Ϊ�����ʽ
	P2REN |= BIT3+BIT6;							// P8.1ʹ��
	P1DIR &=~( BIT3+BIT2);              			// ����P8.1��Ϊ���ģʽ
	P1OUT |= BIT3+BIT2;							// ѡ��P8.1Ϊ�����ʽ
	P1REN = BIT3+BIT2;							// P8.1ʹ��
	#else
	P1DIR &=~( BIT3+BIT2+BIT4+BIT5);              			// ����P8.1��Ϊ���ģʽ
	P1OUT |= BIT3+BIT2+BIT4+BIT5;							// ѡ��P8.1Ϊ�����ʽ
	P1REN = BIT3+BIT2+BIT4+BIT5;							// P8.1ʹ��
	#endif
}
void ScanKey(void)
{
	#ifndef NewBoard
	if((P1IN&0x3c)!=0x3c)				//ͨ��IO��ֵ�ó�����������Ϣ
	{
		delay1(50);						//��ʱȥ��
		if((P1IN&0x3c)!=0x3c)			//ͨ��IO��ֵ�ó�����������Ϣ
		{
			switch(P1IN&0x3c)
			{
				case 0x38 : KeyFlag.S1+=1; ;break;//S1��������
				case 0x1c : KeyFlag.S2=1; ;break;//S1��������
				case 0x2c : KeyFlag.S3=1; ;break;//S1��������
				case 0x34 : KeyFlag.S4=1 ;break;//S4��������
				default : break;
			}
		}
	}
	#else
	{
		if((P1IN&0x04)!=0x04)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay1(5);						//��ʱȥ��
			if((P1IN&0x04)!=0x04)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S1^=1;
			}
		}
		if((P1IN&0x08)!=0x08)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay1(5);						//��ʱȥ��
			if((P1IN&0x08)!=0x08)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S2=1;
			}
		}
		if((P2IN&0x08)!=0x08)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay1(5);						//��ʱȥ��
			if((P2IN&0x08)!=0x08)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S3=1;
			}
		}
		if((P2IN&0x40)!=0x40)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay1(5);						//��ʱȥ��
			if((P2IN&0x40)!=0x40)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S4=1;
			}
		}
	}
	#endif
}
