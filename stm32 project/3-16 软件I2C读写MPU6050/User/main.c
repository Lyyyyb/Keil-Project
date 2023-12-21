#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"
#include "MPU6050.h"
uint8_t ID;
int16_t AX,AY,AZ,GX,GY,GZ;
int main(void)
{
    OLED_Init();
    OLED_ShowString(1,1,"ID");
    MPU6050_Init();

    

    while(1)
    {
      ID=MPU6050_GetID();
      OLED_ShowHexNum(1,4,ID,2);//获取ID号
      MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);//获取加速度计和陀螺仪测量值
      OLED_ShowSignedNum(2,1,AX,5);
      OLED_ShowSignedNum(3,1,AY,5);
      OLED_ShowSignedNum(4,1,AZ,5);
      OLED_ShowSignedNum(2,8,GX,5);
      OLED_ShowSignedNum(3,8,GY,5);
      OLED_ShowSignedNum(4,8,GZ,5);
    }
}

    