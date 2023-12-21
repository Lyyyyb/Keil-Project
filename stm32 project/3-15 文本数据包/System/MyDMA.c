#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)
{
    MyDMA_Size=Size;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//����DMA1ʱ��
    
    DMA_InitTypeDef DMA_InitStruct;//��ʼ��DMA�ṹ��
    DMA_InitStruct.DMA_BufferSize = Size;//��������� ���伸��
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽�� ��������վ����Դ��ַ ���� Ŀ���ַ
    DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//ѡ��Ӳ����������������� ������� һֱ�д����ź�
    DMA_InitStruct.DMA_MemoryBaseAddr =AddrB ;//�洢��վ�����ַ    
    DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;//�洢�����ݿ��
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ַ�Ƿ�����
    DMA_InitStruct.DMA_Mode =DMA_Mode_Normal ;//�Ƿ��Զ���װ
    DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;//����վ�����ַ
    DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte ;//�������ݿ��
    DMA_InitStruct.DMA_PeripheralInc =DMA_PeripheralInc_Enable ;//��ַ�Ƿ�����
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;//ָ��ͨ�������ȼ�

    DMA_Init(DMA1_Channel1,&DMA_InitStruct);
    
    DMA_Cmd(DMA1_Channel1,DISABLE);//ʹ��DMA1��ͨ��1
}

void MyDMA_Transfer(void)
{
    DMA_Cmd(DMA1_Channel1,DISABLE);//ʧ��DMA
    DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size);//���¸������������ֵ
    DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��DMA
    
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//����־λ �ȴ�ת�����
    
    DMA_ClearFlag(DMA1_FLAG_TC1);//�����־λ
}
