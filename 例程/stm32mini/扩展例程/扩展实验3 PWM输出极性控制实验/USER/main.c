#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "pwm.h" 
//ALIENTEK Mini STM32开发板扩展实验3
//PWM输出极性控制实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司


 int main(void)
 {	
 
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	 //初始化与LED连接的硬件接口
	TIM1_PWM_Init(89,0); //不分频。PWM频率=72000/(89+1)=800Khz 
    //可以通过注释：TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_Low);这句，来观看结果，此结果完全软件仿真即可看到。
	//在波形观察里面新建一个PORTA.8的信号，就能看到修改TIM1->CCER第0位的值产生的结果了。
  	while(1)
	{
		TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_Low);	//低电平有效			
 		delay_ms(10);	 	  					 
		TIM_SetCompare1(TIM1,45);		
 		delay_ms(10);	 
		TIM_SetCompare1(TIM1,0);	
		delay_ms(10);	 
		TIM_SetCompare1(TIM1,45);	
		delay_ms(10);	 
		TIM_SetCompare1(TIM1,0);		
		TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_High);	//高电平有效			
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
 
