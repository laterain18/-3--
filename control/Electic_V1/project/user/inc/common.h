#ifndef _COMMON_H_
#define _COMMON_H_

#include "zf_common_typedef.h"

#define LED_PIN                 A14  // 蜂鸣器引脚
#define LED_Init()              gpio_init(LED_PIN,GPO,1,GPO_PUSH_PULL)
#define LED_ON()                gpio_set_level(LED_PIN,0)    // 蜂鸣器打开
#define LED_OFF()               gpio_set_level(LED_PIN,1)    // 蜂鸣器关闭

#define MY_ABS(num)             (((num) > 0) ? (num) : -(num))
#define LIMIT_VAL(a,min,max)    ((a)<(min)?(min):((a)>(max)?(max):(a)))

typedef struct 
{
    uint8 ms_5;
    uint8 ms_20;
    uint8 ms_10;
    uint8 ms_100;
    uint8 mission_1;
    uint8 mission_2;
    uint8 gryo_start;
}FLAG_T;

extern FLAG_T flag; // 全局标志位

#endif // _COMMON_H_