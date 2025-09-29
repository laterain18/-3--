#include "headfile.h"

uint16 distance_count;
uint8 duanlu_flag = 0;
uint8 duanlu_flag1 = 0;
uint32 duanlu_count;
uint8 distance_state_temp;
uint8 circle_num = 1; 
uint16 turn_step = 0; // ת�䲽�������
uint16 turn_base_speed = 1000; // ת������ٶ�
uint8 turn_sensitivity = 6; // ת��������
uint8 trace_status_flag = 1;
uint8 stop_flag = 0;
uint8 add_circle=0;
uint8 CHANNEL_MAP[8][3] = {
    [0] = {0, 0, 0}, // ͨ��1: SEL2=0, SEL1=0, SEL0=0
    [1] = {0, 0, 1}, // ͨ��2
    [2] = {0, 1, 0}, // ͨ��3
    [3] = {0, 1, 1}, // ͨ��4
    [4] = {1, 0, 0}, // ͨ��5
    [5] = {1, 0, 1}, // ͨ��6
    [6] = {1, 1, 0}, // ͨ��7
    [7] = {1, 1, 1}, // ͨ��8
};

uint8 addr0 = 0 , addr1 = 0, addr2 = 0; // ��ַ��״̬
uint16 infrared_value[8] = {0} , infrared_threshold = 2000;
uint8 infrared_value_bin[8] = {0} ; // ���⴫����������ֵ
Drive_Status_enum Drive_Status = 0; 
int16 error = 0; // ƫ��ֵ
uint8 active_sensors = 0;    // ��⵽���ߵĴ���������
uint8 curve_time = 10;
uint8 stright_time = 2;
uint16 curve_count = 0; // ת�����
uint8 cartesian_time = 0; // ת���ʱ
uint8 cartesian_num = 0;
uint16 curve_count_threshold = 500; // ת�������ֵ

void sort_infrared_value_bin(uint16 *arr) {

    uint16 temp[8];
    for (int i = 0; i < 8; i++) {
        temp[i] = arr[i];
    }
    
    // arr[0]->0, arr[1]->4, arr[2]->2, arr[3]->6, arr[4]->1, arr[5]->5, arr[6]->3, arr[7]->7
    arr[0] = temp[0]; // 0 -> 0
    arr[1] = temp[4]; // 1 -> 4
    arr[2] = temp[2]; // 2 -> 2
    arr[3] = temp[6]; // 3 -> 6
    arr[4] = temp[1]; // 4 -> 1
    arr[5] = temp[5]; // 5 -> 5
    arr[6] = temp[3]; // 6 -> 3
    arr[7] = temp[7]; // 7 -> 7
}

void Infrared_Init(void)
{
    // ��ʼ��ADC���ţ�12λ���ȣ�
    adc_init(GRAY_SENSOR_ADC_PIN, ADC_12BIT);
    
    // ��ʼ����ַ��������ΪGPIO���
    gpio_init(GRAY_SENSOR_ADDR0_PIN, GPO, 0, GPO_PUSH_PULL);  // AD0
    gpio_init(GRAY_SENSOR_ADDR1_PIN, GPO, 0, GPO_PUSH_PULL);  // AD1
    gpio_init(GRAY_SENSOR_ADDR2_PIN, GPO, 0, GPO_PUSH_PULL);  // AD2
}

void ADC_SELECT_CHANNEL(uint8 channel)
{
    // ��ȷ��ӳ�䣺ADDR0��ӦSEL0��ADDR1��ӦSEL1��ADDR2��ӦSEL2
    gpio_set_level(GRAY_SENSOR_ADDR0_PIN, CHANNEL_MAP[channel][0]);  // SEL0
    gpio_set_level(GRAY_SENSOR_ADDR1_PIN, CHANNEL_MAP[channel][1]);  // SEL1
    gpio_set_level(GRAY_SENSOR_ADDR2_PIN, CHANNEL_MAP[channel][2]);  // SEL2
    
    
    // ����ȫ�ֱ��������ڲ˵���ʾ
    addr0 = CHANNEL_MAP[channel][0];
    addr1 = CHANNEL_MAP[channel][1];
    addr2 = CHANNEL_MAP[channel][2];
}
/* �������ܣ��ɼ�8��ͨ����ģ��ֵ�����о�ֵ�˲�
   ����˵����������ֵ���л�8·������ͨ������ADC�ɼ� */
void Get_Analog_value(void)
{
    uint8  i; 
    // ����8��������ͨ����������ֵ��
    for(i = 0; i < 8; i++)
    {
        // ������ֵ�����õ�ַ�ߣ�AD2 AD1 AD0��
        // ��1·��AD2=0, AD1=0, AD0=0 (i=0: 000)
        // ��2·��AD2=0, AD1=0, AD0=1 (i=1: 001)  
        // ��3·��AD2=0, AD1=1, AD0=0 (i=2: 010)
        // ��4·��AD2=0, AD1=1, AD0=1 (i=3: 011)
        // ��5·��AD2=1, AD1=0, AD0=0 (i=4: 100)
        // ��6·��AD2=1, AD1=0, AD0=1 (i=5: 101)
        // ��7·��AD2=1, AD1=1, AD0=0 (i=6: 110)
        // ��8·��AD2=1, AD1=1, AD0=1 (i=7: 111)
        ADC_SELECT_CHANNEL(i);

        // ʹ�ÿ��ṩ�ľ�ֵ�˲�����������5����ƽ��
        infrared_value[i] = adc_mean_filter_convert(GRAY_SENSOR_ADC_PIN, ADC_SAMPLE_COUNT);
    }
    sort_infrared_value_bin( infrared_value );
    
    // �ɼ�������ͨ���󣬽��ж�ֵ������
    Convert_To_Binary(infrared_threshold);
}

/* �������ܣ���ģ��ֵת��Ϊ������ֵ������ֵ��ֵ����
   ����˵����threshold - ��ֵ����ֵ */
void Convert_To_Binary(uint16 threshold)
{
    for(unsigned char i = 0; i < 8; i++)
    {
        if(infrared_value[i] < threshold)
        {
            infrared_value_bin[i] = 0;  // ��ɫ���򷴹���棩
        }
        else
        {
            infrared_value_bin[i] = 1;  // ��ɫ����������棩
        }
    }
}
/* �������ܣ���ȡ����������״̬��8λֵ
   ����ֵ��8λ����״̬��ÿλ��Ӧһ���������� */
uint8 Get_Sensor_Digital_State(void)
{
    uint8 digital_state = 0;
    
    for(unsigned char i = 0; i < 8; i++)
    {
        if(infrared_value_bin[i])
        {
            digital_state |= (1 << i);
        }
    }
    
    return digital_state;
}

void get_error(void)
{
    // ��ֵ��ʾ��ƫ����ֵ��ʾ��ƫ
    int8 sensor_weights[8] = {-15, -10, -5, -1, 1, 5, 12, 17};
    
    int16 weighted_sum = 0;      // ��Ȩ��
    active_sensors = 0;          // ���ô���������

    
    // �����Ȩ�����Ծ����������
    for(uint8 i = 0; i < 8; i++)
    {
        if(infrared_value_bin[i] == 1)  // ��⵽����
        {
            weighted_sum += sensor_weights[i];
            active_sensors++;
        }
    }
    
    // ���ݼ�⵽�Ĵ����������������
    if(active_sensors > 0)
    {
        // ����ƽ����Ȩλ����Ϊ���
        error = weighted_sum / active_sensors;
        
        // �������зŴ���߿���������
        error *= 10;  // �ɸ���ʵ�ʵ�����Ҫ�����Ŵ���
    }
    else
    {
        error = 0;
    }
    
    // �������ֵ��Χ����ֹ����Ŀ�����
    if(error > 200) error = 200;
    if(error < -200) error = -200;
}

void Trace_Status_Judge(void)
{
    if(trace_status_flag == 1)
    {
            // if (circle_num > 0)
            // {
//        sort_infrared_value_bin( infrared_value_bin );
                uint8 left_sensors = infrared_value_bin[0] + infrared_value_bin[1] + infrared_value_bin[2];
                uint8 right_sensors = infrared_value_bin[5] + infrared_value_bin[6] + infrared_value_bin[7];
                uint8 middle_sensors = infrared_value_bin[3] + infrared_value_bin[4];
                
                if((left_sensors < 2) && (right_sensors >= 2) && (middle_sensors <= 1))
                {
                    curve_count++;
              //      Drive_Status = Left_Cartesian;
                    if(stop_flag == 1)
                    {
                        Drive_Status = Stop;       
                    }
                }
                else if((middle_sensors >= 1) || (left_sensors > 1 && right_sensors > 1))
                {
                    Drive_Status = Straight;
                    if(curve_count > 0)
                    {
                        cartesian_num++;
                        curve_count = 0;
                    }
										if(stop_flag == 1)
                    {
                        Drive_Status = Stop;       
                    }
                }
								else if(left_sensors + right_sensors + middle_sensors <= 4)
								{
										Drive_Status = Stop; 
								}
                
								
								
//								if(duanlu_flag1 == 0)
//								{
										if(left_sensors + right_sensors + middle_sensors == 8)
										{
												duanlu_count++;
												if(duanlu_count >= 1000)
												{
														duanlu_flag = 1;
//														if(duanlu_flag == 1)
//														{
//															duanlu_flag1 = 1;
//														}
												}
										}
										else
										{
												duanlu_count = 0;
												duanlu_flag = 0;
				//								duanlu_flag1 = 0;
										//		Drive_Status = Straight;
										}
										
										
//							}
//								else
//								{
//									duanlu_flag1 = 2;
//									Drive_Status = Straight;
//								}
            // }
            // else
            // {
            //     Drive_Status = Stop; // ֹͣ
            //     error = 0;           // ƫ��ֵ����     
            // }
    }
//    else
//    {
//        Drive_Status = Left_Cartesian;
//    }
    if(cartesian_num == 4 * circle_num+add_circle)
    {
        stop_flag = 1;
    } 
}

void Trace_Process(void) 
{
    // ����ѭ���߼�
    Get_Analog_value(); // ��ȡ������ֵ
    // ���ݵ�ǰ״ִ̬�в�ͬ�ĵ�������߼�
    if(car_status_flag == 1)
    {
        Trace_Status_Judge();
        if(duanlu_flag == 1)
            Motor_Set_Pwm(2000,3200);
        else
        {
            switch (Drive_Status)
            {
                case Straight:
                        get_error();
                        motor_left.target_speed = motor_right.target_speed = MOTOR_STRAIGHT_SPEED;
                        Motor_Out(motor_left.target_speed, motor_right.target_speed); // ������ PWM
                        trace_status_flag = 1; // ֱ��״̬������ѭ��
                        break;
                case Stop:
                        Car_Stop();
                        break;
                        
                default:
                        // Ĭ��״̬����
                        break;
            }
        }
	}
}
void Tof_Jduge(void)
{
    if(distance_flag == 1)
    {
        distance_count++;
        if(distance_count <= (distance_count1/2))
            distance_state_temp = 1;
        else if((distance_count1/2) < distance_count && distance_count <= distance_count1)
            distance_state_temp = 2;
        else if(distance_count1 < distance_count && distance_count <= (distance_count1 + distance_count2))
            distance_state_temp = 3;
        else if((distance_count1 + distance_count2) < distance_count && distance_count <= (distance_count1 + distance_count2 + distance_count3/2))
            distance_state_temp = 4;
        else 
            distance_state_temp = 5;
    }
    else
        distance_count = 0;
}

void Tof_Process(void)
{
    if(car_status_flag == 1)
    {
        switch(distance_state_temp)
        {
            case 1:
                Motor_Set_Pwm(4000,2000);
            break;
            case 2:
                Motor_Set_Pwm(2000,4000);
            break;
            case 3:
                Motor_Set_Pwm(3000,3000);
            break;
            case 4:
                Motor_Set_Pwm(2000,3200);
            break;
            case 5:
                distance_flag = 0;
            break;
            default:break;
        }
    }
}

