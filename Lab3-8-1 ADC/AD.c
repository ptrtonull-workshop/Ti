/*
 * AD.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */
#include "ad.h"

void ioinit()
{
	P6DIR |= BIT3 + BIT4;																					//����GPIO����
	P3DIR |= BIT5+BIT7;
	P7DIR |= BIT4;
	P8DIR |= BIT1;
}
void AD_Init()
{
	ADC12CTL0 |= ADC12MSC;																				//�Զ�ѭ������ת��
	ADC12CTL0 |= ADC12ON;																					//����ADC12ģ��
	ADC12CTL1 |= ADC12CONSEQ1 ;																		//ѡ��ͨ��ѭ������ת��
	ADC12CTL1 |= ADC12SHP;																				//��������ģʽ
	ADC12MCTL0 |= ADC12INCH_5; 																		//ѡ��ͨ��5�����Ӳ����λ��
	ADC12CTL0 |= ADC12ENC;
}


