#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "deca_device_api.h"
#include "deca_types.h"
#include "deca_regs.h"
#include "port.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM5_Handler;
extern TIM_HandleTypeDef TIM2_Handler;
void TIM5_Init(u32 arr,u16 psc);
void TIM2_Init(u32 arr,u16 psc);
extern uint32 delaySyncTxTime;
#endif

