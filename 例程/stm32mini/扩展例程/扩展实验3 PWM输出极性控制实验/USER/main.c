#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "pwm.h" 
//ALIENTEK Mini STM32��������չʵ��3
//PWM������Կ���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾


 int main(void)
 {	
 
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM1_PWM_Init(89,0); //����Ƶ��PWMƵ��=72000/(89+1)=800Khz 
    //����ͨ��ע�ͣ�TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_Low);��䣬���ۿ�������˽����ȫ������漴�ɿ�����
	//�ڲ��ι۲������½�һ��PORTA.8���źţ����ܿ����޸�TIM1->CCER��0λ��ֵ�����Ľ���ˡ�
  	while(1)
	{
		TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_Low);	//�͵�ƽ��Ч			
 		delay_ms(10);	 	  					 
		TIM_SetCompare1(TIM1,45);		
 		delay_ms(10);	 
		TIM_SetCompare1(TIM1,0);	
		delay_ms(10);	 
		TIM_SetCompare1(TIM1,45);	
		delay_ms(10);	 
		TIM_SetCompare1(TIM1,0);		
		TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_High);	//�ߵ�ƽ��Ч			
 		delay_ms(10);	 	  					 
		TIM_SetCompare1(TIM1,45);			
 		delay_ms(10);	 
		TIM_SetCompare1(TIM1,0);			
		delay_ms(10);	 
		TIM_SetCompare1(TIM1,45);			
		delay_ms(10);	 
		TIM_SetCompare1(TIM1,0);			
	} 
}
 
