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
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);



//�������ȼ�
#define UART2TX_TASK_PRIO			4
//�����ջ��С	
#define UART2TX_STK_SIZE      512
//������ƿ�
OS_TCB Uart2TxTaskTCB;
//�����ջ	
CPU_STK UART2TX_TASK_STK[UART2TX_STK_SIZE];
//������
void uart2tx_task(void *p_arg);


//�������ȼ�
#define UART2RX_TASK_PRIO			6
//�����ջ��С	
#define UART2RX_STK_SIZE      512
//������ƿ�
OS_TCB Uart2RxTaskTCB;
//�����ջ	
CPU_STK UART2RX_TASK_STK[UART2RX_STK_SIZE];
//������
void uart2rx_task(void *p_arg);


int main(void)
{
	//SCB->VTOR = FLASH_BASE | 0x4000;//����ƫ����
   OS_ERR err;
   CPU_SR_ALLOC();
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
	HAL_Init();                     //��ʼ��HAL��
	delay_init(180);                //��ʼ����ʱ����
	UART1_Init(115200);              //��ʼ��USART	
	
	UART2_Init(115200);              //��ʼ��USART
	DMA_Init();
	LED_Init();
	//TIM5_Init(0xffffffff, 90-1);		// ����ʱ��1MHz
	TIM2_Init(5000-1,9000-1);         // ����ʱ��10kHz ��װֵ 5000
	
	Init_Sem();
	Init_OSQ();	
	
 /* printf("+++"); //WIFIģ�����ATģʽ������
	delay_ms(10);
	printf("%c",'a');   //������WIFIģ�����ȷ����a
	delay_ms(5);
	printf("AT+UART=921600,8,1,NONE,NFC\r\n"); //���ô��ڲ���
	delay_ms(200);
	printf("AT+WMODE=STA\r\n"); //����ģ�鹤����STAģʽ
	delay_ms(200);
	printf("AT+WSTA=UWBWIFI,1234567890\r\n"); //����ģ������AP���ֺ�����
	delay_ms(200);
	printf("AT+SOCKB=TCPC,192.168.0.201,8899\r\n"); //����socketbΪclient �����ӷ�������IP��ַ�Ͷ˿ں�
	delay_ms(200);	
	printf("AT+Z\r\n");			//ģ������	
	delay_ms(3000);	*/

  //WIFI_Reload=0;
	//delay_ms(3500);
  //WIFI_Reload=1;
	//WIFI_Reset=0;
	//delay_ms(500);
	//WIFI_Reset=1;
  UART2_Init(921600);  
	//printf("\r\nGNSS_UB351 v1.0\r\n");
	OSInit(&err);		//��ʼ��UCOSIII
	
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	  * )&StartTaskTCB,		//������ƿ�
							 (CPU_CHAR	* )"start task", 		//��������
               (OS_TASK_PTR )start_task, 			//������
               (void		  * )0,					//���ݸ��������Ĳ���
               (OS_PRIO	    )START_TASK_PRIO,     //�������ȼ�
               (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void   	  * )0,					//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ
               (OS_ERR   	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	
	OSStart(&err);      //����UCOSIII
    while(1)
    {
	  } 
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����s
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
		 
	//����uart2�ķ�������
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

	//����uart2�Ľ�������
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
							 
							 
	OS_CRITICAL_EXIT();	//�����ٽ���				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
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
