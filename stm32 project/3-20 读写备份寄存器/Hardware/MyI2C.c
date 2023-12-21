#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//ֱ�Ӷ��庯�����Բ����˿ڵĿ⺯�����з�װ
//1.������ڼ���ʱ
//2.��߿ɶ��ԣ����������ֲ
void MyI2C_W_SCL(uint8_t BitValue)//SCLд
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);//����ҪǿתΪBitAction��ö�����ͣ�
    Delay_us(10);
}
void MyI2C_W_SDA(uint8_t BitValue)//SDAд
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);//�����������
    Delay_us(10);
}
uint8_t MyI2C_R_SDA(void)//SDA��
{
    uint8_t BitValue;
    BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);//��ȡGPIO�ڵ�״̬
    Delay_us(10);
    return BitValue;
}

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����GPIOBʱ��
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//��©���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//�ͷ����� SCL��SDA���ڸߵ�ƽ ��ʱI2C���ߴ��ڿ���״̬
}

void MyI2C_Start(void)//��ʼ����
{
    MyI2C_W_SDA(1);//���ͷ�SDA���ͷ�SCL ��ֹ����Ϊ��ֹ����(���SCLһ��ʼ�ǵ͵�ƽ����ô���ͷ�SCL���ͷ�SDA������ֹ����)   
    MyI2C_W_SCL(1);
    
    MyI2C_W_SDA(0);//������SDA������SCL
    MyI2C_W_SCL(0);
}
void MyI2C_Stop(void)//��ֹ����
{
    MyI2C_W_SDA(0);//������SDA ȷ��֮���ͷ�SDA�ܲ����½��أ����ڳ�����ֹ������SCL�Ըߵ�ƽ���������е�ʱ��Ԫ������SCL�͵�ƽ�������Է������Ԫ��ƴ��)����ֱ������SDA����
    MyI2C_W_SCL(1);//������SCL������SDA
    MyI2C_W_SDA(1);   
}
void MyI2C_SendByte(uint8_t Byte)//����һ���ֽ�
{
    uint8_t i;
    for(i=0;i<8;i++)//һ������8�� д��һ���ֽ�
    {
        MyI2C_W_SDA(Byte & (0x80>>i));//����+��λ�� ȡ����Ӧλ�õ�ֵ
        MyI2C_W_SCL(1);//���ͷ�SCL������SCL ����ʱ����һ������ �ͷ�SCL֮�󣬴ӻ��ͻ����̰Ѹղŷ���SDA�����ݶ���
        MyI2C_W_SCL(0);
    }
}
uint8_t MyI2C_ReceiveByte()//����һ���ֽ�
{
    uint8_t Byte = 0x00;
    uint8_t i ;
    MyI2C_W_SDA(1);//Ϊ�˷�ֹ�������Ŵӻ�д�����ݣ�������Ҫ���ͷ�SDA���ͷ�SDAҲ�൱���л�Ϊ����ģʽ����SCL�͵�ƽʱ���ӻ�������ݷŵ�SDA������ӻ��뷢1�����ͷ�SDA���뷢0��������SDA
    for(i=0;i<8;i++)//�ظ��˴Σ��������ܶ���һ���ֽ���
    {        
        MyI2C_W_SCL(1);//�����ͷ�SCL����SCL�ߵ�ƽ�ڼ䣬��ȡSDA
        if (MyI2C_R_SDA()==1)//��������������SCLʱ��ʱ���ӻ���ȥ�ı�SDA�ĵ�ƽ�������ȡ���������Ǵӻ����Ƶģ���Ҫ�����Ƿ��͵����ݣ�
        {
            Byte |=(0x80>>i);
        }
        MyI2C_W_SCL(0);//������SCL���͵�ƽ�ڼ䣬�ӻ��ͻ����һλ���ݷŵ�SDA��       
    }
    return Byte;
}
void MyI2C_SendAck(uint8_t AckBit)//����Ӧ��λ
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}
uint8_t MyI2C_ReceiveAck()//����Ӧ��λ
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);       
    return AckBit;
}