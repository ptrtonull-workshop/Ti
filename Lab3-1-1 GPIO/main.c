#include <msp430.h>

void main(void)
{
 WDTCTL = WDTPW + WDTHOLD;      //关闭看门狗定时器
  P8DIR |= BIT1;                //设置P8.1口为输出模式
  P1REN |= BIT2;				//使能P1.2上下拉电阻功能
  P1OUT |= BIT2;				//置P1.2为上拉电阻方式

 while (1)
  {
    if (P1IN & BIT2)	        //判断是否按下键，S2按键按下P1.2=0，抬起P1.2=1
	  P8OUT |= BIT1;            //P8.1输出高（LED1点亮）
    else
    P8OUT &= ~BIT1;             //P8.1输出低（LED1熄灭）
  }
}
