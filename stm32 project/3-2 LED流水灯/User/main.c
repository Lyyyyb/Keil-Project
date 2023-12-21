#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //打开GPIOA的时钟
    
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //初始化成员变量 推挽输出
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ; //开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    //GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;//通过按位或 同时选中了3个端口 16进制转化成2进制 16位 每一位对应一个外设
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
 	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化 GPIO
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0); //重置某一位 置0
	//GPIO_SetBits(GPIOA,GPIO_Pin_0); //给某一位设置1
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET); 
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
    while(1)
    {
      
//		//GPIO_WriteBit(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 ,Bit_RESET);//通过按位或 同时选择多个端口
//        Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3,Bit_SET);
//        Delay_ms(100);
        GPIO_Write(GPIOA,~0x0002);//0000 0000 0000 0010 A1处 led点亮 其余熄灭
        Delay_ms(500);
        GPIO_Write(GPIOA,~0x0004);//0000 0000 0000 0100 A2处 led点亮 其余熄灭
        Delay_ms(500);
        GPIO_Write(GPIOA,~0x0008);//0000 0000 0000 1000 A3处 led点亮 其余熄灭
        Delay_ms(500);
    }
}

    