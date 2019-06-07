#include <msp430.h> 
#include "motor.h"
/*
 * main.c
 */


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// �رտ��Ź�
    ioinit();
    while(1)
    {
    	if((P1IN & BIT2) == 0)			//���S1����
    		{
    		DCmotor(1);					//ѡ������ת||LED����
    		P8OUT |= BIT1;				//L1����
    		}
    	if((P1IN & BIT3) == 0)			//���S2����
    		{
    		DCmotor(0);					//ѡ����ֹͣ||LEDϨ��
    		P8OUT &= ~BIT1;				//L1��
    		}
    }
	return 0;
}
