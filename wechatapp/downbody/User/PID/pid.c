#include "pid.h"
#include "bsp_usart.h"

struct pid_uint pid_Task_Letf;
struct pid_uint pid_Task_Right;

/****************************************************************************
*函数名称：PID_Init(void)
*函数功能：初始化PID结构体参数
****************************************************************************/
void PID_Init(void)
{
//乘以1024原因避免出现浮点数运算，全部是整数运算，这样PID控制器运算速度会更快
/***********************左轮速度pid****************************/
	pid_Task_Letf.Kp = 4;//1024 * 0.5;//0.4
 	pid_Task_Letf.Ki = 0.44;//1024 * 0;	
	pid_Task_Letf.Kd = 0;//1024 * 1.949; 
	pid_Task_Letf.Ur = 7000;//1024*7000;
	pid_Task_Letf.Adjust   = 0;
	pid_Task_Letf.En       = 1;
	pid_Task_Letf.speedSet = 0;
	pid_Task_Letf.speedNow = 0;
	reset_Uk(&pid_Task_Letf);		
/***********************右轮速度pid****************************/
	pid_Task_Right.Kp = 5;//1024*0.6;//0.2左下0.5,1.949
 	pid_Task_Right.Ki = 0.5;	//不使用积分
	pid_Task_Right.Kd = 0;//1024*2.2; 
	pid_Task_Right.Ur = 7000;//1024*7000;
	pid_Task_Right.Adjust   = 0;
	pid_Task_Right.En       = 1;
	pid_Task_Right.speedSet = 0;
	pid_Task_Right.speedNow = 0;
	reset_Uk(&pid_Task_Right);
}

/***********************************************************************************************
 函 数 名：void reset_Uk(PID_Uint *p)
 功    能：初始化U_kk,ekk,ekkk
 说    明：在初始化时调用，改变PID参数时有可能需要调用
 入口参数：PID单元的参数结构体 地址
************************************************************************************************/
void reset_Uk(struct pid_uint *p)
{
	p->U_kk=0;
	p->ekk=0;
	p->ekkk=0;
}

/***********************************************************************************************
 函 数 名：s32 PID_commen(int set,int jiance,PID_Uint *p)
 功    能：PID计算函数
 说    明：求任意单个PID的控制量
 入口参数：期望值，实测值，PID单元结构体
 返 回 值：PID控制量
************************************************************************************************/
s32 PID_common(int set,int jiance,struct pid_uint *p)
{
	int ek=0,U_k=0;

	//ek=jiance - set;   
  ek = set - jiance; 	
	
	U_k=p->U_kk + p->Kp*(ek - p->ekk) + p->Ki*ek + p->Kd*(ek - 2*p->ekk + p->ekkk);
	
	p->U_kk=U_k;
  p->ekkk=p->ekk;
	p->ekk=ek;
	
	if(U_k>(p->Ur))		                                    
    U_k=p->Ur;
	if(U_k<-(p->Ur))
		U_k=-(p->Ur);

	return U_k; 
}

/***********************************************************************************
** 函数名称 ：void Pid_Which(struct pid_uint *pl, struct pid_uint *pr)
** 函数功能 ：pid选择函数	      
***********************************************************************************/
void Pid_Which(struct pid_uint *pl, struct pid_uint *pr)
{
	/**********************左轮速度pid*************************/
	if(pl->En == 1)
	{							
//    printf("pls=%d,pln=%d\r\n",pl->speedSet, pl->speedNow);		
		pl->Adjust = PID_common(pl->speedSet, pl->speedNow, pl);	
//    printf("pr=%d,px=%d\r\n",pr->speedSet, pr->speedNow);			
	}	
	else
	{
		pl->Adjust = 0;
		reset_Uk(pl);
		pl->En = 1; 
	}
	/***********************右轮速度pid*************************/
//	 printf("En=%d\r\n",pr->En);	
	if(pr->En == 1)
	{
//		printf("prs=%d,prn=%d\r\n",pr->speedSet, pr->speedNow);		
		pr->Adjust = PID_common(pr->speedSet, pr->speedNow, pr);	
//    printf("pr=%d,px=%d\r\n",pr->speedSet, pr->speedNow);			
	}	
	else
	{
		pr->Adjust = 0;
		reset_Uk(pr);
		pr->En = 1; 
	}
}

/*******************************************************************************
 * 函数名：Pid_Ctrl(int *leftMotor,int  *rightMotor)
 * 描述  ：Pid控制
 *******************************************************************************/
void Pid_Ctrl(int *leftMotor,int  *rightMotor)
{
	Pid_Which(&pid_Task_Letf, &pid_Task_Right);
//  printf("pla=%d,pra=%d\r\n",pid_Task_Letf.Adjust, pid_Task_Right.Adjust);	
//	*leftMotor  += pid_Task_Letf.Adjust;
//	*rightMotor += pid_Task_Right.Adjust;
	*leftMotor  = pid_Task_Letf.Adjust;
	*rightMotor = pid_Task_Right.Adjust;
}


