#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"
int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1,1,"WWDG TEST");
    
    if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)//�鿴���ڿ��Ź���λ�ı�־λ���ж��Ƿ��Ǵ��ڿ��Ź����µĳ���λ
    {
        OLED_ShowString(2,1,"WWDGSET");
        Delay_ms(500);
        OLED_ShowString(2,1,"       ");
        Delay_ms(500);
        
        RCC_ClearFlag();//�����־λ
    }
    else
    {
        OLED_ShowString(3,1,"RST");
        Delay_ms(500);
        OLED_ShowString(3,1,"   ");
        Delay_ms(100);
    }
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);//����WWDGʱ��
    
    WWDG_SetPrescaler(WWDG_Prescaler_8);//����Ԥ��Ƶֵ
    WWDG_SetWindowValue(0x40 | 21);//���ô���ֵ 30ms
    WWDG_Enable(0x40 | 54);//�������ڿ��Ź���ͬʱι�����ڴ��ڿ��Ź�ʹ��λ��1 50ms

    while(1)
    {
        Key_GetNum();
                
        OLED_ShowString(4,1,"FEED");
        Delay_ms(20);
        OLED_ShowString(4,1,"    ");
        Delay_ms(20);
        
        WWDG_SetCounter(0x40 | 54);//�Ⱦ���40ms���ӳ���ι������ֹ���̸�λ�� ͬʱ�ڴ��ڿ��Ź�ʹ��λ��0�����ǲ���رմ��ڿ��Ź�����Ϊ�������Ź�֮�󣬿��Ź��ǲ����ٱ��رյģ����Ƿ�����λ
    }
}

