#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"
#include "Delay.h"
int main(void)
{
    OLED_Init();
    MyRTC_Init();
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//����PWRʱ�ӣ����ִ��������
    
    OLED_ShowString(1,1,"CNT :");
    OLED_ShowString(2,1,"ALR :");
    OLED_ShowString(3,1,"ALRF:");
    
    PWR_WakeUpPinCmd(ENABLE);//WAKE_UP���Ż���                                                                             
    
    uint32_t Alarm = RTC_GetCounter()+10;//�趨����Ϊ10S��
    RTC_SetAlarm(Alarm);//��������ֵ
    OLED_ShowNum(2,6,Alarm,10);//��ʾ����ֵ
    
    while(1)
    {
        
       OLED_ShowNum(1,6, RTC_GetCounter(),10);//��ʾ���� ����CNT������CNT���ALR��ȣ�Ȼ�󴥷����ӱ�־λ��1
       OLED_ShowNum(3,6, RTC_GetFlagStatus(RTC_FLAG_ALR),10);//��ʾ���ӱ�־λ
        
       OLED_ShowString(4,1,"Running");
       Delay_ms(500);
       OLED_ShowString(4,1,"        ");
       Delay_ms(500);
        
       OLED_ShowString(4,9,"STANDBY"); 
       Delay_ms(1000);
       OLED_ShowString(4,1,"        ");
       Delay_ms(500);
       
       OLED_Clear();//����
        
       PWR_EnterSTANDBYMode();//�������ģʽ
        

    }
}

