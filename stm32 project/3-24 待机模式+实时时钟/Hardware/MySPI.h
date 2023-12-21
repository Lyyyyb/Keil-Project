#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_Init(void);//SPI初始化
void MySPI_Start(void);//起始条件
void MySPI_Stop(void);//终止条件
uint8_t MySPI_SwapByte(uint8_t ByteSend);//交换一个字节


#endif
