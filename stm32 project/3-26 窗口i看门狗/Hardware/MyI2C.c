#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//直接定义函数，对操作端口的库函数进行封装
//1.方便后期加延时
//2.提高可读性，方便代码移植
void MyI2C_W_SCL(uint8_t BitValue)//SCL写
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);//这里要强转为BitAction（枚举类型）
    Delay_us(10);
}
void MyI2C_W_SDA(uint8_t BitValue)//SDA写
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);//设置引脚输出
    Delay_us(10);
}
uint8_t MyI2C_R_SDA(void)//SDA读
{
    uint8_t BitValue;
    BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);//读取GPIO口的状态
    Delay_us(10);
    return BitValue;
}

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启GPIOB时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//开漏输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//释放总线 SCL和SDA处于高电平 此时I2C总线处于空闲状态
}

void MyI2C_Start(void)//起始条件
{
    MyI2C_W_SDA(1);//先释放SDA再释放SCL 防止误判为终止条件(如果SCL一开始是低电平，那么先释放SCL再释放SDA就是终止条件)   
    MyI2C_W_SCL(1);
    
    MyI2C_W_SDA(0);//先拉低SDA再拉低SCL
    MyI2C_W_SCL(0);
}
void MyI2C_Stop(void)//终止条件
{
    MyI2C_W_SDA(0);//先拉低SDA 确保之后释放SDA能产生下降沿（由于除了终止条件，SCL以高电平结束，所有的时序单元都会以SCL低电平结束，以方便各单元的拼接)所以直接拉低SDA即可
    MyI2C_W_SCL(1);//先拉高SCL再拉高SDA
    MyI2C_W_SDA(1);   
}
void MyI2C_SendByte(uint8_t Byte)//发送一个字节
{
    uint8_t i;
    for(i=0;i<8;i++)//一共操作8次 写入一个字节
    {
        MyI2C_W_SDA(Byte & (0x80>>i));//右移+按位与 取出对应位置的值
        MyI2C_W_SCL(1);//先释放SCL再拉低SCL 驱动时钟走一个脉冲 释放SCL之后，从机就会立刻把刚才放在SDA的数据读走
        MyI2C_W_SCL(0);
    }
}
uint8_t MyI2C_ReceiveByte()//接收一个字节
{
    uint8_t Byte = 0x00;
    uint8_t i ;
    MyI2C_W_SDA(1);//为了防止主机干扰从机写入数据，主机需要先释放SDA，释放SDA也相当于切换为输入模式，在SCL低电平时，从机会把数据放到SDA，如果从机想发1，就释放SDA，想发0，就拉低SDA
    for(i=0;i<8;i++)//重复八次，主机就能读到一个字节了
    {        
        MyI2C_W_SCL(1);//主机释放SCL，在SCL高电平期间，读取SDA
        if (MyI2C_R_SDA()==1)//当主机不断驱动SCL时钟时，从机会去改变SDA的电平，这个读取到的数据是从机控制的（想要给我们发送的数据）
        {
            Byte |=(0x80>>i);
        }
        MyI2C_W_SCL(0);//再拉低SCL，低电平期间，从机就会把下一位数据放到SDA上       
    }
    return Byte;
}
void MyI2C_SendAck(uint8_t AckBit)//发送应答位
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}
uint8_t MyI2C_ReceiveAck()//接收应答位
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);       
    return AckBit;
}