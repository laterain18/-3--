/*********************************************************************************************************************
* MSPM0G3507 Opensource Library 即（MSPM0G3507 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3507 开源库的一部分
* 
* MSPM0G3507 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          mian
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
********************************************************************************************************************/

#include "headfile.h"
FLAG_T flag =
{
    .ms_5 = 0,
    .ms_10 = 0,
    .ms_20 = 0,
    .ms_100 = 0,
    .mission_1 = 0,
    .mission_2 = 0,
    .gryo_start = 1
};
uint8 Flag_Camera = 0;
uint16 distance = 500;
uint16 distance__count;
uint8 distance_flag = 0;
uint16 distance_count1 = 120;
uint16 distance_count2 = 10;
uint16 distance_count3 = 310;
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************

int main (void)
{
    static uint8 i = 0; // 循环变量
    clock_init(SYSTEM_CLOCK_80M);   // 时钟配置及系统初始化<务必保留>
    debug_init();					// 调试串口信息初始化
	// 此处编写用户代码 例如外设初始化代码等
    gpio_init(A1 , GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(A21, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(A23, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(A2 , GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(B22, GPO, GPIO_LOW, GPO_PUSH_PULL);
    dl1b_init();
    LED_Init();
    Infrared_Init(); // 红外传感器初始化
    // Gyro_Init(); // 陀螺仪初始化
    PID_Param_Init(); // PID 参数初始化
    Key_Init(); // 按键初始化
    oled_init(); // OLED 初始化
    Menu_init(); // 菜单初始化
    Motor_Init(); // 电机初始化
    system_delay_ms(100); // 等待 OLED 初始化完成
    pit_ms_init(PIT_TIM_A1,1,NULL,NULL); // PIT 定时器初始化 每 10 毫秒触发一次中断

    // 此处编写用户代码 例如外设初始化代码等
    while(true)
    {
        if(distance_flag == 0)
            Trace_Process();
        else
            Tof_Process();
        
        if(flag.ms_100)
        {
            flag.ms_100 = 0; // 清除标志位
            Menu_Display();
        }			
        if(flag.ms_5)
        {
            flag.ms_5 = 0; // 清除标志位
            
            // tof获取
            dl1b_get_distance();
            if(dl1b_distance_mm <= distance && gpio_get_level(A1) == 1)
            {
                distance__count++;
                if(distance__count >= 20)
                    distance_flag = 1;
            }
            else
            {
                distance__count = 0;
            }
            
            Menu_Key_Event(); // 处理菜单按键事件
        }
        if(flag.ms_10)
        {
            flag.ms_10 = 0; // 清除标志位
            Tof_Jduge();
        }
        
}
}
