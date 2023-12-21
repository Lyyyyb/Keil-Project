#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{
    OLED_Init();
    MyRTC_Init();
    
    OLED_ShowString(1,1,"Date:xxxx-xx-xx");
    OLED_ShowString(2,1,"Time:xx:xx:xx");
    OLED_ShowString(3,1,"CNT :");
    OLED_ShowString(4,1,"DIV :");
    
    
    while(1)
    {
        MyRTC_ReadTime();//读取时间
        
        OLED_ShowNum(1,6, MyRTC_Time[0],4);
        OLED_ShowNum(1,11, MyRTC_Time[1],2);
        OLED_ShowNum(1,14, MyRTC_Time[2],2);
        OLED_ShowNum(2,6, MyRTC_Time[3],2);
        OLED_ShowNum(2,9, MyRTC_Time[4],2);
        OLED_ShowNum(2,12, MyRTC_Time[5],2);
        
        OLED_ShowNum(3,6, RTC_GetCounter(),10);
        OLED_ShowNum(4,6, (32767-RTC_GetDivider())/32767.0*999,10);//使用毫秒显示，在0~999自增，每自增一轮CNT计数器+1。
    }
}

