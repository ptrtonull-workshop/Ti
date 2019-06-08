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
#include "touch.h"	   
#include "atk_ncr.h"
//ALIENTEK Mini STM32��������չʵ��7
//��дʶ��ʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  
    							   	 
//����¼�Ĺ켣���� 
 atk_ncr_point READ_BUF[200]; 
 int main(void)
 { 
 	u8 i=0;	    	
	u8 tcnt=0;    
	u8 res[10];
	u8 key;		    
	u16 pcnt=0;
	u8 mode=4;	//Ĭ���ǻ��ģʽ	 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2 
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600			 
	LCD_Init();			//��ʼ��Һ�� 
	LED_Init();         //LED��ʼ��	 
	KEY_Init();			//������ʼ��	  													    
	TP_Init();     		//��������ʼ�� 
 	mem_init();			//��ʼ���ڲ��ڴ��	
	alientek_ncr_init();//��ʼ����дʶ��
	POINT_COLOR=RED;  
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	     
	}
RESTART:
	POINT_COLOR=RED;      
 	Show_Str(60,10,200,16,"MiniSTM32������",16,0);				    	 
	Show_Str(60,30,200,16,"��дʶ��ʵ��",16,0);				    	 
	Show_Str(60,50,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
	Show_Str(60,70,200,16,"KEY1:MODE KEY0:Adjust",16,0);			    
	Show_Str(60,90,200,16,"ʶ����:",16,0);			    
	LCD_DrawRectangle(19,114,220,315);
 	POINT_COLOR=BLUE;      
	Show_Str(96,207,200,16,"��д��",16,0);	 
	tcnt=100;
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			TP_Adjust();  	//��ĻУ׼
			LCD_Clear(WHITE); 
			goto RESTART;	//���¼��ؽ���
		}
		if(key==KEY1_PRES)	
		{
			LCD_Fill(20,115,219,314,WHITE);//�����ǰ��ʾ
			mode++;
			if(mode>4)mode=1;
			switch(mode)
			{
				case 1:
					Show_Str(80,207,200,16,"��ʶ������",16,0);	
					break;	 	    
				case 2:
					Show_Str(64,207,200,16,"��ʶ���д��ĸ",16,0);	
					break;	 	    
				case 3:
					Show_Str(64,207,200,16,"��ʶ��Сд��ĸ",16,0);	
					break;	 	    
				case 4:
					Show_Str(88,207,200,16,"ȫ��ʶ��",16,0);	
					break;	 
			}
			tcnt=100;
		}		   
 		tp_dev.scan(0);//ɨ��
 		if(tp_dev.sta&TP_PRES_DOWN)//�а���������
		{				  
			delay_ms(1);//��Ҫ����ʱ,��������Ϊ�а�������.
 			tcnt=0;//�ɿ�ʱ�ļ�������� 	 		    
			if((tp_dev.x[0]<220&&tp_dev.x[0]>=20)&&(tp_dev.y[0]<315&&tp_dev.y[0]>=115))
			{			 
				TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],BLUE);//��ͼ 
				if(pcnt<200)//�ܵ�������200
				{
					if(pcnt)
					{
						if((READ_BUF[pcnt-1].y!=tp_dev.y[0])&&(READ_BUF[pcnt-1].x!=tp_dev.x[0]))//x,y�����
						{
							READ_BUF[pcnt].x=tp_dev.x[0];
							READ_BUF[pcnt].y=tp_dev.y[0]; 
							pcnt++;
						}	
					}else 
					{
						READ_BUF[pcnt].x=tp_dev.x[0];
						READ_BUF[pcnt].y=tp_dev.y[0]; 
						pcnt++;
					}		  
				}   						  				  
			}  
		}else //�����ɿ��� 
		{
			tcnt++;
			delay_ms(10);	  
			//��ʱʶ��
			i++;	 	    
			if(tcnt==40)
			{
				if(pcnt)//����Ч������		 
				{
					printf("�ܵ���:%d\r\n",pcnt);
					alientek_ncr(READ_BUF,pcnt,6,mode,(char*)res);
					printf("ʶ����:%s\r\n",res);
					pcnt=0;	 			    			   
	  				POINT_COLOR=BLUE;//���û�����ɫ
		 			LCD_ShowString(60+72,90,200,16,16,res);	    
				}
				LCD_Fill(20,115,219,314,WHITE);
			} 
		}  
		if(i==30)
		{
			i=0;
			LED0=!LED0;
		}		   
	}     										    			    
}
