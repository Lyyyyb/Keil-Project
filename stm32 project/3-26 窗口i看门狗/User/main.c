#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"
int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1,1,"WWDG TEST");
    
    if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)//查看窗口看门狗复位的标志位，判断是否是窗口看门狗导致的程序复位
    {
        OLED_ShowString(2,1,"WWDGSET");
        Delay_ms(500);
        OLED_ShowString(2,1,"       ");
        Delay_ms(500);
        
        RCC_ClearFlag();//清除标志位
    }
    else
    {
        OLED_ShowString(3,1,"RST");
        Delay_ms(500);
        OLED_ShowString(3,1,"   ");
        Delay_ms(100);
    }
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);//开启WWDG时钟
    
    WWDG_SetPrescaler(WWDG_Prescaler_8);//配置预分频值
    WWDG_SetWindowValue(0x40 | 21);//设置窗口值 30ms
    WWDG_Enable(0x40 | 54);//开启窗口看门狗，同时喂狗并在窗口看门狗使能位置1 50ms

    while(1)
    {
        Key_GetNum();
                
        OLED_ShowString(4,1,"FEED");
        Delay_ms(20);
        OLED_ShowString(4,1,"    ");
        Delay_ms(20);
        
        WWDG_SetCounter(0x40 | 54);//先经过40ms的延迟再喂狗（防止立刻复位） 同时在窗口看门狗使能位置0，但是不会关闭窗口看门狗，因为启动看门狗之后，看门狗是不能再被关闭的，除非发生复位
    }
}

