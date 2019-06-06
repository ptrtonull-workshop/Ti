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
	P1DIR |= BIT5;						//设置P1.5为输出模式
	P8DIR |= BIT1;						//设置P8.1为输出模式
	P1REN |= BIT2+BIT3;					//使能S1，S2的上下拉电阻
	P1OUT |= BIT2+BIT3;					//设置S1，S2为上拉状态
	P2DIR |= BIT2+BIT4+BIT5;              	//设置P2.2,P2.4和P2.5为输出模式
	P2OUT &= ~BIT2;
}
void DCmotor(int p)
{
  switch(p)
  {
  case 0:					//停转
    {
      P1OUT &=~ BIT5;
      P2OUT &=~ BIT5;
      P2OUT &=~ BIT4;
      break;
    }
  case 1:					//正转
    {
      P1OUT |= BIT5;
      P2OUT |= BIT5;
      P2OUT &=~ BIT4;
      break;
    }
  case 2:					//反转
    {
      P1OUT |= BIT5;
      P2OUT &=~ BIT5;
      P2OUT |= BIT4;
      break;
    }
  }
}

