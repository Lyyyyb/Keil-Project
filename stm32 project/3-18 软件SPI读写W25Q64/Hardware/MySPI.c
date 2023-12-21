#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)//�ӻ�ѡ��
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);//��BitValueǿתΪBitActionö������
}
void MySPI_W_SCK(uint8_t BitValue)//ʱ��
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);//��BitValueǿתΪBitActionö������
}
void MySPI_W_MOSI(uint8_t BitValue)//��������ӻ�����
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);//��BitValueǿתΪBitActionö������
}
uint8_t MySPI_R_MISO(void)//��������ӻ����
{
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��GPIOA��ʱ��
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;//��������������ţ�ʱ�ӡ����������Ƭѡ������Ϊ������� SS-PA4 SCK-PA5 MOSI-PA7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//�����������루�������룩��������Ϊ�������� MISO-PA6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��GPIOA
    
    //���ó�ʼ��֮�����ŵ�Ĭ�ϵ�ƽ
    MySPI_W_SS(1);//SS�øߵ�ƽ��Ĭ�ϲ�ѡ��ӻ�
    MySPI_W_SCK(0);//ʹ��SPIģʽ0������Ĭ���ǵ͵�ƽ
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
    uint8_t i,ByteReceive=0x00;
    for(i=0;i<8;i++)
    {
        MySPI_W_MOSI(ByteSend & (0x80>>i));//ͨ�����룬������ȡ����ÿһλ���в���
        //���������Ƴ���Ӧλ�����ݵ�MOSI�� 0x80>>i�����þ��������������ݵ�ĳһλ����ĳ��λ�����߻���������ʽ���ǣ����������������޹أ����ǿ��԰��������͵����ݽ�������
        MySPI_W_SCK(1);//SCK����������ʱ���ӻ����Զ���MOSI�����ݶ���
        if(MySPI_R_MISO()==1)
        {
            ByteReceive |= (0x80>>i);//�������ζ�ȡ�ӻ��ŵ�MISO�ϵ�����
        }
        MySPI_W_SCK(0);//SCK�����½��أ������ʹӻ��Ƴ���һλ
    }   
    return ByteReceive;//�����ʹӻ��û�����
}
//uint8_t MySPI_SwapByte(uint8_t ByteSend)//��λģ��
//{
//    uint8_t i;
//    for(i=0;i<8;i++)
//    {
//        MySPI_W_MOSI(ByteSend & 0x80);
//        ByteSend <<=1;//������λ�����λ���Զ���0�����λ��ȱ
//        MySPI_W_SCK(1);
//        if(MySPI_R_MISO()==1)
//        {
//            ByteSend |= 0x01;//���յ������ݷ���ByteSend�����λ
//        }
//        MySPI_W_SCK(0);
//    }   
//    return ByteSend;
//}
