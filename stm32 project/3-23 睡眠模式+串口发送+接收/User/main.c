#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint16_t Num;
uint8_t RxData;
int main(void)
{
    
    OLED_Init();
    OLED_ShowString(1,1,"RxData");
    Serial_Init();

    while(1)
    {
        if(Serial_GetRxFlag()==1)//������յ�������
        {
            RxData = Serial_GetRxData();//��ȡ����
            Serial_SendByte(RxData);//��������
            OLED_ShowHexNum(1,8,RxData,2);//��ʾ����
        }
        OLED_ShowString(2,1,"Running");
        Delay_ms(500);
        OLED_ShowString(2,1,"        ");
        Delay_ms(500);
        
        __WFI();//���̽���˯��ģʽ
    }
}

