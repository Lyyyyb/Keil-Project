#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"                 //ָ����궨�壬���ӿɶ���

void W25Q64_Init(void)
{
    MySPI_Init();
}
void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)//��ȡID�� ��ָ��ʵ�ֶ෵��ֵ
{
    MySPI_Start();//��ʼ����
    MySPI_SwapByte(W25Q64_JEDEC_ID);//���Ͷ�ȡID�ŵ�ָ����
    *MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//�����������û��س���ID
    *DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//�����������û����豸ID   �豸ID��8λ����ʾ�洢�����ͣ���8λ����ʾ����
    *DID <<= 8;//�ѵ�һ�ζ������豸ID�߰�λ�Ƶ�DID�ĸ�8λȥ
    *DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);//����8λ�͵�8λ������ƴ���������õ�����16λ�豸ID
    MySPI_Stop();//��ֹ����
}

void W25Q64_WriteEnable(void)//дʹ��
{
    MySPI_Start();//��ʼ����
    MySPI_SwapByte(W25Q64_WRITE_ENABLE);//����дʹ�ܵ�ָ����
    MySPI_Stop();//��ֹ����
}
void W25Q64_WaitBusy(void)//�ȴ�BUSYΪ0��оƬ����æ״̬����ȡ״̬�Ĵ���1���ж�оƬ�Ƿ���æ״̬��״̬�Ĵ������������������������ǵȴ�
{
    uint32_t Timeout;//��ֹ��ʱ
    MySPI_Start();//��ʼ����
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);//���Ͷ�ȡ״̬�Ĵ���1��ָ����
    Timeout = 100000;
    while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01)==0x01)//������ȡ�����λ����ȡ״̬�Ĵ���1�����λBUSY���ж����Ƿ�Ϊ1��1��ʾоƬ����æ״̬��0��ʾоƬ����æ״̬��
    {
        Timeout --;
        if (Timeout==0)
        {
            break;//��ʱ�ȴ�������ѭ��
        }
    }
    MySPI_Stop();//��ֹ����
}
void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count)//ҳ��� ͨ��ָ�봫������
{
    uint16_t i;
    
    W25Q64_WriteEnable();//д�����ǰ�������Ƚ���дʹ�ܣ�дʹ�ܽ���֮������һ��ʱ����Ч��һ��ʱ�������дʧ�ܡ�
    
    MySPI_Start();//��ʼ����
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);//����ҳ��̵�ָ���� ��ַ0x12 34 56
    MySPI_SwapByte(Address>>16);//Address����16λ����������ֽڵĵ�ַ 0x12
    MySPI_SwapByte(Address>>8);//Address0����8λ�����ʹθ�λ�ֽڵĵ�ַ�����ڽ����ֽں���ֻ�ܽ���8λ���ݣ����Ը�λ������ʵ�ʷ���0x34�������м���ֽ� 0x34
    MySPI_SwapByte(Address);//������λ���������λ���ֽ� 0x56
    for(i=0;i<Count;i++)
    {
        MySPI_SwapByte(DataArray[i]);//����д�������
    }
    MySPI_Stop();//��ֹ����
    
    W25Q64_WaitBusy();//�º�ȴ�����оƬ����æ״̬���˳�����Ϊд�����������оƬ����æ״̬������Ӧ�µĶ�д������
}
void W25Q64_SectorErace(uint32_t Address)//�������� ����ָ����ַ���ڵ���������
{
    W25Q64_WriteEnable();//д�����ǰ�������Ƚ���дʹ�ܣ�дʹ�ܽ���֮������һ��ʱ����Ч��һ��ʱ�������дʧ�ܡ���֤����������оƬ������æ״̬��
    
    MySPI_Start();//��ʼ����
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);//��������������ָ����
    MySPI_SwapByte(Address>>16);
    MySPI_SwapByte(Address>>8);
    MySPI_SwapByte(Address);
    MySPI_Stop();//��ֹ����
    
    W25Q64_WaitBusy();//�º�ȴ�����оƬ����æ״̬���˳�����Ϊд�����������оƬ����æ״̬������Ӧ�µĶ�д��������֤����������оƬ������æ״̬��
}
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count)//��ȡ����
{
    uint32_t i;
    MySPI_Start();//��ʼ����
    MySPI_SwapByte(W25Q64_READ_DATA);//���Ͷ�ȡ���ݵ�ָ����
    MySPI_SwapByte(Address>>16);
    MySPI_SwapByte(Address>>8);
    MySPI_SwapByte(Address);
    for(i=0;i<Count;i++)
    {
        DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//���������������û��شӻ����͵����� ���ŵ�����Ķ�Ӧλ
    }
    MySPI_Stop();//��ֹ����
}
