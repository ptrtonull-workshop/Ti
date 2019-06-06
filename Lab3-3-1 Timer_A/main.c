#include <msp430.h> 
#include "key.h"

unsigned int i=0;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;				// Stop watchdog timer
	IO_Init();
    TA0CTL |= MC_1 + TASSEL_2 + TACLR;     	//时钟为SMCLK,比较模式，开始时清零计数器
    TA0CCTL0 = CCIE;						//比较器中断使能
    TA0CCR0  = 100;						    //比较值设为50000，相当于50ms的时间间隔
    while(1)
    {
    	ScanKey();							//扫描按键动作
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
/************************定时器中断函数********************************/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)

{
	i++;
	if(i==500)
	{
	P8OUT ^= BIT1;							//形成闪灯效果
	i=0;
	}
	P3OUT ^= BIT6;						//形成鸣叫效果
}
