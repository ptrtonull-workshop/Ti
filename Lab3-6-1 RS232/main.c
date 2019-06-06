#include <msp430.h> 
#include "Paper_Display.h"
#include "uart.h"
#define TimesNewRoman 0
#define Arial 1
#define ArialBlack 2
#define size8 0
#define size16 1
volatile unsigned char DisBuffer[250*16];
unsigned char flag0=1;          									//���嶨ʱ�ı�ʶ����
unsigned char send_data[]={'0','\0'};  								//���͵�����
unsigned char recv_data[]={'0','\0'};								//���ܵ�����
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
    P8DIR |= BIT1;
    UART_RS232_Init();												//��ʼ��RS232�ӿ�
    PaperIO_Int();
    INIT_SSD1673();

    Init_buff();
    display(" RS-232 TX & RX   ", 42, 4,TimesNewRoman,size8,1,0);

   	TimerA_Init();													//��ʼ����ʱ��
   	_EINT();														//�����ж�
    while(1)
	{
    	if(flag0)												//flag0�ڶ�ʱ����ʱ����1sʱ����ֵΪ1
		{
    		display("Send Data: ", 42, 42,0,0,1,0);				//TFT����ʾ�������ݵı�ʶ
    		display("Recv Data: ", 42, 60,0,0,1,0);				//TFT����ʾ�������ݵı�ʶ
			flag0=0;											//flag0����
			display(send_data, 130, 42,0,0,1,0);	    			//TFT������ʾ���͵��ַ�
			UCA1TXBUF=send_data[0];								//дһ���ַ������ͻ��淢������
			send_data[0]++;										//�ַ���1
			if(send_data[0]>'9')								//�ַ�����'9'��������'0'
				send_data[0]='0';
			P8OUT ^= BIT1;
		}
	}
}
#pragma vector=USCI_A1_VECTOR										//USCI�жϷ�����
__interrupt void USCI_A1_ISR(void)
{
switch(__even_in_range(UCA1IV,4))
{
case 0:break;													//���ж�
case 2:															//�����жϴ���
	{
		if(UCRXIFG)												//�ȴ���ɽ���
		{
		recv_data[0]=UCA1RXBUF;									//���ݶ���
		display(recv_data, 130, 60,0,0,1,0); 					//TFT��Ļ����ʾ���յ����ַ�
		DIS_IMG(1);
		}
	}
	break;
case 4:break;													//�����жϲ�����
default:break;													//��������޲���

}
}

#pragma vector = TIMER0_A0_VECTOR									//��ʱ��TA�жϷ�����
__interrupt void Timer_A (void)
{
static unsigned int i=0;
i++;
if(i>=1500)
{
	i=0;
	flag0=1;													//�ı��ʶ���ݵ�ֵ
}
}

