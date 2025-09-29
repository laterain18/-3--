#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"

#define Font_Width                      6               // 字体宽度
#define Font_Height                     8              // 字体高度
#define Line_Height                     1              // 行高
#define Window_X                        1               // 窗口位置X
#define Window_Y                        1               // 窗口位置Y
#define Window_W                        128             // 窗口宽度
#define Window_H                        8             // 窗口高度
#define Line_MaxNum                     (8)//菜单支持的最大行数

#define Menu_Key_Scan() Key_Scan()
#define Menu_Clear() oled_clear()
#define Menu_Show_String(X,Y,Str) oled_show_string(X,Y,Str)
#define Menu_Show_float(X,Y,Dat,Num,PointNum) oled_show_float(X,Y,Dat,Num,PointNum)
#define Menu_Show_Char(X,Y,Char) oled_show_char(X,Y,Char) 
#define Menu_Show_Int(X,Y,Num,length) oled_show_int(X,Y,Num,length) 
#define Menu_Cursor_Set '<'
#define Menu_Select_Set '>'

extern uint8 status ;


/*菜单，具体是哪个页面，这个留给用户自己去添加*/
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

/*事件值，可以留给用户自己去定义，这里我定义了一些我需要的事件*/
typedef enum
{
    /*第一个事件为-1用来定义一个防止重复触发的值*/
    UP                          = -1,
    RETRUN_KEY_Previous         = 0,
    DOWN                        = 1,
    NULL_KEY_EVENT              = 2,  //上
    YES                         = 3,  //下
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
    uint16 Default_Background;    //背景颜色      =      RGB565_BLACK
    uint16 Default_Brush;    //画笔颜色           =       RGB565_WHITE
	uint8 Selected_Way;
}Menu_select;

//菜单数据类型结构体
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

//菜单详情结构体
typedef struct Option
{
    char *String;       // 选项名字
    void (*func)(void); // 函数指针

    uint8 Show_Form;  //用于选择显示变量的类型
    OptionValue value;          // 联合体，节省空间
} Option_Class;

/*菜单标志结构体*/
typedef struct MenuProperty
{
    float Power;
    int Key_flag;
    uint8 Current_Option ;    /*当前正在执行的页面*/
    uint8 Previous_Option ;
    int8 KeyEvent;
    uint8 Clear_Flag;
    uint8 Refresh_Flag;
    int16 Choose_i;
    int16 Show_start;
} Menu ;

extern Menu menu; //菜单结构体

/*菜单初始化*/
void Menu_init(void);
/*菜单跳转*/
void Set_Menu(uint8 Page);
/*菜单显示*/
void Menu_Display(void);
/*菜单处理*/
void Menu_RunMenu(Option_Class *Option_List);
/*按键模式切换*/
void Switch_Keymode(void);
/*按键扫描*/
void Menu_Key_Event(void);
/*写一行文字*/
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