#include "stm32f10x.h"                  // Device header
uint16_t AD_Value [1];
void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//����DMA1ʱ��
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
    
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_Mode =ADC_Mode_Independent ;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;

    ADC_Init(ADC1,&ADC_InitStruct);
    
    
    DMA_InitTypeDef DMA_InitStruct;//��ʼ��DMA�ṹ��
    DMA_InitStruct.DMA_BufferSize = 1;//��������� ���伸��
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽�� ��������վ����Դ��ַ ���� Ŀ���ַ
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//ѡ��Ӳ����������������� Ӳ������
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//�洢��վ�����ַ    
    DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord;//�洢�����ݿ��
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ַ�Ƿ�����
    DMA_InitStruct.DMA_Mode =DMA_Mode_Normal ;//�Ƿ��Զ���װ
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//����վ�����ַ
    DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord ;//�������ݿ��
    DMA_InitStruct.DMA_PeripheralInc =DMA_PeripheralInc_Disable ;//��ַ�Ƿ�����
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;//ָ��ͨ�������ȼ�

    DMA_Init(DMA1_Channel1,&DMA_InitStruct);
    
    DMA_Cmd(DMA1_Channel1,DISABLE);//ʹ��DMA1��ͨ��1
    ADC_DMACmd(ADC1,ENABLE);//����DMA�����ź� ��ADC1��ΪDMA������ź�Դ
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

void AD_GetValue(void)
{
    DMA_Cmd(DMA1_Channel1,DISABLE);//ʧ��DMA
    DMA_SetCurrDataCounter(DMA1_Channel1,1);//���¸������������ֵ
    DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��DMA
    
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//����־λ �ȴ�ת�����
    DMA_ClearFlag(DMA1_FLAG_TC1);//�����־λ

}