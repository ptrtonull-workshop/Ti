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
//ALIENTEK Mini STM32开发板扩展实验4
//MP3播放器实验
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
 int main(void)
 { 
	u8 key,fontok=0; 	  
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	  
	uart_init(9600);	 	//串口初始化为9600			 
	LCD_Init();				//初始化液晶 
	LED_Init();         	//LED初始化	 
	KEY_Init();				//按键初始化	  	
	VS_Init();				//初始化VS1053													    
	usmart_dev.init(72);	//usmart初始化	
 	mem_init();				//初始化内存池	    
 	exfuns_init();			//为fatfs相关变量申请内存  
  	f_mount(fs[0],"0:",1); 	//挂载SD卡 
 	f_mount(fs[1],"1:",1); 	//挂载FLASH.
RST:
 	POINT_COLOR=RED;//设置字体为红色	   
	LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	LCD_ShowString(60,70,200,16,16,"Music Player TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"KEY0:NEXT KEY1:PREV");	  
	LCD_ShowString(60,130,200,16,16,"WK_UP:PAUSE/PLAY");	  
	LCD_ShowString(60,150,200,16,16,"2014/3/25");
	while(SD_Initialize())
	{
		LCD_ShowString(60,170,200,16,16,"SD Card Error");
		delay_ms(200);
		LCD_Fill(20,170,200+20,170+16,WHITE);
		delay_ms(200);		       
	}
	fontok=font_init();		//检查字库是否OK 
	if(fontok)				//需要更新字库				 
	{							  
        LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32"); 
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16);//从SD卡更新字库
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//清屏	 
		goto RST;
	}   		    
	while(1)
	{
		Show_Str(60,170,200,16,"存储器测试...",16,0);
		printf("Ram Test:0X%04X\r\n",VS_Ram_Test());//打印RAM测试结果	    
		Show_Str(60,170,200,16,"正弦波测试...",16,0);
 		VS_Sine_Test();	   
		Show_Str(60,170,200,16,"<<音乐播放器>>",16,0); 
		mp3_play();
	}
}
 
