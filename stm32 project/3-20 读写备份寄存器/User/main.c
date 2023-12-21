#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "W25Q64.h"
#include "Key.h"

uint8_t KeyNum;
uint16_t ArrayWrite[]={0x1234,0x5678};
uint16_t ArrayRead[2];

int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1,1,"W:");
    OLED_ShowString(2,1,"R:");
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//开启PWR的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//开启BKP的时钟
    
    PWR_BackupAccessCmd(ENABLE);//使能对后备区域的访问（BKP和RTC）
    

    while(1)
    {
        KeyNum=Key_GetNum();
        
        if(KeyNum==1)
        {
            ArrayWrite[0]++;
            ArrayWrite[1]++;
            
            BKP_WriteBackupRegister(BKP_DR1,ArrayWrite[0]);//写入BKP
            BKP_WriteBackupRegister(BKP_DR2,ArrayWrite[1]);//写入BKP
            
            OLED_ShowHexNum(1,3,ArrayWrite[0],4);
            OLED_ShowHexNum(1,8,ArrayWrite[1],4);
        }
        ArrayRead[0]=BKP_ReadBackupRegister(BKP_DR1);//读取BKP
        ArrayRead[1]=BKP_ReadBackupRegister(BKP_DR2);//读取BKP
        OLED_ShowHexNum(2,3,ArrayRead[0],4);
        OLED_ShowHexNum(2,8,ArrayRead[1],4);
    }
}

