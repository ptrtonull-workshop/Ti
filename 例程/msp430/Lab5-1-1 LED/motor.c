/*
 * motor.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#include <msp430.h>
#include "motor.h"
void ioinit()
{
	P1DIR |= BIT5;						//����P1.5Ϊ���ģʽ
	P8DIR |= BIT1;						//����P8.1Ϊ���ģʽ
	P1REN |= BIT2+BIT3;					//ʹ��S1��S2������������
	P1OUT |= BIT2+BIT3;					//����S1��S2Ϊ����״̬
	P2DIR |= BIT2+BIT4+BIT5;              	//����P2.2,P2.4��P2.5Ϊ���ģʽ
	P2OUT &= ~BIT2;
}
void DCmotor(int p)
{
  switch(p)
  {
  case 0:					//ͣת
    {
      P1OUT &=~ BIT5;
      P2OUT &=~ BIT5;
      P2OUT &=~ BIT4;
      break;
    }
  case 1:					//��ת
    {
      P1OUT |= BIT5;
      P2OUT |= BIT5;
      P2OUT &=~ BIT4;
      break;
    }
  case 2:					//��ת
    {
      P1OUT |= BIT5;
      P2OUT &=~ BIT5;
      P2OUT |= BIT4;
      break;
    }
  }
}

