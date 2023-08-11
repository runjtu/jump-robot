#ifndef __MOTOR1_H
#define __MOTOR1_H
 
 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
 
#define High    1
#define Low     0
 
#define IN1(a) if (a)  \
	GPIO_SetBits(GPIOA,GPIO_Pin_2);\
else  \
	GPIO_ResetBits(GPIOA,GPIO_Pin_2)
 
#define IN2(a) if (a)  \
	GPIO_SetBits(GPIOA,GPIO_Pin_3);\
else  \
	GPIO_ResetBits(GPIOA,GPIO_Pin_3)
 
#define IN3(a) if (a)  \
	GPIO_SetBits(GPIOA,GPIO_Pin_4);\
else  \
	GPIO_ResetBits(GPIOA,GPIO_Pin_4)
 
#define IN4(a) if (a)  \
	GPIO_SetBits(GPIOA,GPIO_Pin_5);\
else  \
	GPIO_ResetBits(GPIOA,GPIO_Pin_5)
 
void Motor_12_Config(void);
 
void Motor_1_STOP(void);
void Motor_1_PRun(void);
void Motor_1_NRun(void);
 
void Motor_2_STOP(void);
void Motor_2_PRun(void);
void Motor_2_NRun(void);
 
#endif
