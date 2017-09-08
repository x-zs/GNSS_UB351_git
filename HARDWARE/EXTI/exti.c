/*
 *
 * ����������	
 *
 * ��ǰ�汾��1.0
 *
 * �������ߣ�
 * �������ڣ�
 *
 * �޸��ߣ�
 * �޸�ʱ�䣺
 * �޸�ԭ��
 */

#include "port.h"
#include "deca_device_api.h"
#include "exti.h"

//�ⲿ�жϳ�ʼ��
//decairq: PB5

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();               //����GPIOBʱ��
			
    GPIO_Initure.Pin=GPIO_PIN_5;     						//PB5
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
   //�ж���1-PB5
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //ʹ���ж���1
}


void process_deca_irq(void)
{
    do{
        dwt_isr();
    }while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == 1); //while IRS line active (ARM can only do edge sensitive interrupts)
}

//�жϷ�����
void EXTI9_5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif		
  
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);//�����жϴ����ú���

  process_deca_irq();
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif		
} 
