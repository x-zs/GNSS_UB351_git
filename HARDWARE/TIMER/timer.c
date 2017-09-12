#include "timer.h"
#include "UB351.h"
#include "dma.h"
u8 IR_500ms=0;
TIM_HandleTypeDef TIM5_Handler;
TIM_HandleTypeDef TIM2_Handler;
//ͨ�ö�ʱ��5��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��2!(��ʱ��2����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM5_Init(u32 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��2
    TIM5_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start(&TIM5_Handler); 								//ʹ�ܶ�ʱ��5   
}

void TIM2_Init(u32 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��2
    TIM2_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); 								//ʹ�ܶ�ʱ��2   
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,3,2);    //�����ж����ȼ�����ռ���ȼ�3�������ȼ�2
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	}
	else if (htim->Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM2ʱ��
//		HAL_NVIC_SetPriority(TIM5_IRQn,1,4);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�4
//		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�   
	}
}

uint32 delaySyncTxTime;
u8 Send_flag=0;
extern u8 start_flag;
extern u16 rx2_len;
extern u16 rx_len1;
//extern u16 lose_number;
void TIM2_IRQHandler(void)
{	IR_500ms+=1;
	if(IR_500ms==2)
	{
//	 lose_number=lose_number+1;

	 LED0=!LED0;//ÿ�ν��յ����ݺ���˸һ��	
	 Send_flag=1;		
   HAL_UART_Receive_DMA(&UART1_Handler,&UART1_rxBuf[0],RECEIVELEN);				//������������
	 UB351_Transmit(UART1_rxBuf,rx_len1,1);
   rx_len1=0;
	 IR_500ms=0;
	}
	HAL_TIM_IRQHandler(&TIM2_Handler);		
}
