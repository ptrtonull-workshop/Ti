#ifndef __FT5426_H
#define __FT5426_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//7寸电容触摸屏-FT206驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/10/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define CT_MAX_TOUCH    5		//电容触摸屏最大支持的点数
#define TP_PRES_DOWN 0x80       //触屏被按下	  
#define TP_CATH_PRES 0x40       //有按键按下了 

//电容触摸屏控制器
typedef struct
{
	u8   (*init)(void);			//初始化触摸屏控制器
	u8   (*scan)(u8);		    //扫描触摸屏 	 
	u16 x[CT_MAX_TOUCH];		//触摸X坐标
	u16 y[CT_MAX_TOUCH];		//触摸Y坐标 
	u8  sta;					//笔的状态 
								//b7:按下1/松开0; 
	                            //b6:0,没有按键按下;1,有按键按下. 
								//b5:保留
								//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)			   	    	  
}ft_ctp_dev;

extern ft_ctp_dev tp_dev;
//与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
//IO操作函数	 
#define FT_RST    				PCout(13)   //FT5206复位引脚
#define FT_INT    				PCin(1)     //FT5206中断引脚	

//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令
  
//FT5206部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT5206模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器


u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT5206_Init(void);
u8 FT5206_Scan(u8 mode);

#endif

















