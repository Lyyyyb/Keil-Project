#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "Delay.h"
#include "Oled.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)//��ֹ��ѭ��
{
    uint32_t Timeout;
    Timeout = 10000;
    while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
    {
        Timeout--;
        if(Timeout==0)
        {
            break;
        }
    }
}
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)//ָ����ַд 8λ�ӻ���ַ 8λ����
{
    I2C_GenerateSTART(I2C2,ENABLE);//������ʼ������Ӳ��I2C������������ʽ�ĺ�����û����ʱ���������ں�������֮�����Ƕ�Ҫ�ȴ���Ӧ�ı�־λ����
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�EV5�¼�������ģʽ��ѡ�� ����ģʽѡ�� STM32Ĭ��Ϊ�ӻ���������ʼ�������Ϊ������

    I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//���ʹӻ���ַ �� ��дλ
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//�ȴ�EV6�¼� ����ģʽ��ѡ��
    
    I2C_SendData(I2C2,RegAddress);//����ָ���Ĵ�����ַ 
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);//�ȴ�EV8�¼� �ֽ����ڷ���
    
    I2C_SendData(I2C2,Data);//��������(���������һ���ֽ�)
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//�ȴ�EV8_2 �ֽ��Ѿ��������
    
    I2C_GenerateSTOP(I2C2,ENABLE);//������ֹ����
}
uint8_t MPU6050_ReadReg(uint8_t RegAddress)//ָ����ַ��
{
    uint8_t Data;
    
    I2C_GenerateSTART(I2C2,ENABLE);//������ʼ������Ӳ��I2C������������ʽ�ĺ�����û����ʱ���������ں�������֮�����Ƕ�Ҫ�ȴ���Ӧ�ı�־λ����
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�EV5�¼�������ģʽ��ѡ�� ����ģʽѡ�� STM32Ĭ��Ϊ�ӻ���������ʼ�������Ϊ������

    I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//���ʹӻ���ַ �� ��дλ д
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//�ȴ�EV6�¼� ����ģʽ��ѡ��
    
    I2C_SendData(I2C2,RegAddress);//����ָ���Ĵ�����ַ 
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//�ȴ�EV8�¼� �ֽڷ������
    
    I2C_GenerateSTART(I2C2,ENABLE);//�ظ���ʼ����
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�EV5�¼�������ģʽ��ѡ�� ����ģʽѡ�� STM32Ĭ��Ϊ�ӻ���������ʼ�������Ϊ������
    
    I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);//���ʹӻ���ַ �� ��дλ ��
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//�ȴ�EV6�¼� ����ģʽ��ѡ��
    
    I2C_AcknowledgeConfig(I2C2,DISABLE);//����ACK=0 ����Ӧ��
    I2C_GenerateSTOP(I2C2,ENABLE);//������ֹ����
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED);//�ȴ�EV7�¼�  
    
    Data = I2C_ReceiveData(I2C2);
    
    I2C_AcknowledgeConfig(I2C2,ENABLE);

    return Data;
}
void MPU6050_Init(void)
{
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);//����I2C2��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����GPIOB��ʱ��
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;//���ÿ�©���(��GPIO�Ŀ���Ȩ����Ӳ������)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��GPIOB
    
    
//    MyI2C_Init();  
    
    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_Ack =I2C_Ack_Enable ;//ACKӦ��λ����
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;//ָ��STM32��Ϊ�ӻ���������Ӧ��λ�ĵ�ַ����Ӧ7λ��10λ
    I2C_InitStruct.I2C_ClockSpeed = 100000;//SCL��ʱ��Ƶ�� 50KHZ
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_16_9;//ʱ��ռ�ձ� 2��1 �͵�ƽ�¼��͸ߵ�ƽ�¼���2��1�ı�����ϵ
    I2C_InitStruct.I2C_Mode =I2C_Mode_I2C ;//I2C��ģʽ
    I2C_InitStruct.I2C_OwnAddress1 = 0x00 ;//�����ַ
    I2C_Init(I2C2,&I2C_InitStruct);

    
    I2C_Cmd(I2C2,ENABLE);
    
    
    //��ʼ������
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);//��Դ����Ĵ���1 ���˯�� ѡ��������ʱ��
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);//��Դ����Ĵ���2 6�����������
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);//�����ʷ�Ƶ ������ƵΪ10
    MPU6050_WriteReg(MPU6050_CONFIG,0x06);//���üĴ��� �˲��������
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);//���������üĴ��� ѡ���������
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//���ٶȼ����üĴ��� ѡ���������
    //������֮��MPU6050�ڲ������������ϵؽ�������ת���ˣ���������ݾʹ�������ݼĴ�����

}
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);    
}
void MPU6050_GetData(int16_t * AccX,int16_t * AccY,int16_t * AccZ,//��ȡ���ݼĴ���
                     int16_t * GyroX,int16_t * GyroY,int16_t * GyroZ)//ͨ��ָ�룬�������������ĵ�ַ���ݵ��Ӻ��������Ӻ����У�ͨ�����ݹ����ĵ�ַ�������������ı������������Ӻ���������������������ֵ�������Ӻ�����Ҫ���ص�ֵ
{
    uint16_t DataH,DataL;
    //������I2C��ȡ����ֽ��Ż�
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//��8λ����
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);//��8λ����
    *AccX = (DataH<<8) | DataL;//��8λ����8λ�ٻ��ϵ�8λ���õ����ٶȼ�X���16λ���ݣ��ٰѶ���������ͨ��ָ�뷵�ػ�ȥ
    //���16λ������һ���ò����ʾ���з�����
    
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH<<8) | DataL;
    
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH<<8) | DataL;
    
    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH<<8) | DataL;
    
    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH<<8) | DataL;
    
    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH<<8) | DataL;
}
