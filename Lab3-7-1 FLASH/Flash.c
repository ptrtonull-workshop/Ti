#include <msp430.h>
#include "Flash.h"
/*************************** 宏定义 ******************************/




/*************************** 写字节  ******************************/

void FlashWriteByte(uint address,uchar *byte,uint count)
{
	FCTL3 = FWKEY;			//Flash解锁
	while(FCTL3 & BUSY);	//等待忙
	FCTL1 = FWKEY + WRT;	//选择写入模式
	while(count--)
	{
		*((uchar *)address) = *(byte++); //把单字节缓冲区的数据写入对应的地址
		address = address + 2;//写入的数据地址按顺序累加
	}
	FCTL1 = FWKEY;			//清除写入模式
	FCTL3 = FWKEY + LOCK;	//Flash上锁
	while(FCTL3 & BUSY);	//等待忙
}

/*************************** 写字  ******************************/

void FlashWriteWord(uint address,uint *word,uint count)
{
	FCTL3 = FWKEY;
	while(FCTL3 & BUSY);
	FCTL1 = FWKEY + WRT;
	while(count--)
	{
		*((uint *)address) = *(word++);//把双字节缓冲区的数据写入对应的地址
		address = address + 2;
	}
	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
	while(FCTL3 & BUSY);
}

/*************************** 读FLASH ***************************/

void FlashRead(uint address,uint *buf,uint mode,uint length)
{
	if(mode)//判断读取模式：0 表示 单字节 ； 1 表示双字节。
	{
		while(length--)//判断读取数据的个数
		{
			*(buf++) = *((uint *)address);//把读取的双字节数据送入数据读取缓冲区
			address = address + 2;
		}
	}
	else
	{
		while(length--)
		{
			*((uchar *)buf++) = *((uchar *)address);//把读取的单字节数据送入数据读取缓冲区
			address = address + 2;
		}
	}

}

/*************************** 擦除FLASH ***************************/

void FlashErase(uint address,uint mode,uint length)
{
	FCTL3 = FWKEY;
	while(FCTL3 & BUSY);
	FCTL1 = FWKEY + ERASE;
	if(mode)//判断数据模式
	{
		while(length--)//判断擦除数据个数
		{
			*((uint *)address) = DELETE;//将对应地址上的双字节数据擦除
			address = address + 2;
		}
	}
	else
	{
		while(length--)
		{
			*((uchar *)address) = DELETE;//将对应地址上的单字节数据擦除
			address = address + 2;
		}
	}
	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
	while(FCTL3 & BUSY);
}
