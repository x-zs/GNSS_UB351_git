#include "UB351.h"
#include "dma.h"
#include "string.h"
extern u8 flag;
extern u16 rx_len1;
extern u16 rx_len2;					
u8 Frame_Type=0;
u16 Frame_Len=0;
u8 Frame_LenH=0;
u8 Frame_LenL=0;
extern u16 lose_number;
extern u8 start_flag;
u8 Frame_number=0;
u8 Frame_Head[12]={0xaa,0x55,0xa5,0x5a,Station_Type,Station_Number,0,0,0,0,0,0};//֡ͷ=��ʼ֡+ �ڵ�����+�ڵ���+֡����+���ȵ��ֽ�+���ȸ��ֽ�
void UB351_Transmit(u8 *message,u16 length,u8 Frame_type)
{	u8 Frame_check=0;
	u16 i=0;
	u8 Lose_LenH=0;
	u8 Lose_LenL=0;
	Frame_Len=length+1; 
	if(Frame_type==3)Frame_Len=12+1;
	Frame_Head[11]=(Frame_Len& 0xFF00)>>8; //���ȸ��ֽ�
	Frame_Head[10]=(u8)(Frame_Len&0xFF);    //���ȵ��ֽ�
	Frame_Len=0;	
	Frame_Head[9]=Frame_type;		
	Frame_Head[8]=Frame_number;  //����֡���
	Frame_number+=1;		
	for(i=0;i<12;i++)
	{
	 Frame_check^=Frame_Head[i];
		printf("%c",Frame_Head[i]);
	}			
	for(i=0;i<length;i++)
	{
	 printf("%c",message[i]);
	 Frame_check^=message[i];
	}
 if(Frame_type==3)
 {
	 if(start_flag==0)
	 {printf("%c",0x00);
		printf("%c",0x00);
		Frame_check^=0x00;
		Frame_check^=0x00;
	 }
	 else 
	 {Lose_LenL=(u8)(lose_number&0xFF)-1; //������ �ĵ��ֽ�
		Lose_LenH=(lose_number& 0xFF00)>>8; //�������� ���ֽ�
	  printf("%c",Lose_LenL);  // ������ֽ� 
		printf("%c",Lose_LenH);  //������ֽ�
		Frame_check^=Lose_LenL;   //���У��
		Frame_check^=Lose_LenH;   //
	 }
	 for(i=0;i<10;i++)
	{
		printf("%c",255);//֡�غ�״̬
		Frame_check^=0xff;	
	}
 }
	printf("%c",Frame_check);		 //���У��λ
	

}
