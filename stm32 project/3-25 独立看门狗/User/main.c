#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"
int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1,1,"IWDG TEST");
    
    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)//查看独立看门狗复位的标志位，判断是否是独立看门狗导致的程序复位
    {
        OLED_ShowString(2,1,"IWDGSET");
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
    
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//解除写保护
    IWDG_SetPrescaler(IWDG_Prescaler_16);//配置预分频值
    IWDG_SetReload(2499);//配置重装值
    
    IWDG_ReloadCounter();//在启动前先喂一次狗，这样启动后的第一个喂狗周期，就是1000ms。
    IWDG_Enable();//启动看门狗
    //喂狗或使能的时候，会在键寄存器写入0x5555之外的值，又顺便给寄存器写保护了，不用再手动执行写保护。
    
    while(1)
    {
        Key_GetNum();//按住按键不放，主循环就会阻塞，主循环阻塞，不能及时喂狗，独立看门狗就会复位。
        
        IWDG_ReloadCounter();//喂狗
        
        OLED_ShowString(4,1,"FEED");
        Delay_ms(200);
        OLED_ShowString(4,1,"    ");
        Delay_ms(600);
    }
}

