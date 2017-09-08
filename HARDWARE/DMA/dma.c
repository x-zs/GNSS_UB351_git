#include "dma.h"
#include "usart.h"
#include "spi.h"
#include "sys.h"
#include "os.h"

DMA_HandleTypeDef  UART2TxDMA_Handler;      //UART2接收通道DMA句柄，DMA1/Str6/Chan4
DMA_HandleTypeDef  UART2RxDMA_Handler;			//UART2接收通道DMA句柄，DMA1/Str5/Chan4

DMA_HandleTypeDef  UART1TxDMA_Handler;      //UART2接收通道DMA句柄，DMA1/Str6/Chan4
DMA_HandleTypeDef  UART1RxDMA_Handler;			//UART2接收通道DMA句柄，DMA1/Str5/Chan4


DMA_HandleTypeDef  SPI1TxDMA_Handler;				//SPI2发送通道DMA句柄，DMA1/Str4/Chan0
DMA_HandleTypeDef  SPI1RxDMA_Handler;				//SPI2接收通道DMA句柄，DMA1/Str3/Chan0

//uint8_t UART1_rxBuf2[RECEIVELEN];
uint8_t UART1_rxBuf[RECEIVELEN];
uint8_t SPI_RX_FLAG=0;
uint8_t SPI_TX_FLAG=0;

//UART2 DMA 配置
void UART2_DMA_Config(void)
{ 
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA1时钟使能
    
	__HAL_LINKDMA(&UART1_Handler,hdmatx,UART1TxDMA_Handler);    //将DMA与USART2联系起来(发送DMA)
	__HAL_LINKDMA(&UART1_Handler,hdmarx,UART1RxDMA_Handler);
    
    //Tx DMA配置
  UART1TxDMA_Handler.Instance=DMA2_Stream7;                            //数据流选择
  UART1TxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //通道选择
  UART1TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
  UART1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
  UART1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
  UART1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
  UART1TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
  UART1TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
  UART1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
  UART1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
  UART1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
  UART1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
  UART1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	
  HAL_DMA_DeInit(&UART1TxDMA_Handler);   
  HAL_DMA_Init(&UART1TxDMA_Handler);
	
	//Rx DMA配置
	UART1RxDMA_Handler.Instance=DMA2_Stream5;                            //数据流选择
	UART1RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //通道选择
	UART1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
	UART1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	UART1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	UART1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	UART1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	UART1RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
	UART1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	UART1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	UART1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	UART1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	UART1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输

	HAL_DMA_DeInit(&UART1RxDMA_Handler);   
	HAL_DMA_Init(&UART1RxDMA_Handler);
}

void SPI1_DMA_Config(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();															//DMA1时钟使能 
    
  __HAL_LINKDMA(&SPI1_Handler,hdmatx,SPI1TxDMA_Handler);    //将DMA句柄与SPI2句柄联系起来(发送DMA)
	__HAL_LINKDMA(&SPI1_Handler,hdmarx,SPI1RxDMA_Handler);    //将DMA句柄与SPI2句柄联系起来(接收DMA)
    
	//Tx DMA配置
	SPI1TxDMA_Handler.Instance=DMA2_Stream3;                            //数据流选择
	SPI1TxDMA_Handler.Init.Channel=DMA_CHANNEL_3;                       //通道选择
	SPI1TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
	SPI1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	SPI1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	SPI1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	SPI1TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	SPI1TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
	SPI1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	SPI1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	SPI1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	SPI1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	SPI1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输

	HAL_DMA_DeInit(&SPI1TxDMA_Handler);   
	HAL_DMA_Init(&SPI1TxDMA_Handler);
	
	//Rx DMA配置
	SPI1RxDMA_Handler.Instance=DMA2_Stream2;                            //数据流选择
	SPI1RxDMA_Handler.Init.Channel=DMA_CHANNEL_3;                       //通道选择
	SPI1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
	SPI1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	SPI1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	SPI1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	SPI1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	SPI1RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
	SPI1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	SPI1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	SPI1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	SPI1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	SPI1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输

	HAL_DMA_DeInit(&SPI1RxDMA_Handler);   
	HAL_DMA_Init(&SPI1RxDMA_Handler);
}

void DMA_Init(void)
{
	//SPI1 DMA config
	SPI1_DMA_Config();                                   //SPI1 DMA配置
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 1, 2);       //抢占优先级为1，子优先级为1
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);               //使能SPI1 DMA发送中断
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 2);       //抢占优先级为1，子优先级为1
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);               //使能SPI1 DMA接收中断
	
	//UART1 DMA config
	UART2_DMA_Config();
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 4);       //抢占优先级为1，子优先级为4
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 1, 3);       //抢占优先级为1，子优先级为5	
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	//USART2启动DMA接收
	HAL_UART_DMAStop(&UART1_Handler);//先关一下，否则DMA接收不到第一包数据（不管几字节）
	HAL_UART_Receive_DMA(&UART1_Handler, &UART1_rxBuf[0], RECEIVELEN);
}

//SPI1 DMA发送中断
void DMA2_Stream3_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif					
	SPI_TX_FLAG=1;
	HAL_DMA_IRQHandler(SPI1_Handler.hdmatx);
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif	
}

//SPI1 DMA接收中断 
void DMA2_Stream2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif		
//	DW_CSN = 1;
	SPI_RX_FLAG=1;
	HAL_DMA_IRQHandler(SPI1_Handler.hdmarx);
	//HAL_DMA_IRQHandler(UART3_Handler.hdmatx);
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif	
}

//USART2 DMA发送中断
void DMA2_Stream7_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif			
	HAL_DMA_IRQHandler(UART1_Handler.hdmatx);
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}

//USART2 DMA接收中断
void DMA2_Stream5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif			
	HAL_DMA_IRQHandler(UART1_Handler.hdmarx);
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}


