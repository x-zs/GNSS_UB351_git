#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "deca_device_api.h"
#include "deca_types.h"
#include "deca_regs.h"
#include "port.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM5_Handler;
extern TIM_HandleTypeDef TIM2_Handler;
void TIM5_Init(u32 arr,u16 psc);
void TIM2_Init(u32 arr,u16 psc);
extern uint32 delaySyncTxTime;
#endif

