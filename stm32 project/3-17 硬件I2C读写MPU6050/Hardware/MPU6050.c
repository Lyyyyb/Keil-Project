#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "Delay.h"
#include "Oled.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)//防止死循环
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
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)//指定地址写 8位从机地址 8位数据
{
    I2C_GenerateSTART(I2C2,ENABLE);//生成起始条件（硬件I2C函数不是阻塞式的函数（没有延时），所以在函数结束之后，我们都要等待相应的标志位，）
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件（主机模式已选择） 主机模式选择 STM32默认为从机，发送起始条件后变为主机，

    I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//发送从机地址 和 读写位
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//等待EV6事件 发送模式已选择
    
    I2C_SendData(I2C2,RegAddress);//发送指定寄存器地址 
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);//等待EV8事件 字节正在发送
    
    I2C_SendData(I2C2,Data);//发送数据(发送完最后一个字节)
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//等待EV8_2 字节已经发送完毕
    
    I2C_GenerateSTOP(I2C2,ENABLE);//生成终止条件
}
uint8_t MPU6050_ReadReg(uint8_t RegAddress)//指定地址读
{
    uint8_t Data;
    
    I2C_GenerateSTART(I2C2,ENABLE);//生成起始条件（硬件I2C函数不是阻塞式的函数（没有延时），所以在函数结束之后，我们都要等待相应的标志位，）
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件（主机模式已选择） 主机模式选择 STM32默认为从机，发送起始条件后变为主机，

    I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//发送从机地址 和 读写位 写
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//等待EV6事件 发送模式已选择
    
    I2C_SendData(I2C2,RegAddress);//发送指定寄存器地址 
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//等待EV8事件 字节发送完成
    
    I2C_GenerateSTART(I2C2,ENABLE);//重复起始条件
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件（主机模式已选择） 主机模式选择 STM32默认为从机，发送起始条件后变为主机，
    
    I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);//发送从机地址 和 读写位 读
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//等待EV6事件 接受模式已选择
    
    I2C_AcknowledgeConfig(I2C2,DISABLE);//设置ACK=0 不给应答
    I2C_GenerateSTOP(I2C2,ENABLE);//产生终止条件
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED);//等待EV7事件  
    
    Data = I2C_ReceiveData(I2C2);
    
    I2C_AcknowledgeConfig(I2C2,ENABLE);

    return Data;
}
void MPU6050_Init(void)
{
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);//开启I2C2的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启GPIOB的时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;//复用开漏输出(将GPIO的控制权交给硬件外设)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIOB
    
    
//    MyI2C_Init();  
    
    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_Ack =I2C_Ack_Enable ;//ACK应答位配置
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;//指定STM32作为从机，可以相应几位的地址，响应7位或10位
    I2C_InitStruct.I2C_ClockSpeed = 100000;//SCL的时钟频率 50KHZ
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_16_9;//时钟占空比 2：1 低电平事件和高电平事件是2：1的比例关系
    I2C_InitStruct.I2C_Mode =I2C_Mode_I2C ;//I2C的模式
    I2C_InitStruct.I2C_OwnAddress1 = 0x00 ;//自身地址
    I2C_Init(I2C2,&I2C_InitStruct);

    
    I2C_Cmd(I2C2,ENABLE);
    
    
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
