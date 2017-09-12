#include "includes.h"
#include "sys.h"
#include "signal.h"
#include "delay.h"
#include "dma.h"
#include "usart.h"
//#include "spi.h"
#include "exti.h"
#include "timer.h"
#include  "led.h"
#include "stmflash.h"
#include  "UB351.h"
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);



//任务优先级
#define UART2TX_TASK_PRIO			4
//任务堆栈大小	
#define UART2TX_STK_SIZE      512
//任务控制块
OS_TCB Uart2TxTaskTCB;
//任务堆栈	
CPU_STK UART2TX_TASK_STK[UART2TX_STK_SIZE];
//任务函数
void uart2tx_task(void *p_arg);


//任务优先级
#define UART2RX_TASK_PRIO			6
//任务堆栈大小	
#define UART2RX_STK_SIZE      512
//任务控制块
OS_TCB Uart2RxTaskTCB;
//任务堆栈	
CPU_STK UART2RX_TASK_STK[UART2RX_STK_SIZE];
//任务函数
void uart2rx_task(void *p_arg);


int main(void)
{
	//SCB->VTOR = FLASH_BASE | 0x4000;//设置偏移量
   OS_ERR err;
   CPU_SR_ALLOC();
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz   
	HAL_Init();                     //初始化HAL库
	delay_init(180);                //初始化延时函数
	UART1_Init(115200);              //初始化USART	
	
	UART2_Init(115200);              //初始化USART
	DMA_Init();
	LED_Init();
	//TIM5_Init(0xffffffff, 90-1);		// 计数时钟1MHz
	TIM2_Init(5000-1,9000-1);         // 计数时钟10kHz 重装值 5000
	
	Init_Sem();
	Init_OSQ();	
	
 /* printf("+++"); //WIFI模块进入AT模式的命令
	delay_ms(10);
	printf("%c",'a');   //串口向WIFI模块输出确认码a
	delay_ms(5);
	printf("AT+UART=921600,8,1,NONE,NFC\r\n"); //配置串口参数
	delay_ms(200);
	printf("AT+WMODE=STA\r\n"); //设置模块工作在STA模式
	delay_ms(200);
	printf("AT+WSTA=UWBWIFI,1234567890\r\n"); //设置模块连接AP名字和密码
	delay_ms(200);
	printf("AT+SOCKB=TCPC,192.168.0.201,8899\r\n"); //设置socketb为client 和连接服务器的IP地址和端口号
	delay_ms(200);	
	printf("AT+Z\r\n");			//模块重启	
	delay_ms(3000);	*/

  //WIFI_Reload=0;
	//delay_ms(3500);
  //WIFI_Reload=1;
	//WIFI_Reset=0;
	//delay_ms(500);
	//WIFI_Reset=1;
  UART2_Init(921600);  
	//printf("\r\nGNSS_UB351 v1.0\r\n");
	OSInit(&err);		//初始化UCOSIII
	
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	  * )&StartTaskTCB,		//任务控制块
							 (CPU_CHAR	* )"start task", 		//任务名字
               (OS_TASK_PTR )start_task, 			//任务函数
               (void		  * )0,					//传递给任务函数的参数
               (OS_PRIO	    )START_TASK_PRIO,     //任务优先级
               (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
               (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
               (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
               (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
               (OS_TICK	    )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
               (void   	  * )0,					//用户补充的存储区
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //任务选项,为了保险起见，所有任务都保存浮点寄存器的值
               (OS_ERR   	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	
	OSStart(&err);      //开启UCOSIII
    while(1)
    {
	  } 
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,设置默认的时间片长度s
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
		 
	//创建uart2的发送任务
	OSTaskCreate((OS_TCB  	* )&Uart2TxTaskTCB,		
							 (CPU_CHAR	* )"uart2tx task", 		
               (OS_TASK_PTR )uart2tx_task, 			
               (void		  * )0,					
               (OS_PRIO	    )UART2TX_TASK_PRIO,     	
               (CPU_STK   * )&UART2TX_TASK_STK[0],	
               (CPU_STK_SIZE)UART2TX_STK_SIZE/10,	
               (CPU_STK_SIZE)UART2TX_STK_SIZE,		
               (OS_MSG_QTY  )0,					
               (OS_TICK	    )0,					
               (void   	  * )0,				
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
               (OS_ERR 	  * )&err);

	//创建uart2的接收任务
	OSTaskCreate((OS_TCB  	* )&Uart2RxTaskTCB,		
							 (CPU_CHAR	* )"uart2rx task", 		
               (OS_TASK_PTR )uart2rx_task, 			
               (void		  * )0,					
               (OS_PRIO	    )UART2RX_TASK_PRIO,     	
               (CPU_STK   * )&UART2RX_TASK_STK[0],	
               (CPU_STK_SIZE)UART2RX_STK_SIZE/10,	
               (CPU_STK_SIZE)UART2RX_STK_SIZE,		
               (OS_MSG_QTY  )0,					
               (OS_TICK	    )0,					
               (void   	  * )0,				
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
               (OS_ERR 	  * )&err);
							 
							 
	OS_CRITICAL_EXIT();	//进入临界区				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
}

void uart2tx_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;	
	while(1)
	{
	OSSemPend(&SEM_DW1000_Dry, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	}
}
//u16 lose_number=0;	

u16 rx2_len=0;
void uart2rx_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE ms;

//	uint8 *message;
	void  *m_msg;
	
	while(1)
	{	
	 
		m_msg = OSQPend(&OSQ_UART1RxMsgQ, 0, OS_OPT_PEND_BLOCKING, &ms, &ts, &err);		
		rx2_len=ms;

		//UB351_Transmit(m_msg,ms,1);
	}
}
