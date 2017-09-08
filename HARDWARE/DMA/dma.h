#ifndef __DMA_H
#define __DMA_H
#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//DMA��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//extern DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA���

extern DMA_HandleTypeDef  UART1TxDMA_Handler;      //UART1����ͨ��DMA�����DMA2/Str7/Chan4
extern DMA_HandleTypeDef  UART1RxDMA_Handler;      //UART1����ͨ��DMA�����DMA2/Str5/Chan4

extern DMA_HandleTypeDef  UART2TxDMA_Handler;      //UART2����ͨ��DMA���, DMA1/Str6/Chan4
extern DMA_HandleTypeDef  UART2RxDMA_Handler;			 //UART2����ͨ��DMA�����DMA1/Str5/Chan4

extern DMA_HandleTypeDef  UART3RxDMA_Handler;      //UART3����ͨ��DMA�����DMA1/Str1/Chan4

extern DMA_HandleTypeDef  SPI1TxDMA_Handler;				//SPI1����ͨ��DMA�����DMA1/Str4/Chan0
extern DMA_HandleTypeDef  SPI1RxDMA_Handler;				//SPI1����ͨ��DMA�����DMA1/Str3/Chan0

//extern uint8_t UART1_rxBuf2[RECEIVELEN];
extern uint8_t UART1_rxBuf[RECEIVELEN];
extern uint8_t SPI_RX_FLAG;
extern uint8_t SPI_TX_FLAG;

void DMA_Init(void);

#endif
