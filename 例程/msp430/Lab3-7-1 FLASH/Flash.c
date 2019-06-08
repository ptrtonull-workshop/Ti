#include <msp430.h>
#include "Flash.h"
/*************************** �궨�� ******************************/




/*************************** д�ֽ�  ******************************/

void FlashWriteByte(uint address,uchar *byte,uint count)
{
	FCTL3 = FWKEY;			//Flash����
	while(FCTL3 & BUSY);	//�ȴ�æ
	FCTL1 = FWKEY + WRT;	//ѡ��д��ģʽ
	while(count--)
	{
		*((uchar *)address) = *(byte++); //�ѵ��ֽڻ�����������д���Ӧ�ĵ�ַ
		address = address + 2;//д������ݵ�ַ��˳���ۼ�
	}
	FCTL1 = FWKEY;			//���д��ģʽ
	FCTL3 = FWKEY + LOCK;	//Flash����
	while(FCTL3 & BUSY);	//�ȴ�æ
}

/*************************** д��  ******************************/

void FlashWriteWord(uint address,uint *word,uint count)
{
	FCTL3 = FWKEY;
	while(FCTL3 & BUSY);
	FCTL1 = FWKEY + WRT;
	while(count--)
	{
		*((uint *)address) = *(word++);//��˫�ֽڻ�����������д���Ӧ�ĵ�ַ
		address = address + 2;
	}
	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
	while(FCTL3 & BUSY);
}

/*************************** ��FLASH ***************************/

void FlashRead(uint address,uint *buf,uint mode,uint length)
{
	if(mode)//�ж϶�ȡģʽ��0 ��ʾ ���ֽ� �� 1 ��ʾ˫�ֽڡ�
	{
		while(length--)//�ж϶�ȡ���ݵĸ���
		{
			*(buf++) = *((uint *)address);//�Ѷ�ȡ��˫�ֽ������������ݶ�ȡ������
			address = address + 2;
		}
	}
	else
	{
		while(length--)
		{
			*((uchar *)buf++) = *((uchar *)address);//�Ѷ�ȡ�ĵ��ֽ������������ݶ�ȡ������
			address = address + 2;
		}
	}

}

/*************************** ����FLASH ***************************/

void FlashErase(uint address,uint mode,uint length)
{
	FCTL3 = FWKEY;
	while(FCTL3 & BUSY);
	FCTL1 = FWKEY + ERASE;
	if(mode)//�ж�����ģʽ
	{
		while(length--)//�жϲ������ݸ���
		{
			*((uint *)address) = DELETE;//����Ӧ��ַ�ϵ�˫�ֽ����ݲ���
			address = address + 2;
		}
	}
	else
	{
		while(length--)
		{
			*((uchar *)address) = DELETE;//����Ӧ��ַ�ϵĵ��ֽ����ݲ���
			address = address + 2;
		}
	}
	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
	while(FCTL3 & BUSY);
}
