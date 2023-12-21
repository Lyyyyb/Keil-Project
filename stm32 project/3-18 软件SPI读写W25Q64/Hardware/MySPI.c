#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)//从机选择
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);//将BitValue强转为BitAction枚举类型
}
void MySPI_W_SCK(uint8_t BitValue)//时钟
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);//将BitValue强转为BitAction枚举类型
}
void MySPI_W_MOSI(uint8_t BitValue)//主机输出从机输入
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);//将BitValue强转为BitAction枚举类型
}
uint8_t MySPI_R_MISO(void)//主机输入从机输出
{
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开GPIOA的时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;//将主机的输出引脚（时钟、主机输出和片选）配置为推挽输出 SS-PA4 SCK-PA5 MOSI-PA7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//将主机的输入（主机输入）引脚配置为上拉输入 MISO-PA6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化GPIOA
    
    //配置初始化之后引脚的默认电平
    MySPI_W_SS(1);//SS置高电平，默认不选择从机
    MySPI_W_SCK(0);//使用SPI模式0，所以默认是低电平
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
    uint8_t i,ByteReceive=0x00;
    for(i=0;i<8;i++)
    {
        MySPI_W_MOSI(ByteSend & (0x80>>i));//通过掩码，依次提取数据每一位进行操作
        //主机依次移出对应位的数据到MOSI上 0x80>>i的作用就是用来挑出数据的某一位或者某几位，或者换种描述方式就是，用来屏蔽其他的无关，我们可以把这种类型的数据叫做掩码
        MySPI_W_SCK(1);//SCK产生上升沿时，从机会自动把MOSI的数据读走
        if(MySPI_R_MISO()==1)
        {
            ByteReceive |= (0x80>>i);//主机依次读取从机放到MISO上的数据
        }
        MySPI_W_SCK(0);//SCK产生下降沿，主机和从机移出下一位
    }   
    return ByteReceive;//主机和从机置换数据
}
//uint8_t MySPI_SwapByte(uint8_t ByteSend)//移位模型
//{
//    uint8_t i;
//    for(i=0;i<8;i++)
//    {
//        MySPI_W_MOSI(ByteSend & 0x80);
//        ByteSend <<=1;//左移移位，最低位会自动补0，最低位空缺
//        MySPI_W_SCK(1);
//        if(MySPI_R_MISO()==1)
//        {
//            ByteSend |= 0x01;//把收到的数据放在ByteSend的最低位
//        }
//        MySPI_W_SCK(0);
//    }   
//    return ByteSend;
//}
