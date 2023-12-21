#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)//从机选择
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);//将BitValue强转为BitAction枚举类型
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//打开SPI1的时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//SS还是用软件模拟，配置为通用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;//SCK和MOSI为外设控制的输出，配置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//MISO为硬件外设的输入信号，配置为上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//波特率预分频器，用来配置SCK时钟的频率
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//时钟相位 选择是在第一个边沿还是第二个边沿采样 CPOL和CPHA用于配置SPI模式
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//时钟极性 SCK默认是高电平还是低电平
    SPI_InitStruct.SPI_CRCPolynomial = 7;//CRC校验的多项式
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//配置8位/16位数据帧
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//配置SPI裁剪引脚 配置为双线全双工
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//配置高位先行还是低位先行
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//选择SPI的模式 指定当前设备是SPI的主机还是从机
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(SPI1,&SPI_InitStruct);//SPI初始化
    
    SPI_Cmd(SPI1,ENABLE);//使能SPI1
    
    MySPI_W_SS(1);//默认给SS输出高电平，不选中从机

}
void MySPI_Start(void)//起始条件
{
    MySPI_W_SS(0);//SS从高电平转换到低电平
}
void MySPI_Stop(void)//终止条件
{
    MySPI_W_SS(1);//SS从低电平转换到高电平
}
uint8_t MySPI_SwapByte(uint8_t ByteSend)//交换一个字节 ByteSend是我们传进来的参数，要通过交换一个字节的时序发送出去，返回值是ByteReceive，是通过交换一个字节接收到的数据。
{
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);//判断TXE标志位，等待TXE为1，发送寄存器空 
    //无需手动清除TXE标志位，写入SPI_DRs时，会自动清除标志位
    SPI_I2S_SendData(SPI1,ByteSend);//写入数据，将ByteSend写入TDR，之后ByteSend自动转入移位寄存器
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);//判断RXNE标志位，等待RXNE为1，接收寄存器非空，表示收到了一个字节，同时也表示发送时序产生完成
    //无需手动清除RXNE标志位，读取SPI_DR时，会自动清除标志位
    return SPI_I2S_ReceiveData(SPI1);//把ByteSend置换回来的数据，作为返回值输出出去
}
