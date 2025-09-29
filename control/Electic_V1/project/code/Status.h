#ifndef _STATUS_H_
#define _STATUS_H_
#include "common.h"
extern  uint8 car_status_flag;
void Car_Start(void);
void Car_Stop(void);

void On_Status0_Change(void);
void On_Status1_Change(void);
void On_Status2_Change(void);
void On_Status3_Change(void);
void On_Status4_Change(void);
void Ready(void);
void Set_curcle_number1(void);
void Set_curcle_number2(void);
void Set_curcle_number3(void);
void Set_curcle_number4(void);
void Set_curcle_number5(void);
void add_curcle_number0(void);
void add_curcle_number1(void);
void add_curcle_number2(void);
void add_curcle_number3(void);
void add_curcle_number4(void);
void add_curcle_number5(void);
#endif
