#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "lcd.h"   
#include "key.h" 
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	
#include "vs10XX.h"	
#include "mp3player.h"
#include "recorder.h"
//ALIENTEK Mini STM32��������չʵ��5
//¼����ʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  

 int main(void)
 { 
	u8 key,fontok=0;  
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600			 
	LCD_Init();				//��ʼ��Һ�� 
	LED_Init();         	//LED��ʼ��	 
	KEY_Init();				//������ʼ��	  	
	VS_Init();				//��ʼ��VS1053													    
	usmart_dev.init(72);	//usmart��ʼ��	
 	mem_init();				//��ʼ���ڴ��	    
 	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
  	f_mount(fs[0],"0:",1); 	//����SD�� 
 	f_mount(fs[1],"1:",1); 	//����FLASH.
RST:
 	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	LCD_ShowString(60,30,200,16,16,"Mini STM32");	
	LCD_ShowString(60,50,200,16,16,"RECORDER TEST");	
	LCD_ShowString(60,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,90,200,16,16,"KEY0:STOP&SAVE");
	LCD_ShowString(60,110,200,16,16,"KEY1:REC/PAUSE");	  
	LCD_ShowString(60,130,200,16,16,"WK_UP:PLAY ");	  
	LCD_ShowString(60,150,200,16,16,"2014/3/26");
	while(SD_Initialize())
	{
		LCD_ShowString(60,170,200,16,16,"SD Card Error");
		delay_ms(200);
		LCD_Fill(20,170,200+20,170+16,WHITE);
		delay_ms(200);		       
	}
	fontok=font_init();		//����ֿ��Ƿ�OK 
	if(fontok)				//��Ҫ�����ֿ�				 
	{							  
        LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32"); 
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16);//��SD�������ֿ�
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//����	 
		goto RST;
	}   		    
	while(1)
	{
		Show_Str(60,170,200,16,"�洢������...",16,0);
		printf("Ram Test:0X%04X\r\n",VS_Ram_Test());//��ӡRAM���Խ��	    
		Show_Str(60,170,200,16,"���Ҳ�����...",16,0);
 		VS_Sine_Test();	   
		Show_Str(60,170,200,16,"<<¼����ʵ��>>",16,0); 
		recoder_play();
	}
}






