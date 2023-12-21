#include "stm32f10x.h"                  // Device header
uint16_t AD_Value [1];
void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//开启DMA1时钟
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
    
    
    DMA_InitTypeDef DMA_InitStruct;//初始化DMA结构体
    DMA_InitStruct.DMA_BufferSize = 1;//传输计数器 传输几次
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向 配置外设站点是源地址 还是 目标地址
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//选择硬件触发还是软件触发 硬件触发
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//存储器站点基地址    
    DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord;//存储器数据宽度
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//地址是否自增
    DMA_InitStruct.DMA_Mode =DMA_Mode_Normal ;//是否自动重装
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//外设站点基地址
    DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord ;//外设数据宽度
    DMA_InitStruct.DMA_PeripheralInc =DMA_PeripheralInc_Disable ;//地址是否自增
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;//指定通道的优先级

    DMA_Init(DMA1_Channel1,&DMA_InitStruct);
    
    DMA_Cmd(DMA1_Channel1,DISABLE);//使能DMA1的通道1
    ADC_DMACmd(ADC1,ENABLE);//开启DMA触发信号 将ADC1作为DMA请求的信号源
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

void AD_GetValue(void)
{
    DMA_Cmd(DMA1_Channel1,DISABLE);//失能DMA
    DMA_SetCurrDataCounter(DMA1_Channel1,1);//重新给传输计数器赋值
    DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA
    
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//检测标志位 等待转运完成
    DMA_ClearFlag(DMA1_FLAG_TC1);//清除标志位

}