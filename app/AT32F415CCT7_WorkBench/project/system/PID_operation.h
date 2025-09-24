#ifndef __PID_OPERATION_H__
#define __PID_OPERATION_H__
#include "at32f415.h"

typedef struct PID
{
	float Kp; //  Proportional Const  Pϵ��
	float Ki; //  Integral Const      Iϵ��
	float Kd; //  Derivative Const    Dϵ��

	volatile float PrevError; //  Error[-2]
	volatile float LastError; //  Error[-1]
	volatile float Error;	  //  Error[0 ]
	volatile float DError;	  // pid->Error - pid->LastError
	volatile float SumError;  //  Sums of Errors

	volatile float output;

	volatile float Integralmax; // ����������ֵ
	volatile float outputmax;	// ���������ֵ
} PID;

extern PID pid_210;
extern PID pid_245;
void PID_Init(PID *pid, float Kp, float Ki, float Kd, float Limit_value);
void PID_Clear(PID *pid);
void PID_Clear_I(PID *pid);
float PID_Position_Calc(PID *pid, float Target_val, float Actual_val);
#endif
