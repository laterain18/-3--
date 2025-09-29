#include "headfile.h"

float gyro_zero_x = 0.0f; // 陀螺仪零点偏移
float gyro_zero_y = 0.0f; // 陀螺仪零点偏移
float gyro_zero_z = 0.0f; // 陀螺仪零点偏移
float gyro_x_rad = 0.0f; // 陀螺仪 X 轴角速度（弧度）
float gyro_y_rad = 0.0f; // 陀螺仪 Y 轴角速度（弧度）
float gyro_z_rad = 0.0f; // 陀螺仪 Z 轴角速度（弧度）
float acc_x = 0.0f; // 加速度计 X 轴加速度（m/s?）
float acc_y = 0.0f; // 加速度计 Y 轴加速度（m/s?）
float acc_z = 0.0f; // 加速度计 Z 轴加速度（m/s?）
float PI = 3.1415926f; // 圆周
float Lowpass_Alpha = 0.9; // 低通滤波系数

float Kp = 5.0f; //不要调整这里的KP，这里的KP实际上并不生效
#define Ki 0.0000f
#define halfT 0.022f; // 采样周期的一半，用于求解四元数微分方程时计算角增量
float pitch = 0, roll = 0, yaw = 0;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // 初始姿态四元数，由上篇博文提到的变换四元数公式得来
float exInt = 0, eyInt = 0, ezInt = 0; // 当前加计测得的重力加速度在三轴上的分量
float I_ex, I_ey, I_ez; // 误差积分
float alpha_a = 0.9;
uint8 power_on_flag = 1;
int yaw_round_count = 0;
float yaw_last = 0;
float yaw_total_angle = 0;

void Gyro_Init(void)
{
    uint16 i;
    LED_ON();
    Init_ICM42688();  // 初始化陀螺仪
    system_delay_ms(2000); // 等待陀螺仪稳定
    for(i = 0; i < 1000; i++)
    {
        Get_Gyro_ICM42688(); // 获取陀螺仪数据
        gyro_zero_x += icm42688_gyro_x; // 累加 X 轴数据
        gyro_zero_y += icm42688_gyro_y; // 累加 Y 轴数据
        gyro_zero_z += icm42688_gyro_z; // 累加 Z 轴数据
    }
    gyro_zero_x /= 1000.0f; // 计算 X 轴零点偏移
    gyro_zero_y /= 1000.0f; // 计算 Y 轴零点偏移
    gyro_zero_z /= 1000.0f; // 计算 Z 轴零点偏移
    flag.gryo_start = 1; // 设置陀螺仪开始标志
    LED_OFF();
}

void Get_Gyro(void)
{
    Get_Gyro_ICM42688(); // 获取陀螺仪数据
    
    icm42688_gyro_x -= gyro_zero_x; // 减去 X 轴零点偏移
    icm42688_gyro_y -= gyro_zero_y; // 减去 Y 轴零点偏移
    icm42688_gyro_z -= gyro_zero_z; // 减去 Z 轴零点偏移

	if(fabsf(icm42688_gyro_x)<0.2)
	    icm42688_gyro_x = 0;

    if(fabsf(icm42688_gyro_y)<0.2)
        icm42688_gyro_y = 0;

    if(fabsf(icm42688_gyro_z)<0.2)
        icm42688_gyro_z = 0;

}

void Get_Acc(void)
{
    static float last_acc_x, last_acc_y, last_acc_z;

    Get_Acc_ICM42688(); // 获取加速度计数据

    acc_x = last_acc_x * Lowpass_Alpha + (1 - Lowpass_Alpha) * icm42688_acc_x;
    last_acc_x = acc_x;
    acc_x = acc_x * GRAVITY_G;

    acc_y = last_acc_y * Lowpass_Alpha + (1 - Lowpass_Alpha) * icm42688_acc_y;
    last_acc_y = acc_y;
    acc_y = acc_y * GRAVITY_G;

    acc_z = last_acc_z * Lowpass_Alpha + (1 - Lowpass_Alpha) * icm42688_acc_z;
    last_acc_z = acc_z;
    acc_z = acc_z * GRAVITY_G;
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    static uint16 i;
    if(power_on_flag == 1)
    {
        if(i++ < 600)   //上电3s强制收敛
        {
            Kp = 50;
            LED_ON();
        }
        else
        {
            LED_OFF();
            power_on_flag = 0;
            Kp = 1.0;
        }
    }

    float q0temp, q1temp, q2temp, q3temp;
    float norm, vx, vy, vz, ex, ey, ez;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q1q1 = q1 * q1;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    if (ax * ay * az != 0)
    {
        // 计算加速度模长
        float a_norm = sqrt(ax * ax + ay * ay + az * az);
        float g = GRAVITY_G;
        float sigma = 0.2f; // 经验设定的噪声标准差
        float chi_square = ((a_norm - g) * (a_norm - g)) / (sigma * sigma);

        // 动态调整权重
        float alpha = 0.1f / (0.1f + sqrt(chi_square));
        static float l_alpha;
        alpha = (1-alpha_a) * alpha + alpha_a * l_alpha;
        l_alpha = alpha;
        float dynamic_Kp = Kp * alpha;
        float dynamic_Ki = Ki * 1;

        // 单位化加速度
        norm = sqrt(ax * ax + ay * ay + az * az);
        ax /= norm;
        ay /= norm;
        az /= norm;

        // 计算重力分量
        vx = 2.0f * (q1q3 - q0q2);
        vy = 2.0f * (q0q1 + q2q3);
        vz = q0q0 - q1q1 - q2q2 + q3q3;

        // 计算误差
        ex = (ay * vz - az * vy);
        ey = (az * vx - ax * vz);
        ez = (ax * vy - ay * vx);

        // 误差积分
        I_ex += dynamic_Ki * ex;
        I_ey += dynamic_Ki * ey;
        I_ez += dynamic_Ki * ez;
        exInt = Ki * I_ex;
        eyInt = Ki * I_ey;
        ezInt = Ki * I_ez;

        // PI修正
        gx = gx + dynamic_Kp * ex + exInt;
        gy = gy + dynamic_Kp * ey + eyInt;
        gz = gz + dynamic_Kp * ez + ezInt;

        // 四元数更新
        q0temp = q0;
        q1temp = q1;
        q2temp = q2;
        q3temp = q3;

        q0 = q0 + (-q1temp * gx - q2temp * gy - q3temp * gz) * halfT;
        q1 = q1 + (q0temp * gx + q2temp * gz - q3temp * gy) * halfT;
        q2 = q2 + (q0temp * gy - q1temp * gz + q3temp * gx) * halfT;
        q3 = q3 + (q0temp * gz + q1temp * gy - q2temp * gx) * halfT;

        // 单位化
        norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 /= norm;
        q1 /= norm;
        q2 /= norm;
        q3 /= norm;

        // 欧拉角转换
        pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.259f;
        roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.259f;
        yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.259f;

        if (yaw - yaw_last > 180.0f)
            yaw_round_count --;
        else if (yaw - yaw_last < -180.0f)
            yaw_round_count ++;
        yaw_total_angle = 360.0f * yaw_round_count + yaw;
        yaw_last = yaw;
    }
}

void Gyro_Process(void)
{
    gyro_x_rad=icm42688_gyro_x * PI / 180.0;
    gyro_y_rad=icm42688_gyro_y * PI / 180.0;
    gyro_z_rad=icm42688_gyro_z * PI / 180.0;
    Get_Acc();
    IMUupdate(gyro_x_rad,gyro_y_rad,gyro_z_rad,acc_x,acc_y,acc_z);
}