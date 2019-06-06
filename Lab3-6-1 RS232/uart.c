/*
 * uart.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#include <msp430.h>
#include "uart.h"



void UART_RS232_Init(void)						 //RS232�ӿڳ�ʼ������
{

	   P4SEL |= BIT5+BIT4;                       // P4.4,5ʹ�����蹦�� = UCA1 TXD/RXD
	   UCA1CTL1 |= UCSWRST;                      // ��λUSCI
	   UCA1CTL1 |= UCSSEL_2;                     // ����SMCLKʱ�ӣ����ڷ����ض�������
	   UCA1BR0 = 175;                            // ���ò����ʣ� 1MHz ������= 115200
	   UCA1BR1 = 0;
	   UCA1MCTL |= UCBRS_1 + UCBRF_0;
	   UCA1CTL1 &= ~UCSWRST;                     // ������λ
	   UCA1IE |= UCRXIE;                         // ʹ��UCA1�����ж�

}

void TimerA_Init(void)							//��ʱ��TA��ʼ������
{
	TA0CTL |= MC_1 + TASSEL_2 + TACLR;			//ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
	TA0CCTL0 = CCIE;							//�Ƚ����ж�ʹ��
	TA0CCR0  = 50000;							//�Ƚ�ֵ��Ϊ50000���൱��50ms��ʱ����
}
