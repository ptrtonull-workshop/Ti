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
  WDTCTL = WDTPW + WDTHOLD;     		// �رտ��Ź�
  P8DIR |= BIT1;                		// P8.1/L1��Ϊ���
  	  	  	  	  	  	  	  	  	  	// ���ñȽ���B
  CBCTL0 |= CBIPEN + CBIPSEL_5; 		// ʹ�����ˣ�����ͨ��ѡ��CB5
  CBCTL1 |= CBPWRMD_1;          		// �����ĵ�Դ��Ӧģʽ
  CBCTL2 |= CBRSEL;             		// ʹ���ڲ��ο���ѹѡ��
  CBCTL2 |= CBRS_1+CBREFL_1;    		// �ο���ѹѡ��Ϊ1.5V
  CBCTL3 |= BIT5;               		// �ر�CB5���뻺��
  __delay_cycles(75);           		// ��ʱ�ȴ�
  CBINT &= ~(CBIFG + CBIIFG);   		// ����жϱ�־
  CBINT  |= CBIE;               		// ʹ�ܱȽ��жϣ���Եѡ��Ϊ������
 CBCTL1 |= CBON;               			// �򿪱Ƚ���
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

