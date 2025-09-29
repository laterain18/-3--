#include "headfile.h"

PID_T Motor_Speed_PID; 
PID_T Motor_Turn_PID; 
void PID_Param_Init(void)
{
    PID_Init(&Motor_Speed_PID, 0.0, 0.0, 0.0);
    PID_Init(&Motor_Turn_PID, 50.0, 0.0, 5.0);
}

void PID_Init(PID_T *sptr, float kp, float ki, float kd)
{
    sptr->last_Ek = 0; // �ϴ�ƫ��ֵ��ʼ��
    sptr->Ek_sum = 0;  // ���ϴ�ƫ��ֵ��ʼ��
    sptr->Kp = kp;     // ��������
    sptr->Ki = ki;     // ���ֳ���
    sptr->Kd = kd;     // ΢�ֳ���
    sptr->OUT = 0;
    sptr->OUT1 = 0;
    sptr->P_Out = 0;
    sptr->I_Out = 0;
    sptr->D_Out = 0;
    sptr->OLS_Order = 0;
}

float Positional_PID(PID_T *PID, float SetValue, float ActualValue, float Max_I)
{
    float PIDInc;
    PID->Ek = SetValue - ActualValue;
    PID->Ek_sum += PID->Ek;

    if (PID->Ek_sum > Max_I)
        PID->Ek_sum = Max_I;
    if (PID->Ek_sum < -Max_I)
        PID->Ek_sum = -Max_I;

    PIDInc = (PID->Kp * PID->Ek) +
             (PID->Ki * PID->Ek_sum) +
             (PID->Kd * (PID->Ek - PID->last_Ek));
    PID->last_Ek = PID->Ek;
    return PIDInc;
}

void Set_PID_Param(PID_T *pid, float p, float i, float d)
{
    pid->Kp = p; // ���ñ���ϵ�� P
    pid->Ki = i; // ���û���ϵ�� I
    pid->Kd = d; // ����΢��ϵ�� D
}