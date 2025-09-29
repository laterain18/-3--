#ifndef _TRACE_H_
#define _TRACE_H_

#include "common.h"

#define CONDITON1 1
#define CONDITON2 2
#define CONDITON3 3
#define CONDITON4 4

typedef enum
{
    Straight = 0,
    Left_Cartesian,
    Stop,
}Drive_Status_enum;

extern uint8 duanlu_flag;
extern uint16 distance_count;
extern uint16 distance_count1;
extern uint16 distance_count2;
extern uint16 distance_count3;
extern uint16 distance;
extern uint8 distance_flag;
extern uint16 infrared_value[8]; // 红外传感器值
extern uint8 infrared_value_bin[8]; // 红外传感器二进制值
extern uint16 infrared_threshold; // 二值化阈值
extern Drive_Status_enum Drive_Status; // 驱动状态
extern int16 error;
extern uint8 addr0  , addr1, addr2; // 地址线状态
extern uint16 curve_count; // 转向计数
extern uint8 circle_num; // 剩余圈数
extern uint8 active_sensors;    // 检测到黑线的传感器数量
extern uint8 curve_time; // 转向时间
extern uint8 stright_time;
extern uint16 turn_step; // 转弯步骤计数器
extern int16 turn_pwm_increment; // 每步PWM增量
extern uint16 turn_base_speed; // 转弯基础速度
extern uint8 turn_sensitivity; // 转弯灵敏度
extern uint8 cartesian_num;
extern uint8 trace_status_flag;
extern uint8 stop_flag;
extern uint8 add_circle;
// 地址控制宏定义（根据真值表，0表示接通对应路数）
#define Switch_Address_0(state) gpio_set_level(GRAY_SENSOR_ADDR0_PIN, state)  // AD0控制
#define Switch_Address_1(state) gpio_set_level(GRAY_SENSOR_ADDR1_PIN, state)  // AD1控制  
#define Switch_Address_2(state) gpio_set_level(GRAY_SENSOR_ADDR2_PIN, state)  // AD2控制


#define MOTOR_STRAIGHT_SPEED    (3000) 
#define MOTOR_TURN_SPEED        (500) 
// #define MOTOR_TURN_SPEED        (0) 

void Infrared_Init(void);
void Get_Analog_value(void);
void Convert_To_Binary(uint16 threshold);
uint8 Get_Sensor_Digital_State(void);
void get_error(void);
void Trace_Process(void) ;
void Trace_Status_Judge(void);
void Tof_Jduge(void);
void Tof_Process(void);

// 八路灰度传感器硬件定义
#define GRAY_SENSOR_ADC_PIN     ADC0_CH7_A22    // ADC采集引脚
#define GRAY_SENSOR_ADDR0_PIN   A24             // 地址线0
#define GRAY_SENSOR_ADDR1_PIN   A27             // 地址线1  
#define GRAY_SENSOR_ADDR2_PIN   B25             // 地址线2

// 传感器配置参数
#define ADC_SAMPLE_COUNT       5               // 每通道采样次数

#endif