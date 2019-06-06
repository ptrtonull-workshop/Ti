#include <msp430.h> 
#include "Paper_Display.h"
#include "uart.h"
#define TimesNewRoman 0
#define Arial 1
#define ArialBlack 2
#define size8 0
#define size16 1
volatile unsigned char DisBuffer[250*16];
unsigned char flag0=1;          									//定义定时的标识变量
unsigned char send_data[]={'0','\0'};  								//发送的数据
unsigned char recv_data[]={'0','\0'};								//接受的数据
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
    P8DIR |= BIT1;
    UART_RS232_Init();												//初始化RS232接口
    PaperIO_Int();
    INIT_SSD1673();

    Init_buff();
    display(" RS-232 TX & RX   ", 42, 4,TimesNewRoman,size8,1,0);

   	TimerA_Init();													//初始化定时器
   	_EINT();														//开启中断
    while(1)
	{
    	if(flag0)												//flag0在定时器计时到达1s时被赋值为1
		{
    		display("Send Data: ", 42, 42,0,0,1,0);				//TFT屏显示发送数据的标识
    		display("Recv Data: ", 42, 60,0,0,1,0);				//TFT屏显示接收数据的标识
			flag0=0;											//flag0清零
			display(send_data, 130, 42,0,0,1,0);	    			//TFT屏上显示发送的字符
			UCA1TXBUF=send_data[0];								//写一个字符到发送缓存发送数据
			send_data[0]++;										//字符加1
			if(send_data[0]>'9')								//字符超过'9'则重新置'0'
				send_data[0]='0';
			P8OUT ^= BIT1;
		}
	}
}
#pragma vector=USCI_A1_VECTOR										//USCI中断服务函数
__interrupt void USCI_A1_ISR(void)
{
switch(__even_in_range(UCA1IV,4))
{
case 0:break;													//无中断
case 2:															//接收中断处理
	{
		if(UCRXIFG)												//等待完成接收
		{
		recv_data[0]=UCA1RXBUF;									//数据读出
		display(recv_data, 130, 60,0,0,1,0); 					//TFT屏幕上显示接收到的字符
		DIS_IMG(1);
		}
	}
	break;
case 4:break;													//发送中断不处理
default:break;													//其他情况无操作

}
}

#pragma vector = TIMER0_A0_VECTOR									//定时器TA中断服务函数
__interrupt void Timer_A (void)
{
static unsigned int i=0;
i++;
if(i>=1500)
{
	i=0;
	flag0=1;													//改变标识数据的值
}
}

