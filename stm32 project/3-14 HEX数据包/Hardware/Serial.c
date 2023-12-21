#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


uint8_t Serial_RxFlag;
uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;
    USART_InitStruct.USART_Mode =USART_Mode_Tx|USART_Mode_Rx ;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART_InitStruct);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel =USART1_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
    
    USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
    uint16_t i;
    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Array[i]);
    }
}
void Serial_SendString(char *String)//char *String == char String []
{
    uint8_t i ;
    for(i=0;String[i]!='\0';i++)
    {
       Serial_SendByte(String[i]);
    }

}
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
    uint32_t Result = 1;
    while(Y--)
    {
        Result *=X;
        
    }
    return Result;


}
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
    uint8_t i ;
    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
    }
}
int fputc(int ch,FILE *f)
{
    Serial_SendByte(ch);
    return ch;

}
void Serial_Printf(char*format,...)
{
    char String[100];
    va_list arg;
    va_start(arg,format);
    vsprintf(String,format,arg);
    va_end(arg);
    Serial_SendString(String);
}
void Serial_SendPacket(void)//发送HEX数据包
{
    Serial_SendByte(0xFF);//包头
    Serial_SendArray(Serial_TxPacket,4);//数据位 固定长度
    Serial_SendByte(0xFE);//包尾
}
uint8_t Serial_GetRxFlag(void)
{
    if(Serial_RxFlag==1)//表示收到了包尾
    {
        Serial_RxFlag=0;//清除标志位
        return 1;
    }
    return 0;
}

void USART1_IRQHandler(void)//在我们发送一个数据包时，由于数据是一个字节一个字节接收，读完一个字节的数据之后，下一个字节的数据会立刻进入到数据寄存器，从而不断申请中断
{   //静态变量 函数进入只会初始化一次0，在函数退出后，数据仍然有效，状态机前后关联，所以要用静态变量
    static uint8_t RxState = 0;//状态变量S
    static uint8_t pRxPacket = 0;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);//获取RxDate
        if(RxState==0)//RxState==0 进入等待包头的程序
        {
            if (RxData==0xFF)//收到包头 转移RxState的状态
            {
                RxState=1;
                pRxPacket=0;//个数清零
            }
            else
            {
                RxState=0;
            };
        }
        else if(RxState ==1)//RxState==1 进入接收数据的程序
        {
            Serial_RxPacket[pRxPacket] = RxData;//将数据存入我们创建好的数组
            pRxPacket++;//记录接收了几个数据
            if(pRxPacket>=4)
            {
                RxState=2;
            }
        }
        else if(RxState==2)//RxState 进入等待包尾的程序
        {
            if(RxData==0xFE)//判断是不是包尾 代表数据位接收到了
            {
                RxState=0;
                Serial_RxFlag=1;
            }
        }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除标志位
    }


}
