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
    volatile unsigned int ADvalue = 0;							//�����жϱ���
    char t[5];
	char disp[11]={'0','1','2','3','4','5','6','7','8','9','.'};
	float V1;
	unsigned int V2;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P8DIR |= BIT1;
    P8OUT &=~ BIT1;
    P1DIR |= BIT5;
    P2DIR |= BIT4+BIT5;
    P1OUT |= BIT5;
    P2OUT |= BIT5;
    P2OUT &=~ BIT4;
    initClock();
    ioinit();
    ADInit();													//AD��ʼ��
    PaperIO_Int();
    INIT_SSD1673();

    Init_buff();

    display(" INA210 TEST  ", 42, 4,TimesNewRoman,size8,1,0);


    while(1)
    {
    	ADvalue = ADC12MEM0;									//��ȡADֵ
    	V1=ADvalue;
		V2=(V1/4095*3300)*0.98;										//�����ѹֵ
		t[0]=disp[V2/1000];
		t[1]=disp[V2/100%10];
		t[2]=disp[V2%100/10];
		t[3]='\n';
		display(" I =  ", 42, 48,TimesNewRoman,size8,1,0);
		display(t, 80, 48,TimesNewRoman,size8,1,0);
		display(" mA  ", 104, 48,TimesNewRoman,size8,1,0);
	    DIS_IMG(1);
	    __delay_cycles(40000000);
    }

}
