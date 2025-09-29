#include "headfile.h"

float gyro_zero_x = 0.0f; // ���������ƫ��
float gyro_zero_y = 0.0f; // ���������ƫ��
float gyro_zero_z = 0.0f; // ���������ƫ��
float gyro_x_rad = 0.0f; // ������ X ����ٶȣ����ȣ�
float gyro_y_rad = 0.0f; // ������ Y ����ٶȣ����ȣ�
float gyro_z_rad = 0.0f; // ������ Z ����ٶȣ����ȣ�
float acc_x = 0.0f; // ���ٶȼ� X ����ٶȣ�m/s?��
float acc_y = 0.0f; // ���ٶȼ� Y ����ٶȣ�m/s?��
float acc_z = 0.0f; // ���ٶȼ� Z ����ٶȣ�m/s?��
float PI = 3.1415926f; // Բ��
float Lowpass_Alpha = 0.9; // ��ͨ�˲�ϵ��

float Kp = 5.0f; //��Ҫ���������KP�������KPʵ���ϲ�����Ч
#define Ki 0.0000f
#define halfT 0.022f; // �������ڵ�һ�룬���������Ԫ��΢�ַ���ʱ���������
float pitch = 0, roll = 0, yaw = 0;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��ʼ��̬��Ԫ��������ƪ�����ᵽ�ı任��Ԫ����ʽ����
float exInt = 0, eyInt = 0, ezInt = 0; // ��ǰ�ӼƲ�õ��������ٶ��������ϵķ���
float I_ex, I_ey, I_ez; // ������
float alpha_a = 0.9;
uint8 power_on_flag = 1;
int yaw_round_count = 0;
float yaw_last = 0;
float yaw_total_angle = 0;

void Gyro_Init(void)
{
    uint16 i;
    LED_ON();
    Init_ICM42688();  // ��ʼ��������
    system_delay_ms(2000); // �ȴ��������ȶ�
    for(i = 0; i < 1000; i++)
    {
        Get_Gyro_ICM42688(); // ��ȡ����������
        gyro_zero_x += icm42688_gyro_x; // �ۼ� X ������
        gyro_zero_y += icm42688_gyro_y; // �ۼ� Y ������
        gyro_zero_z += icm42688_gyro_z; // �ۼ� Z ������
    }
    gyro_zero_x /= 1000.0f; // ���� X �����ƫ��
    gyro_zero_y /= 1000.0f; // ���� Y �����ƫ��
    gyro_zero_z /= 1000.0f; // ���� Z �����ƫ��
    flag.gryo_start = 1; // ���������ǿ�ʼ��־
    LED_OFF();
}

void Get_Gyro(void)
{
    Get_Gyro_ICM42688(); // ��ȡ����������
    
    icm42688_gyro_x -= gyro_zero_x; // ��ȥ X �����ƫ��
    icm42688_gyro_y -= gyro_zero_y; // ��ȥ Y �����ƫ��
    icm42688_gyro_z -= gyro_zero_z; // ��ȥ Z �����ƫ��

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

    Get_Acc_ICM42688(); // ��ȡ���ٶȼ�����

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
        if(i++ < 600)   //�ϵ�3sǿ������
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
        // ������ٶ�ģ��
        float a_norm = sqrt(ax * ax + ay * ay + az * az);
        float g = GRAVITY_G;
        float sigma = 0.2f; // �����趨��������׼��
        float chi_square = ((a_norm - g) * (a_norm - g)) / (sigma * sigma);

        // ��̬����Ȩ��
        float alpha = 0.1f / (0.1f + sqrt(chi_square));
        static float l_alpha;
        alpha = (1-alpha_a) * alpha + alpha_a * l_alpha;
        l_alpha = alpha;
        float dynamic_Kp = Kp * alpha;
        float dynamic_Ki = Ki * 1;

        // ��λ�����ٶ�
        norm = sqrt(ax * ax + ay * ay + az * az);
        ax /= norm;
        ay /= norm;
        az /= norm;

        // ������������
        vx = 2.0f * (q1q3 - q0q2);
        vy = 2.0f * (q0q1 + q2q3);
        vz = q0q0 - q1q1 - q2q2 + q3q3;

        // �������
        ex = (ay * vz - az * vy);
        ey = (az * vx - ax * vz);
        ez = (ax * vy - ay * vx);

        // ������
        I_ex += dynamic_Ki * ex;
        I_ey += dynamic_Ki * ey;
        I_ez += dynamic_Ki * ez;
        exInt = Ki * I_ex;
        eyInt = Ki * I_ey;
        ezInt = Ki * I_ez;

        // PI����
        gx = gx + dynamic_Kp * ex + exInt;
        gy = gy + dynamic_Kp * ey + eyInt;
        gz = gz + dynamic_Kp * ez + ezInt;

        // ��Ԫ������
        q0temp = q0;
        q1temp = q1;
        q2temp = q2;
        q3temp = q3;

        q0 = q0 + (-q1temp * gx - q2temp * gy - q3temp * gz) * halfT;
        q1 = q1 + (q0temp * gx + q2temp * gz - q3temp * gy) * halfT;
        q2 = q2 + (q0temp * gy - q1temp * gz + q3temp * gx) * halfT;
        q3 = q3 + (q0temp * gz + q1temp * gy - q2temp * gx) * halfT;

        // ��λ��
        norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 /= norm;
        q1 /= norm;
        q2 /= norm;
        q3 /= norm;

        // ŷ����ת��
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