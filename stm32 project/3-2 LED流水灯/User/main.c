#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //��GPIOA��ʱ��
    
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //��ʼ����Ա���� �������
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ; //��©���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    //GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;//ͨ����λ�� ͬʱѡ����3���˿� 16����ת����2���� 16λ ÿһλ��Ӧһ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
 	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ�� GPIO
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0); //����ĳһλ ��0
	//GPIO_SetBits(GPIOA,GPIO_Pin_0); //��ĳһλ����1
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET); 
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
    while(1)
    {
      
//		//GPIO_WriteBit(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 ,Bit_RESET);//ͨ����λ�� ͬʱѡ�����˿�
//        Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3,Bit_SET);
//        Delay_ms(100);
        GPIO_Write(GPIOA,~0x0002);//0000 0000 0000 0010 A1�� led���� ����Ϩ��
        Delay_ms(500);
        GPIO_Write(GPIOA,~0x0004);//0000 0000 0000 0100 A2�� led���� ����Ϩ��
        Delay_ms(500);
        GPIO_Write(GPIOA,~0x0008);//0000 0000 0000 1000 A3�� led���� ����Ϩ��
        Delay_ms(500);
    }
}

    