//******************************************************************************
// MSP430F5529 Demo - COMPB interrupt capability; Vcompare is compared against
//                    internal 1.5V reference
//
// Description: Use CompB and internal reference to determine if input'Vcompare'
//    is high of low.  For the first time, when Vcompare exceeds the 1.5V internal
//	  reference, CBIFG is set and device enters the CompB ISR. In the ISR CBIES is
//	  toggled such that when Vcompare is less than 1.5V internal reference, CBIFG is set.
//    LED is toggled inside the CompB ISR
//
//                 MSP430F5529
//             ------------------
//         /|\|                  |
//          | |                  |
//          --|RST      P6.5/CB5 |<--Vcompare
//            |                  |
//            |            P6.3  |--> LED 'ON'(Vcompare>1.5V); 'OFF'(Vcompare<1.5V)
//            |                  |
//

//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;     		// 关闭看门狗
  P8DIR |= BIT1;                		// P8.1/L1设为输出
  	  	  	  	  	  	  	  	  	  	// 配置比较器B
  CBCTL0 |= CBIPEN + CBIPSEL_5; 		// 使能正端，输入通道选择CB5
  CBCTL1 |= CBPWRMD_1;          		// 正常的电源供应模式
  CBCTL2 |= CBRSEL;             		// 使能内部参考电压选择
  CBCTL2 |= CBRS_1+CBREFL_1;    		// 参考电压选择为1.5V
  CBCTL3 |= BIT5;               		// 关闭CB5输入缓存
  __delay_cycles(75);           		// 延时等待
  CBINT &= ~(CBIFG + CBIIFG);   		// 清除中断标志
  CBINT  |= CBIE;               		// 使能比较中断，边缘选择为上升沿
 CBCTL1 |= CBON;               			// 打开比较器
  while(1)
  {
	  if(CBCTL1&CBOUT)
	  {
		  P8OUT|=0x02;
	  }
	  else
		  P8OUT&=~0x02;
  }

}

