#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[]={2023,1,1,23,59,55};//�� �� �� ʱ �� ��

void MyRTC_SetTime(void);
void MyRTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//����PWR��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//����BKP��ʱ��
    
    PWR_BackupAccessCmd(ENABLE);//ʹ�ܶԺ�����BKP��RTC���ķ���
    
    if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5)//��ȡBKP�Ĵ������ж��Ƿ�Ϊ���㣨�Ƿ�����Լ�д������ݣ�������ʵ��RTCʱ�Ӹ�λ������ʱ��
    {
        RCC_LSEConfig(RCC_LSE_ON);//����LSEʱ��

        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!= SET);//�ȴ�LSEʱ���������

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��RTCCLKʱ��Դ

        RCC_RTCCLKCmd(ENABLE);//ʹ��RTCCLK
        
        RTC_WaitForSynchro();//�ȴ�ͬ��
        RTC_WaitForLastTask();//�ȴ���һ��д��������
        
        RTC_SetPrescaler(32768-1);//����Ԥ��Ƶ�����⺯���Լ�����ý�������ģʽ�Ĵ��벢��Ҳ�Դ��˳�����ģʽ�Ĵ���

        RTC_WaitForLastTask();//�ȴ���һ��д��������
        
        MyRTC_SetTime();//�Ѹ�����ʱ��д��RTC
    
        BKP_WriteBackupRegister(BKP_DR1,0xA5A5);//д��BKP�Ĵ��� 0xA5A5
    }
    else
    {
        RTC_WaitForSynchro();//�ȴ�ͬ��
        RTC_WaitForLastTask();//�ȴ���һ��д�������� 
    }
}
void MyRTC_SetTime(void)//����ʱ�䣬�������ʱ�䣬ת��Ϊ������д�뵽CNT��
{
    time_t time_cnt;
    struct tm time_date;
    time_date.tm_year = MyRTC_Time[0]-1900;
    time_date.tm_mon = MyRTC_Time[1]-1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min = MyRTC_Time[4];
    time_date.tm_sec = MyRTC_Time[5];
    
    time_cnt=mktime(&time_date)-8*60*60;//������ʱ��ת��Ϊ���������д��ʱ��ʱ����ȥ8��Сʱ����������Ϊ��ȡ��ʱ�������8��Сʱ������ƫ�ơ�
    
    RTC_SetCounter(time_cnt);//д��CNT�����������ó�ʼʱ��
    RTC_WaitForLastTask();//�ȴ���һ�β���д�����
}
void MyRTC_ReadTime(void)//��ȡʱ��
{
    time_t time_cnt;
    struct tm time_date;
    
    time_cnt = RTC_GetCounter()+8*60*60;//��ȡCNT��ֵ������8��Сʱ������ƫ�ƣ�������ʱ��
    
    time_date =* localtime(&time_cnt);//���������ת��Ϊ����ʱ��
    
    MyRTC_Time[0]=time_date.tm_year+1900;
    MyRTC_Time[1]=time_date.tm_mon+1;
    MyRTC_Time[2]=time_date.tm_mday;
    MyRTC_Time[3]=time_date.tm_hour;
    MyRTC_Time[4]=time_date.tm_min;
    MyRTC_Time[5]=time_date.tm_sec;
}
