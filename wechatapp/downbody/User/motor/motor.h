#ifndef __MOTOR_H
#define __MOTOR_H
 
 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
 
#include "pid.h"

extern int    motorLeft,motorRight;                      //电机PWM变量
//速度
extern int    leftSpeedNow; 
extern int    rightSpeedNow; 
//乘以1000之后的速度设定值
extern int    leftSpeedSet; 
extern int    rightSpeedSet; 
 
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




#define PWMA   TIM1->CCR1    //PA8
//#define AIN2   PBout(15)
//#define AIN1   PBout(14)
//#define BIN1   PBout(13)
//#define BIN2   PBout(12)
#define PWMB   TIM1->CCR4    //PA11


void Set_Pwm(int moto1,int moto2);
//u8 Turn_Off(int voltage);
int myabs(int a);

 
void Motor_12_Config(void);

void Motor_1_STOP(void);
void Motor_1_PRun(void);
void Motor_1_NRun(void);
 
void Motor_2_STOP(void);
void Motor_2_PRun(void);
void Motor_2_NRun(void);



#endif
