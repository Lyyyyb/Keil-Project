#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Count.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{
    OLED_Init();
    Timer_Init();
    OLED_ShowString(1,1,"count:");
    while(1)
    {
       OLED_ShowNum(1,8,Num,5);
    }    
}

