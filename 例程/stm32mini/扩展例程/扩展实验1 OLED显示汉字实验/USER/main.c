#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "oled.h" 
#include "text.h" 
//ALIENTEK Mini STM32��������չʵ�� 1
//OLED��ʾ����ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
   

 int main(void)
 {	
 
	delay_init();	     //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ� 
	OLED_Init();		//��ʼ��OLED 
	OLED_ShowString(16,0,"ALIENTEK",24); 	//24*24����
	OLED_ShowString(28,24,"MiniSTM32",16);  //16*16����
	OLED_Show_Font(28,40,0);				//��
	OLED_Show_Font(28+24,40,1);				//��
	OLED_Show_Font(28+48,40,2);				//�� 
	OLED_Refresh_Gram();//������ʾ��OLED	  
	while(1) 
	{		 
		delay_ms(500);
		LED0=!LED0;
	}	  
}



