#include "headfile.h"

uint16 distance_count;
uint8 duanlu_flag = 0;
uint8 duanlu_flag1 = 0;
uint32 duanlu_count;
uint8 distance_state_temp;
uint8 circle_num = 1; 
uint16 turn_step = 0; // 转弯步骤计数器
uint16 turn_base_speed = 1000; // 转弯基础速度
uint8 turn_sensitivity = 6; // 转弯灵敏度
uint8 trace_status_flag = 1;
uint8 stop_flag = 0;
uint8 add_circle=0;
uint8 CHANNEL_MAP[8][3] = {
    [0] = {0, 0, 0}, // 通道1: SEL2=0, SEL1=0, SEL0=0
    [1] = {0, 0, 1}, // 通道2
    [2] = {0, 1, 0}, // 通道3
    [3] = {0, 1, 1}, // 通道4
    [4] = {1, 0, 0}, // 通道5
    [5] = {1, 0, 1}, // 通道6
    [6] = {1, 1, 0}, // 通道7
    [7] = {1, 1, 1}, // 通道8
};

uint8 addr0 = 0 , addr1 = 0, addr2 = 0; // 地址线状态
uint16 infrared_value[8] = {0} , infrared_threshold = 2000;
uint8 infrared_value_bin[8] = {0} ; // 红外传感器二进制值
Drive_Status_enum Drive_Status = 0; 
int16 error = 0; // 偏差值
uint8 active_sensors = 0;    // 检测到黑线的传感器数量
uint8 curve_time = 10;
uint8 stright_time = 2;
uint16 curve_count = 0; // 转向计数
uint8 cartesian_time = 0; // 转向计时
uint8 cartesian_num = 0;
uint16 curve_count_threshold = 500; // 转向计数阈值

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
    // 初始化ADC引脚（12位精度）
    adc_init(GRAY_SENSOR_ADC_PIN, ADC_12BIT);
    
    // 初始化地址控制引脚为GPIO输出
    gpio_init(GRAY_SENSOR_ADDR0_PIN, GPO, 0, GPO_PUSH_PULL);  // AD0
    gpio_init(GRAY_SENSOR_ADDR1_PIN, GPO, 0, GPO_PUSH_PULL);  // AD1
    gpio_init(GRAY_SENSOR_ADDR2_PIN, GPO, 0, GPO_PUSH_PULL);  // AD2
}

void ADC_SELECT_CHANNEL(uint8 channel)
{
    // 正确的映射：ADDR0对应SEL0，ADDR1对应SEL1，ADDR2对应SEL2
    gpio_set_level(GRAY_SENSOR_ADDR0_PIN, CHANNEL_MAP[channel][0]);  // SEL0
    gpio_set_level(GRAY_SENSOR_ADDR1_PIN, CHANNEL_MAP[channel][1]);  // SEL1
    gpio_set_level(GRAY_SENSOR_ADDR2_PIN, CHANNEL_MAP[channel][2]);  // SEL2
    
    
    // 更新全局变量，用于菜单显示
    addr0 = CHANNEL_MAP[channel][0];
    addr1 = CHANNEL_MAP[channel][1];
    addr2 = CHANNEL_MAP[channel][2];
}
/* 函数功能：采集8个通道的模拟值并进行均值滤波
   参数说明：根据真值表切换8路传感器通道进行ADC采集 */
void Get_Analog_value(void)
{
    uint8  i; 
    // 遍历8个传感器通道（根据真值表）
    for(i = 0; i < 8; i++)
    {
        // 根据真值表设置地址线（AD2 AD1 AD0）
        // 第1路：AD2=0, AD1=0, AD0=0 (i=0: 000)
        // 第2路：AD2=0, AD1=0, AD0=1 (i=1: 001)  
        // 第3路：AD2=0, AD1=1, AD0=0 (i=2: 010)
        // 第4路：AD2=0, AD1=1, AD0=1 (i=3: 011)
        // 第5路：AD2=1, AD1=0, AD0=0 (i=4: 100)
        // 第6路：AD2=1, AD1=0, AD0=1 (i=5: 101)
        // 第7路：AD2=1, AD1=1, AD0=0 (i=6: 110)
        // 第8路：AD2=1, AD1=1, AD0=1 (i=7: 111)
        ADC_SELECT_CHANNEL(i);

        // 使用库提供的均值滤波函数，采样5次求平均
        infrared_value[i] = adc_mean_filter_convert(GRAY_SENSOR_ADC_PIN, ADC_SAMPLE_COUNT);
    }
    sort_infrared_value_bin( infrared_value );
    
    // 采集完所有通道后，进行二值化处理
    Convert_To_Binary(infrared_threshold);
}

/* 函数功能：将模拟值转换为二进制值（简单阈值二值化）
   参数说明：threshold - 二值化阈值 */
void Convert_To_Binary(uint16 threshold)
{
    for(unsigned char i = 0; i < 8; i++)
    {
        if(infrared_value[i] < threshold)
        {
            infrared_value_bin[i] = 0;  // 白色（或反光表面）
        }
        else
        {
            infrared_value_bin[i] = 1;  // 黑色（或吸光表面）
        }
    }
}
/* 函数功能：获取传感器数字状态的8位值
   返回值：8位数字状态（每位对应一个传感器） */
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
    // 负值表示左偏，正值表示右偏
    int8 sensor_weights[8] = {-15, -10, -5, -1, 1, 5, 12, 17};
    
    int16 weighted_sum = 0;      // 加权和
    active_sensors = 0;          // 重置传感器计数

    
    // 计算加权和与活跃传感器数量
    for(uint8 i = 0; i < 8; i++)
    {
        if(infrared_value_bin[i] == 1)  // 检测到黑线
        {
            weighted_sum += sensor_weights[i];
            active_sensors++;
        }
    }
    
    // 根据检测到的传感器数量计算误差
    if(active_sensors > 0)
    {
        // 计算平均加权位置作为误差
        error = weighted_sum / active_sensors;
        
        // 对误差进行放大，提高控制灵敏度
        error *= 10;  // 可根据实际调试需要调整放大倍数
    }
    else
    {
        error = 0;
    }
    
    // 限制误差值范围，防止过大的控制量
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
            //     Drive_Status = Stop; // 停止
            //     error = 0;           // 偏差值清零     
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
    // 处理循迹逻辑
    Get_Analog_value(); // 获取传感器值
    // 根据当前状态执行不同的电机控制逻辑
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
                        Motor_Out(motor_left.target_speed, motor_right.target_speed); // 输出电机 PWM
                        trace_status_flag = 1; // 直行状态下允许循迹
                        break;
                case Stop:
                        Car_Stop();
                        break;
                        
                default:
                        // 默认状态处理
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

