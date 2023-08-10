/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
//#include "./led/bsp_led.h" 
#include "bsp_GeneralTim.h" 
#include "bsp_SysTick.h"
#include "bsp_AdvanceTim.h"
#include "bsp_adc.h"
#include "motor.h" 

extern void TimingDelay_Decrement(void);
extern volatile uint32_t time;
extern __IO uint16_t ADC_ConvertedValue;
float ADC_ConvertedValueLocal;  
extern int sign;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
	
	
void ADC_IRQHandler(void)
{	
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
	{
		// 读取ADC的转换值
		ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
	}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
		if(ucTemp=='1')
		   {
	          Motor_2_NRun();//蓄力
       		  SysTick_Delay_Ms(33000);
	          Motor_2_STOP();
		    }
		else if(ucTemp=='2')//展翅
		{

           	Motor_2_PRun();//解蓄力
						SysTick_Delay_Ms(400); 
						Motor_2_STOP();//蓄力电机停止
						Motor_1_NRun();//展翅
						SysTick_Delay_Ms(1000);//延时		      
			      Motor_1_STOP();//展翅电机停止

		}
		else if(ucTemp=='T')//展翅
		{
			      sign=1;

		}
				else if(ucTemp=='S')//展翅
		{               
	          Motor_2_STOP();
			      sign=2;
		}
		else if(ucTemp=='3')
		{
           	Motor_1_PRun();//收翅
	          SysTick_Delay_Ms(1000);
            Motor_1_STOP();
			      Motor_2_STOP();
//			      USART_SendData(DEBUG_USARTx,ADC_ConvertedValueLocal);
		
		}
		else if(ucTemp=='4')//zhanchi
		{
           	Motor_1_NRun();//展翅
	          SysTick_Delay_Ms(1000);
            Motor_1_STOP();//收翅
//			      USART_SendData(DEBUG_USARTx,ADC_ConvertedValueLocal);
		}
		else if(ucTemp=='6')//zhanchi
		{
			      Motor_2_PRun();//解蓄力
						SysTick_Delay_Ms(600); 
						Motor_2_STOP();//蓄力电机停止
			      Motor_1_NRun();//展翅
						SysTick_Delay_Ms(1000);//延时		      
			      Motor_1_STOP();//展翅电机停止
    }
	  else if(ucTemp=='0')
		{

	          Motor_2_NRun();//蓄力
       		  SysTick_Delay_Ms(25000);
	          Motor_2_STOP();	
		 }
		else if(ucTemp=='A')//展翅
		{

           	Motor_2_PRun();//解蓄力
						SysTick_Delay_Ms(320); 
						Motor_2_STOP();//蓄力电机停止
						Motor_1_NRun();//展翅
						SysTick_Delay_Ms(1000);//延时		      
			      Motor_1_STOP();//展翅电机停止

		}

		else 
		{
//			LED_YELLOW;
			        USART_SendData(USART3,ucTemp);
		}
//		 USART_SendData(DEBUG_USARTx,Data);
//	   USART_SendData(DEBUG_USARTx,ucTemp); 
     USART_ClearITPendingBit(DEBUG_USARTx,USART_IT_RXNE);		
	}	 
}


void  TIM4_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM4, TIM_IT_Update) != RESET ) 
	{	
		time=time+1;
		TIM_ClearITPendingBit( TIM4 , TIM_FLAG_Update);  		 
	}		 	
}

void  USART3_IRQHandler()
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{	
		  
	}
	 USART_ClearITPendingBit(USART3,USART_IT_RXNE);	
}
//void USART3_IRQHandler(void)
//{
//  uint8_t ucTemp;
//	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	   {
//			 
//	   }
//}
	
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


void SysTick_Handler(void)
{
	TimingDelay_Decrement();	
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
