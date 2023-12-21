#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Count.h"
#include "Timer.h"
#include "PWM.h"
#include "IC.h"
#include "AD.h"
#include "MyDMA.h"
uint16_t Num;
uint8_t DataA[]={0x01,0x02,0x03,0x04};
uint8_t DataB[]={0,0,0,0};

int main(void)
{
    
    OLED_Init();
    AD_Init();
    
    while(1)
    {
        AD_GetValue();
        OLED_ShowNum(1,1,AD_Value[0],5);
    }

     
    
}

