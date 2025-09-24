#include "PID_operation.h"

// �����ṹ��
PID pid_210;
PID pid_245;

// Ϊ�˷�ֹ����������ۻ��������������޷���һ�ֳ�����������
// ���ƻ�����ķ�ֵ���Է�ֹ������������ӣ��Ӷ�������ϵͳ���ۻ����������Ա���ϵͳ������Ӧ���߲��ȶ���
volatile float abs_limit(volatile float value, volatile float ABS_MAX) // �����޷����������ֵ��
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

// �����ﴫ��ָ�룬�޸�ʱ���޸�ָ�����ֵ��
float PID_Position_Calc(PID *pid, float Target_val, float Actual_val) // λ��ʽPID
{
    pid->Error = Target_val - Actual_val; // ��pid Pϵ����ˡ��������ֵ ��ǰ��ֵ=Ŀ��ֵ-ʵ��ֵ

#if 0

	if (pid->SumError < 0)
	    pid->SumError = 0;
    else 
    {
        pid->SumError += pid->Error;    //��pid Iϵ����ˡ���̬���ֵ ��������Ϊ����ܺͣ���������
    }
#endif

    pid->SumError += pid->Error; // ��pid Iϵ����ˡ���̬���ֵ ��������Ϊ����ܺͣ���������

    if (pid->SumError >= pid->Integralmax)
    {
        pid->SumError = pid->Integralmax;
    }

    if (pid->SumError <= 0)
    {
        pid->SumError = 0;
    }

    pid->DError = pid->Error - pid->LastError; // ��pid Dϵ����ˡ� ΢����-������
    pid->output = pid->Kp * pid->Error +
                  abs_limit(pid->Ki * pid->SumError, pid->Integralmax) +
                  pid->Kd * pid->DError;
    pid->LastError = pid->Error; // �������

    // ����������ֵ����ֹ����ͻ�����⡣���outputmax�����ֵ
    if (pid->output > pid->outputmax)
    {
        pid->output = pid->outputmax;
    }

    if (pid->output < -pid->outputmax)
    {
        //        pid->output = -pid->outputmax;
        pid->output = 0;
    }

    return pid->output; // ���Ϊpwmֵ
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

// ���PID����
void PID_Clear(PID *pid)
{
    pid->PrevError = pid->LastError = pid->Error = pid->SumError = pid->output = 0;
}
// ���������
void PID_Clear_I(PID *pid)
{
    pid->SumError = 0;
}
