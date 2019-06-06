#include <msp430.h> 
#include "key.h"

unsigned int i=0;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;				// Stop watchdog timer
	IO_Init();
    TA0CTL |= MC_1 + TASSEL_2 + TACLR;     	//ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
    TA0CCTL0 = CCIE;						//�Ƚ����ж�ʹ��
    TA0CCR0  = 100;						    //�Ƚ�ֵ��Ϊ50000���൱��50ms��ʱ����
    while(1)
    {
    	ScanKey();							//ɨ�谴������
    	if(KeyFlag.S1==1)
    	{
    		KeyFlag.S1=0;
    		__enable_interrupt();
    	}
    	if(KeyFlag.S2==1)
		{
			KeyFlag.S2=0;
			 __disable_interrupt();

		}

    }
}
/************************��ʱ���жϺ���********************************/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)

{
	i++;
	if(i==500)
	{
	P8OUT ^= BIT1;							//�γ�����Ч��
	i=0;
	}
	P3OUT ^= BIT6;						//�γ�����Ч��
}
