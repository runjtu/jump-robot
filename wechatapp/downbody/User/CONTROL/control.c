#include "control.h"	
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "bsp_usart.h"

extern int motorLeft;
extern int motorRight;         

/**
  * 函数功能: PID调节函数，5ms定时调用，速度环控制左右两侧电机
  * 输入参数: leftSpeedSet——左侧电机目标速度
	*           rightSpeedSet——右侧电机目标速度
  * 返 回 值: 无
  * 说    明: 无
  */
void pid_control(int leftpeedSet,int rightpeedSet) 
{
	Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);  //获取左右轮子真实速度
	printf("leftSpeed=%d,rightSpeed=%d\r\n",leftSpeedNow,rightSpeedNow);
//	printf("%d\r\n",rightSpeedNow);
	pid_Task_Letf.speedSet  = leftpeedSet;	        //设定左侧电机目标速度
	pid_Task_Right.speedSet = rightpeedSet;        //设定右侧电机目标速度
	pid_Task_Letf.speedNow  = leftSpeedNow;	        //记录左侧电机当前实测速度
	pid_Task_Right.speedNow = rightSpeedNow;        //记录右侧电机当前实测速度
	
	Pid_Ctrl(&motorLeft,&motorRight);               //执行PID控制
	Set_Pwm(motorLeft,motorRight);		              //赋值给PWM寄存器
//	printf("motorLeft=%d，motorRight=%d\r\n",motorLeft,motorRight);
} 








