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

int main(void)
{
    
    OLED_Init();
    Serial_Init();
    //Serial_SendByte('A');
    //uint8_t MyArray[]={0x42,0x43,0x44,0x45};
    //Serial_SendArray(MyArray,4);
    Serial_SendString("Hello World!\r\n");
    Serial_SendNumber(12345,5);
    printf("Num=%d\r\n",666);
    
    char String[100];
    sprintf(String,"Num=%d\r\n",777);//将格式化字符输出到一个字符串中 然后再发送这个字符串
    Serial_SendString(String);
    Serial_Printf("Num=%d\r\n",888);
    Serial_SendString("你好世界");
    while(1)
    {

    }

     
    
}

