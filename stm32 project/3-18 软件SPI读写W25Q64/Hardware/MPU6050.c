#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "MyI2C.h"
#define MPU6050_ADDRESS 0xD0
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)//ָ����ַд 8λ�ӻ���ַ 8λ����
{
    MyI2C_Start();//��ʼ����
    MyI2C_SendByte(MPU6050_ADDRESS);//����Ѱַ 0xD0 ���ӻ���ַ7λ+��ʼλ1λ��8λ
    MyI2C_ReceiveAck();//��������Ӧ��
    MyI2C_SendByte(RegAddress);//ָ���Ĵ�����ַ
    MyI2C_ReceiveAck();//����Ӧ��
    MyI2C_SendByte(Data);//д��ָ���Ĵ�����ַ�µ�����  ����forѭ����� ���Ͷ���ֽ�
    MyI2C_ReceiveAck();//����Ӧ��
    MyI2C_Stop();//��ֹ����
}
uint8_t MPU6050_ReadReg(uint8_t RegAddress)//ָ����ַ��
{
    uint8_t Data;
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);//ָ����ַ ����������MPU6050�ĵ�ǰ��ַָ��
    MyI2C_ReceiveAck();
    
    MyI2C_Start();//�ظ���ʼ����
    MyI2C_SendByte(MPU6050_ADDRESS|0x01);//����0x01����дλΪ1
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();//����forѭ����� ���ն���ֽ� ���Ƕ������һ��Ҫ����Ӧ��
    MyI2C_SendAck(1);//���ӻ�Ӧ�� 1����Ӧ�� 0��Ӧ�� �ӻ�������յ�Ӧ��ͻ������������ 
    MyI2C_Stop();
    
    return Data;
}
void MPU6050_Init(void)
{
    MyI2C_Init();
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
