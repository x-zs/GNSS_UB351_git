#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2015/6/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	
	  	
extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler;
extern UART_HandleTypeDef UART3_Handler;

//如果想串口中断接收，请不要注释以下宏定义
void UART1_Init(u32 bound);
void UART2_Init(u32 bound);
void UART3_Init(u32 bound);
void UART1_send(u8 *p,u16 lenth);
/* USER CODE BEGIN Private defines */  
#define RECEIVELEN 800
#endif
