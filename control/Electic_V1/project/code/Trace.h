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
extern uint16 infrared_value[8]; // ���⴫����ֵ
extern uint8 infrared_value_bin[8]; // ���⴫����������ֵ
extern uint16 infrared_threshold; // ��ֵ����ֵ
extern Drive_Status_enum Drive_Status; // ����״̬
extern int16 error;
extern uint8 addr0  , addr1, addr2; // ��ַ��״̬
extern uint16 curve_count; // ת�����
extern uint8 circle_num; // ʣ��Ȧ��
extern uint8 active_sensors;    // ��⵽���ߵĴ���������
extern uint8 curve_time; // ת��ʱ��
extern uint8 stright_time;
extern uint16 turn_step; // ת�䲽�������
extern int16 turn_pwm_increment; // ÿ��PWM����
extern uint16 turn_base_speed; // ת������ٶ�
extern uint8 turn_sensitivity; // ת��������
extern uint8 cartesian_num;
extern uint8 trace_status_flag;
extern uint8 stop_flag;
extern uint8 add_circle;
// ��ַ���ƺ궨�壨������ֵ��0��ʾ��ͨ��Ӧ·����
#define Switch_Address_0(state) gpio_set_level(GRAY_SENSOR_ADDR0_PIN, state)  // AD0����
#define Switch_Address_1(state) gpio_set_level(GRAY_SENSOR_ADDR1_PIN, state)  // AD1����  
#define Switch_Address_2(state) gpio_set_level(GRAY_SENSOR_ADDR2_PIN, state)  // AD2����


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

// ��·�Ҷȴ�����Ӳ������
#define GRAY_SENSOR_ADC_PIN     ADC0_CH7_A22    // ADC�ɼ�����
#define GRAY_SENSOR_ADDR0_PIN   A24             // ��ַ��0
#define GRAY_SENSOR_ADDR1_PIN   A27             // ��ַ��1  
#define GRAY_SENSOR_ADDR2_PIN   B25             // ��ַ��2

// ���������ò���
#define ADC_SAMPLE_COUNT       5               // ÿͨ����������

#endif