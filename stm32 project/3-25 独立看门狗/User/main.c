#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"
int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1,1,"IWDG TEST");
    
    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)//�鿴�������Ź���λ�ı�־λ���ж��Ƿ��Ƕ������Ź����µĳ���λ
    {
        OLED_ShowString(2,1,"IWDGSET");
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
    
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//���д����
    IWDG_SetPrescaler(IWDG_Prescaler_16);//����Ԥ��Ƶֵ
    IWDG_SetReload(2499);//������װֵ
    
    IWDG_ReloadCounter();//������ǰ��ιһ�ι�������������ĵ�һ��ι�����ڣ�����1000ms��
    IWDG_Enable();//�������Ź�
    //ι����ʹ�ܵ�ʱ�򣬻��ڼ��Ĵ���д��0x5555֮���ֵ����˳����Ĵ���д�����ˣ��������ֶ�ִ��д������
    
    while(1)
    {
        Key_GetNum();//��ס�������ţ���ѭ���ͻ���������ѭ�����������ܼ�ʱι�����������Ź��ͻḴλ��
        
        IWDG_ReloadCounter();//ι��
        
        OLED_ShowString(4,1,"FEED");
        Delay_ms(200);
        OLED_ShowString(4,1,"    ");
        Delay_ms(600);
    }
}

