#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[]={2023,1,1,23,59,55};//年 月 日 时 分 秒

void MyRTC_SetTime(void);
void MyRTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//开启PWR的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//开启BKP的时钟
    
    PWR_BackupAccessCmd(ENABLE);//使能对后备区域（BKP和RTC）的访问
    
    if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5)//读取BKP寄存器，判断是否为清零（是否等于自己写入的数据），用于实现RTC时钟复位不重置时间
    {
        RCC_LSEConfig(RCC_LSE_ON);//启动LSE时钟

        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!= SET);//等待LSE时钟启动完成

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择RTCCLK时钟源

        RCC_RTCCLKCmd(ENABLE);//使能RTCCLK
        
        RTC_WaitForSynchro();//等待同步
        RTC_WaitForLastTask();//等待上一次写入操作完成
        
        RTC_SetPrescaler(32768-1);//配置预分频器，库函数自己会调用进入配置模式的代码并且也自带退出配置模式的代码

        RTC_WaitForLastTask();//等待上一次写入操作完成
        
        MyRTC_SetTime();//把给定的时间写入RTC
    
        BKP_WriteBackupRegister(BKP_DR1,0xA5A5);//写入BKP寄存器 0xA5A5
    }
    else
    {
        RTC_WaitForSynchro();//等待同步
        RTC_WaitForLastTask();//等待上一次写入操作完成 
    }
}
void MyRTC_SetTime(void)//设置时间，把数组的时间，转换为秒数，写入到CNT中
{
    time_t time_cnt;
    struct tm time_date;
    time_date.tm_year = MyRTC_Time[0]-1900;
    time_date.tm_mon = MyRTC_Time[1]-1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min = MyRTC_Time[4];
    time_date.tm_sec = MyRTC_Time[5];
    
    time_cnt=mktime(&time_date)-8*60*60;//将日期时间转换为秒计数器，写入时间时，减去8个小时的秒数，因为读取的时候加上了8个小时的秒数偏移。
    
    RTC_SetCounter(time_cnt);//写入CNT计数器，设置初始时间
    RTC_WaitForLastTask();//等待上一次操作写入完成
}
void MyRTC_ReadTime(void)//读取时间
{
    time_t time_cnt;
    struct tm time_date;
    
    time_cnt = RTC_GetCounter()+8*60*60;//读取CNT的值，加上8个小时的秒数偏移，代表北京时间
    
    time_date =* localtime(&time_cnt);//将秒计数器转换为日期时间
    
    MyRTC_Time[0]=time_date.tm_year+1900;
    MyRTC_Time[1]=time_date.tm_mon+1;
    MyRTC_Time[2]=time_date.tm_mday;
    MyRTC_Time[3]=time_date.tm_hour;
    MyRTC_Time[4]=time_date.tm_min;
    MyRTC_Time[5]=time_date.tm_sec;
}
