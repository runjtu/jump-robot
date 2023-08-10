#include "stm32f10x.h"    //官方库
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "bsp_GeneralTim.h" 
#include "motor.h"
#include "time.h"
#include "bsp_SysTick.h"
#include "./led/bsp_led.h" 
#include "bsp_usart.h"
#include "bsp_adc.h"

uint32_t time = 0;   //0.02s
extern __IO uint16_t ADC_ConvertedValue;
int sign;
// quanju变量，用于保存转换计算后的电压值 	 
extern float ADC_ConvertedValueLocal;  

int main(void)
{
	Motor_12_Config(); //298机驱动初始化
	SysTick_Init();
  //延时函数初始化
	GENERAL_TIM_Init();
	USART_Config();
	GENERAL_TIM4_Init();
	ADCx_Init();
	usart3_init();
	sign=0;
	
	
 
	while(1)
	{
		        if(sign==1)
						{
		        Motor_2_NRun();
						}//蓄力
       		  else if(sign==2)
						{
						Motor_2_STOP();
						}
						else
            {
							Motor_1_STOP();
							Motor_2_STOP();
						}	         
  }
}

