#include <msp430.h> 
#include "motor.h"
/*
 * main.c
 */


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// 关闭看门狗
    ioinit();
    while(1)
    {
    	if((P1IN & BIT2) == 0)			//检测S1按下
    		{
    		DCmotor(1);					//选择电机正转||LED点亮
    		P8OUT |= BIT1;				//L1发亮
    		}
    	if((P1IN & BIT3) == 0)			//检测S2按下
    		{
    		DCmotor(0);					//选择电机停止||LED熄灭
    		P8OUT &= ~BIT1;				//L1灭
    		}
    }
	return 0;
}
