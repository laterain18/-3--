#include "headfile.h"
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

MOTOR_STATE motor_left;
MOTOR_STATE motor_right; 


//-------------------------------------------------------------------------------------------------------------------
// �������     �����ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��    
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void Motor_Init(void)
{
    motor_left.current_speed  = 0;
    motor_left.target_speed   = 0;
    motor_left.output_duty    = 0;
    motor_left.target_curve_speed = 300; // ��ʼ��Ŀ�������ٶ�

    motor_right.current_speed = 0;
    motor_right.target_speed  = 0;
    motor_right.output_duty   = 0;
    motor_right.target_curve_speed = 2500; // ��ʼ��Ŀ�������ٶ�

    gpio_init(MOTOR_LEFT_DIR_1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_LEFT_DIR_2, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_RIGHT_DIR_1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_RIGHT_DIR_2, GPO, GPIO_LOW, GPO_PUSH_PULL); 

    pwm_init(MOTOR_LEFT_PWM_CHANNEL,12500,0); 
    pwm_init(MOTOR_RIGHT_PWM_CHANNEL,12500,0); 
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������
// ����˵��     pwm_left = ����ռ�ձ�
// ����˵��     pwm_right = �ҵ��ռ�ձ�
// ���ز���     void
// ʹ��ʾ��     
// ��ע��Ϣ     
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
    // ֹͣ����
    pwm_set_duty(MOTOR_RIGHT_PWM_CHANNEL, 0);
    // ֹͣ�ҵ��
    pwm_set_duty(MOTOR_LEFT_PWM_CHANNEL, 0);
}
void Motor_Out(int left_target_pwm, int right_target_pwm)
{
//    int right_target_pwm = -right_target_pwm1;
    int16 turn_output_pwm = 0;
    turn_output_pwm = Positional_PID(&Motor_Turn_PID, 0, error, 2000); // ����ת�� PID ������
    LIMIT_VAL(turn_output_pwm, -10000, 10000); // ����ת�������Χ

    left_target_pwm += turn_output_pwm;  // ��������Ŀ�� PWM
    right_target_pwm -= turn_output_pwm; // �����ҵ��Ŀ�� PWM
    // ����Ŀ���ٶ�
    motor_left.output_duty = left_target_pwm;
    motor_right.output_duty = right_target_pwm;
    // ��� PWM
    Motor_Set_Pwm(motor_left.output_duty,motor_right.output_duty);

}
// ͨ�ñ����ƽ�Ŀ��ֵ����
// current_value: ��ǰֵ��ָ��
// target_value: Ŀ��ֵ
// proportion: �ƽ�����ϵ�� (0~1)��ֵԽ��ƽ�Խ��
// ����ֵ: ���º�ĵ�ǰֵ
float Proportional_Approach(float current_value, float target_value, float proportion)
{
    if(proportion < 0.0f) proportion = 0.0f;
    if(proportion > 1.0f) proportion = 1.0f;
    
    current_value = current_value + (target_value - current_value) * proportion;
    return current_value;
}

// ���Ͱ汾�ı����ƽ�����
int16 Proportional_Approach_Int(int16 current_value, int16 target_value, float proportion)
{
    if(proportion < 0.0f) proportion = 0.0f;
    if(proportion > 1.0f) proportion = 1.0f;
    
    current_value = current_value + (int16)((target_value - current_value)  * proportion);
    return current_value;
}