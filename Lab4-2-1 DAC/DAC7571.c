/*
 * DAC7571.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */
#include "dac7571.h"
void DACValue(unsigned int Value)
{
	unsigned char i,j;
	//Value>>=4;
	j=(unsigned char)Value;
	i=(Value>>8)&0x0f;
	start();
	senddata(0x98);
	waitack();
	senddata(i);
	waitack();
	senddata(j);
	waitack();
	stop();
}


