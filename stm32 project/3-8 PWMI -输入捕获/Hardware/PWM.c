#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    
    TIM_InternalClockConfig(TIM2);//�رմ�ģʽ Ԥ��Ƶ��ֱ�����ڲ�ʱ������
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision =TIM_CKD_DIV1 ;//1��Ƶ 
    TIM_TimeBaseInitStruct.TIM_CounterMode  =TIM_CounterMode_Up ;//�����ؼ���
    TIM_TimeBaseInitStruct.TIM_Period = 100-1;//�Զ���װ�ؼĴ���
    TIM_TimeBaseInitStruct.TIM_Prescaler =720-1 ;//Ԥ��Ƶ��
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
        
    TIM_Cmd(TIM2,ENABLE);//ʹ��ʱ��

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable ;
    TIM_OCInitStruct.TIM_Pulse = 0; //CCR
    TIM_OC1Init(TIM2,&TIM_OCInitStruct);


}

void PWM_SetCompare1(uint16_t Compare )
{
    TIM_SetCompare1(TIM2,Compare);
}
void PWM_SetPrescaler(uint16_t Prescaler)
{
    TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Immediate);
}