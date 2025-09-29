#ifndef _PID_H_
#define _PID_H_

#include "common.h"

typedef struct {
    float target_val;          
    float Kp;                  
    float Ki;                  
    float Kd;                  
    float Ek;                  
    float last_Ek;             
    float Ek_sum;              
    float OUT;                 
    float OUT1;
    float P_Out;
    float I_Out;
    float D_Out;
    unsigned short OLS_Order;               //最小二乘法求微分样本数
} PID_T;

extern PID_T Motor_Speed_PID; 
extern PID_T Motor_Turn_PID; 

void PID_Param_Init(void);
void PID_Init(PID_T *sptr, float kp, float ki, float kd);
float Positional_PID(PID_T *PID, float SetValue, float ActualValue, float Max_I);
void Set_PID_Param(PID_T *pid, float p, float i, float d);

#endif