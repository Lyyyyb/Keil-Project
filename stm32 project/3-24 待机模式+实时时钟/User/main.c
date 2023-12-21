#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"
#include "Delay.h"
int main(void)
{
    OLED_Init();
    MyRTC_Init();
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//开启PWR时钟，保持代码独立性
    
    OLED_ShowString(1,1,"CNT :");
    OLED_ShowString(2,1,"ALR :");
    OLED_ShowString(3,1,"ALRF:");
    
    PWR_WakeUpPinCmd(ENABLE);//WAKE_UP引脚唤醒                                                                             
    
    uint32_t Alarm = RTC_GetCounter()+10;//设定闹钟为10S后
    RTC_SetAlarm(Alarm);//设置闹钟值
    OLED_ShowNum(2,6,Alarm,10);//显示闹钟值
    
    while(1)
    {
        
       OLED_ShowNum(1,6, RTC_GetCounter(),10);//显示秒数 随着CNT的增大，CNT会和ALR相等，然后触发闹钟标志位置1
       OLED_ShowNum(3,6, RTC_GetFlagStatus(RTC_FLAG_ALR),10);//显示闹钟标志位
        
       OLED_ShowString(4,1,"Running");
       Delay_ms(500);
       OLED_ShowString(4,1,"        ");
       Delay_ms(500);
        
       OLED_ShowString(4,9,"STANDBY"); 
       Delay_ms(1000);
       OLED_ShowString(4,1,"        ");
       Delay_ms(500);
       
       OLED_Clear();//清屏
        
       PWR_EnterSTANDBYMode();//进入待机模式
        

    }
}

