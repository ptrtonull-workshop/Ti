#include <msp430.h>
#include "Include.h"
#include "Paper_Display.h"

#if 1
const unsigned char init_data[]={
0x82,0x00,0x00,0x00,0xAA,0x00,0x00,0x00,
0xAA,0xAA,0x00,0x00,0xAA,0xAA,0xAA,0x00,
0x55,0xAA,0xAA,0x00,0x55,0x55,0x55,0x55,
0xAA,0xAA,0xAA,0xAA,0x55,0x55,0x55,0x55,
0xAA,0xAA,0xAA,0xAA,0x15,0x15,0x15,0x15,
0x05,0x05,0x05,0x05,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x41,0x45,0xF1,0xFF,0x5F,0x55,0x01,0x00,
0x00,0x00,

};
#else
const unsigned char init_data[]={
0x00,0x00,0x00,0x55,0x00,0x00,0x55,0x55,
0x00,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0xAA,0x55,0x55,0xAA,0xAA,0xAA,0xAA,
0x05,0x05,0x05,0x05,0x15,0x15,0x15,0x15,
0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x34,0x32,0xF1,0x74,0x14,0x00,0x00,0x00,
0x00,0x00,};
#endif


void MyRESET()
{
	nRST_L;
	DELAY_mS(10);//1ms
 	nRST_H;
  	DELAY_mS(10);//1ms
}
void DELAY_100nS(unsigned int delaytime)   // 30us
{
	int i,j;
	for(i=0;i<delaytime;i++)
		for(j=0;j<10;j++);
}

void DELAY_mS(unsigned int delaytime)    	// 1ms
{
	int i,j;
	for(i=0;i<delaytime;i++)
		{for(j=0;j<2000;j++);}
}
void DELAY_S(unsigned int delaytime)     //  1s
{
	int i;
	for(i=0;i<delaytime;i++)
        {
		__delay_cycles(3276);
        }
}
void DELAY_M(unsigned int delaytime)     //  1M
{
	int i;
	for(i=0;i<delaytime;i++)
		DELAY_S(60);
}
void READBUSY()
{
  while(1)
  {	 //=1 BUSY
     if((P2IN & BIT2)==0)
    	 break;
  }
  DELAY_M(2);
}
void FIRST_PICTURE()
{
	SPI4W_WRITECOM(0x21);
 	SPI4W_WRITEDATA(0x83);
	SPI4W_WRITECOM(0x22);
 	SPI4W_WRITEDATA(0xC4);
}
void INIT_SPD2701()
{
    MyRESET();
    READBUSY();
#if 0
	//initial code
	SPI4W_WRITECOM(0x10);//exit deep sleep mode
	SPI4W_WRITEDATA(0x00);
	SPI4W_WRITECOM(0x11);//data enter mode
	SPI4W_WRITEDATA(0x03);
	SPI4W_WRITECOM(0x44);//set RAM x address start/end, in page 36
	SPI4W_WRITEDATA(0x00);//RAM x address start at 00h;
	//  SPI4W_WRITEDATA(0x1f);
	SPI4W_WRITEDATA(0x11);//RAM x address end at 11h(17)->72: [because 1F(31)->128 and 12(18)->76]
	SPI4W_WRITECOM(0x45);//set RAM y address start/end, in page 37
	SPI4W_WRITEDATA(0x00);//RAM y address start at 00h;
	//  SPI4W_WRITEDATA(0xb3);
	SPI4W_WRITEDATA(0xAB);//RAM y address start at ABh(171)->172: [because B3(179)->180]
	SPI4W_WRITECOM(0x4E);//set RAM x address count to 0;
	SPI4W_WRITEDATA(0x00);
	SPI4W_WRITECOM(0x4F);//set RAM y address count to 0;
	SPI4W_WRITEDATA(0x00);
	SPI4W_WRITECOM(0x21);
	SPI4W_WRITEDATA(0x03);
	SPI4W_WRITECOM(0xF0);//booster feedback used, in page 37
	SPI4W_WRITEDATA(0x1F);
	WRITE_LUT();
	////////////////////////////////////////////////////////
	SPI4W_WRITECOM(0x2C);//vcom
	SPI4W_WRITEDATA(0xA0);
#ifdef D1000ms
	SPI4W_WRITECOM(0x3B);
	SPI4W_WRITEDATA(0x0f);
#endif
	SPI4W_WRITECOM(0x3C);//board
	SPI4W_WRITEDATA(0x63);
	SPI4W_WRITECOM(0x22);//display updata sequence option ,in page 33
	SPI4W_WRITEDATA(0xC4);//enable sequence: clk -> CP -> LUT -> initial display -> pattern display
#else
        //initial code
	SPI4W_WRITECOM(0x10);//exit deep sleep mode
	SPI4W_WRITEDATA(0x00);
	SPI4W_WRITECOM(0x11);//data enter mode
	SPI4W_WRITEDATA(0x03);
	SPI4W_WRITECOM(0x44);//set RAM x address start/end, in page 36
	SPI4W_WRITEDATA(0x00);//RAM x address start at 00h;
	SPI4W_WRITEDATA(0x11);//RAM x address end at 11h(17)->72: [because 1F(31)->128 and 12(18)->76]

	SPI4W_WRITECOM(0x45);//set RAM y address start/end, in page 37
    SPI4W_WRITEDATA(0x00);//RAM y address start at 00h;
    SPI4W_WRITEDATA(0xAB);//RAM y address start at ABh(171)->172: [because B3(179)->180]


	SPI4W_WRITECOM(0x4E);//set RAM x address count to 0;
	SPI4W_WRITEDATA(0x00);
	SPI4W_WRITECOM(0x4F);//set RAM y address count to 0;
	SPI4W_WRITEDATA(0xab);

	SPI4W_WRITECOM(0x21);
	SPI4W_WRITEDATA(0x03);
	SPI4W_WRITECOM(0xF0);//booster feedback used, in page 37
	SPI4W_WRITEDATA(0x1F);

    SPI4W_WRITECOM(0x2C);//vcom
	SPI4W_WRITEDATA(0xA0);

    SPI4W_WRITECOM(0x3C);//board
	SPI4W_WRITEDATA(0x63);
	SPI4W_WRITECOM(0x22);//display updata sequence option ,in page 33
	SPI4W_WRITEDATA(0xC4);//enable sequence: clk -> CP -> LUT -> initial display -> pattern display
	WRITE_LUT();
	////////////////////////////////////////////////////////



#endif

}

void WRITE_LUT()
{
	unsigned char i;
	SPI4W_WRITECOM(0x32);//write LUT register
	for(i=0;i<90;i++)
        SPI4W_WRITEDATA(init_data[i]);//write LUT register
}

void SetpointXY(unsigned int xs,unsigned int xe,unsigned int ys,unsigned int ye)
{
	SPI4W_WRITECOM(0x44);//set RAM x address start/end, in page 36
	SPI4W_WRITEDATA(xs);//RAM x address start at 00h;
	SPI4W_WRITEDATA(xe);//RAM x address end at 11h(17)->72: [because 1F(31)->128 and 12(18)->76]

	SPI4W_WRITECOM(0x45);//set RAM y address start/end, in page 37
    SPI4W_WRITEDATA(ys);//RAM y address start at 00h;
    SPI4W_WRITEDATA(ye);//RAM y address start at ABh(171)->172: [because B3(179)->180]

	SPI4W_WRITECOM(0x4E);//set RAM x address count to 0;
	SPI4W_WRITEDATA(xs);
	SPI4W_WRITECOM(0x4F);//set RAM y address count to 0;
	SPI4W_WRITEDATA(ye);

	SPI4W_WRITECOM(0x24);
}

/*void DisplayString(const char* str, unsigned int x0, unsigned int y0)
{

	int i;
    SetpointXY(x0,x0+1,y0,y0+15);

	for(i = 0; i<strlen((const char*)str);i++)
	{
		if(((unsigned char)(*(str+i))) >= 161)
		{
			temp[0] = *(str+i);
			temp[1] = '\0';
			return;
		}

		else
		{
			qm = *(str+i);

			ulOffset = (unsigned char)(qm) * 16;

            for (j = 0; j < 16; j ++)
            {
				ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
           	}

            for(i = 0; i<(strlen((const char*)str))*128;i++)
            {
            	SPI4W_WRITEDATA(ywbuf[i]);
           }

		}
	}

	DELAY_S(200);
	FIRST_PICTURE();
	SPI4W_WRITECOM(0x20);
	DELAY_mS(1);
	READBUSY();
}

void displaychar(unsigned char str,unsigned int x,unsigned int y,const unsigned char buff)
{
	int i;
	unsigned int tempbuff[]={0x00,0x00,0x03,0x1C,0x07,0x00,0x00,0x00,0x04,0x3C,0xC4,0x40,0x40,0xE4,0x1C,0x04}
	for(i=0;i<8;i++)
	{
		buff[y*72+x+i] = tempbuff[i]
	}
}
*/
void display(int pic_n,unsigned char delay)
{
	int i;
	  SetpointXY(0,0x11,0,0xab);
	  SPI4W_WRITECOM(0x24);
	switch (pic_n)
	{
		case 0:
		{
			  for(i=0;i<3096;i++)
			  {
			    SPI4W_WRITEDATA(gImage_audio[i]);
			  }
			  break;
		}
		default:
		{
			  for(i=0;i<3096;i++)
			  {
			    SPI4W_WRITEDATA(0x00);
			  }
			  break;
		}

	}
	DELAY_S(20);
	FIRST_PICTURE();
	SPI4W_WRITECOM(0x20);
	DELAY_mS(100);
	READBUSY();
	DELAY_S(20);
	closebump();
	DELAY_S(5000);
}

void SET_EYEONE()
{

}
void closebump()
{
  SPI4W_WRITECOM(0x22);//display updata sequence option ,in page 33
  SPI4W_WRITEDATA(0x03);
  //enterdeepsleep();
  SPI4W_WRITECOM(0x20);
}
void enterdeepsleep()
{
  SPI4W_WRITECOM(0x10);
  SPI4W_WRITEDATA(0x01);
}
void image_a0()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_180[i]);
  }
}

void image_a1()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_185[i]);
  }
}

void image_a2()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_190[i]);
  }
}

void image_a3()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_195[i]);
  }
}

void image_a4()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_200[i]);
  }
}

void image_a5()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_205[i]);
  }
}

void image_a6()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_210[i]);
  }
}

void image_a7()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_215[i]);
  }
}

void image_a8()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_220[i]);
  }
}

void image_a9()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(gImage_225[i]);
  }
}

void image_a10()
{
  int i;
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
    SPI4W_WRITEDATA(0x00);
  }
}
void image_a11()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<3096;i++)
  {
	SPI4W_WRITEDATA(0xFF);
  }
}

void image_a12()
{
  int i;
  SetpointXY(0,0x11,0,0xab);
  SPI4W_WRITECOM(0x24);
  for(i=0;i<1548;i++)
  {
    SPI4W_WRITEDATA(0xFF);
  }
  for(i=0;i<1548;i++)
  {
    SPI4W_WRITEDATA(0x00);
  }
}

void SPI4W_WRITECOM(unsigned char INIT_COM)
{
	unsigned char TEMPCOM;
	unsigned char scnt;
	TEMPCOM=INIT_COM;
	nCS_H;
	nCS_L;
	SCLK_L;
	nDC_L;
	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
			SDA_H;
		else
			SDA_L;
		DELAY_100nS(10);
		SCLK_H;
		DELAY_100nS(10);
		SCLK_L;
		TEMPCOM=TEMPCOM<<1;
		DELAY_100nS(10);
	}
	nCS_H;
}

void SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
	unsigned char TEMPCOM;
	unsigned char scnt;
	TEMPCOM=INIT_DATA;
	nCS_H;
	nCS_L;
	SCLK_L;
	nDC_H;
	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
			SDA_H;
		else
			SDA_L;
		DELAY_100nS(10);
		SCLK_H;
		DELAY_100nS(10);
		SCLK_L;
		TEMPCOM=TEMPCOM<<1;
		DELAY_100nS(10);
	}
	nCS_H;
}

void PaperIO_Int(void)
{
	P1DIR |= BIT4;
	P2DIR |= BIT7;
	P2DIR &= ~BIT2;
	P3DIR |= BIT2 + BIT3 + BIT4;

}



