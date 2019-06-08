#include "ctiic.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//7寸电容触摸屏驱动-IIC通信部分	  
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
//初始化IIC
void CT_IIC_Init(void)
{				
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟  	

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;		//PC0&PC3 推挽输出	 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
  GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_3);						 //PA.8 输出高
 
}
//产生IIC起始信号
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda线输出
	CT_IIC_SDA=1;	  	  
	CT_IIC_SCL=1;
	delay_us(1);
 	CT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	CT_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda线输出
	CT_IIC_SCL=0;
	CT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	CT_IIC_SCL=1; 
	CT_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(1);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入  
	CT_IIC_SDA=1;delay_us(1);	   
	CT_IIC_SCL=1;delay_us(1);	 
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		}
	}
	CT_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void CT_IIC_Ack(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_IIC_SDA=0;
	delay_us(1);
	CT_IIC_SCL=1;
	delay_us(1);
	CT_IIC_SCL=0;
}
//不产生ACK应答		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_IIC_SDA=1;
	delay_us(1);
	CT_IIC_SCL=1;
	delay_us(1);
	CT_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        CT_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	      
		CT_IIC_SCL=1;
		delay_us(1); 
		CT_IIC_SCL=0;	
		delay_us(1);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        CT_IIC_SCL=0; 	    	   
		delay_us(1);
		CT_IIC_SCL=1;
        receive<<=1;
        if(CT_READ_SDA)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//发送nACK
	else CT_IIC_Ack(); //发送ACK   
 	return receive;
}




