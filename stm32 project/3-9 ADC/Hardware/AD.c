#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//����ADC1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIOA��ʱ��
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADCCLK 6��Ƶ
    
    GPIO_InitTypeDef GPIO_InitStructure;//��ʼ��GPIO�ṹ��
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//���ó�ģ������ģʽ ��ģʽ��GPIO����Ч��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//���ù����������ͨ�� PA0��ӦADC��ͨ��0 ����λ�� ����ʱ�䣨��ADDCLK�йأ� 
    
    ADC_InitTypeDef ADC_InitStruct;//��ʼ��ADC�ṹ��
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//��������ת��ģʽ ����ת�����ǵ���ת�� 
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�������ݶ��� ����뻹���Ҷ���
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�����ⲿ����Դ �����������ʹ��������� None
    ADC_InitStruct.ADC_Mode =ADC_Mode_Independent ;//����ADC����ģʽ ����ģʽ����˫ADCģʽ
    ADC_InitStruct.ADC_NbrOfChannel = 1;//����ͨ����Ŀ �����ָ����ɨ��ģʽ�£��ܹ����õ�����ͨ��
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;//����ɨ��ģʽ ɨ��ģʽ���Ƿ�ɨ��ģʽ
 
    ADC_Init(ADC1,&ADC_InitStruct);
    
    ADC_Cmd(ADC1,ENABLE);//ʹ��ADC
    //ADCУ׼
    ADC_ResetCalibration(ADC1);//��λУ׼
    while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ���λУ׼���
    ADC_StartCalibration(ADC1);//��ʼУ׼
    while(ADC_GetCalibrationStatus(ADC1));//�ȴ�У׼���
}

uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������ת�� �����Ϳ��Դ���ADC��ʼ����ת����
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//��ȡ��־λ״̬ �ȴ�ת����� EOC������ת����ɱ�־λ 
    
    return ADC_GetConversionValue(ADC1);//��ȡADת�����ֵ
}