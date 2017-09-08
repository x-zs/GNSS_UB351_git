/*
 * Copyright (c) 2017 山东泉清通信有限责任公司 All rights reserved.
 *
 * 功能描述：	打印调试
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
#ifndef SIGNAL_H
#define SIGNAL_H

#include "os.h"

#ifdef  __cplusplus
extern "C" {
#endif

extern OS_SEM SEM_DW1000_Dry;
extern OS_SEM SEM_SYNC_TXE;
	
extern OS_Q OSQ_TxMsgQ;		
extern OS_Q OSQ_UART1RxMsgQ;
extern OS_Q OSQ_UART3RxMsgQ;	
	
void Init_Sem(void);
void Init_OSQ(void);	

#ifdef  __cplusplus
}
#endif

#endif
