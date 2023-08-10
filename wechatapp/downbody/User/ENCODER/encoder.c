#include "encoder.h"
#include "bsp_usart.h"

__IO int16_t EncoderLeft_overflow_cnt = 0;       /* 定时器溢出次数(左) */
__IO int16_t EncoderRight_overflow_cnt = 0;      /* 定时器溢出次数(右) */

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能PA端口时钟

	/* 配置编码器A相、B相输入引脚模式： */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	  //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					        //根据设定参数初始化GPIOA

	/* 时基结构体初始化 */
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;        //定时器周期：TIM_Period+1，设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;                    //设置预分频，输出脉冲频率：72MHz/(ENCODER_TIM_Prescaler+1)/(ENCODER_TIM_Period+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数器计数模式，设置为向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);               //初始化定时器
	
	/* 定时器编码器模式配置 */
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//使用编码器模式3，上升沿计数
  TIM_ICStructInit(&TIM_ICInitStructure);         //填入缺省值
	TIM_ICInitStructure.TIM_ICFilter = 6;           //选择输入比较滤波器 
  TIM_ICInit(TIM2, &TIM_ICInitStructure);         //将TIM_ICInitStructure中的指定参数初始化TIM2
	
	/* 编码器NVIC配置 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //配置中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //设置抢占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //设置子优先级为2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);

  /* 清除所有挂起中断 */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                  //清除TIM2的更新标志位
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
//	//Reset counter
//	TIM_SetCounter(TIM2,0);
	//===============================================
	TIM2->CNT = 0;  //编码器计数值初始化
	//===============================================
	TIM_Cmd(TIM2, ENABLE); 
}

/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能PA端口时钟

	/* 配置编码器A相、B相输入引脚模式： */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	  //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);					        //根据设定参数初始化GPIOA

	/* 时基结构体初始化 */
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;        //定时器周期：TIM_Period+1，设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;                    //设置预分频，输出脉冲频率：72MHz/(ENCODER_TIM_Prescaler+1)/(ENCODER_TIM_Period+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数器计数模式，设置为向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //初始化定时器
	
	/* 定时器编码器模式配置 */
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//使用编码器模式3，上升沿计数
  TIM_ICStructInit(&TIM_ICInitStructure);         //填入缺省值
	TIM_ICInitStructure.TIM_ICFilter = 6;           //选择输入比较滤波器 
  TIM_ICInit(TIM4, &TIM_ICInitStructure);         //将TIM_ICInitStructure中的指定参数初始化TIM4
	
	/* 编码器NVIC配置 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //配置中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //设置抢占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //设置子优先级为3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);

  /* 清除所有挂起中断 */
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);                  //清除TIM4的更新标志位
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
//	//Reset counter
//	TIM_SetCounter(TIM4,0);
	//===============================================
	TIM4->CNT = 0;  //编码器计数值初始化
	//===============================================
	TIM_Cmd(TIM4, ENABLE); 
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)   //溢出中断
	{
		if (!(TIM2->CR1 & TIM_CR1_DIR))
		{
			--EncoderRight_overflow_cnt;    //编码器计数值[向上]溢出
		}
		else
		{
			++EncoderRight_overflow_cnt;    //编码器计数值[向下]溢出
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除中断标志位
	}
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)   //溢出中断
	{
		if (!(TIM4->CR1 & TIM_CR1_DIR))
		{
			--EncoderLeft_overflow_cnt;    //编码器计数值[向上]溢出
		}
		else
		{
			++EncoderLeft_overflow_cnt;    //编码器计数值[向下]溢出
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     //清除中断标志位
	}
}

/**************************************************************************
函数功能：读取编码器脉冲差值
入口参数：TIM_TypeDef * TIMx
返回  值：无
**************************************************************************/
u16 getTIMx_DetaCnt(TIM_TypeDef * TIMx)
{
	u16 cnt;
	cnt = TIMx->CNT-0;
//	printf("c=%d\r\n",cnt);
	TIMx->CNT = 0;
	return cnt;
}

/**************************************************************************
函数功能：读取编码器脉冲累计值
入口参数：TIM_TypeDef * TIMx
返回  值：无
**************************************************************************/
int32_t getTIMx_Cnt(TIM_TypeDef * TIMx)
{
	int32_t cnt;
	if(TIMx == TIM2)  //右侧编码器
	{
	  cnt = EncoderRight_overflow_cnt * (1 + TIMx->ARR) + TIMx->CNT;
	}
	if(TIMx == TIM4)  //左侧编码器
	{
		cnt = EncoderLeft_overflow_cnt * (1 + TIMx->ARR) + TIMx->CNT;
	}
	return cnt;
}

/**************************************************************************
函数功能：计算左右轮速
入口参数：int *leftSpeed,int *rightSpeed
返回  值：无
		//计算左右车轮线速度，正向速度为正值 ，反向速度为负值，速度为乘以1000之后的速度 mm/s
		//一定时间内的编码器变化值*转化率（转化为直线上的距离m）*200s（5ms计算一次） 得到 m/s *1000转化为int数据

		一圈的脉冲数：
			左：7
			右：7
		轮子半径：0.0015
		轮子周长：2*pi*r
		一个脉冲的距离：
			左：0.001345714m
			右：0.001345714m
		速度分辨率：
			左：0.0120m/s 12.0mm/s
			右：0.0181m/s 18.1mm/s
**************************************************************************/
static int32_t leftWheelEncoderNow    = 0;
static int32_t rightWheelEncoderNow   = 0;
static int32_t leftWheelEncoderLast   = 0;
static int32_t rightWheelEncoderLast  = 0;	
void Get_Motor_Speed(int *leftSpeed,int *rightSpeed)
{
	//记录本次左右编码器数据
//	leftWheelEncoderNow += getTIMx_DetaCnt(TIM4);
//	rightWheelEncoderNow+= getTIMx_DetaCnt(TIM2);
	leftWheelEncoderNow = getTIMx_Cnt(TIM4);
	rightWheelEncoderNow = getTIMx_Cnt(TIM2);
//	printf("left=%d,right=%d\r\n",leftWheelEncoderNow,rightWheelEncoderNow);
		
	//5ms测速    	   	
//	*leftSpeed   = leftWheelEncoderNow / 7 * 20;  
//	*rightSpeed  = rightWheelEncoderNow / 7 * 20;	
	*leftSpeed   = (leftWheelEncoderNow - leftWheelEncoderLast)*100;    //计算左侧电机转速，转/min
	*rightSpeed  = (rightWheelEncoderNow - rightWheelEncoderLast)*100;  //计算右侧电机转速，转/min
//  printf("leftSpeed=%d,rightSpeed=%d\r\n",*leftSpeed,*rightSpeed);

	//记录上次编码器数据
	leftWheelEncoderLast  = leftWheelEncoderNow;
	rightWheelEncoderLast = rightWheelEncoderNow;                   
}


