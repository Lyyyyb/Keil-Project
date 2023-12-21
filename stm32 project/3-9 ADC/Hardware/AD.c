#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//开启ADC1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA的时钟
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//配置ADCCLK 6分频
    
    GPIO_InitTypeDef GPIO_InitStructure;//初始化GPIO结构体
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//配置成模拟输入模式 该模式下GPIO是无效的
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//配置规则组的输入通道 PA0对应ADC的通道0 序列位置 采样时间（与ADDCLK有关） 
    
    ADC_InitTypeDef ADC_InitStruct;//初始化ADC结构体
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//配置连续转换模式 连续转换还是单次转换 
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//配置数据对齐 左对齐还是右对齐
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//配置外部触发源 由于我们这次使用软件触发 None
    ADC_InitStruct.ADC_Mode =ADC_Mode_Independent ;//配置ADC工作模式 独立模式还是双ADC模式
    ADC_InitStruct.ADC_NbrOfChannel = 1;//配置通道数目 这个是指定在扫描模式下，总共会用到几个通道
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;//配置扫描模式 扫描模式还是非扫描模式
 
    ADC_Init(ADC1,&ADC_InitStruct);
    
    ADC_Cmd(ADC1,ENABLE);//使能ADC
    //ADC校准
    ADC_ResetCalibration(ADC1);//复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));//等待复位校准完成
    ADC_StartCalibration(ADC1);//开始校准
    while(ADC_GetCalibrationStatus(ADC1));//等待校准完成
}

uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换 这样就可以触发ADC开始进行转换了
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//获取标志位状态 等待转换完成 EOC规则组转换完成标志位 
    
    return ADC_GetConversionValue(ADC1);//获取AD转换后的值
}