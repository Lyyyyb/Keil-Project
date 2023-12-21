#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum ;

int main(void)
{
    int count = 0;
    Key_Init();
    LED_Init();
    OLED_Init();
    OLED_ShowString(1,1,"count:");
    while(1)
    {
        KeyNum= Key_GetNum();
        if(KeyNum == 1)
        {
            //LED_ON();
            LED_Turn();//°´¼ü·´×ª
            count++;
            OLED_ShowNum(1,8,count,2);
        }else{
        
        }
    }
}

    