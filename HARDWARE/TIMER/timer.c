#include "timer.h"
#include "UB351.h"
#include "dma.h"
u8 IR_500ms=0;
TIM_HandleTypeDef TIM5_Handler;
TIM_HandleTypeDef TIM2_Handler;
//通用定时器5初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器2!(定时器2挂在APB1上，时钟为HCLK/2)
void TIM5_Init(u32 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //通用定时器2
    TIM5_Handler.Init.Prescaler=psc;                     //分频系数
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM5_Handler.Init.Period=arr;                        //自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start(&TIM5_Handler); 								//使能定时器5   
}

void TIM2_Init(u32 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); 								//使能定时器2   
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,3,2);    //设置中断优先级，抢占优先级3，子优先级2
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}
	else if (htim->Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM2时钟
//		HAL_NVIC_SetPriority(TIM5_IRQn,1,4);    //设置中断优先级，抢占优先级1，子优先级4
//		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断   
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

	 LED0=!LED0;//每次接收到数据后闪烁一次	
	 Send_flag=1;		
   HAL_UART_Receive_DMA(&UART1_Handler,&UART1_rxBuf[0],RECEIVELEN);				//重新启动接收
	 UB351_Transmit(UART1_rxBuf,rx_len1,1);
   rx_len1=0;
	 IR_500ms=0;
	}
	HAL_TIM_IRQHandler(&TIM2_Handler);		
}
