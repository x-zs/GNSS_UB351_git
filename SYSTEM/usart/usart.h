#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2015/6/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	
	  	
extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler;
extern UART_HandleTypeDef UART3_Handler;

//����봮���жϽ��գ��벻Ҫע�����º궨��
void UART1_Init(u32 bound);
void UART2_Init(u32 bound);
void UART3_Init(u32 bound);
void UART1_send(u8 *p,u16 lenth);
/* USER CODE BEGIN Private defines */  
#define RECEIVELEN 800
#endif
