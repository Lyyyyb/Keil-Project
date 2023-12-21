#include "stm32f10x.h"                  // Device header

extern uint16_t Num;
void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//外设时钟使能
    
    TIM_InternalClockConfig(TIM2);//关闭从模式 预分频器直接由内部时钟驱动
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision =TIM_CKD_DIV1 ;//1分频 
    TIM_TimeBaseInitStruct.TIM_CounterMode  =TIM_CounterMode_Up ;//上升沿计数
    TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//自动重装载寄存器 ARR
    TIM_TimeBaseInitStruct.TIM_Prescaler =7200-1 ;//预分频器  PSC
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
  
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;
    NVIC_Init(&NVIC_InitStructure);
    
    
    TIM_Cmd(TIM2,ENABLE);//使能时钟

}


void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//检查标志位
    {
        Num++;
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位 否则会一直请求中断
    }


}
