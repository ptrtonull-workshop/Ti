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
    volatile unsigned int ADvalue = 0;							//设置判断变量
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
    ADInit();													//AD初始化
    PaperIO_Int();
    INIT_SSD1673();

    Init_buff();

    display(" INA210 TEST  ", 42, 4,TimesNewRoman,size8,1,0);


    while(1)
    {
    	ADvalue = ADC12MEM0;									//读取AD值
    	V1=ADvalue;
		V2=(V1/4095*3300)*0.98;										//计算电压值
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
