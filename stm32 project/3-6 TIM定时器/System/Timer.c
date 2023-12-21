#include "stm32f10x.h"                  // Device header

extern uint16_t Num;
void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ��ʹ��
    
    TIM_InternalClockConfig(TIM2);//�رմ�ģʽ Ԥ��Ƶ��ֱ�����ڲ�ʱ������
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision =TIM_CKD_DIV1 ;//1��Ƶ 
    TIM_TimeBaseInitStruct.TIM_CounterMode  =TIM_CounterMode_Up ;//�����ؼ���
    TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//�Զ���װ�ؼĴ���
    TIM_TimeBaseInitStruct.TIM_Prescaler =7200-1 ;//Ԥ��Ƶ��
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
  
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;
    NVIC_Init(&NVIC_InitStructure);
    
    
    TIM_Cmd(TIM2,ENABLE);//ʹ��ʱ��
}


void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//����־λ
    {
        Num++;
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//�����־λ �����һֱ�����ж�
    }
}
