#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //打开GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //初始化成员变量 推挽输出
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ; //开漏输出
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
 	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化 GPIO
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0); //重置某一位 置0
	//GPIO_SetBits(GPIOA,GPIO_Pin_0); //给某一位设置1
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET); 
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
    while(1)
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        Delay_ms(100);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
//        Delay_ms(100);
//        //GPIO_WriteBit(GPIOA,GPIO_Pin_0,0); 会报警告 混入了别的枚举类型 
//        Delay_ms(100);
//        GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)1);//需要进行强制类型转换
//        Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
//        Delay_ms(100);
    }
}

    