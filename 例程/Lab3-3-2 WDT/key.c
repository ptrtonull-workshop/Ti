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

/************************IO�ڳ�ʼ��********************************/

void IO_Init(void)
{
	P8DIR |= BIT1;              			// ����P8.1��Ϊ���ģʽ  ����LED��
	P8OUT |= BIT1;							// ѡ��P8.1Ϊ�����ʽ
	P8REN |= BIT1;							// P8.1ʹ��
	P3DIR |= BIT6;              			// ����P3.6��Ϊ���ģʽ  ���Ʒ�����
	P3OUT |= BIT6;							// ѡ��P3.6Ϊ�����ʽ
	P3REN |= BIT6;							// P3.6ʹ��
	P2DIR |= BIT2;
	P2OUT &=~ BIT2;

#ifdef NewBoard
	P2DIR &= ~(BIT3+BIT6);              	// ����P8.1��Ϊ���ģʽ
	P2OUT |= BIT3+BIT6;						// ѡ��P8.1Ϊ�����ʽ
	P2REN |= BIT3+BIT6;						// P8.1ʹ��
	P1DIR &=~( BIT3+BIT2);              	// ����P8.1��Ϊ���ģʽ
	P1OUT |= BIT3+BIT2;						// ѡ��P8.1Ϊ�����ʽ
	P1REN = BIT3+BIT2;						// P8.1ʹ��
	#else
	P1DIR &=~( BIT3+BIT2+BIT4+BIT5);        // ����P8.1��Ϊ���ģʽ
	P1OUT |= BIT3+BIT2+BIT4+BIT5;			// ѡ��P8.1Ϊ�����ʽ
	P1REN = BIT3+BIT2+BIT4+BIT5;			// P8.1ʹ��
	#endif
}

void ScanKey(void)
{
	#ifndef NewBoard
	if((P1IN&0x3c)!=0x3c)					//ͨ��IO��ֵ�ó�����������Ϣ
	{
		delay(50);							//��ʱȥ��
		if((P1IN&0x3c)!=0x3c)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			switch(P1IN&0x3c)
			{
				case 0x38 : KeyFlag.S1=1; ;break;//S1��������
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
			delay(50);						//��ʱȥ��
			if((P1IN&0x04)!=0x04)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S1=1;
			}
		}
		if((P1IN&0x08)!=0x08)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay(50);						//��ʱȥ��
			if((P1IN&0x08)!=0x08)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S2=1;
			}
		}
		if((P2IN&0x08)!=0x08)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay(50);						//��ʱȥ��
			if((P2IN&0x08)!=0x08)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S3=1;
			}
		}
		if((P2IN&0x40)!=0x40)				//ͨ��IO��ֵ�ó�����������Ϣ
		{
			delay(50);						//��ʱȥ��
			if((P2IN&0x40)!=0x40)			//ͨ��IO��ֵ�ó�����������Ϣ
			{
				KeyFlag.S4=1;
			}
		}
	}
	#endif
}
