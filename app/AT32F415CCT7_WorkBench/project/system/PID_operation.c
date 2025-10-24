#include "PID_operation.h"

// 创建结构体
PID pid_210;
PID pid_245;

// 为了防止积分项过度累积，引入积分项的限幅是一种常见的做法。
// 限制积分项的幅值可以防止积分项过度增加，从而限制了系统的累积误差。这样可以避免系统过度响应或者不稳定。
volatile float abs_limit(volatile float value, volatile float ABS_MAX) // 积分限幅，设置最大值。
{
    if (value > ABS_MAX)
    {
        value = ABS_MAX;
    }
    if (value < -ABS_MAX)
    {
        value = -ABS_MAX;
    }

    return value;
}

float PID_Position_Calc(PID *pid, float Target_val, float Actual_val) 
{
    pid->Error = Target_val - Actual_val; 

    pid->SumError += pid->Error; 

    if (pid->SumError >= pid->Integralmax)
    {
        pid->SumError = pid->Integralmax;
    }
    if (pid->SumError <= 0)
    {
        pid->SumError = 0;
    }

    pid->DError = pid->Error - pid->LastError; 
    pid->output = pid->Kp * pid->Error +
                  abs_limit(pid->Ki * pid->SumError, pid->Integralmax) +
                  pid->Kd * pid->DError;
    pid->LastError = pid->Error; 

    if (pid->output > pid->outputmax)
    {
        pid->output = pid->outputmax;
    }

    if (pid->output <= 0)
    {
        pid->output = 0;
    }

    return pid->output; 
}

void PID_Init(PID *pid, float Kp, float Ki, float Kd, float Limit_value)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->PrevError = pid->LastError = pid->Error = pid->SumError = pid->output = 0;
    pid->Integralmax = Limit_value;
    pid->outputmax = Limit_value;
}

// 清除PID数据
void PID_Clear(PID *pid)
{
    pid->PrevError = pid->LastError = pid->Error = pid->SumError = pid->output = 0;
}
// 清除积分项
void PID_Clear_I(PID *pid)
{
    pid->SumError = 0;
}
