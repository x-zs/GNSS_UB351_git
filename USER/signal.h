/*
 * Copyright (c) 2017 ɽ��Ȫ��ͨ���������ι�˾ All rights reserved.
 *
 * ����������	��ӡ����
 *
 * ��ǰ�汾��1.0
 *
 * �������ߣ�����
 * �������ڣ�2017-2-16
 *
 * �޸��ߣ�
 * �޸�ʱ�䣺
 * �޸�ԭ��
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
