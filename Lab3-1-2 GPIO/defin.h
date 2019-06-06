/*
 * defin.h
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#ifndef DEFIN_H_
#define DEFIN_H_
#include <stdint.h>
#include <msp430.h>
typedef struct                   //以指针形式定义P1口的各个寄存器
{
  const volatile uint8_t* PxIN;     //定义一个不会被编译的无符号字符型变量
  volatile uint8_t* PxOUT;
  volatile uint8_t* PxDIR;
  volatile uint8_t* PxREN;
  volatile uint8_t* PxSEL;
} GPIO_TypeDef;
const GPIO_TypeDef GPIO8 =
{&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL};

const GPIO_TypeDef GPIO3 =
{&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL};

const GPIO_TypeDef GPIO7 =
{&P7IN, &P7OUT, &P7DIR, &P7REN, &P7SEL};

const GPIO_TypeDef GPIO6 =
{&P6IN, &P6OUT, &P6DIR, &P6REN, &P6SEL};


const GPIO_TypeDef* LED_GPIO[4] = {  &GPIO8, &GPIO3, &GPIO6, &GPIO3};
const uint8_t LED_PORT[4] = { BIT1, BIT7, BIT4, BIT5};


#endif /* DEFIN_H_ */
