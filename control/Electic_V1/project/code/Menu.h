#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"

#define Font_Width                      6               // ������
#define Font_Height                     8              // ����߶�
#define Line_Height                     1              // �и�
#define Window_X                        1               // ����λ��X
#define Window_Y                        1               // ����λ��Y
#define Window_W                        128             // ���ڿ��
#define Window_H                        8             // ���ڸ߶�
#define Line_MaxNum                     (8)//�˵�֧�ֵ��������

#define Menu_Key_Scan() Key_Scan()
#define Menu_Clear() oled_clear()
#define Menu_Show_String(X,Y,Str) oled_show_string(X,Y,Str)
#define Menu_Show_float(X,Y,Dat,Num,PointNum) oled_show_float(X,Y,Dat,Num,PointNum)
#define Menu_Show_Char(X,Y,Char) oled_show_char(X,Y,Char) 
#define Menu_Show_Int(X,Y,Num,length) oled_show_int(X,Y,Num,length) 
#define Menu_Cursor_Set '<'
#define Menu_Select_Set '>'

extern uint8 status ;


/*�˵����������ĸ�ҳ�棬��������û��Լ�ȥ���*/
typedef enum
{
    MAIN_PAGE            = 0,
    SETTING_PAGE,
    Gyroscope_PAGE,
    GyroData_PAGE,
    Motor_Page,
    Vaule_Page,
    AdcData2_Page,
    PID_Page,
    Start_Page,
    Task1_Page,
		Status_Page,
		NumSet_Page,
		Addcirclue_Page
} MENU_OPTION;

/*�¼�ֵ�����������û��Լ�ȥ���壬�����Ҷ�����һЩ����Ҫ���¼�*/
typedef enum
{
    /*��һ���¼�Ϊ-1��������һ����ֹ�ظ�������ֵ*/
    UP                          = -1,
    RETRUN_KEY_Previous         = 0,
    DOWN                        = 1,
    NULL_KEY_EVENT              = 2,  //��
    YES                         = 3,  //��
    START                       = 4,
} EVENT_CODE;

typedef enum
{
    SHOW_CHAR                   = 1,
    SHOW_FLOAT                  = 2,
    SHOW_INT16                    = 3,
    SHOW_NOTHIONG               = 4,
    SHOW_UINT8                  = 5,
    SHOW_UINT16                 = 6,
    SHOW_INT32                  = 7,
    SHOW_UINT32                 = 8,
} SHOW_FORM_ENUM;

typedef enum
{
    SELECT_MODE                 = 0,
    ARGUMENT_MODE               = 1,
    REVERSE_MODE                = 2,
} Key_FUNTIONMODE;

typedef struct Menu_Select_Of
{
    uint16 Default_Background;    //������ɫ      =      RGB565_BLACK
    uint16 Default_Brush;    //������ɫ           =       RGB565_WHITE
	uint8 Selected_Way;
}Menu_select;

//�˵��������ͽṹ��
typedef union
{
    char *onechar;
    float *onefloat;
    int16 *oneint16;
    uint8  *oneuint8;
    uint16 *oneuint16;
    int32  *oneint32;
    uint32 *oneuint32;
} OptionValue;

//�˵�����ṹ��
typedef struct Option
{
    char *String;       // ѡ������
    void (*func)(void); // ����ָ��

    uint8 Show_Form;  //����ѡ����ʾ����������
    OptionValue value;          // �����壬��ʡ�ռ�
} Option_Class;

/*�˵���־�ṹ��*/
typedef struct MenuProperty
{
    float Power;
    int Key_flag;
    uint8 Current_Option ;    /*��ǰ����ִ�е�ҳ��*/
    uint8 Previous_Option ;
    int8 KeyEvent;
    uint8 Clear_Flag;
    uint8 Refresh_Flag;
    int16 Choose_i;
    int16 Show_start;
} Menu ;

extern Menu menu; //�˵��ṹ��

/*�˵���ʼ��*/
void Menu_init(void);
/*�˵���ת*/
void Set_Menu(uint8 Page);
/*�˵���ʾ*/
void Menu_Display(void);
/*�˵�����*/
void Menu_RunMenu(Option_Class *Option_List);
/*����ģʽ�л�*/
void Switch_Keymode(void);
/*����ɨ��*/
void Menu_Key_Event(void);
/*дһ������*/
void Menu_PrintfOptionStr(uint8 X, uint8 Y, char *format, ...);

void MainPage_Run(void);
void SettingPage_Run(void);
void GyroscopePage_Run(void);
void GyroDataPage_Run(void);
void MotorPage_Run(void);
void AdcData1Page_Run(void);
void AdcData2Page_Run(void);
void PIDPage_Run(void);
void Task1Page_Run(void);
void StartPage_Run(void);
void StatusPage_Run(void);
void NumSetPage_Run(void);
void AddcirclePage_Run(void);
#endif