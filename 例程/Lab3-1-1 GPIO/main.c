#include <msp430.h>

void main(void)
{
 WDTCTL = WDTPW + WDTHOLD;      //�رտ��Ź���ʱ��
  P8DIR |= BIT1;                //����P8.1��Ϊ���ģʽ
  P1REN |= BIT2;				//ʹ��P1.2���������蹦��
  P1OUT |= BIT2;				//��P1.2Ϊ�������跽ʽ

 while (1)
  {
    if (P1IN & BIT2)	        //�ж��Ƿ��¼���S2��������P1.2=0��̧��P1.2=1
	  P8OUT |= BIT1;            //P8.1����ߣ�LED1������
    else
    P8OUT &= ~BIT1;             //P8.1����ͣ�LED1Ϩ��
  }
}
