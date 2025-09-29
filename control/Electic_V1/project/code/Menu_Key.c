#include "headfile.h"

Key_T Key[5];

void Key_Init(void)
{
	gpio_init(KEY_UP_PIN,GPI,1,GPI_PULL_UP);
	gpio_init(KEY_DOWN_PIN,GPI,1,GPI_PULL_UP);
	gpio_init(KEY_LEFT_PIN,GPI,1,GPI_PULL_UP);
	gpio_init(KEY_RIGHT_PIN,GPI,1,GPI_PULL_UP);
	gpio_init(KEY_SELECT_PIN,GPI,1,GPI_PULL_UP);

	Key[0].KeyState = 0;
	Key[1].KeyState = 0;
	Key[2].KeyState = 0;
	Key[3].KeyState = 0;
	Key[4].KeyState = 0;
}

void Key_Scan(void)
{
	uint8 i = 0;
    for(i = 0; i < KEY_NUM; i++)
    {
        uint8 key_now = Key_select(i);

        if(key_now == 0)
        {
            if(Key[i].KeyCount < KEY_SHOCK_PERIOD)
                Key[i].KeyCount++;
            else if(Key[i].KeyState == 0)
            {
                Key[i].KeyState = 1;
                Key[i].g_KeyState = 1;
            }
        }
        else
        {
            if(Key[i].KeyCount > 0)
                Key[i].KeyCount--;
            else
                Key[i].KeyState = 0;
        }
    }
}

uint8 Key_select(uint8 i)
{
	switch(i){
		case 0:{
			return gpio_get_level(KEY_UP_PIN);
		}break;
		
		case 1:{
			return gpio_get_level(KEY_DOWN_PIN);
		}break;
		
		case 2:{
			return gpio_get_level(KEY_LEFT_PIN);
		}break;
		
		case 3:{
			return gpio_get_level(KEY_RIGHT_PIN);
		}break;
		
		case 4:{
			return gpio_get_level(KEY_SELECT_PIN);
		}break;
	}
	return -1;
}	

uint8 Key_returnState(uint8 i)
{
	if(Key[i].g_KeyState == 1)
    {
		Key[i].g_KeyState = 0;
		return 1; 
	}
    else
    {
		return 0;
	}
}