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
    sptr->last_Ek = 0; // 上次偏差值初始化
    sptr->Ek_sum = 0;  // 上上次偏差值初始化
    sptr->Kp = kp;     // 比例常数
    sptr->Ki = ki;     // 积分常数
    sptr->Kd = kd;     // 微分常数
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
    pid->Kp = p; // 设置比例系数 P
    pid->Ki = i; // 设置积分系数 I
    pid->Kd = d; // 设置微分系数 D
}