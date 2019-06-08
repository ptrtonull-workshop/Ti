#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//7寸 TFT液晶驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//无
//////////////////////////////////////////////////////////////////////////////////	 

   

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
				     	 					    
//写寄存器函数
//data:寄存器值
void LCD_WR_REG(u8 data)
{ 
 	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
 	LCD_WR_SET;
 	LCD_CS_SET;   
}	  
//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{										   
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出0
 	LCD_RS_SET;
	LCD_CS_CLR;	 
	LCD_RD_CLR;
 	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
 	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;  
}
//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}   
//读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号  
	return LCD_RD_DATA(); 
} 
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
} 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}  		 
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{			  
	u16 t=0;
	u16 temp;	
	LCD_SetCursor(x,y);
 	GPIOB->CRL=0X88888888; 	//PB0-7  上拉输入
	GPIOB->CRH=0X88888888;	//PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;    	//全部输出高
 	while(t<0X1FFF)
	{
	 	LCD_RS_CLR;			//读状态寄存器
		LCD_CS_CLR;
		//读取数据(读寄存器时,并不需要读2次)
		LCD_RD_CLR;
	 	LCD_RD_SET;
		temp=DATAIN;  
		LCD_CS_SET; 
		if(temp&0X0001)break;
		t++;
	}   
 	LCD_RS_SET;//读像素值
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;
 	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET;  
	GPIOB->CRL=0X33333333; 	//PB0-7  上拉输出
	GPIOB->CRH=0X33333333; 	//PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    	//全部输出高
  	return t;				//返回读到的颜色	  														 
}
//LCD背光设置
//pwm:背光等级,0~63.越大越亮.
void LCD_BackLightSet(u8 pwm)
{	
	lcddev.sysreg&=~0X003F;					//清除之前的设置
	lcddev.sysreg|=pwm&0X3F;				//设置新的值		
	LCD_WriteReg(LCD_PREF,lcddev.sysreg);	//写LCD_PREF寄存器	   	 	 
}
//扫描方向上,X的终点坐标.
void LCD_EndXSet(u16 x)
{													    		
	LCD_WriteReg(LCD_END_X,x);				//设置X终坐标  	 	 
}
//LCD开启显示
void LCD_DisplayOn(void)
{	
	LCD_WriteReg(LCD_PREF,lcddev.sysreg);	//恢复LCD_PREF寄存器	   	 
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(LCD_PREF,0);				//关闭TFT,相当于把背光关掉，无背光，无显示  	 
}   
//设置当前显示层
//layer:当前显示层 
void LCD_SetDisplayLayer(u16 layer)
{	 
	lcddev.sysreg&=~0X0E00;				   	//清除之前的设置
	lcddev.sysreg|=(layer&0X07)<<9;			//设置新的值
	LCD_WriteReg(LCD_PREF,lcddev.sysreg);	//写LCD_PREF寄存器	   	 	 
} 
//设置当前操作层
//layer:当前显示层 
void LCD_SetOperateLayer(u16 layer)
{	 
	lcddev.sysreg&=~0X7000;					//清除之前的设置
	lcddev.sysreg|=(layer&0X07)<<12;		//设置新的值
	LCD_WriteReg(LCD_PREF,lcddev.sysreg);	//写LCD_PREF寄存器	   	 	 
}  	     
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	LCD_WriteReg(lcddev.setycmd,Ypos);	//设置Y坐标 	 
	LCD_WriteReg(lcddev.setxcmd,Xpos);	//设置X坐标 	 
} 		 										 
//设置LCD的自动扫描方向
//注意:我们的驱动器,只支持左右扫描设置,不支持上下扫描设置   	   
void LCD_Scan_Dir(u8 dir)
{			   
    switch(dir)
	{
		case L2R_U2D:	//从左到右,从上到下
			LCD_WriteReg(LCD_MIRROR,1);	//写LCD_MIRROR寄存器
			break;
		case L2R_D2U:	//从左到右,从下到上
			LCD_WriteReg(LCD_MIRROR,3);	//写LCD_MIRROR寄存器
			break;
		case R2L_U2D:	//从右到左,从上到下
			LCD_WriteReg(LCD_MIRROR,0);	//写LCD_MIRROR寄存器
			break;
		case R2L_D2U:	//从右到左,从下到上
			LCD_WriteReg(LCD_MIRROR,2);	//写LCD_MIRROR寄存器
			break;	
		default:		//其他,默认从左到右,从上到下
			LCD_WriteReg(LCD_MIRROR,1);	//写LCD_MIRROR寄存器
			break;	 
	}
}  
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
}  	
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	  		 		   
	LCD_WriteReg(lcddev.setycmd,y);	//设置Y坐标 	 
	LCD_WriteReg(lcddev.setxcmd,x);	//设置X坐标  
	LCD_WR_REG(lcddev.wramcmd);
	LCD_WR_DATA(color); 
} 
//设置LCD显示方向（6804不支持横屏显示）
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{	 
}	  
//初始化lcd
void LCD_Init(void)
{ 
	u16 i=0;
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //使能PORTB,C时钟和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTC6~10复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  PORTB推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);

   	LCD_RST=0;
	delay_ms(100);
	LCD_RST=1;		    
	while(i<0X1FFF) 
	{
		if(LCD_RD_DATA()&0x0001)break;//等待控制器准备好
		i++;
  	} 
	lcddev.setxcmd=LCD_CUR_X;	//设置写X坐标指令
 	lcddev.setycmd=LCD_CUR_Y;	//设置写Y坐标指令
 	lcddev.wramcmd=LCD_PIXELS;	//设置写入GRAM的指令
 	lcddev.width=800;			//设置宽度
 	lcddev.height=480;			//设置高度
	LCD_Scan_Dir(L2R_U2D);		//设置默认扫描方向.	 
	LCD_SetDisplayLayer(0);	 	//显示层为0
	LCD_SetOperateLayer(0);		//操作层也为0
	LCD_EndXSet(799);			//x终点坐标为800
	LCD_BackLightSet(63);		//背光设置为最亮
	LCD_Clear(WHITE);			//清屏
}  		  
  
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//得到总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
}  
//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
	}
} 
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD_WR_DATA(color[i*width+j]);//写入数据 
	}	  
} 
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
}								  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }    
	}else//叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}






























