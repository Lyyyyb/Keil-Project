#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Count.h"
#include "Timer.h"
#include "PWM.h"

uint16_t Num;

int main(void)
{
    OLED_Init();
    PWM_Init();
    OLED_ShowString(1,1,"PWM:");
    while(1)
    {
       for(Num=0;Num<=100;Num++)
        {
          PWM_SetCompare1(Num);
          OLED_ShowNum(1,8,Num,5);
          Delay_ms(10);  
        }
        
       for(Num=0;Num<=100;Num++)
        {
          PWM_SetCompare1(100-Num);
          OLED_ShowNum(1,8,Num,5);
          Delay_ms(10);  
        }
      
    }

     
    
}

