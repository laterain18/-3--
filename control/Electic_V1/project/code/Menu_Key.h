#ifndef _MENU_KEY_H_
#define _MENU_KEY_H_

#include "common.h"

#define KEY_UP_PIN          A28
#define KEY_DOWN_PIN        A29
#define KEY_LEFT_PIN        A30
#define KEY_RIGHT_PIN       A18
#define KEY_SELECT_PIN      A31
#define KEY_NUM             5
#define KEY_SHOCK_PERIOD    4 // �����������

typedef struct 
{
	uint16 KeyCount;		//��ʾ������������
	uint8 KeyState;
	uint8 g_KeyState;		//��ʾ������־��1�����£�0�����ɿ�	
}Key_T;

extern Key_T Key[5];

void Key_Scan(void);
void Key_Init(void);
uint8 Key_select(uint8 i);
uint8 Key_returnState(uint8 i);


#endif