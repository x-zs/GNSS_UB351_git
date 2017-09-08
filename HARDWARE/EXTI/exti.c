/*
 *
 * 功能描述：	
 *
 * 当前版本：1.0
 *
 * 初稿作者：
 * 初稿日期：
 *
 * 修改者：
 * 修改时间：
 * 修改原因：
 */

#include "port.h"
#include "deca_device_api.h"
#include "exti.h"

//外部中断初始化
//decairq: PB5

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();               //开启GPIOB时钟
			
    GPIO_Initure.Pin=GPIO_PIN_5;     						//PB5
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
   //中断线1-PB5
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //使能中断线1
}


void process_deca_irq(void)
{
    do{
        dwt_isr();
    }while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == 1); //while IRS line active (ARM can only do edge sensitive interrupts)
}

//中断服务函数
void EXTI9_5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif		
  
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);//调用中断处理公用函数

  process_deca_irq();
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif		
} 
