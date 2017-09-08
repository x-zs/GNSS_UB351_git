/*! ----------------------------------------------------------------------------
 * @file	deca_spi.c
 * @brief	SPI access functions
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "deca_spi.h"
#include "deca_device_api.h"
#include "port.h"
#include "spi.h"


/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetospi()
 *
 * Low level abstract function to write to the SPI
 * Takes two separate byte buffers for write header and write data
 * returns 0 for success, or -1 for error
 */
int writetospi_serial(uint16 headerLength, const uint8 *headerBuffer, uint32 bodyLength, const uint8 *bodyBuffer)
{
	int i;
  decaIrqStatus_t  stat ;

  stat = decamutexon() ;
	
	DW_CSN = 0;	
	
//	HAL_SPI_Transmit(&SPI1_Handler, (uint8_t *)headerBuffer, headerLength, 1000);
	
//	HAL_SPI_Transmit(&SPI1_Handler, (uint8_t *)bodyBuffer, bodyLength, 1000);
	
	for(i=0; i<headerLength; i++)
  {
		SPI1->DR = headerBuffer[i];

    	while ((SPI1->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);

    	SPI1->DR ;
  }

    for(i=0; i<bodyLength; i++)
    {
     	SPI1->DR = bodyBuffer[i];

    	while((SPI1->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);

		SPI1->DR ;
	}
	
	DW_CSN = 1;	

  decamutexoff(stat) ;

    return 0;
} // end writetospi()


/*! ------------------------------------------------------------------------------------------------------------------
 * Function: readfromspi()
 *
 * Low level abstract function to read from the SPI
 * Takes two separate byte buffers for write header and read data
 * returns the offset into read buffer where first byte of read data may be found,
 * or returns -1 if there was an error
 */
int readfromspi_serial(uint16 headerLength, const uint8 *headerBuffer, uint32 readlength, uint8 *readBuffer)
{
		int i;
    decaIrqStatus_t  stat ;

    stat = decamutexon() ;
	
		DW_CSN = 0;	
	
//		HAL_SPI_Transmit(&SPI1_Handler, (uint8_t *)headerBuffer, headerLength, 1000);
	
//		HAL_SPI_Receive(&SPI1_Handler, (uint8_t *)readBuffer, readlength, 1000);
	  for(i=0; i<headerLength; i++)
    {
    	SPI1->DR = headerBuffer[i];

     	while((SPI1->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);

     	readBuffer[0] = SPI1->DR ; // Dummy read as we write the header
    }

    for(i=0; i<readlength; i++)
    {
    	SPI1->DR = 0;  // Dummy write as we read the message body

    	while((SPI1->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);
 
	   	readBuffer[i] = SPI1->DR ;//port_SPIx_receive_data(); //this clears RXNE bit
    }
	
		DW_CSN = 1;	
	
	
    decamutexoff(stat) ;

    return 0;
} // end readfromspi()

