#include "headfile.h"
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

MOTOR_STATE motor_left;
MOTOR_STATE motor_right; 


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机初始化
// 参数说明     void
// 返回参数     void
// 使用示例    
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void Motor_Init(void)
{
    motor_left.current_speed  = 0;
    motor_left.target_speed   = 0;
    motor_left.output_duty    = 0;
    motor_left.target_curve_speed = 300; // 初始化目标曲线速度

    motor_right.current_speed = 0;
    motor_right.target_speed  = 0;
    motor_right.output_duty   = 0;
    motor_right.target_curve_speed = 2500; // 初始化目标曲线速度

    gpio_init(MOTOR_LEFT_DIR_1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_LEFT_DIR_2, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_RIGHT_DIR_1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_RIGHT_DIR_2, GPO, GPIO_LOW, GPO_PUSH_PULL); 

    pwm_init(MOTOR_LEFT_PWM_CHANNEL,12500,0); 
    pwm_init(MOTOR_RIGHT_PWM_CHANNEL,12500,0); 
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机输出
// 参数说明     pwm_left = 左电机占空比
// 参数说明     pwm_right = 右电机占空比
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void Motor_Set_Pwm(int pwm_left,int pwm_right)
{
    pwm_left = LIMIT_VAL(pwm_left, -8000, 8000);
    pwm_right = LIMIT_VAL(pwm_right, -8000, 8000);

    motor_left.output_duty  = pwm_left;
    motor_right.output_duty = pwm_right;

    if(pwm_left >= 0)
    {
        gpio_set_level(MOTOR_LEFT_DIR_2,1);
        gpio_set_level(MOTOR_LEFT_DIR_1,0);
        pwm_set_duty(MOTOR_LEFT_PWM_CHANNEL, pwm_left);
    }
    else
    {
        gpio_set_level(MOTOR_LEFT_DIR_2,0);
        gpio_set_level(MOTOR_LEFT_DIR_1,1);
        pwm_set_duty(MOTOR_LEFT_PWM_CHANNEL, -pwm_left);
    }
    if(pwm_right >= 0)
    {
        gpio_set_level(MOTOR_RIGHT_DIR_2,1);
        gpio_set_level(MOTOR_RIGHT_DIR_1,0);
        pwm_set_duty(MOTOR_RIGHT_PWM_CHANNEL, pwm_right);
    }
    else 
    {
        gpio_set_level(MOTOR_RIGHT_DIR_2,0);
        gpio_set_level(MOTOR_RIGHT_DIR_1,1);
        pwm_set_duty(MOTOR_RIGHT_PWM_CHANNEL, -pwm_right);
    }
}

void Motor_Stop(void)
{
    // 停止左电机
    pwm_set_duty(MOTOR_RIGHT_PWM_CHANNEL, 0);
    // 停止右电机
    pwm_set_duty(MOTOR_LEFT_PWM_CHANNEL, 0);
}
void Motor_Out(int left_target_pwm, int right_target_pwm)
{
//    int right_target_pwm = -right_target_pwm1;
    int16 turn_output_pwm = 0;
    turn_output_pwm = Positional_PID(&Motor_Turn_PID, 0, error, 2000); // 调用转向 PID 控制器
    LIMIT_VAL(turn_output_pwm, -10000, 10000); // 限制转向输出范围

    left_target_pwm += turn_output_pwm;  // 调整左电机目标 PWM
    right_target_pwm -= turn_output_pwm; // 调整右电机目标 PWM
    // 设置目标速度
    motor_left.output_duty = left_target_pwm;
    motor_right.output_duty = right_target_pwm;
    // 输出 PWM
    Motor_Set_Pwm(motor_left.output_duty,motor_right.output_duty);

}
// 通用比例逼近目标值函数
// current_value: 当前值的指针
// target_value: 目标值
// proportion: 逼近比例系数 (0~1)，值越大逼近越快
// 返回值: 更新后的当前值
float Proportional_Approach(float current_value, float target_value, float proportion)
{
    if(proportion < 0.0f) proportion = 0.0f;
    if(proportion > 1.0f) proportion = 1.0f;
    
    current_value = current_value + (target_value - current_value) * proportion;
    return current_value;
}

// 整型版本的比例逼近函数
int16 Proportional_Approach_Int(int16 current_value, int16 target_value, float proportion)
{
    if(proportion < 0.0f) proportion = 0.0f;
    if(proportion > 1.0f) proportion = 1.0f;
    
    current_value = current_value + (int16)((target_value - current_value)  * proportion);
    return current_value;
}