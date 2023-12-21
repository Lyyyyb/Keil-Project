#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "MyI2C.h"
#define MPU6050_ADDRESS 0xD0
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)//指定地址写 8位从机地址 8位数据
{
    MyI2C_Start();//起始条件
    MyI2C_SendByte(MPU6050_ADDRESS);//主机寻址 0xD0 （从机地址7位+起始位1位）8位
    MyI2C_ReceiveAck();//主机接收应答
    MyI2C_SendByte(RegAddress);//指定寄存器地址
    MyI2C_ReceiveAck();//接收应答
    MyI2C_SendByte(Data);//写入指定寄存器地址下的数据  可以for循环多次 发送多个字节
    MyI2C_ReceiveAck();//接收应答
    MyI2C_Stop();//终止条件
}
uint8_t MPU6050_ReadReg(uint8_t RegAddress)//指定地址读
{
    uint8_t Data;
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);//指定地址 就是设置了MPU6050的当前地址指针
    MyI2C_ReceiveAck();
    
    MyI2C_Start();//重复起始条件
    MyI2C_SendByte(MPU6050_ADDRESS|0x01);//或上0x01，读写位为1
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();//可以for循环多次 接收多个字节 但是读完最后一次要给非应答
    MyI2C_SendAck(1);//给从机应答 1不给应答 0给应答 从机如果接收到应答就会继续发送数据 
    MyI2C_Stop();
    
    return Data;
}
void MPU6050_Init(void)
{
    MyI2C_Init();
    //初始化配置
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);//电源管理寄存器1 解除睡眠 选择陀螺仪时钟
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);//电源管理寄存器2 6个轴均不待机
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);//采样率分频 采样分频为10
    MPU6050_WriteReg(MPU6050_CONFIG,0x06);//配置寄存器 滤波参数最大
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);//陀螺仪配置寄存器 选择最大量程
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//加速度计配置寄存器 选择最大量程
    //配置完之后，MPU6050内部就在连续不断地进行数据转换了，输出的数据就存放在数据寄存器中
}
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
void MPU6050_GetData(int16_t * AccX,int16_t * AccY,int16_t * AccZ,//获取数据寄存器
                     int16_t * GyroX,int16_t * GyroY,int16_t * GyroZ)//通过指针，把主函数变量的地址传递到子函数来。子函数中，通过传递过来的地址，操作主函数的变量，这样，子函数结束后，主函数变量的值，就是子函数想要返回的值
{
    uint16_t DataH,DataL;
    //可以用I2C读取多个字节优化
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);//低8位数据
    *AccX = (DataH<<8) | DataL;//高8位左移8位再或上低8位，得到加速度计X轴的16位数据，再把读到的数据通过指针返回回去
    //这个16位数据是一个用补码表示的有符号数
    
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
