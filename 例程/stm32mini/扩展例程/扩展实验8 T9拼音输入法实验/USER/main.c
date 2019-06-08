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
#include "pyinput.h"
#include "string.h"
//ALIENTEK Mini STM32��������չʵ��8
//T9ƴ�����뷨ʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  
   

   							   	 
//���ֱ�
const u8* kbd_tbl[9]={"��","2","3","4","5","6","7","8","9",};
//�ַ���
const u8* kbs_tbl[9]={"DEL","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz",};

//���ؼ��̽���
//x,y:������ʼ����
void py_load_ui(u16 x,u16 y)
{
	u16 i;
	POINT_COLOR=RED;
	LCD_DrawRectangle(x,y,x+180,y+120);						   
	LCD_DrawRectangle(x+60,y,x+120,y+120);						   
	LCD_DrawRectangle(x,y+40,x+180,y+80);
	POINT_COLOR=BLUE;
	for(i=0;i<9;i++)
	{
		Show_Str_Mid(x+(i%3)*60,y+4+40*(i/3),(u8*)kbd_tbl[i],16,60);		
		Show_Str_Mid(x+(i%3)*60,y+20+40*(i/3),(u8*)kbs_tbl[i],16,60);		
	}  		 					   
}
//����״̬����
//x,y:��������
//key:��ֵ��0~8��
//sta:״̬��0���ɿ���1�����£�
void py_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>8)return;
	if(sta)LCD_Fill(x+j*60+1,y+i*40+1,x+j*60+59,y+i*40+39,GREEN);
	else LCD_Fill(x+j*60+1,y+i*40+1,x+j*60+59,y+i*40+39,WHITE); 
	Show_Str_Mid(x+j*60,y+4+40*i,(u8*)kbd_tbl[keyx],16,60);		
	Show_Str_Mid(x+j*60,y+20+40*i,(u8*)kbs_tbl[keyx],16,60);		 
}
//�õ�������������
//x,y:��������
//����ֵ��������ֵ��1~9��Ч��0,��Ч��
u8 py_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,û���κΰ������£�1~9��1~9�Ű�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*60+60)&&tp_dev.x[0]>(x+j*60)&&tp_dev.y[0]<(y+i*40+40)&&tp_dev.y[0]>(y+i*40))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					py_key_staset(x,y,key_x-1,0);
					key_x=key;
					py_key_staset(x,y,key_x-1,1);
				}
				break;
			}
		}  
	}else if(key_x) 
	{
		py_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
							   
//��ʾ���.
//index:0,��ʾû��һ��ƥ��Ľ��.���֮ǰ����ʾ
//   ����,������	
void py_show_result(u8 index)
{
 	LCD_ShowNum(30+144,125,index,1,16); 		//��ʾ��ǰ������
	LCD_Fill(30+40,125,30+40+48,130+16,WHITE);	//���֮ǰ����ʾ
 	LCD_Fill(30+40,145,30+200,145+48,WHITE);	//���֮ǰ����ʾ    
	if(index)
	{
 		Show_Str(30+40,125,200,16,t9.pymb[index-1]->py,16,0); 	//��ʾƴ��
		Show_Str(30+40,145,160,48,t9.pymb[index-1]->pymb,16,0); //��ʾ��Ӧ�ĺ���
		printf("\r\nƴ��:%s\r\n",t9.pymb[index-1]->py);	//�������ƴ��
		printf("���:%s\r\n",t9.pymb[index-1]->pymb);	//����������
	}
}	 	


 int main(void)
 { 
 	u8 i=0;	    	  	    
	u8 result_num;
	u8 cur_index;
	u8 key;
	u8 inputstr[7];		//�������6���ַ�+������
	u8 inputlen;		//���볤��	   
		 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2 
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600			 
	LCD_Init();			//��ʼ��Һ�� 
	LED_Init();         //LED��ʼ��	 
	KEY_Init();			//������ʼ��	  													    
	TP_Init();     		//��������ʼ��
	usmart_dev.init(72);//usmart��ʼ��	
 	mem_init();	//��ʼ���ڲ��ڴ��	

RESTART:
	POINT_COLOR=RED;      
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	     
	}
 	Show_Str(60,5,200,16,"Mini STM32������",16,0);				    	 
	Show_Str(60,25,200,16,"ƴ�����뷨ʵ��",16,0);				    	 
	Show_Str(60,45,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
 	Show_Str(30,65,200,16,"  KEY1:�Ϸ�  KEY0:�·�",16,0);	 
 	Show_Str(30,85,200,16,"KEY_UP:��������",16,0);	 
	Show_Str(30,105,200,16,"����:        ƥ��:  ",16,0);
 	Show_Str(30,125,200,16,"ƴ��:        ��ǰ:  ",16,0);
 	Show_Str(30,145,210,32,"���:",16,0);
	py_load_ui(30,195);
	memset(inputstr,0,7);	//ȫ������
	inputlen=0;				//���볤��Ϊ0
	result_num=0;			//��ƥ��������
	cur_index=0;			
	while(1)
	{
		i++;
		delay_ms(10);
		key=py_get_keynum(30,195);
		if(key)
		{
			if(key==1)//ɾ��
			{
				if(inputlen)inputlen--;
				inputstr[inputlen]='\0';//��ӽ�����
			}else 
			{
				inputstr[inputlen]=key+'0';//�����ַ�
				if(inputlen<7)inputlen++;
			}
			if(inputstr[0]!=NULL)
			{
				key=t9.getpymb(inputstr);	//�õ�ƥ��Ľ����
				if(key)//�в���ƥ��/��ȫƥ��Ľ��
				{
					result_num=key;			//��ƥ����
					cur_index=1;			//��ǰΪ��һ������ 
					if(key&0X80)		   	//�ǲ���ƥ��
					{
						inputlen=key&0X7F;	//��Чƥ��λ��
						inputstr[inputlen]='\0';//��ƥ���λ��ȥ��
						if(inputlen>1)result_num=t9.getpymb(inputstr);//���»�ȡ��ȫƥ���ַ���
					}  
				}else 						//û���κ�ƥ��
				{				   	
					inputlen--;
					inputstr[inputlen]='\0';
				}
			}else
			{
				cur_index=0;
				result_num=0;
			}
			LCD_Fill(30+40,105,30+40+48,110+16,WHITE);	//���֮ǰ����ʾ
			LCD_ShowNum(30+144,105,result_num,1,16); 	//��ʾƥ��Ľ����
			Show_Str(30+40,105,200,16,inputstr,16,0);	//��ʾ��Ч�����ִ�		 
	 		py_show_result(cur_index);					//��ʾ��cur_index��ƥ����
		}	 
		if(result_num)	//����ƥ��Ľ��	
		{	  
			key=KEY_Scan(0);
			switch(key)
			{
				case KEY1_PRES://�Ϸ�
					if(cur_index<result_num)cur_index++;
					else cur_index=1;
					py_show_result(cur_index);	//��ʾ��cur_index��ƥ����
					break;
 				case KEY0_PRES://�·�
	   				if(cur_index>1)cur_index--;
					else cur_index=result_num;
					py_show_result(cur_index);	//��ʾ��cur_index��ƥ����
					break;
				case WKUP_PRES://�������
 					LCD_Fill(30+40,145,30+200,145+48,WHITE);	//���֮ǰ����ʾ    
					goto RESTART;	 		 	   
			}   	 
		}
		if(i==30)
		{
			i=0;
			LED0=!LED0;
		}		   
	}     										    			    
}


