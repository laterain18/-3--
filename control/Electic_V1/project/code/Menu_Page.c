#include "headfile.h"

#define FUNC_NULL ((void (*)(void))0)
uint8 status = 0;
void Option_Class_Init(Option_Class *self, char *str, void (*func)(void), uint8 show_form, void *value_ptr)
{
    self->String = str;
    self->func = func;
    self->Show_Form = show_form;
    switch(show_form)
    {
        case SHOW_FLOAT:
            self->value.onefloat = (float *)value_ptr;
            break;
        case SHOW_INT16:
            self->value.oneint16 = (int16 *)value_ptr;
            break;
        case SHOW_CHAR:
            self->value.onechar = (char *)value_ptr;
            break;
        case SHOW_UINT8:
            self->value.oneuint8 = (uint8 *)value_ptr;
            break;
        case SHOW_UINT16:
            self->value.oneuint16 = (uint16 *)value_ptr;
            break;
        case SHOW_INT32:
            self->value.oneint32 = (int32 *)value_ptr;
            break;
        case SHOW_UINT32:
            self->value.oneuint32 = (uint32 *)value_ptr;
            break;
        default:
            self->value.oneuint32 = 0;
            break;
    }
}

void SettingPage_Set(void)
{
	Set_Menu(SETTING_PAGE);
}

void GyrscopePage_Set(void)
{
	Set_Menu(Gyroscope_PAGE);
}

void GyrsDataPage_Set(void)
{
	Set_Menu(GyroData_PAGE);
}

void MotorPage_Set(void)
{
	Set_Menu(Motor_Page);
}

void AdcData1Page_Set(void)
{
	Set_Menu(Vaule_Page);
}

void AdcData2Page_Set(void)
{
	Set_Menu(AdcData2_Page);
}

void PIDPage_Set(void)
{
	Set_Menu(PID_Page);
}

void StartPage_Set(void)
{
	Set_Menu(Start_Page);
}

void Task1Page_Set(void)
{
	Set_Menu(Task1_Page);
}
void StatusPage_Set(void)
{
    Set_Menu(Status_Page);        /* 与已有页面枚举 Status_Page 对应 */
}
void NumSetPage_Set(void)
{
    Set_Menu(NumSet_Page);   // 必须在 menu.h 里加枚举值
}
void AddcircluePage_Set(void)
{
    Set_Menu(Addcirclue_Page);   // 必须在 menu.h 里加枚举值
}
void MainPage_Run(void)
{
    Option_Class Menu_MainOptionList[10];
    if(menu.Refresh_Flag)
    {
        menu.Refresh_Flag = 1;
        Option_Class_Init(&Menu_MainOptionList[0], "Start", StartPage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[1], "Setting", SettingPage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[2], "Gyrscope", GyrscopePage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[3], "Motor", MotorPage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[4], "AdcData1", AdcData1Page_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[5], "AdcData2", AdcData2Page_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[6], "PID", PIDPage_Set, SHOW_NOTHIONG, NULL);
				Option_Class_Init(&Menu_MainOptionList[7], "Status", StatusPage_Set, SHOW_NOTHIONG, NULL);
				Option_Class_Init(&Menu_MainOptionList[8], "Addcor", AddcircluePage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_MainOptionList[9], "..",FUNC_NULL , SHOW_NOTHIONG, NULL);
    }

    Menu_RunMenu(Menu_MainOptionList);

}

void SettingPage_Run(void)
{
    Option_Class Menu_SettingList[2];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_SettingList[0], "Trace_Flag:%d", FUNC_NULL, SHOW_UINT8,  &trace_status_flag);
        // Option_Class_Init(&Menu_SettingList[1], "addr1:%d", FUNC_NULL, SHOW_UINT8,  &addr1);
        // Option_Class_Init(&Menu_SettingList[2], "addr2:%d", FUNC_NULL, SHOW_UINT8,  &addr2);
        
        
        Option_Class_Init(&Menu_SettingList[1], "..", FUNC_NULL, SHOW_NOTHIONG, NULL);
    }
    Menu_RunMenu(Menu_SettingList);
}

void GyroscopePage_Run(void)
{
    Option_Class Menu_GyrocopeList[3];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_GyrocopeList[0], "Gyro_Init", Gyro_Init, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_GyrocopeList[1], "Data", GyrsDataPage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_GyrocopeList[2], "..", FUNC_NULL, SHOW_NOTHIONG, NULL);
    }

    Menu_RunMenu(Menu_GyrocopeList);
}

void GyroDataPage_Run(void)
{
    Option_Class Menu_GyroDataList[10];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_GyroDataList[0], "Gyrox:%.3f", FUNC_NULL, SHOW_FLOAT, &icm42688_gyro_x);
        Option_Class_Init(&Menu_GyroDataList[1], "Gyroy:%.3f", FUNC_NULL, SHOW_FLOAT, &icm42688_gyro_y);
        Option_Class_Init(&Menu_GyroDataList[2], "Gyroz:%.3f", FUNC_NULL, SHOW_FLOAT, &icm42688_gyro_z);
        Option_Class_Init(&Menu_GyroDataList[3], "Accx:%.3f", FUNC_NULL, SHOW_FLOAT, &icm42688_acc_x);
        Option_Class_Init(&Menu_GyroDataList[4], "Accy:%.3f", FUNC_NULL, SHOW_FLOAT, &icm42688_acc_y);
        Option_Class_Init(&Menu_GyroDataList[5], "Accz:%.3f", FUNC_NULL, SHOW_FLOAT, &icm42688_acc_z);
        Option_Class_Init(&Menu_GyroDataList[6], "pitch:%.3f", FUNC_NULL, SHOW_FLOAT, &pitch);
        Option_Class_Init(&Menu_GyroDataList[7], "yaw:%.3f", FUNC_NULL, SHOW_FLOAT, &yaw);
        Option_Class_Init(&Menu_GyroDataList[8], "roll:%.3f", FUNC_NULL, SHOW_FLOAT, &roll);
        Option_Class_Init(&Menu_GyroDataList[9], "..", FUNC_NULL, SHOW_NOTHIONG,NULL );
    }

    Menu_RunMenu(Menu_GyroDataList);
}

void MotorPage_Run(void)
{
    Option_Class Menu_MotorList[8];
    if(menu.Refresh_Flag)
    {
        // Option_Class_Init(&Menu_MotorList[0], "ExpectSpeed:%.3f", Switch_Keymode, SHOW_INT16,  &Expect_Speed);
        Option_Class_Init(&Menu_MotorList[0], "Left_Target:%d", Switch_Keymode, SHOW_INT16,  &motor_left.target_speed);
        Option_Class_Init(&Menu_MotorList[1], "Right_Target:%d", Switch_Keymode, SHOW_INT16,  &motor_right.target_speed);
        Option_Class_Init(&Menu_MotorList[1], "Turn_Sens:%d", Switch_Keymode, SHOW_UINT8,  &turn_sensitivity);
        Option_Class_Init(&Menu_MotorList[2], "Left_Out:%d", FUNC_NULL, SHOW_INT16,  &motor_left.output_duty);
        Option_Class_Init(&Menu_MotorList[3], "Rightt_Out:%d", FUNC_NULL, SHOW_INT16,  &motor_right.output_duty);
        Option_Class_Init(&Menu_MotorList[4], "Drive_Status:%d", FUNC_NULL, SHOW_UINT8,  &Drive_Status);
        Option_Class_Init(&Menu_MotorList[5], "Curve_Count:%d", FUNC_NULL, SHOW_UINT16,  &curve_count);
        Option_Class_Init(&Menu_MotorList[6], "Cartesian_Num:%d", FUNC_NULL, SHOW_UINT8,  &cartesian_num);
        
        Option_Class_Init(&Menu_MotorList[7], "..", FUNC_NULL, SHOW_NOTHIONG,NULL );
    }

    Menu_RunMenu(Menu_MotorList);
}
void AdcData1Page_Run(void)
{
    Option_Class Menu_AdcData1List[9];
    if(menu.Refresh_Flag)
    {
        // Option_Class_Init(&Menu_MotorList[0], "ExpectSpeed:%.3f", Switch_Keymode, SHOW_INT16,  &Expect_Speed);
        Option_Class_Init(&Menu_AdcData1List[0], "adc_0:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[0]);
        Option_Class_Init(&Menu_AdcData1List[1], "adc_1:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[1]);
        Option_Class_Init(&Menu_AdcData1List[2], "adc_2:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[2]);
        Option_Class_Init(&Menu_AdcData1List[3], "adc_3:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[3]);
        Option_Class_Init(&Menu_AdcData1List[4], "adc_4:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[4]);
        Option_Class_Init(&Menu_AdcData1List[5], "adc_5:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[5]);
        Option_Class_Init(&Menu_AdcData1List[6], "adc_6:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[6]);
        Option_Class_Init(&Menu_AdcData1List[7], "adc_7:%d", FUNC_NULL, SHOW_UINT16,  &infrared_value[7]);
        Option_Class_Init(&Menu_AdcData1List[8], "..", FUNC_NULL, SHOW_NOTHIONG,NULL );
    }

    Menu_RunMenu(Menu_AdcData1List);
}

void AdcData2Page_Run(void)
{
    Option_Class Menu_AdcData2List[13];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_AdcData2List[0], "bin_0:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[0]);
        Option_Class_Init(&Menu_AdcData2List[1], "bin_1:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[1]);
        Option_Class_Init(&Menu_AdcData2List[2], "bin_2:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[2]);
        Option_Class_Init(&Menu_AdcData2List[3], "bin_3:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[3]);
        Option_Class_Init(&Menu_AdcData2List[4], "bin_4:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[4]);
        Option_Class_Init(&Menu_AdcData2List[5], "bin_5:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[5]);
        Option_Class_Init(&Menu_AdcData2List[6], "bin_6:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[6]);
        Option_Class_Init(&Menu_AdcData2List[7], "bin_7:%d", FUNC_NULL, SHOW_UINT8,  &infrared_value_bin[7]);
        Option_Class_Init(&Menu_AdcData2List[8], "threshold:%d", Switch_Keymode, SHOW_UINT16,  &infrared_threshold);
        Option_Class_Init(&Menu_AdcData2List[9], "error:%d", FUNC_NULL, SHOW_INT16,  &error);
        Option_Class_Init(&Menu_AdcData2List[10], "DriveStatus:%d", FUNC_NULL, SHOW_UINT8,  &Drive_Status);
        Option_Class_Init(&Menu_AdcData2List[11], "..", FUNC_NULL, SHOW_NOTHIONG,NULL );
    }

    Menu_RunMenu(Menu_AdcData2List);
}

void PIDPage_Run(void)
{
    Option_Class Menu_PIDList[7];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_PIDList[0], "Turn_Kp:%.3f", Switch_Keymode, SHOW_FLOAT,  &Motor_Turn_PID.Kp);
        Option_Class_Init(&Menu_PIDList[1], "Turn_Ki:%.3f", Switch_Keymode, SHOW_FLOAT,  &Motor_Turn_PID.Ki);
        Option_Class_Init(&Menu_PIDList[2], "Turn_Kd:%.3f", Switch_Keymode, SHOW_FLOAT,  &Motor_Turn_PID.Kd);
    
        Option_Class_Init(&Menu_PIDList[3], "..", FUNC_NULL, SHOW_NOTHIONG,NULL );
    }

    Menu_RunMenu(Menu_PIDList);
}

void StartPage_Run(void)
{
    Option_Class Menu_StartList[7];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_StartList[0], "Car_Start", Car_Start, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_StartList[1], "Car_Stop", Car_Stop, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_StartList[2], "Task1", Task1Page_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_StartList[3], "Status:%d", FUNC_NULL, SHOW_UINT8, &car_status_flag);
        Option_Class_Init(&Menu_StartList[4], "DriveMode:%d", FUNC_NULL, SHOW_UINT8, &Drive_Status);
        Option_Class_Init(&Menu_StartList[5], "ActiveSen:%d", FUNC_NULL, SHOW_UINT8, &active_sensors);
        Option_Class_Init(&Menu_StartList[6], "..", FUNC_NULL, SHOW_NOTHIONG,NULL );
    }

    Menu_RunMenu(Menu_StartList);
}

void Task1Page_Run(void)
{
    Option_Class Menu_Task1List[4];
    if(menu.Refresh_Flag)
    {
        Option_Class_Init(&Menu_Task1List[0], "Car_Start", Car_Start, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_Task1List[1], "CartesianNum:%d", FUNC_NULL, SHOW_UINT8, &cartesian_num);
        Option_Class_Init(&Menu_Task1List[2], "CircleNum:%d", Switch_Keymode, SHOW_UINT8, &circle_num);
        Option_Class_Init(&Menu_Task1List[3], "..", FUNC_NULL, SHOW_NOTHIONG, NULL);
    }

    Menu_RunMenu(Menu_Task1List);
}
void StatusPage_Run(void)
{
    /* 7 行：6 个状态 + 1 个返回 */
    Option_Class Menu_StatusList[7];
//		Menu_Clear();
//        Menu_Show_String(0, 0, "Status OK!");  // 强制显示
    if (menu.Refresh_Flag)
    {
//        menu.Refresh_Flag = 0;

        /* 第 0~4 行：显示状态 + 绑定翻转函数 */
        Option_Class_Init(&Menu_StatusList[0], "Status0:", NumSetPage_Set, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_StatusList[1], "Status1:", On_Status1_Change, SHOW_NOTHIONG,NULL);
        Option_Class_Init(&Menu_StatusList[2], "Status2:", On_Status2_Change, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_StatusList[3], "Status3:", On_Status3_Change, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&Menu_StatusList[4], "Status4:", On_Status4_Change, SHOW_NOTHIONG, NULL);
				Option_Class_Init(&Menu_StatusList[5], "Ready:%d", Ready, SHOW_UINT8, &status);
//				if (status == 0)
//            Option_Class_Init(&Menu_StatusList[6], "NumSet", NumSetPage_Set, SHOW_NOTHIONG, NULL);
////        else
////            Option_Class_Init(&Menu_StatusList[6], "..", FUNC_NULL, SHOW_NOTHIONG, NULL);
				Option_Class_Init(&Menu_StatusList[6], "..", FUNC_NULL, SHOW_NOTHIONG, NULL);
    }
    

    Menu_RunMenu(Menu_StatusList);
}
void NumSetPage_Run(void)
{
    Option_Class list[6];
    if (menu.Refresh_Flag)
    {
//        menu.Refresh_Flag = 0;

        Option_Class_Init(&list[0], "Set Num=1", Set_curcle_number1,SHOW_NOTHIONG, NULL);
        Option_Class_Init(&list[1], "Set Num=2", Set_curcle_number2, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&list[2], "Set Num=3", Set_curcle_number3, SHOW_NOTHIONG, NULL);
			  Option_Class_Init(&list[3], "Set Num=4", Set_curcle_number4, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&list[4], "Set Num=5:%d", Set_curcle_number5, SHOW_UINT8, &circle_num);
        Option_Class_Init(&list[5], "..", NULL, SHOW_NOTHIONG, NULL);
    }

    Menu_RunMenu(list);
}
void AddcirclePage_Run(void)
{
    Option_Class list[7];
    if (menu.Refresh_Flag)
    {
//        menu.Refresh_Flag = 0;

        Option_Class_Init(&list[0], "Add cor=0", add_curcle_number0,SHOW_NOTHIONG, NULL);
        Option_Class_Init(&list[1], "Add cor=1", add_curcle_number1, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&list[2], "Add cor=2", add_curcle_number2, SHOW_NOTHIONG, NULL);
			  Option_Class_Init(&list[3], "Add cor=3", add_curcle_number3, SHOW_NOTHIONG, NULL);
				Option_Class_Init(&list[4], "Add cor=4", add_curcle_number4, SHOW_NOTHIONG, NULL);
        Option_Class_Init(&list[5], "Add cor=5:%d", add_curcle_number5, SHOW_UINT8, &add_circle);
        Option_Class_Init(&list[6], "..", NULL, SHOW_NOTHIONG, NULL);
    }

    Menu_RunMenu(list);
}