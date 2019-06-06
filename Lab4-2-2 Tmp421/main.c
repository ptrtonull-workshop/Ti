#include <msp430.h>
#include "IIC.h"
#include "TMP421.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P8DIR |= BIT1;
    unsigned char a1=0,a2=0;
    unsigned char ca1[3],ca2[3];

    Clockint();					//时钟初始化
    PaperIO_Int();				//IO初始化
    INIT_SSD1673();				//电子纸初始化
    InitIIC();					//I2C初始化
    TMP_Init();					//tmp421初始化
    while(1)
    {
    Init_buff();
    a1=R_I2C(0x55,0x00)-64;
    a2=R_I2C(0x55,0x10);
    if(a1>3)
    {
    	P8OUT ^= BIT1;
    }
    a2 = a2 >> 4;
    a2 = (a2 * 625)/100;
    ca1[0] =(a1/10)+0x30;
    ca1[1] =(a1%10)+0x30;
    ca1[2] =='\n';
    ca2[0] =(a2/10)+0x30;
    ca2[1] =(a2%10)+0x30;
    ca2[2] =='\n';
	display("Local Temp  :    ", 26, 48,0,0,1,0);
	display(&ca1, 126, 48,0,0,1,0);
	display(".", 142, 48,0,0,1,0);
	display(&ca2, 150, 48,0,0,1,0);
	display("     ", 166, 48,0,0,1,0);
	DIS_IMG(1);
	__delay_cycles(100000000);
   }
}
