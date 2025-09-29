#include "headfile.h"

Menu menu;
Menu_select Menu_selectStructure =
{
    RGB565_BLACK,
    RGB565_WHITE,
    Menu_Cursor_Set,
};

OptionValue Num_Pointer;// ����ָ��

void Menu_init(void)
{
    menu.Power = 0.001000001; // ����Ĭ�Ϲ���
    menu.Key_flag = SELECT_MODE; // ����Ĭ�ϰ���ģʽ
    menu.Current_Option = MAIN_PAGE; // ���õ�ǰҳ��Ϊ��ҳ��
    menu.Previous_Option = MAIN_PAGE; // ������һ��ҳ��Ϊ��ҳ��
    menu.KeyEvent = NULL_KEY_EVENT; // ��ʼ�������¼�
    menu.Clear_Flag = 1; // �����־λ
    menu.Refresh_Flag = 1; // ˢ�±�־λ
    menu.Choose_i = 0; // ��ʼ��ѡ���±�
    menu.Show_start = 0; // ��ʼ����ʾ��ʼ�±�
}
/*�˵���ʾ*/
void Menu_Display(void)
{
    static uint8 Last_Option = 0; // �ϴ���ʾ�Ĳ˵�ѡ��

    if (menu.Current_Option != Last_Option)
    {
        Last_Option = menu.Current_Option; // �����ϴ���ʾ�Ĳ˵�ѡ��
        menu.Choose_i = 0;                 // ��ʼ��ѡ���±�
        menu.Show_start = 0;               // ��ʼ����ʾ��ʼ�±�
    }

    switch (menu.Current_Option)
    {
    case MAIN_PAGE:
        menu.Refresh_Flag = 1;
        MainPage_Run();
        break;
    case SETTING_PAGE:
        menu.Refresh_Flag = 1;
        SettingPage_Run();
        break;
    case Gyroscope_PAGE:
        menu.Refresh_Flag = 1;
        GyroscopePage_Run();
        break;
    case GyroData_PAGE:
        menu.Refresh_Flag = 1;
        GyroDataPage_Run();
        break;
    case Motor_Page:
        menu.Refresh_Flag = 1;
        MotorPage_Run();
        break;
    case Vaule_Page:
        menu.Refresh_Flag = 1;
        AdcData1Page_Run();
        break;
    case AdcData2_Page:
        menu.Refresh_Flag = 1;
        AdcData2Page_Run();
        break;
    case PID_Page:
        menu.Refresh_Flag = 1;
        PIDPage_Run();
        break;
    case Start_Page:
        menu.Refresh_Flag = 1;
        StartPage_Run();
        break;
    case Task1_Page:
        menu.Refresh_Flag = 1;
        Task1Page_Run();
        break; 
		case Status_Page:
				menu.Refresh_Flag = 1;
				StatusPage_Run();
				break;
		case NumSet_Page:
				menu.Refresh_Flag = 1;
				NumSetPage_Run();
				break;		
		case Addcirclue_Page:
				menu.Refresh_Flag = 1;
				AddcirclePage_Run();
				break;	
    default:
        break;
    }
}

/*�˵���ת*/
void Set_Menu(uint8 Page)
{
    menu.Current_Option = Page;
}

/*����ɨ��*/
void Menu_Key_Event(void)
{
    Menu_Key_Scan();

    if (Key_returnState(0) == 1)
    {
        menu.KeyEvent = UP;
    }
    if (Key_returnState(1) == 1)
    {
        menu.KeyEvent = DOWN;
    }
    if (Key_returnState(2) == 1)
    {
        menu.KeyEvent = RETRUN_KEY_Previous;
    }
    if (Key_returnState(3) == 1)
    {
        menu.KeyEvent = START;
    }
    if (Key_returnState(4) == 1)
    {
        menu.KeyEvent = YES;
    }
}

/*�ı䰴ťģʽ*/
void Switch_Keymode(void)
{
    menu.Key_flag = !menu.Key_flag;
}

/*�˵�ÿһ�д�ӡ����Ļ��*/
void Menu_PrintfOptionStr(uint8 X, uint8 Y, char *format, ...)
{
    char String[20]; // �����ַ�����
    int String_length = 0;
    va_list arg;                   // ����ɱ�����б��������͵ı���arg
    va_start(arg, format);         // ��format��ʼ�����ղ����б�arg����
    vsprintf(String, format, arg); // ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
    va_end(arg);                   // ��������arg
    String_length = strlen(String);
    memset((String + String_length), ' ', 20 - String_length);
    String[19] = '\0';
    Menu_Show_String(X, Y, String); // OLED��ʾ�ַ����飨�ַ������������ַ���
}

void Adjust_OptionValue(uint8 show_type, float delta)
{
    switch (show_type)
    {
    case SHOW_FLOAT:
        (*Num_Pointer.onefloat) += delta;
        break;
    case SHOW_INT16:
        (*Num_Pointer.oneint16) += (int16)delta;
        break;
    case SHOW_UINT8:
        *Num_Pointer.oneuint8 += (int8)delta;
        break;
    case SHOW_UINT16:
        *Num_Pointer.oneuint16 += (int16)delta;
        break;
    case SHOW_INT32:
        *Num_Pointer.oneint32 += (int32)delta;
        break;
    case SHOW_UINT32:
        *Num_Pointer.oneuint32 += (int32)delta;
        break;
    case SHOW_CHAR:
        *Num_Pointer.onechar += (char)delta;
        break;
    default:
        break;
    }
}

/*��ť����ѡ��˵�ģʽ��*/
int8 Option_select(Option_Class *Option_List, int16 Show_i)
{
    if (menu.KeyEvent == YES && Option_List[Show_i].func)
    {
        Option_List[Show_i].func();
        menu.Clear_Flag = 1;
        menu.Refresh_Flag = 1;
    }
    else if (menu.KeyEvent == RETRUN_KEY_Previous)
    {
        Set_Menu(MAIN_PAGE);
        menu.Clear_Flag = 1;
        menu.Refresh_Flag = 1;
    }
    else if (menu.KeyEvent == UP || menu.KeyEvent == DOWN)
    {
        menu.Refresh_Flag = 1;
        return menu.KeyEvent;
    }
    else if (menu.KeyEvent == START)
    {
        return 0; // ����0��ʾû�д���
    }
    return 0;
}

/*��ť������������ģʽ��*/
void Argument_Change(uint8 Show_Type)
{
    Menu_selectStructure.Selected_Way = Menu_Select_Set;
    switch (menu.KeyEvent)
    {
    case YES:
        menu.Power = menu.Power * 10;
        if (menu.Power >= 10000)
        {
            menu.Power = 0.00100001;
        }
        menu.Refresh_Flag = 1;
        break;
    case UP:
        Adjust_OptionValue(Show_Type, menu.Power);
        menu.Refresh_Flag = 1;
        break;
    case DOWN:
        menu.Power = -1 * menu.Power;
        Adjust_OptionValue(Show_Type, menu.Power);
        menu.Power = -1 * menu.Power;
        menu.Refresh_Flag = 1;
        break;
    case RETRUN_KEY_Previous:
        Switch_Keymode();
        menu.Refresh_Flag = 1;
        Menu_selectStructure.Selected_Way = Menu_Cursor_Set;
        break;
    }
}

void Set_Num_Pointer(Option_Class *option)
{
    switch (option->Show_Form)
    {
    case SHOW_FLOAT:
        Num_Pointer.onefloat = option->value.onefloat;
        break;
    case SHOW_INT16:
        Num_Pointer.oneint16 = option->value.oneint16;
        break;
    case SHOW_UINT8:
        Num_Pointer.oneuint8 = option->value.oneuint8;
        break;
    case SHOW_UINT16:
        Num_Pointer.oneuint16 = option->value.oneuint16;
        break;
    case SHOW_INT32:
        Num_Pointer.oneint32 = option->value.oneint32;
        break;
    case SHOW_UINT32:
        Num_Pointer.oneuint32 = option->value.oneuint32;
        break;
    case SHOW_CHAR:
        Num_Pointer.onechar = option->value.onechar;
        break;
    default:
        break;
    }
}

/*�˵�ĩβ��ʾѡ��*/
void Show_Typechose(Option_Class *Option_List, int16 i, int16 Start_i)
{
    Option_Class *opt = &Option_List[Start_i + i];
    switch (opt->Show_Form)
    {
    case SHOW_CHAR:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.onechar);
        break;
    case SHOW_FLOAT:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.onefloat);
        break;
    case SHOW_INT16:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.oneint16);
        break;
    case SHOW_UINT8:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.oneuint8);
        break;
    case SHOW_UINT16:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.oneuint16);
        break;
    case SHOW_INT32:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.oneint32);
        break;
    case SHOW_UINT32:
        Menu_PrintfOptionStr(Window_X, Window_Y + i * Line_Height, opt->String, *opt->value.oneuint32);
        break;
    case SHOW_NOTHIONG:
        Menu_Show_String(Window_X, Window_Y + i * Line_Height, opt->String);
        break;
    default:
        break;
    }
}

/*�˵�����*/
void Menu_RunMenu(Option_Class *Option_List)
{
    int16 i;
    static int16 Option_MaxNum = 0;        // ����Ϊ��̬����,�����Ϊ,����������,������һ�βŻ�ͳ��Option_MaxNum�Ĵ�С
    int16 Cur_i_Ceiling = Line_MaxNum - 1; // ��ʾ��������
    int8 Roll_Event = 0;

    if (menu.Clear_Flag)
    {
        menu.Clear_Flag = 0;
        Menu_Clear();
        // ����˵�������
        for (Option_MaxNum = 0; Option_List[Option_MaxNum].String[0] != '.'; Option_MaxNum++)
            ;
    }
    // ����һ�������ʾ����
    if (Option_MaxNum < Cur_i_Ceiling)
        Cur_i_Ceiling = Option_MaxNum;

    if (menu.Refresh_Flag)
    {
        menu.Refresh_Flag = 0;

        // ������Ϣ
        Menu_Show_float(80, 0, menu.Power, 4, 3);

        // ��ʾ�����ڵĲ˵���
        for (i = 0; i < Cur_i_Ceiling; i++)
        {
            Menu_Show_Char(Window_W - Font_Width - 1 - Window_X,
                            (uint8)(Window_Y + (i * Line_Height)),
                            (i == menu.Choose_i) ? Menu_selectStructure.Selected_Way : ' ');
            Show_Typechose(Option_List, i, menu.Show_start);
        }

        // ���õ���ָ��
        Set_Num_Pointer(&Option_List[menu.Choose_i + menu.Show_start]);
    }

    // ����ģʽ����
    if (menu.Key_flag == SELECT_MODE)
    {
        Roll_Event = Option_select(Option_List, menu.Choose_i + menu.Show_start);
        menu.KeyEvent = NULL_KEY_EVENT;
    }
    if (menu.Key_flag == ARGUMENT_MODE)
    {
        Argument_Change(Option_List[menu.Choose_i + menu.Show_start].Show_Form);
        menu.KeyEvent = NULL_KEY_EVENT;
    }

    // ������ʹ��ڹ���
    if (Roll_Event)
    {
        menu.Choose_i += Roll_Event;
        // ����Խ��
        if (menu.Choose_i < 0)
        {
            if (menu.Show_start > 0)
            {
                menu.Show_start--;
                menu.Choose_i = 0;
            }
            else
            {
                // ������ѭ�������һ��
                menu.Show_start = (Option_MaxNum > Cur_i_Ceiling) ? Option_MaxNum - Cur_i_Ceiling : 0;
                menu.Choose_i = (Option_MaxNum < Cur_i_Ceiling) ? Option_MaxNum - 1 : Cur_i_Ceiling - 1;
            }
        }
        // ����Խ��
        else if (menu.Choose_i > Cur_i_Ceiling - 1)
        {
            if (menu.Show_start < Option_MaxNum - Cur_i_Ceiling)
            {
                menu.Show_start++;
                menu.Choose_i = Cur_i_Ceiling - 1;
            }
            else
            {
                // ���׺�ѭ������һ��
                menu.Show_start = 0;
                menu.Choose_i = 0;
            }
        }
    }
}