/*
 * ad.c
 *
 *  Created on: 2014-7-16
 *      Author: Administrator
 */
#include "ad.h"
void ioinit()
{
	P1DIR |= BIT5;						//����P1.5Ϊ���ģʽ
	P8DIR |= BIT1;						//����P8.1Ϊ���ģʽ
	P1REN |= BIT2+BIT3;					//ʹ��S1��S2������������
	P1OUT |= BIT2+BIT3;					//����S1��S2Ϊ����״̬
	P2DIR |= BIT2+BIT4+BIT5;            //����P2.4��P2.5Ϊ���ģʽ
	P2OUT |= BIT2;
}
void ADInit(void)
{
	ADC12CTL0 |= ADC12MSC;									//�Զ�ѭ������ת��
	ADC12CTL0 |= ADC12ON;									//����ADC12ģ��
	ADC12CTL1 |= ADC12CONSEQ1 ;								//ѡ��ͨ��ѭ������ת��
	ADC12CTL1 |= ADC12SHP;									//��������ģʽ
	ADC12MCTL0 |= ADC12INCH_6; 								//ѡ��ͨ��5�����Ӳ����λ��
	ADC12CTL0 |= ADC12ENC;
	ADC12CTL0 |= ADC12SC;
}


