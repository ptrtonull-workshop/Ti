#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "oled.h" 
#include "text.h" 
//ALIENTEK Mini STM32开发板扩展实验 1
//OLED显示汉字实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
   

 int main(void)
 {	
 
	delay_init();	     //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	LED_Init();		  	//初始化与LED连接的硬件接口 
	OLED_Init();		//初始化OLED 
	OLED_ShowString(16,0,"ALIENTEK",24); 	//24*24字体
	OLED_ShowString(28,24,"MiniSTM32",16);  //16*16字体
	OLED_Show_Font(28,40,0);				//开
	OLED_Show_Font(28+24,40,1);				//发
	OLED_Show_Font(28+48,40,2);				//板 
	OLED_Refresh_Gram();//更新显示到OLED	  
	while(1) 
	{		 
		delay_ms(500);
		LED0=!LED0;
	}	  
}



