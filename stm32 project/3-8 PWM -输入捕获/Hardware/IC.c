#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//外设时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    
    TIM_InternalClockConfig(TIM3);//关闭从模式 预分频器直接由内部时钟驱动
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision =TIM_CKD_DIV1 ;//1分频 
    TIM_TimeBaseInitStruct.TIM_CounterMode  =TIM_CounterMode_Up ;//上升沿计数
    TIM_TimeBaseInitStruct.TIM_Period = 65536-1;//自动重装载寄存器
    TIM_TimeBaseInitStruct.TIM_Prescaler =720-1 ;//预分频器
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter = 0xF;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICPrescaler =TIM_ICPSC_DIV1 ;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3,&TIM_ICInitStruct);
    
    TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//输入触发源
    
    TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);//从模式
    
    TIM_Cmd(TIM3,ENABLE);
    
}

uint32_t IC_GetFreq(void)
{
    return 1000000 / TIM_GetCapture1(TIM3);
}
