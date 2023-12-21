#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //��GPIOA��ʱ��
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //��ʼ����Ա���� �������
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ; //��©���
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
 	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ�� GPIO
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0); //����ĳһλ ��0
	//GPIO_SetBits(GPIOA,GPIO_Pin_0); //��ĳһλ����1
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET); 
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
    while(1)
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        Delay_ms(100);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
//        Delay_ms(100);
//        //GPIO_WriteBit(GPIOA,GPIO_Pin_0,0); �ᱨ���� �����˱��ö������ 
//        Delay_ms(100);
//        GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)1);//��Ҫ����ǿ������ת��
//        Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
//        Delay_ms(100);
    }
}

    