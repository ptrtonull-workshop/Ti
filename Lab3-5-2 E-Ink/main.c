#include <msp430.h> 
#include "Paper_Display.h"
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
    PaperIO_Int();
    INIT_SSD1673();
    Init_buff();
//    display("                        ", 42, 0,TimesNewRoman,size8,1,0);
//   display(" Huatsing Instruments   ", 42, 16,TimesNewRoman,size8,1,0);
//    display("012abcABC",80, 0,TimesNewRoman,size16,0,0);
//    display("012abcABC", 0,50,Arial,size8,1,0);
//    display("012abcABC",80,50,Arial,size16,1,0);
//    display("012abcABC", 0,90,ArialBlack,size8,1,0);
//    display("012abcABC",80,90,ArialBlack,size16,1,0);

//    DIS_IMG(1);
    DIS_IMG(2);
//    DIS_IMG(3);
    while(1)
    {
    }

}
