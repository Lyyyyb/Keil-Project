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
            OLED_ShowHexNum(1,1,RxData,2);//��ʾ����
        
        }
    }

     
    
}

