#ifndef _UB351_H
#define _UB351_H
#include "sys.h"
#include "stdio.h"	
#define Station_Number 2
#define Station_Type 1
extern u8 IR_500ms;
void UB351_Receive(u8 *message,u16 length);
void UB351_Transmit(u8 *message,u16 length,u8 Frame_type);
#endif
