#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "lcd.h" 
#include "image2lcd.h"
#include "stdlib.h"
//ALIENTEK Mini STM32��������չʵ��6
//�ڲ�flashͼƬ��ʾʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  

extern const u8 gImage_image1[];//ͼƬ����(������Ϣͷ),�洢��image1.c����.

 int main(void)
 { 
	u8 i=0;
  	HEADCOLOR *imginfo;
	u16 x=0,y=0;
	u16 x0,y0;
 	imginfo=(HEADCOLOR*)gImage_image1;	//�õ��ļ���Ϣ 
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();			//LED��ʼ��
 	LCD_Init(); 	    //LCD��ʼ��					 
	delay_ms(1500);		//�ȴ�1.5��	  
	srand(imginfo->h*imginfo->w);
	while(1) 
	{		   
		if(i==0)
		{
			LCD_Clear(0X0000);//����  
			if(imginfo->w>=lcddev.width||imginfo->h>=lcddev.height)
			{
				POINT_COLOR=RED;   	
				delay_ms(200);
				LCD_ShowString(10,70,200,16,16,"The Picture is too large");	
				delay_ms(200);
				continue;
			}
			x0=x;y0=y;
			while((x+imginfo->w)>lcddev.width||x==x0)//������Ļ�ߴ���
			{
				x=rand();//��������xֵ
			}
			while((y+imginfo->h)>lcddev.height||y==y0)//������Ļ�ߴ���
			{
				y=rand();//��������yֵ
			}
			image_display(x,y,(u8*)gImage_image1);//��ָ����ַ��ʾͼƬ
		}
		i++;
		if(i>10)i=0; 
		LED0=!LED0;					 
		delay_ms(200);	
	}											    
}

