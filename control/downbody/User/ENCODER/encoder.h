#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h" 

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);

u16 getTIMx_DetaCnt(TIM_TypeDef * TIMx);
int32_t getTIMx_Cnt(TIM_TypeDef * TIMx);
void Get_Motor_Speed(int *leftSpeed,int *rightSpeed);
void left1(int *leftSpeed1,int *rightSpeed1);
#endif
