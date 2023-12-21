#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)
{
    MyDMA_Size=Size;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//开启DMA1时钟
    
    DMA_InitTypeDef DMA_InitStruct;//初始化DMA结构体
    DMA_InitStruct.DMA_BufferSize = Size;//传输计数器 传输几次
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向 配置外设站点是源地址 还是 目标地址
    DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//选择硬件触发还是软件触发 软件触发 一直有触发信号
    DMA_InitStruct.DMA_MemoryBaseAddr =AddrB ;//存储器站点基地址    
    DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;//存储器数据宽度
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//地址是否自增
    DMA_InitStruct.DMA_Mode =DMA_Mode_Normal ;//是否自动重装
    DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;//外设站点基地址
    DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte ;//外设数据宽度
    DMA_InitStruct.DMA_PeripheralInc =DMA_PeripheralInc_Enable ;//地址是否自增
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;//指定通道的优先级

    DMA_Init(DMA1_Channel1,&DMA_InitStruct);
    
    DMA_Cmd(DMA1_Channel1,DISABLE);//使能DMA1的通道1
}

void MyDMA_Transfer(void)
{
    DMA_Cmd(DMA1_Channel1,DISABLE);//失能DMA
    DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size);//重新给传输计数器赋值
    DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA
    
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//检测标志位 等待转运完成
    
    DMA_ClearFlag(DMA1_FLAG_TC1);//清除标志位
}
