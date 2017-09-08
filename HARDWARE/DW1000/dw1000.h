#include "deca_device_api.h"
#include "deca_types.h"
#include "deca_regs.h"
#include "port.h"
#include "spi.h"
#include "timer.h"
#include "sys.h"
#include "includes.h"
#include "signal.h"
#include "delay.h"
#include "stmflash.h"

#define MASTER_ANCHOR		1
#define SLAVE_ANCHOR		2
#define TEST_TXPOWER		3
#define MAX_POSITION_NUMBER (50)

typedef struct
{
    uint8 channel ;
    uint8 prf ;
    uint8 datarate ;
    uint8 preambleCode ;
    uint8 preambleLength ;
    uint8 pacSize ;
    uint8 nsSFD ;
    uint16 sfdTO ;
} chConfig_t ;

typedef struct
{
		uint8 role;
		uint8 version;
		uint8 devID;
		uint8 slotTime;
		chConfig_t channleConfig;
	
}config;

extern config configData;

typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  sequence_number 02
    uint8 panID[2];                             	//  PAN ID 03-04
    uint8 destAddr[8];             	//  05-12 using 64 bit addresses
    uint8 sourceAddr[8];           	//  13-20 using 64 bit addresses
    uint8 messageData[104] ; //  22-124 (application data and any user payload)
    uint8 fcs[2] ;                              	//  125-126  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} srd_msg_dlsl ;

typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  sequence_number 02
    uint8 panID[2];                             	//  PAN ID 03-04
    uint8 destAddr[2];             	//  05-06
    uint8 sourceAddr[2];           	//  07-08
    uint8 messageData[116] ; //  09-124 (application data and any user payload)
    uint8 fcs[2] ;                              	//  125-126  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} srd_msg_dsss ;

typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  sequence_number 02
    uint8 panID[2];                             	//  PAN ID 03-04
    uint8 destAddr[8];             	//  05-12 using 64 bit addresses
    uint8 sourceAddr[2];           	//  13-14
    uint8 messageData[110] ; //  15-124 (application data and any user payload)
    uint8 fcs[2] ;                              	//  125-126  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} srd_msg_dlss ;

typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  sequence_number 02
    uint8 panID[2];                             	//  PAN ID 03-04
    uint8 destAddr[2];             	//  05-06
    uint8 sourceAddr[8];           	//  07-14 using 64 bit addresses
    uint8 messageData[110] ; //  15-124 (application data and any user payload)
    uint8 fcs[2] ;                              	//  125-126  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} srd_msg_dssl ;

typedef struct
{
	uint8  type;			// event type - if 0 there is no event in the queue
	uint8  type_save;		// holds the event type - does not clear (used to show what event has been processed)
	uint8  type_pend;	    // set if there is a pending event
	uint16 rxLength ;		// length of RX data (does not apply to TX events)

	uint64 timeStamp ;		// last timestamp (Tx or Rx) - 40 bit DW1000 time

	uint32 timeStamp32l ;		   // last tx/rx timestamp - low 32 bits of the 40 bit DW1000 time
	uint32 timeStamp32h ;		   // last tx/rx timestamp - high 32 bits of the 40 bit DW1000 time

	uint32 uTimeStamp ;			  //32 bit system counter (ms) - STM32 tick time (at time of IRQ)

	union {
			//holds received frame (after a good RX frame event)
			uint8   frame[128];
    	srd_msg_dlsl rxmsg_ll ; //64 bit addresses
			srd_msg_dssl rxmsg_sl ;
			srd_msg_dlss rxmsg_ls ;
			srd_msg_dsss rxmsg_ss ; //16 bit addresses
	}msgu;

	//uint32 eventtime ;
	//uint32 eventtimeclr ;
	uint8 gotit;			//stores the instance function which processed the event (used for debug)
}event_data_t ;

extern event_data_t dw_event;

typedef struct
{
	uint64 slotTime;
	uint8 rangeNum;
	uint8 anchorMessageFlag;
	uint8 anchorMessageID;
	uint64 anchorMessageTxTime;
	uint64 anchorMessageRxTime;

	uint8 tagMessageFlag;
	uint16 tagMessageID;
	uint64 tagMessageTxTime;
	uint64 tagMessageRxTime;
	uint8 tagrangeNum;
  uint8 imurefreshflag;
  uint8 acc_meas[6];
  uint8 gyro_meas[6];
} instance_data_t ;

typedef struct
{
	uint8 position_vaild;
	uint8 tag_id;
	uint8 range_number;
	uint8 position[9];
}position_data_t;

extern position_data_t position_data[MAX_POSITION_NUMBER];
extern uint8 positionNumber;
extern uint8 positionIdxOut;
extern uint8 positionIdxIn;
extern instance_data_t instance_data;
extern srd_msg_dsss sync_message;

void readConfigData(void);
uint64 convertmicrosectodevicetimeu (double microsecu);
void instance_txcallback(const dwt_callback_data_t *txd);
void instance_rxcallback(const dwt_callback_data_t *rxd);
int32 init_dw1000(void);
void instance_putposition(position_data_t newposition);
position_data_t* instance_getposition(void);
void instance_clearposition(void);
int8 positionMessageHandler(uint8 *message,uint16 length);
