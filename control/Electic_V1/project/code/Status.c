#include "headfile.h"
uint8 car_status_flag = 0;
uint8 test1_flag = 0;
void Car_Start(void)
{
    system_delay_ms(200);
    car_status_flag = 1;
}

void Car_Stop(void)
{
    stop_flag = 0;
	car_status_flag = 0;
    Motor_Set_Pwm(0, 0);
    Motor_Turn_PID.OUT = 0; // 清除 PID 输出
    Motor_Turn_PID.Ek = 0;
    Motor_Turn_PID.Ek_sum = 0; // 清除 PID 累计误差
    Motor_Turn_PID.last_Ek = 0; // 清除 PID 上次误差
    Motor_Turn_PID.P_Out = 0; // 清除 PID 比例
    Motor_Turn_PID.I_Out = 0; // 清除 PID 积分
    Motor_Turn_PID.D_Out = 0; // 清除 PID 微分
}



void On_Status0_Change(void)
 { status = 0; 

 }
void On_Status1_Change(void) 
{ status = 1; 
//	gpio_toggle_level(B22);
		gpio_set_level(A21, 0); 
		gpio_set_level(A23, 0); 


}
void On_Status2_Change(void)
 { status= 2; 
//	 	gpio_toggle_level(B22);

		gpio_set_level(A21, 1); 
		gpio_set_level(A23, 0); 
 }
void On_Status3_Change(void) 
{ status = 3;
//		gpio_toggle_level(B22);

		gpio_set_level(A21, 0); 
		gpio_set_level(A23, 1); 
}
void On_Status4_Change(void) 
{ status= 4; 
//		gpio_toggle_level(B22);

		gpio_set_level(A21, 1); 
		gpio_set_level(A23, 1); 
}
void Ready (void)
{
//		gpio_toggle_level(B22);

		gpio_set_level(A2, 1); 
}
void Set_curcle_number1(void)
{
//	gpio_toggle_level(B22);
		circle_num=1;
}
void Set_curcle_number2(void)
{
//	gpio_toggle_level(B22);
		circle_num=2;
}
void Set_curcle_number3(void)
{
//	gpio_toggle_level(B22);
		circle_num=3;
}
void Set_curcle_number4(void)
{
//	gpio_toggle_level(B22);
		circle_num=4;
}
void Set_curcle_number5(void)
{
//	gpio_toggle_level(B22);
		circle_num=5;
}
void add_curcle_number0(void)
{
//	gpio_toggle_level(B22);
		add_circle=0;
}
void add_curcle_number1(void)
{
//	gpio_toggle_level(B22);
		add_circle=1;
}
void add_curcle_number2(void)
{
//	gpio_toggle_level(B22);
		add_circle=2;
}
void add_curcle_number3(void)
{
//	gpio_toggle_level(B22);
		add_circle=3;
}
void add_curcle_number4(void)
{
//	gpio_toggle_level(B22);
		add_circle=4;
}
void add_curcle_number5(void)
{
//	gpio_toggle_level(B22);
		add_circle=5;
}