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
//ALIENTEK Mini STM32开发板扩展实验7
//手写识别实验
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
    							   	 
//最大记录的轨迹点数 
 atk_ncr_point READ_BUF[200]; 
 int main(void)
 { 
 	u8 i=0;	    	
	u8 tcnt=0;    
	u8 res[10];
	u8 key;		    
	u16 pcnt=0;
	u8 mode=4;	//默认是混合模式	 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2 
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600			 
	LCD_Init();			//初始化液晶 
	LED_Init();         //LED初始化	 
	KEY_Init();			//按键初始化	  													    
	TP_Init();     		//触摸屏初始化 
 	mem_init();			//初始化内部内存池	
	alientek_ncr_init();//初始化手写识别
	POINT_COLOR=RED;  
 	while(font_init()) 				//检查字库
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示	     
	}
RESTART:
	POINT_COLOR=RED;      
 	Show_Str(60,10,200,16,"MiniSTM32开发板",16,0);				    	 
	Show_Str(60,30,200,16,"手写识别实验",16,0);				    	 
	Show_Str(60,50,200,16,"正点原子@ALIENTEK",16,0);				    	 
	Show_Str(60,70,200,16,"KEY1:MODE KEY0:Adjust",16,0);			    
	Show_Str(60,90,200,16,"识别结果:",16,0);			    
	LCD_DrawRectangle(19,114,220,315);
 	POINT_COLOR=BLUE;      
	Show_Str(96,207,200,16,"手写区",16,0);	 
	tcnt=100;
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			TP_Adjust();  	//屏幕校准
			LCD_Clear(WHITE); 
			goto RESTART;	//重新加载界面
		}
		if(key==KEY1_PRES)	
		{
			LCD_Fill(20,115,219,314,WHITE);//清除当前显示
			mode++;
			if(mode>4)mode=1;
			switch(mode)
			{
				case 1:
					Show_Str(80,207,200,16,"仅识别数字",16,0);	
					break;	 	    
				case 2:
					Show_Str(64,207,200,16,"仅识别大写字母",16,0);	
					break;	 	    
				case 3:
					Show_Str(64,207,200,16,"仅识别小写字母",16,0);	
					break;	 	    
				case 4:
					Show_Str(88,207,200,16,"全部识别",16,0);	
					break;	 
			}
			tcnt=100;
		}		   
 		tp_dev.scan(0);//扫描
 		if(tp_dev.sta&TP_PRES_DOWN)//有按键被按下
		{				  
			delay_ms(1);//必要的延时,否则老认为有按键按下.
 			tcnt=0;//松开时的计数器清空 	 		    
			if((tp_dev.x[0]<220&&tp_dev.x[0]>=20)&&(tp_dev.y[0]<315&&tp_dev.y[0]>=115))
			{			 
				TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],BLUE);//画图 
				if(pcnt<200)//总点数少于200
				{
					if(pcnt)
					{
						if((READ_BUF[pcnt-1].y!=tp_dev.y[0])&&(READ_BUF[pcnt-1].x!=tp_dev.x[0]))//x,y不相等
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
		}else //按键松开了 
		{
			tcnt++;
			delay_ms(10);	  
			//延时识别
			i++;	 	    
			if(tcnt==40)
			{
				if(pcnt)//有有效的输入		 
				{
					printf("总点数:%d\r\n",pcnt);
					alientek_ncr(READ_BUF,pcnt,6,mode,(char*)res);
					printf("识别结果:%s\r\n",res);
					pcnt=0;	 			    			   
	  				POINT_COLOR=BLUE;//设置画笔蓝色
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
