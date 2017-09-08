/*
 * Copyright (c) 2017 山东泉清通信有限责任公司 All rights reserved.
 *
 * 功能描述：	任务信号量功能模块
 *
 * 当前版本：1.0
 *
 * 初稿作者：齐勇
 * 初稿日期：2017-2-16
 *
 * 修改者：
 * 修改时间：
 * 修改原因：
 */
#include "os.h"
#include "signal.h"

OS_SEM SEM_DW1000_Dry;
OS_SEM SEM_SYNC_TXE;

OS_Q OSQ_TxMsgQ;
OS_Q OSQ_UART1RxMsgQ;
OS_Q OSQ_UART3RxMsgQ;

void Init_OSQ(void)
{
	OS_ERR err;
	OSQCreate(&OSQ_TxMsgQ, "TxMsg OSQ", 50, &err);
	OSQCreate(&OSQ_UART1RxMsgQ, "RxMsg OSQ for UART1", 10, &err);
	OSQCreate(&OSQ_UART3RxMsgQ, "RxMsg OSQ for UART3", 10, &err);	
}

void Init_Sem(void)
{
	OS_ERR err;
	OSSemCreate(&SEM_DW1000_Dry, "SEM for Data Receive", (OS_SEM_CTR)0, &err);	
	OSSemCreate(&SEM_SYNC_TXE, "SEM for Sync Message Tx", (OS_SEM_CTR)0, &err);	
}
