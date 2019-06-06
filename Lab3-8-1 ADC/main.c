#include <msp430.h> 
#include "Paper_Display.h"
#include "ad.h"
#define TimesNewRoman 0
#define Arial 1
#define ArialBlack 2
#define size8 0
#define size16 1
volatile unsigned char DisBuffer[250*16];
void initClock()
{
	 UCSCTL6 &= ~XT1OFF; //����XT1
	 P5SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
	 UCSCTL6 &= ~XT2OFF;          //��XT2
	 __bis_SR_register(SCG0);
	 UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
	 UCSCTL1 = DCORSEL_4;       //DCOƵ�ʷ�Χ��28.2MHZ����
	 UCSCTL2 = FLLD_5 + 1;       //D=16��N=1
	 UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLKʱ��ԴΪXT2CLK��DCOCLK=D*(N+1)*(FLLREFCLK/n);DCOCLKDIV=(N+1)*(FLLREFCLK/n);
	 UCSCTL4 = SELA_4 + SELS_3 +SELM_3;    //ACLK��ʱ��ԴΪDCOCLKDIV,MCLK\SMCLK��ʱ��ԴΪDCOCLK
	 UCSCTL5 = DIVA_5 +DIVS_1;      //ACLK��DCOCLKDIV��32��Ƶ�õ���SMCLK��DCOCLK��2��Ƶ�õ�
	             //����MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ

//	 __bic_SR_register(SCG0);                   //Enable the FLL control loop

}

int main(void) {
	int i=180;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P8DIR |= BIT1;
    P8OUT &=~ BIT1;
    initClock();
	ioinit();
	AD_Init();
    PaperIO_Int();
    INIT_SSD1673();
    Init_buff();
    display(" ADC INPUT ", 82, 4,TimesNewRoman,size8,1,0);
    DIS_IMG(2);
	volatile unsigned int value = 0;												//�����жϱ���
    while(1)
    {
		ADC12CTL0 |= ADC12SC;														//��ʼ����ת��
		value = ADC12MEM0;															//�ѽ����������
		if(value > 5)																	//�жϽ����Χ
			P8OUT |= BIT1;
		else
			P8OUT &= ~BIT1;
		if(value >= 800)
			P3OUT |= BIT7;
		else
			P3OUT &= ~BIT7;
		if(value >= 1600)
			P7OUT |= BIT4;
		else
			P7OUT &= ~BIT4;
		if(value >= 2400)
			P6OUT |= BIT3;
		else
			P6OUT &= ~BIT3;
		if(value >= 3200)
			P6OUT |= BIT4;
		else
			P6OUT &= ~BIT4;
		if(value >= 4000)
			P3OUT |= BIT5;
		else
			P3OUT &= ~BIT5;
		__delay_cycles(200000 );
    }

}
