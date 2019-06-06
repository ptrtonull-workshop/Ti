#include <msp430.h>
#include "defin.h"


int main( void )
{
	WDTCTL = WDTPW + WDTHOLD;
	int i;
	for(i=0;i<4;++i)
		*LED_GPIO[i]->PxDIR |= LED_PORT[i]; 			//���ø�LED�����ڶ˿�Ϊ�������
	P1REN |= 0x0C; 										//ʹ�ܰ����˿��ϵ�����������
	P1OUT |= 0x0C; 										//����״̬
	P2REN |= 0x48;										//ʹ�ܰ����˿��ϵ�����������
	P2OUT |= 0x48;
	uint8_t last_btn1 = 0x0C, last_btn2 = 0x48, cur_btn1, cur_btn2, temp,temp1;
    while(1)
  	{
	  	  cur_btn1 = P1IN & 0x0F;
	      temp = (cur_btn1 ^ last_btn1) & last_btn1; 	//�ҳ�����������İ���     11111111  11111011
	  	  last_btn1 = cur_btn1;
	  	  if(temp&(1<<2))
	  		  *LED_GPIO[0]->PxOUT ^= LED_PORT[0];     //��ת��Ӧ��LED*/
	  	  if(temp&(1<<3))
	  		  *LED_GPIO[1]->PxOUT ^= LED_PORT[1];     //��ת��Ӧ��LED*/

	  	  cur_btn2 = P2IN & 0xff;
	  	  temp1 = (cur_btn2 ^ last_btn2) & last_btn2;
	  	  last_btn2 = cur_btn2;
	  	  if(temp1 & (1 << 6))
	  	  *LED_GPIO[3]->PxOUT ^= LED_PORT[3]; 			//��ת��Ӧ��LED
	  	  if(temp1 & (1 <<3))
	  	  *LED_GPIO[2]->PxOUT ^= LED_PORT[2]; 			//��ת��Ӧ��LED

   }
}