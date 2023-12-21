#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA时钟

    GPIO_InitTypeDef GPIO_InitStructure;//初始化GPIO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//USART1的TX
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;//波特率
    USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;//硬件流控制 不使用流控
    USART_InitStruct.USART_Mode =USART_Mode_Tx ;//串口模式 发送
    USART_InitStruct.USART_Parity = USART_Parity_No;//校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长
    USART_Init(USART1,&USART_InitStruct);
    
    USART_Cmd(USART1,ENABLE);//使能USART1
}

void Serial_SendByte(uint8_t Byte)//发送数据
{
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//等待TDR中的数据转入移位寄存器
}
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
    uint16_t i;
    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Array[i]);//遍历数组 打印数组
    }
}
void Serial_SendString(char *String)//char *String == char String []
{
    uint8_t i ;
    for(i=0;String[i]!='\0';i++)
    {
       Serial_SendByte(String[i]);//字符串自带结束标志位’\0‘
    }

}
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
    uint32_t Result = 1;
    while(Y--)
    {
        Result *=X;//次方
        
    }
    return Result;


}
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
    uint8_t i ;
    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');//12345/10000%10=1   12345/1000%10 =2 12345/100%10=3
        //由于要以字符形式显示，还要再加一个偏移，0x30 即字符0对应的编码
    }
}
int fputc(int ch,FILE *f)
{
    Serial_SendByte(ch);
    return ch;

}
void Serial_Printf(char*format,...)//可变参数
{
    char String[100];
    va_list arg;
    va_start(arg,format);
    vsprintf(String,format,arg);
    va_end(arg);
    Serial_SendString(String);
}
