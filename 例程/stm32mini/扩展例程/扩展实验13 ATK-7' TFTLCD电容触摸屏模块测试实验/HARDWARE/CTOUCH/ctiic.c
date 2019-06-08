#include "ctiic.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//7����ݴ���������-IICͨ�Ų���	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//��
//////////////////////////////////////////////////////////////////////////////////	 
//��ʼ��IIC
void CT_IIC_Init(void)
{				
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��  	

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;		//PC0&PC3 �������	 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
  GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_3);						 //PA.8 �����
 
}
//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�����
	CT_IIC_SDA=1;	  	  
	CT_IIC_SCL=1;
	delay_us(1);
 	CT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	CT_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�����
	CT_IIC_SCL=0;
	CT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	CT_IIC_SCL=1; 
	CT_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(1);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
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
	CT_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
 	CT_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        CT_IIC_SCL=0; 	    	   
		delay_us(1);
		CT_IIC_SCL=1;
        receive<<=1;
        if(CT_READ_SDA)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//����nACK
	else CT_IIC_Ack(); //����ACK   
 	return receive;
}




