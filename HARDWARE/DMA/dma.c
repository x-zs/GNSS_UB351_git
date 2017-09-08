#include "dma.h"
#include "usart.h"
#include "spi.h"
#include "sys.h"
#include "os.h"

DMA_HandleTypeDef  UART2TxDMA_Handler;      //UART2����ͨ��DMA�����DMA1/Str6/Chan4
DMA_HandleTypeDef  UART2RxDMA_Handler;			//UART2����ͨ��DMA�����DMA1/Str5/Chan4

DMA_HandleTypeDef  UART1TxDMA_Handler;      //UART2����ͨ��DMA�����DMA1/Str6/Chan4
DMA_HandleTypeDef  UART1RxDMA_Handler;			//UART2����ͨ��DMA�����DMA1/Str5/Chan4


DMA_HandleTypeDef  SPI1TxDMA_Handler;				//SPI2����ͨ��DMA�����DMA1/Str4/Chan0
DMA_HandleTypeDef  SPI1RxDMA_Handler;				//SPI2����ͨ��DMA�����DMA1/Str3/Chan0

//uint8_t UART1_rxBuf2[RECEIVELEN];
uint8_t UART1_rxBuf[RECEIVELEN];
uint8_t SPI_RX_FLAG=0;
uint8_t SPI_TX_FLAG=0;

//UART2 DMA ����
void UART2_DMA_Config(void)
{ 
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA1ʱ��ʹ��
    
	__HAL_LINKDMA(&UART1_Handler,hdmatx,UART1TxDMA_Handler);    //��DMA��USART2��ϵ����(����DMA)
	__HAL_LINKDMA(&UART1_Handler,hdmarx,UART1RxDMA_Handler);
    
    //Tx DMA����
  UART1TxDMA_Handler.Instance=DMA2_Stream7;                            //������ѡ��
  UART1TxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //ͨ��ѡ��
  UART1TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
  UART1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
  UART1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
  UART1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
  UART1TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
  UART1TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
  UART1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
  UART1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
  UART1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
  UART1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
  UART1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	
  HAL_DMA_DeInit(&UART1TxDMA_Handler);   
  HAL_DMA_Init(&UART1TxDMA_Handler);
	
	//Rx DMA����
	UART1RxDMA_Handler.Instance=DMA2_Stream5;                            //������ѡ��
	UART1RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //ͨ��ѡ��
	UART1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
	UART1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	UART1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	UART1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	UART1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	UART1RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
	UART1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	UART1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	UART1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	UART1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	UART1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���

	HAL_DMA_DeInit(&UART1RxDMA_Handler);   
	HAL_DMA_Init(&UART1RxDMA_Handler);
}

void SPI1_DMA_Config(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();															//DMA1ʱ��ʹ�� 
    
  __HAL_LINKDMA(&SPI1_Handler,hdmatx,SPI1TxDMA_Handler);    //��DMA�����SPI2�����ϵ����(����DMA)
	__HAL_LINKDMA(&SPI1_Handler,hdmarx,SPI1RxDMA_Handler);    //��DMA�����SPI2�����ϵ����(����DMA)
    
	//Tx DMA����
	SPI1TxDMA_Handler.Instance=DMA2_Stream3;                            //������ѡ��
	SPI1TxDMA_Handler.Init.Channel=DMA_CHANNEL_3;                       //ͨ��ѡ��
	SPI1TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
	SPI1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	SPI1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	SPI1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	SPI1TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	SPI1TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
	SPI1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	SPI1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	SPI1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	SPI1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	SPI1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���

	HAL_DMA_DeInit(&SPI1TxDMA_Handler);   
	HAL_DMA_Init(&SPI1TxDMA_Handler);
	
	//Rx DMA����
	SPI1RxDMA_Handler.Instance=DMA2_Stream2;                            //������ѡ��
	SPI1RxDMA_Handler.Init.Channel=DMA_CHANNEL_3;                       //ͨ��ѡ��
	SPI1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
	SPI1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	SPI1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	SPI1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	SPI1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	SPI1RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
	SPI1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	SPI1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	SPI1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	SPI1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	SPI1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���

	HAL_DMA_DeInit(&SPI1RxDMA_Handler);   
	HAL_DMA_Init(&SPI1RxDMA_Handler);
}

void DMA_Init(void)
{
	//SPI1 DMA config
	SPI1_DMA_Config();                                   //SPI1 DMA����
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 1, 2);       //��ռ���ȼ�Ϊ1�������ȼ�Ϊ1
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);               //ʹ��SPI1 DMA�����ж�
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 2);       //��ռ���ȼ�Ϊ1�������ȼ�Ϊ1
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);               //ʹ��SPI1 DMA�����ж�
	
	//UART1 DMA config
	UART2_DMA_Config();
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 4);       //��ռ���ȼ�Ϊ1�������ȼ�Ϊ4
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 1, 3);       //��ռ���ȼ�Ϊ1�������ȼ�Ϊ5	
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	//USART2����DMA����
	HAL_UART_DMAStop(&UART1_Handler);//�ȹ�һ�£�����DMA���ղ�����һ�����ݣ����ܼ��ֽڣ�
	HAL_UART_Receive_DMA(&UART1_Handler, &UART1_rxBuf[0], RECEIVELEN);
}

//SPI1 DMA�����ж�
void DMA2_Stream3_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif					
	SPI_TX_FLAG=1;
	HAL_DMA_IRQHandler(SPI1_Handler.hdmatx);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif	
}

//SPI1 DMA�����ж� 
void DMA2_Stream2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif		
//	DW_CSN = 1;
	SPI_RX_FLAG=1;
	HAL_DMA_IRQHandler(SPI1_Handler.hdmarx);
	//HAL_DMA_IRQHandler(UART3_Handler.hdmatx);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif	
}

//USART2 DMA�����ж�
void DMA2_Stream7_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif			
	HAL_DMA_IRQHandler(UART1_Handler.hdmatx);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}

//USART2 DMA�����ж�
void DMA2_Stream5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif			
	HAL_DMA_IRQHandler(UART1_Handler.hdmarx);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}


