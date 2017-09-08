#include "dw1000.h"

config configData;

event_data_t dw_event;
instance_data_t instance_data;
srd_msg_dsss sync_message;
position_data_t position_data[MAX_POSITION_NUMBER];
uint8 positionNumber;
uint8 positionIdxOut;
uint8 positionIdxIn;

void readConfigData()
{
//		STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)&configData,13);
//		if(configData.version != 1)
		{
			configData.role = SLAVE_ANCHOR;
			configData.version = 1;
			configData.devID = 1;
			configData.slotTime = 5;
			configData.channleConfig.channel = 2;
			configData.channleConfig.prf = DWT_PRF_64M;
			configData.channleConfig.datarate = DWT_BR_6M8;
			configData.channleConfig.preambleCode = 9;
			configData.channleConfig.preambleLength = DWT_PLEN_128;
			configData.channleConfig.pacSize = DWT_PAC8;
			configData.channleConfig.nsSFD = 0;
			configData.channleConfig.sfdTO = 129;
//			STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)&configData,13);
		}
}

void sync_message_init()
{
		int i;
		sync_message.seqNum = 0;
		sync_message.frameCtrl[0] = 0x41;
		sync_message.frameCtrl[1] = 0x88;
		sync_message.sourceAddr[0] = 0;
		sync_message.sourceAddr[1] = 0;
		sync_message.destAddr[0] = 0xFF;
		sync_message.destAddr[1] = 0xFF;
		sync_message.messageData[FCODE] = 0x81;
		sync_message.messageData[POLL_ANCHOR_ID] = configData.devID;
		sync_message.messageData[POLL_RNUM] = instance_data.rangeNum = 0;
		sync_message.messageData[BLOCK_NUM] = configData.slotTime;
		sync_message.messageData[SLOT_NUM] = 10;
		sync_message.messageData[SLOT_NUM+1] = 0;
		for(i = 0; i < 9; i++)
		{
			sync_message.messageData[TAG_POS + i*11] = 0xFF;
			memset(&(sync_message.messageData[TAG_POS + i*11 + 1]),0,10);
		}
}

uint64 convertmicrosectodevicetimeu (double microsecu)
{
    uint64 dt;
    long double dtime;

    dtime = (microsecu / (double) DWT_TIME_UNITS) / 1e6 ;

    dt =  (uint64) (dtime) ;

    return dt;
}

void instance_txcallback(const dwt_callback_data_t *txd)
{
	OS_ERR err;
	
	TIM2->CNT = 400;
	delaySyncTxTime += instance_data.slotTime >> 8;
	OSSemPost(&SEM_SYNC_TXE, OS_OPT_POST_1, &err);		
	
	dwt_setrxtimeout(0); //reconfigure the timeout
	dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

void instance_rxcallback(const dwt_callback_data_t *rxd)
{
	uint8 rxTimeStamp[5]  = {0, 0, 0, 0, 0};

  uint8 rxd_event = 0;
	uint8 fcode_index  = 0;
//	uint8 srcAddr_index = 0;
	OS_ERR err;


    //if we got a frame with a good CRC - RX OK
  if(rxd->event == DWT_SIG_RX_OKAY)
	{

		//need to process the frame control bytes to figure out what type of frame we have received
		if(((rxd->fctrl[0] == 0x41) || (rxd->fctrl[0] == 0x61))
				&&
				((rxd->fctrl[1] & 0xCC) == 0x88)) //short address
		{

			fcode_index = 9; //function code is in first byte after source address
//			srcAddr_index = 5 + 2;
			rxd_event = DWT_SIG_RX_OKAY;
		}
		else
		{
			rxd_event = 99; //not supported - all TREK1000 frames are short addressed
		}

    //read RX timestamp
    dwt_readrxtimestamp(rxTimeStamp) ;
    dwt_readrxdata((uint8 *)&dw_event.msgu.frame[0], rxd->datalength, 0);  // Read Data Frame
		
		
    dw_event.timeStamp32l =  (uint32)rxTimeStamp[0] + ((uint32)rxTimeStamp[1] << 8) + ((uint32)rxTimeStamp[2] << 16) + ((uint32)rxTimeStamp[3] << 24);
    dw_event.timeStamp = rxTimeStamp[4];
    dw_event.timeStamp <<= 32;
    dw_event.timeStamp += dw_event.timeStamp32l;
    dw_event.timeStamp32h = ((uint32)rxTimeStamp[4] << 24) + (dw_event.timeStamp32l >> 8);


		//if Listener then just report the received frame to the instance (application)
		if(rxd_event == DWT_SIG_RX_OKAY) //Process good/known frame types
		{
//			uint16 sourceAddress = (((uint16)dw_event.msgu.frame[srcAddr_index+1]) << 8) + dw_event.msgu.frame[srcAddr_index];

				//check if this is a TWR message (and also which one)
			switch(dw_event.msgu.frame[fcode_index])
			{
				case 0x81:
				{
					instance_data.rangeNum = dw_event.msgu.frame[POLL_RNUM+fcode_index] ;
					instance_data.anchorMessageID = dw_event.msgu.frame[POLL_ANCHOR_ID+fcode_index] ;
					instance_data.anchorMessageFlag = TRUE;
					instance_data.anchorMessageTxTime = 0;
					memcpy(&(instance_data.anchorMessageTxTime), &(dw_event.msgu.frame[POLL_PTXT+fcode_index]), 4);
					instance_data.anchorMessageTxTime = instance_data.anchorMessageTxTime << 8;
					instance_data.anchorMessageRxTime = dw_event.timeStamp;
					
					OSSemPost(&SEM_DW1000_Dry, OS_OPT_POST_1, &err);
				}
				break;
				case 0x70:
				{
					instance_data.tagrangeNum = dw_event.msgu.frame[RES_RNUM+fcode_index];
					instance_data.tagMessageFlag = TRUE;
					instance_data.tagMessageTxTime = 0;
					memcpy(&(instance_data.tagMessageTxTime), &(dw_event.msgu.frame[RES_TX+fcode_index]), 4);
					instance_data.tagMessageTxTime = instance_data.tagMessageTxTime << 8;
					instance_data.tagMessageID = (int16) (/*((uint16) dw_event.msgu.frame[TAG_ID1+fcode_index] << 8)*/
																					+ dw_event.msgu.frame[TAG_ID0+fcode_index]);
					instance_data.tagMessageRxTime = dw_event.timeStamp;

					memcpy(&(instance_data.gyro_meas[0]),&(dw_event.msgu.frame[GYRO_DATA+fcode_index]),6);
					memcpy(&(instance_data.acc_meas[0]),&(dw_event.msgu.frame[ACC_DATA+fcode_index]),6);
					instance_data.imurefreshflag = dw_event.msgu.frame[IMU_REFRESH+fcode_index];
					
					OSSemPost(&SEM_DW1000_Dry, OS_OPT_POST_1, &err);					
				}
				break;

				default:  //process rx frame
				{
					dw_event.type_pend = DWT_SIG_DW_IDLE;
				}
				break;
			}
			dwt_setrxtimeout(0); //reconfigure the timeout
			dwt_rxenable(DWT_START_RX_IMMEDIATE);
		}
		else //if (rxd_event == SIG_RX_UNKNOWN) //need to re-enable the rx (got unknown frame type)
		{
			dwt_setrxtimeout(0); //reconfigure the timeout
			dwt_rxenable(DWT_START_RX_IMMEDIATE);
		}
	}
	else
	{
		dwt_setrxtimeout(0); //reconfigure the timeout
		dwt_rxenable(DWT_START_RX_IMMEDIATE);
	}
}

int32 init_dw1000()
{
    uint32 devID ;
    int result;
		dwt_config_t dwtconfigData;
		dwt_txconfig_t  configTX ;

    SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_32);  //max SPI before PLLs configured is ~4M

    //this is called here to wake up the device (i.e. if it was in sleep mode before the restart)
    devID = dwt_readdevid() ;
    if(DWT_DEVICE_ID != devID) //if the read of device ID fails, the DW1000 could be asleep
    {
        DW_CSN = 0;  //CS low
        delay_ms(1);   //200 us to wake up then waits 5ms for DW1000 XTAL to stabilise
        DW_CSN = 1;  //CS high
        delay_ms(7);
        devID = dwt_readdevid() ;
        // SPI not working or Unsupported Device ID
        if(DWT_DEVICE_ID != devID)
            return(-1) ;
        //clear the sleep bit - so that after the hard reset below the DW does not go into sleep
        dwt_softreset();
    }
		printf("ID = %08x\r\n",devID);
    //reset the DW1000 by driving the RSTn line low
    reset_DW1000();

    result = dwt_initialise(DWT_LOADUCODE | DWT_LOADLDOTUNE | DWT_LOADTXCONFIG | DWT_LOADANTDLY| DWT_LOADXTALTRIM);
		dwt_setleds(3);
		if (DWT_SUCCESS != result)
    {
        return (-1) ;   // device initialise has failed
    }
		dwt_configeventcounters(1); 
		dwt_setautorxreenable(0); //disable auto RX re-enable
    dwt_setdblrxbuffmode(0); //disable double RX buffer
		dwt_setinterrupt(DWT_INT_TFRS | DWT_INT_RFCG | (DWT_INT_ARFE | DWT_INT_RFSL | DWT_INT_SFDT | DWT_INT_RPHE | DWT_INT_RFCE | DWT_INT_RFTO | DWT_INT_RXPTO), 1);
		dwt_setcallbacks(instance_txcallback, instance_rxcallback);
		
    if (0 > result) return(-1) ; // Some failure has occurred

    SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_4); //increase SPI to max
    devID = dwt_readdevid() ;

    if (DWT_DEVICE_ID != devID)   // Means it is NOT DW1000 device
    {
        // SPI not working or Unsupported Device ID
        return(-1) ;
    }
		
		dwtconfigData.chan = configData.channleConfig.channel ;
    dwtconfigData.rxCode =  configData.channleConfig.preambleCode ;
    dwtconfigData.txCode = configData.channleConfig.preambleCode ;
    dwtconfigData.prf = configData.channleConfig.prf ;
    dwtconfigData.dataRate = configData.channleConfig.datarate ;
    dwtconfigData.txPreambLength = configData.channleConfig.preambleLength;
    dwtconfigData.rxPAC = configData.channleConfig.pacSize ;
    dwtconfigData.nsSFD = configData.channleConfig.nsSFD ;
    dwtconfigData.phrMode = DWT_PHRMODE_STD ;
    dwtconfigData.sfdTO = configData.channleConfig.sfdTO ;
		
		dwt_configure(&dwtconfigData, DWT_LOADXTALTRIM) ;
		
		configTX.PGdly = 0xc2;
		configTX.power = 0x69696969;
		
		dwt_configuretxrf(&configTX);
		
		dwt_setrxantennadelay(16495);
		dwt_settxantennadelay(16495);
		
		sync_message_init();
		instance_clearposition();
		instance_data.slotTime = convertmicrosectodevicetimeu(configData.slotTime * 1000);

    return devID;
}

void instance_putposition(position_data_t newposition)
{
	position_data[positionIdxIn] = newposition;
	position_data[positionIdxIn].position_vaild = TRUE;

	positionIdxIn++;

	if(MAX_POSITION_NUMBER == positionIdxIn)
	{
		positionIdxIn = 0;
	}
	positionNumber += 1;
}

position_data_t position_data_g;

position_data_t* instance_getposition()
{
	int indexOut = positionIdxOut;

	if(position_data[indexOut].position_vaild == FALSE)
	{
		position_data_g.position_vaild = FALSE;
		return &position_data_g;
	}

	position_data_g = position_data[indexOut] ;
	position_data_g.position_vaild = position_data[indexOut].position_vaild;

	position_data[indexOut].position_vaild = FALSE;
	positionIdxOut++;
	if(MAX_POSITION_NUMBER == positionIdxOut) //wrap the counter
	{
		positionIdxOut = 0;
	}
	positionNumber -= 1;
	return &position_data_g;
}

void instance_clearposition(void)
{
	int i = 0;

	for(i=0; i<MAX_POSITION_NUMBER; i++)
	{
     memset(&position_data[i], 0, sizeof(position_data_t));
	}

	positionIdxIn = 0;
	positionIdxOut = 0;
	positionNumber = 0;
}

int8 positionMessageHandler(uint8 *message,uint16 length)
{
	int i;
	int position_number = 0;
	for(i = 0; i <= 150 - 104; i++)
	{
		if((message[i] == 0xAA) && (message[i+1] == 0x55) && (message[i+2] == 0xA5) && (message[i+3] == 0x5A))
			break;
		if(i == 46)
			return -1;
	}
	if(message[i+4] == 0x81)
	{
		int j;
		for (j = 0; j < message[i+5] ; j++)
		{
			position_data_t newposition;
			newposition.tag_id = message[j*11 + i + 6];
			newposition.range_number = message[j*11 + i + 7];
			memcpy(&(newposition.position[0]), &(message[j*11 + i + 8]),9);
			instance_putposition(newposition);
			position_number ++;
		}
	}
	return position_number;
}
