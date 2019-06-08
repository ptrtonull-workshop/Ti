#ifndef __PWM_H
#define __PWM_H
#include "sys.h"


#define LED0_PWM_VAL TIM1->CCR1   

void TIM1_PWM_Init(u16 arr,u16 psc);

#endif
