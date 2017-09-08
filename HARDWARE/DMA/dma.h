#ifndef __DMA_H
#define __DMA_H
#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//DMA驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//extern DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA句柄

extern DMA_HandleTypeDef  UART1TxDMA_Handler;      //UART1发送通道DMA句柄，DMA2/Str7/Chan4
extern DMA_HandleTypeDef  UART1RxDMA_Handler;      //UART1接收通道DMA句柄，DMA2/Str5/Chan4

extern DMA_HandleTypeDef  UART2TxDMA_Handler;      //UART2发送通道DMA句柄, DMA1/Str6/Chan4
extern DMA_HandleTypeDef  UART2RxDMA_Handler;			 //UART2接收通道DMA句柄，DMA1/Str5/Chan4

extern DMA_HandleTypeDef  UART3RxDMA_Handler;      //UART3接收通道DMA句柄，DMA1/Str1/Chan4

extern DMA_HandleTypeDef  SPI1TxDMA_Handler;				//SPI1发送通道DMA句柄，DMA1/Str4/Chan0
extern DMA_HandleTypeDef  SPI1RxDMA_Handler;				//SPI1接收通道DMA句柄，DMA1/Str3/Chan0

//extern uint8_t UART1_rxBuf2[RECEIVELEN];
extern uint8_t UART1_rxBuf[RECEIVELEN];
extern uint8_t SPI_RX_FLAG;
extern uint8_t SPI_TX_FLAG;

void DMA_Init(void);

#endif
