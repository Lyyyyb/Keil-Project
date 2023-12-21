#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;//波特率
    USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;//硬件流控制
    USART_InitStruct.USART_Mode =USART_Mode_Tx|USART_Mode_Rx ;//同时开启发送和接收
    USART_InitStruct.USART_Parity = USART_Parity_No;//校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长
    USART_Init(USART1,&USART_InitStruct);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启RXNE标志位到NVIC的输出
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组
    NVIC_InitTypeDef NVIC_InitStruct;//初始化NVIC
    NVIC_InitStruct.NVIC_IRQChannel =USART1_IRQn ;//中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//优先级
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
uint8_t Serial_GetRxFlag(void)
{
    if(Serial_RxFlag==1)//手动设置标志位，表示已经读取了DR
    {
        Serial_RxFlag=0;
        return 1;
    }
    return 0;

}
uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;

}
void USART1_IRQHandler(void)//中断函数
{
    if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//判断标志位
    {
        Serial_RxData=USART_ReceiveData(USART1);//读取DR
        Serial_RxFlag=1;
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除标志位
    
    }
}
