#include <msp430.h>
#include <stdint.h>
#include "captouch.h"



void initClock()
{
	 // Set up XT1
	P5SEL |= BIT4+BIT5;                       					// ѡ�� XT1
	UCSCTL6 &= ~(XT1OFF);                     					// XT1 ����
	UCSCTL6 |= XCAP_3;                        					// Internal load cap

	P5SEL |= BIT2 + BIT3; 										//XT2���Ź���ѡ��
	UCSCTL6 &= ~XT2OFF; 										//����XT2
   while (SFRIFG1 & OFIFG) 										//�ȴ�XT1��XT2��DCO�ȶ�
   {
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }

  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; 		//����DCO�������ܷ�

  UCSCTL1 = DCORSEL_5; 											//6000kHz~23.7MHz
  UCSCTL2 = 20000000 / (4000000 / 16);	 						//XT2Ƶ�ʽϸߣ���Ƶ����Ϊ��׼�ɻ�ø��ߵľ���
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; 					//XT2����16��Ƶ����Ϊ��׼
  while (SFRIFG1 & OFIFG) 										//�ȴ�XT1��XT2��DCO�ȶ�
  {
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; 						//�趨����CLK�ķ�Ƶ
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; 		//�趨����CLK��ʱ��Դ
}



int main( void )
{
	int i,lastflag[2]={0,0};
	WDTCTL = WDTPW + WDTHOLD;									//�رտ��Ź���ֹʱ�������λ
	_DINT();
	initClock();												//��ʼ��ʼ��
	initCapTouch();												//��ʼ����������
	P2DIR|=BIT2;
	P2OUT&=~BIT2;
	for(i=0;i<2;++i)
		*LED_GPIO[i]->PxDIR |= LED_PORT[i]; 					//���ø�LED�����ڶ˿�Ϊ�������
	_EINT();
	while(1)
	{
		uint32_t temp[2] ={0,0};
		for(i=1;i>=0;i--)
		{
		  temp[i]= CapTouch_ReadChannel(i);						//��������������Ƶ��ֵ
		  if((temp[i]>captouch_max)&&(lastflag[i]==0))
		  {														//������ֵ��������ֵʱ��ת
			  *LED_GPIO[i]->PxOUT ^= LED_PORT[i];				//��Ӧ��LEDָʾ��
			  lastflag[i]=1;
		  }
		  else
			  if(!(temp[i]>captouch_max))
			  {
				  lastflag[i]=0;
			  }
		}
		__delay_cycles(8000000);

	}
	return 0;
}
