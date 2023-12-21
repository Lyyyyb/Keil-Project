#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Count.h"


int main(void)
{
    int count = 0;
    Count_Init();
    OLED_Init();
    OLED_ShowString(1,1,"count:");
    while(1)
    {
        OLED_ShowNum(1,7,Get_Count(),5);
    }

     
    
}


    