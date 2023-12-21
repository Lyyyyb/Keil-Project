#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//����USART1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIOAʱ��

    GPIO_InitTypeDef GPIO_InitStructure;//��ʼ��GPIO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//USART1��TX
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;//������
    USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;//Ӳ�������� ��ʹ������
    USART_InitStruct.USART_Mode =USART_Mode_Tx ;//����ģʽ ����
    USART_InitStruct.USART_Parity = USART_Parity_No;//У��λ
    USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλ
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;//�ֳ�
    USART_Init(USART1,&USART_InitStruct);
    
    USART_Cmd(USART1,ENABLE);//ʹ��USART1
}

void Serial_SendByte(uint8_t Byte)//��������
{
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ�TDR�е�����ת����λ�Ĵ���
}
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
    uint16_t i;
    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Array[i]);//�������� ��ӡ����
    }
}
void Serial_SendString(char *String)//char *String == char String []
{
    uint8_t i ;
    for(i=0;String[i]!='\0';i++)
    {
       Serial_SendByte(String[i]);//�ַ����Դ�������־λ��\0��
    }

}
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
    uint32_t Result = 1;
    while(Y--)
    {
        Result *=X;//�η�
        
    }
    return Result;


}
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
    uint8_t i ;
    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');//12345/10000%10=1   12345/1000%10 =2 12345/100%10=3
        //����Ҫ���ַ���ʽ��ʾ����Ҫ�ټ�һ��ƫ�ƣ�0x30 ���ַ�0��Ӧ�ı���
    }
}
int fputc(int ch,FILE *f)
{
    Serial_SendByte(ch);
    return ch;

}
void Serial_Printf(char*format,...)//�ɱ����
{
    char String[100];
    va_list arg;
    va_start(arg,format);
    vsprintf(String,format,arg);
    va_end(arg);
    Serial_SendString(String);
}
