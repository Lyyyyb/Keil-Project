#include "stm32f10x.h"                  // Device header
#include "delay.h"

uint16_t Count ; 
void Count_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;//初始化结构体
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);//中断引脚选择
    
    EXTI_InitTypeDef Exti_InitStructure; //配置中断
    Exti_InitStructure.EXTI_Line= EXTI_Line5; //14号线路
    Exti_InitStructure.EXTI_LineCmd = ENABLE; //开启中断
    Exti_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt ;//中断模式
    Exti_InitStructure.EXTI_Trigger =EXTI_Trigger_Rising ;//下降沿触发
    EXTI_Init(&Exti_InitStructure);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//指定中断分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel  =EXTI9_5_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//设置抢占优先级和相应优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);//初始化NVIC
}
uint16_t Get_Count(void)
{
    return Count;
}

void EXTI9_5_IRQHandler(void)
{
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
    {
        Delay_ms(20);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0);
        Delay_ms(20);
         if(EXTI_GetITStatus(EXTI_Line5)==SET)
   {

      
        Count++;
        EXTI_ClearITPendingBit(EXTI_Line5);
   }
        
    };
  

}
