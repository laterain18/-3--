#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "common.h"

typedef struct
{
    int16 current_speed;                 // 当前速度
    int16 target_speed;                  // 目标速度
    int16 output_duty;                   // 输出占空比
    int16 target_curve_speed;                   // 输出 PWM

}MOTOR_STATE;
extern MOTOR_STATE motor_left;
extern MOTOR_STATE motor_right;

#define MOTOR_LEFT_PWM_CHANNEL  PWM_TIM_A0_CH3_B2
#define MOTOR_RIGHT_PWM_CHANNEL PWM_TIM_G6_CH1_B3

#define MOTOR_LEFT_DIR_1 A14
#define MOTOR_LEFT_DIR_2 A13
#define MOTOR_RIGHT_DIR_1 A17
#define MOTOR_RIGHT_DIR_2 A16

void Motor_Init(void);
void Motor_Set_Pwm(int pwm_left,int pwm_right);
void Motor_Stop(void);
void Motor_Out(int left_target_pwm, int right_target_pwm);
float Proportional_Approach(float current_value, float target_value, float proportion);
int16 Proportional_Approach_Int(int16 current_value, int16 target_value, float proportion);

#endif