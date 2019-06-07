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
	 UCSCTL6 &= ~XT1OFF; //启动XT1
	 P5SEL |= BIT2 + BIT3; //XT2引脚功能选择
	 UCSCTL6 &= ~XT2OFF;          //打开XT2
	 __bis_SR_register(SCG0);
	 UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
	 UCSCTL1 = DCORSEL_4;       //DCO频率范围在28.2MHZ以下
	 UCSCTL2 = FLLD_5 + 1;       //D=16，N=1
	 UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLK时钟源为XT2CLK；DCOCLK=D*(N+1)*(FLLREFCLK/n);DCOCLKDIV=(N+1)*(FLLREFCLK/n);
	 UCSCTL4 = SELA_4 + SELS_3 +SELM_3;    //ACLK的时钟源为DCOCLKDIV,MCLK\SMCLK的时钟源为DCOCLK
	 UCSCTL5 = DIVA_5 +DIVS_1;      //ACLK由DCOCLKDIV的32分频得到，SMCLK由DCOCLK的2分频得到
	             //最终MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ

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
	volatile unsigned int value = 0;												//设置判断变量
    while(1)
    {
		ADC12CTL0 |= ADC12SC;														//开始采样转换
		value = ADC12MEM0;															//把结果赋给变量
		if(value > 5)																	//判断结果范围
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
