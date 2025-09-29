#ifndef _GYRO_PROCESS_H_
#define _GYRO_PROCESS_H_

#define GYRO_SAMPLE_TIME 0.01f // 陀螺仪采样时间，单位为秒
#define GRAVITY_G 9.80665f


extern float pitch, roll, yaw;
extern float yaw_total_angle;

void Gyro_Init(void);
void Get_Gyro(void);
void Get_Acc(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void Gyro_Process(void);

#endif // _GYRO_PROCESS_H_