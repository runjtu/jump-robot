#include "motor.h"  //导入led头文件
#include "stm32f10x.h"  //导入STM32官方库
#include "stm32f10x_rcc.h"  //导入STM32的RCC时钟库
#include "bsp_SysTick.h"       //延时库
 
void Motor_12_Config(void)  //定义初始化函数
{
	GPIO_InitTypeDef GPIO_InitStructure;   //定义GPIO_InitTypeDef结构体
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);  //开启引脚时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //定义IN引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置输出功率
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化GPIOA的引脚参数,写进
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); //所有引脚拉低
}
 
void Motor_1_STOP(void)
{
	IN1(High);
	IN2(High);
}
 
void Motor_1_PRun(void)
{
	IN1(Low);
	IN2(High);
}
 
void Motor_1_NRun(void)
{
	IN1(High);
	IN2(Low);
}
 
void Motor_2_STOP(void)
{
	IN3(Low);
	IN4(Low);
}
 
void Motor_2_PRun(void)
{
	IN3(Low);
	IN4(High);
}
 
void Motor_2_NRun(void)
{
	IN3(High);
	IN4(Low);
}



