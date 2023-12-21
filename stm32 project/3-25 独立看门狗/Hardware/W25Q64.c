#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"                 //指令码宏定义，增加可读性

void W25Q64_Init(void)
{
    MySPI_Init();
}
void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)//读取ID号 用指针实现多返回值
{
    MySPI_Start();//开始条件
    MySPI_SwapByte(W25Q64_JEDEC_ID);//发送读取ID号的指令码
    *MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//用无用数据置换回厂商ID
    *DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//用无用数据置换回设备ID   设备ID高8位，表示存储器类型；低8位，表示容量
    *DID <<= 8;//把第一次读到的设备ID高八位移到DID的高8位去
    *DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);//将高8位和低8位的数据拼接起来，得到完整16位设备ID
    MySPI_Stop();//终止条件
}

void W25Q64_WriteEnable(void)//写使能
{
    MySPI_Start();//开始条件
    MySPI_SwapByte(W25Q64_WRITE_ENABLE);//发送写使能的指令码
    MySPI_Stop();//终止条件
}
void W25Q64_WaitBusy(void)//等待BUSY为0（芯片结束忙状态）读取状态寄存器1，判断芯片是否处于忙状态，状态寄存器可以连续读出，方便我们等待
{
    uint32_t Timeout;//防止超时
    MySPI_Start();//开始条件
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);//发送读取状态寄存器1的指令码
    Timeout = 100000;
    while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01)==0x01)//用掩码取出最低位，读取状态寄存器1的最低位BUSY，判断其是否为1，1表示芯片处于忙状态，0表示芯片结束忙状态。
    {
        Timeout --;
        if (Timeout==0)
        {
            break;//超时等待，跳出循环
        }
    }
    MySPI_Stop();//终止条件
}
void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count)//页编程 通过指针传递数组
{
    uint16_t i;
    
    W25Q64_WriteEnable();//写入操作前，必须先进行写使能，写使能仅对之后跟随的一条时序有效，一条时序结束后，写失能。
    
    MySPI_Start();//开始条件
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);//发送页编程的指令码 地址0x12 34 56
    MySPI_SwapByte(Address>>16);//Address右移16位，发送最高字节的地址 0x12
    MySPI_SwapByte(Address>>8);//Address0右移8位，发送次高位字节的地址，由于交换字节函数只能接收8位数据，所以高位舍弃，实际发送0x34，就是中间的字节 0x34
    MySPI_SwapByte(Address);//舍弃高位，就是最低位的字节 0x56
    for(i=0;i<Count;i++)
    {
        MySPI_SwapByte(DataArray[i]);//发送写入的数据
    }
    MySPI_Stop();//终止条件
    
    W25Q64_WaitBusy();//事后等待，等芯片结束忙状态再退出。因为写入操作结束后，芯片进入忙状态，不响应新的读写操作。
}
void W25Q64_SectorErace(uint32_t Address)//扇区擦除 擦除指定地址所在的整个扇区
{
    W25Q64_WriteEnable();//写入操作前，必须先进行写使能，写使能仅对之后跟随的一条时序有效，一条时序结束后，写失能。保证函数结束后，芯片不处于忙状态。
    
    MySPI_Start();//开始条件
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);//发送扇区擦除的指令码
    MySPI_SwapByte(Address>>16);
    MySPI_SwapByte(Address>>8);
    MySPI_SwapByte(Address);
    MySPI_Stop();//终止条件
    
    W25Q64_WaitBusy();//事后等待，等芯片结束忙状态再退出。因为写入操作结束后，芯片进入忙状态，不响应新的读写操作。保证函数结束后，芯片不处于忙状态。
}
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count)//读取数据
{
    uint32_t i;
    MySPI_Start();//开始条件
    MySPI_SwapByte(W25Q64_READ_DATA);//发送读取数据的指令码
    MySPI_SwapByte(Address>>16);
    MySPI_SwapByte(Address>>8);
    MySPI_SwapByte(Address);
    for(i=0;i<Count;i++)
    {
        DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//主机用无用数据置换回从机发送的数据 并放到数组的对应位
    }
    MySPI_Stop();//终止条件
}
