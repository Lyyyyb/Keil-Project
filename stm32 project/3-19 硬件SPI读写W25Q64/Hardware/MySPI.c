#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)//�ӻ�ѡ��
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);//��BitValueǿתΪBitActionö������
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��GPIOA��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//��SPI1��ʱ��
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//SS���������ģ�⣬����Ϊͨ���������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;//SCK��MOSIΪ������Ƶ����������Ϊ�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//MISOΪӲ������������źţ�����Ϊ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//������Ԥ��Ƶ������������SCKʱ�ӵ�Ƶ��
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//ʱ����λ ѡ�����ڵ�һ�����ػ��ǵڶ������ز��� CPOL��CPHA��������SPIģʽ
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//ʱ�Ӽ��� SCKĬ���Ǹߵ�ƽ���ǵ͵�ƽ
    SPI_InitStruct.SPI_CRCPolynomial = 7;//CRCУ��Ķ���ʽ
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//����8λ/16λ����֡
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����SPI�ü����� ����Ϊ˫��ȫ˫��
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//���ø�λ���л��ǵ�λ����
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//ѡ��SPI��ģʽ ָ����ǰ�豸��SPI���������Ǵӻ�
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(SPI1,&SPI_InitStruct);//SPI��ʼ��
    
    SPI_Cmd(SPI1,ENABLE);//ʹ��SPI1
    
    MySPI_W_SS(1);//Ĭ�ϸ�SS����ߵ�ƽ����ѡ�дӻ�

}
void MySPI_Start(void)//��ʼ����
{
    MySPI_W_SS(0);//SS�Ӹߵ�ƽת�����͵�ƽ
}
void MySPI_Stop(void)//��ֹ����
{
    MySPI_W_SS(1);//SS�ӵ͵�ƽת�����ߵ�ƽ
}
uint8_t MySPI_SwapByte(uint8_t ByteSend)//����һ���ֽ� ByteSend�����Ǵ������Ĳ�����Ҫͨ������һ���ֽڵ�ʱ���ͳ�ȥ������ֵ��ByteReceive����ͨ������һ���ֽڽ��յ������ݡ�
{
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);//�ж�TXE��־λ���ȴ�TXEΪ1�����ͼĴ����� 
    //�����ֶ����TXE��־λ��д��SPI_DRsʱ�����Զ������־λ
    SPI_I2S_SendData(SPI1,ByteSend);//д�����ݣ���ByteSendд��TDR��֮��ByteSend�Զ�ת����λ�Ĵ���
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);//�ж�RXNE��־λ���ȴ�RXNEΪ1�����ռĴ����ǿգ���ʾ�յ���һ���ֽڣ�ͬʱҲ��ʾ����ʱ��������
    //�����ֶ����RXNE��־λ����ȡSPI_DRʱ�����Զ������־λ
    return SPI_I2S_ReceiveData(SPI1);//��ByteSend�û����������ݣ���Ϊ����ֵ�����ȥ
}
