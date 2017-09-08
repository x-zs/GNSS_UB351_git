#include "spi.h"
#include "signal.h"
//#include "exti.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/16/16
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//����˵��
//V1.1 20160116
//������SPI2��Ӧ�ô���
////////////////////////////////////////////////////////////////////////////////// 	 

SPI_HandleTypeDef SPI1_Handler;  //SPI2���


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{
    SPI1_Handler.Instance=SPI1;                      //SPI1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;          //����SPI����ģʽ������Ϊ��ģʽ
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;    //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_LOW; //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //�ر�TIģʽ
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI1_Handler.Init.CRCPolynomial=7;               //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI1_Handler);
    SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_8);
	
		SPI1_Handler.hdmarx->Instance->PAR = (uint32_t)&SPI1_Handler.Instance->DR;
		SPI1_Handler.hdmatx->Instance->PAR = (uint32_t)&SPI1_Handler.Instance->DR;
		SET_BIT(SPI1_Handler.Instance->CR2 , SPI_CR2_RXDMAEN);
		SET_BIT(SPI1_Handler.Instance->CR2 , SPI_CR2_TXDMAEN);
    __HAL_SPI_ENABLE(&SPI1_Handler);                 //ʹ��SPI1
    //SPI2_ReadWriteByte(0Xff);                        //��������
}
//SPI1�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
	
		if (hspi->Instance==SPI1)
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();       //ʹ��GPIOBʱ��
			__HAL_RCC_SPI1_CLK_ENABLE();        //ʹ��SPI2ʱ��		

			//SPI1���ų�ʼ��PA5,6,7
			GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
			GPIO_Initure.Pull=GPIO_PULLUP;                  //����
			GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
			GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //����ΪSPI1
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);             //��ʼ��
			
			//SPI1���ų�ʼ��PA4��ʹ��������Ƶ�Ƭѡ
			GPIO_Initure.Pin=GPIO_PIN_4;										//PA4
			GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;					//�������
			GPIO_Initure.Pull=GPIO_PULLUP;									//����
			GPIO_Initure.Speed=GPIO_SPEED_FAST;							//����
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);							//��ʼ��		

			//DW1000��λ����
			GPIO_Initure.Pin = GPIO_PIN_0;
			GPIO_Initure.Mode=GPIO_MODE_INPUT;	
			GPIO_Initure.Speed=GPIO_SPEED_FAST;	
			HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			
		}
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI1_Handler);            //�ر�SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI1_Handler);             //ʹ��SPI
    
}

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}

