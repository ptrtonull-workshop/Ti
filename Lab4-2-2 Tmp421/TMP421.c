/*
 * TMP421.c
 *
 *  Created on: 2015Äê11ÔÂ18ÈÕ
 *      Author: Lenovo
 */
#include "TMP421.h"
#include "IIC.h"
void TMP_Init(void)
{
	W_I2C(0x54,0x9,0x4);
	W_I2C(0x54,0x21,0x3);
	W_I2C(0x54,0x0b,0x07);
}


