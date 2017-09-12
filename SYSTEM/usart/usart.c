#include "usart.h"
#include "delay.h"
#include "signal.h"
#include "exti.h"
#include "dma.h"
#include "string.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif 
void UART1_send(u8 *p,u16 lenth)
{u16 i=0;
for(;i<lenth;i++)
	{	
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕    
	USART1->DR = *(p+i);
	}   
}
UART_HandleTypeDef UART1_Handler; //UART句柄
UART_HandleTypeDef UART2_Handler;
UART_HandleTypeDef UART3_Handler;

//初始化IO 串口1 
//bound:波特率
void UART1_Init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    					//USART1
	UART1_Handler.Init.BaudRate=bound;				    				//波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   	//字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    			//收发模式
	HAL_UART_Init(&UART1_Handler);					    					//HAL_UART_Init()会使能UART1
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

	//打开空闲中断 初始化DMA接收
	//UsartType1.dmaSend_flag = USART_DMA_SENDOVER;
	//HAL_UART_Receive_DMA(&UART1_Handler, UsartType1.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&UART1_Handler, UART_IT_IDLE);
	
}

void UART2_Init(u32 bound)
{
	//UART 初始化设置
	UART2_Handler.Instance=USART2;					    					//UART2
	UART2_Handler.Init.BaudRate=bound;				    				//波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   	//字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    			//收发模式
	HAL_UART_Init(&UART2_Handler);					    					//HAL_UART_Init()会使能UART2
	
	//HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

	//打开空闲中断 初始化DMA接收
	//UsartType2.dmaSend_flag = USART_DMA_SENDOVER;
	__HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_IDLE);
}

void UART3_Init(u32 bound)
{
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    					//UART3
	UART3_Handler.Init.BaudRate=bound;				    				//波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   	//字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    			//收发模式
	HAL_UART_Init(&UART3_Handler);					    					//HAL_UART_Init()会使能UART3
	
	//HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

	//打开空闲中断 初始化DMA接收
	//UsartType3.dmaSend_flag = USART_DMA_SENDOVER;
	__HAL_UART_ENABLE_IT(&UART3_Handler, UART_IT_IDLE);
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)						//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();				//使能GPIOB时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;				//PB6,7
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;						//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;								//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;						//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;				//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   			//初始化PB6,7
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);						//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,0);			//抢占优先级3，子优先级3
	}
	else if (huart->Instance==USART3)				//如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();					//使能GPIOD时钟
		__HAL_RCC_USART3_CLK_ENABLE();				//使能USART3时钟
	
		GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;			//PD8,9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;					//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;							//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;					//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;			//复用为USART3
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   			//初始化

		HAL_NVIC_EnableIRQ(USART3_IRQn);						//使能USART3中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,3,3);			//抢占优先级3，子优先级0
	}
	else if (huart->Instance==USART2)					//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();						//使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();					//使能USART2时钟
	
		GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;			//PA2,3
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;					//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;							//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;					//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART2;			//复用为USART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   			//初始化

		HAL_NVIC_EnableIRQ(USART2_IRQn);						//使能USART2中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,3,1);			//抢占优先级3，子优先级1
	}

}
//u8 flag=0;
u16 rx_len1=0;
extern u8 Send_flag;
extern u8 start_flag;
extern u8 IR_500ms;
//u16 rx_len2=0;
//串口接收空闲中断
void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{
	
	uint32_t temp;
	uint32_t rx_len;
	OS_ERR err;	

	if (huart->Instance == USART1)
	{
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&UART1_Handler);
//			HAL_UART_DMAStop(&UART2_Handler);
			UART1_Handler.Instance->CR3 &= ~USART_CR3_DMAR;
			HAL_DMA_Abort(UART1_Handler.hdmarx);
			UART1_Handler.State = HAL_UART_STATE_READY;
			temp = UART1_Handler.hdmarx->Instance->NDTR;
			rx_len =  RECEIVELEN - temp;
			rx_len1=rx_len1+rx_len;	
		 if(UART1_rxBuf[0]==0xd3&&(UART1_rxBuf[3]==0x3F||UART1_rxBuf[3]==0x03||UART1_rxBuf[3]==0x43||UART1_rxBuf[3]==0x46))
			{	if(start_flag==0)
				{	//Send_flag=0;
				 //OSQPost(&OSQ_UART1RxMsgQ, &UART1_rxBuf[0], rx_len1, OS_OPT_POST_FIFO, &err);				
				 TIM2->CNT=0;//将计数器清零
				 IR_500ms=1;//观测量帧发送后的500ms后再发送信息状态帧
				 start_flag=1;
				}
				else 
				{
		  	HAL_UART_Receive_DMA(&UART1_Handler,&UART1_rxBuf[rx_len1],RECEIVELEN);						
				}
				
			}
			else 
			{ UART1_rxBuf[rx_len]=0;
			 if(strcmp(UART1_rxBuf,"[COM1]\r\n")==0)
				{
					UART1_send(" MOVINGBASESTATION ENABLE\r\n",27);
					UART1_send("LOG COM1 RTCM1075 ONTIME 1\r\n",28);
					UART1_send("LOG COM1 RTCM1125 ONTIME 1\r\n",28);
					UART1_send("LOG COM1 RTCM63 ONTIME 1\r\n",26);
					UART1_send("LOG COM1 RTCM1019 ONTIME 1\r\n",28);
				}
			 HAL_UART_Receive_DMA(&UART1_Handler,&UART1_rxBuf[0],RECEIVELEN);
			}
			
			//printf("11");
		}
	}
	
	if (huart->Instance == USART2)
	{
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&UART2_Handler);
		}
	}
}



//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	UsartReceive_IDLE(&UART1_Handler);		//调用接收空闲中断
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif


}

void USART2_IRQHandler(void)                	
{ 
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	UsartReceive_IDLE(&UART2_Handler);		//调用接收空闲中断
	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
	
}

void USART3_IRQHandler(void)                	
{ 
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	UsartReceive_IDLE(&UART3_Handler);		//调用接收空闲中断
	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}

