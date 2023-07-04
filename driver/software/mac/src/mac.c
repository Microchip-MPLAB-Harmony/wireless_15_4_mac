/*******************************************************************************
  MAC Source

  File Name:
    mac.c
  Summary:
    Runs the MAC scheduler
  Description:
    This module runs the MAC scheduler.

 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
//DOM-IGNORE-END

/* === Includes ============================================================= */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "pal.h"
#include "bmm.h"
#include "qmm.h"
#include "phy.h"
#include "ieee_mac_const.h"
#include "mac_msg_const.h"
#include "mac_api.h"
#include "mac_msg_types.h"
#include "mac_data_structures.h"
#include "stack_config.h"
#include "mac_internal.h"
#include "mac_config.h"
#include "mac.h"
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
#include "mac_security.h"
#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

/* === Macros =============================================================== */

/* === Globals ============================================================== */
#define MAC_GUARD_TIME_US 1000
#define READY_TO_SLEEP    1

/**
 * Current state of the MAC state machine
 */
MAC_State_t macState;

/**
 * Current state of scanning process.
 */
MAC_ScanState_t macScanState;

/**
 * Current state of syncronization with parent
 * (either coordinator or PAN coordinator).
 */
MAC_SyncState_t macSyncState;

/**
 * Current state of MAC poll state machine,
 * e.g. polling for data, polling for Association Response, etc.
 */
MAC_PollState_t macPollState;

/**
 * Radio sleep state
 */
MAC_RadioSleepState_t macRadioSleepState;

/**
 * Global parse data
 */
Parse_t macParseData;

/*
 * Flag indicating that RxEnable is still active.
 */
bool macRxEnabled;

/*
 * Variables for duplicate detection.
 * In order to detect duplicated frames, the DSN and Source Address of the
 * last received data frame need to be stored.
 */
uint8_t macLastDsn;
uint64_t macLastSrcAddr;

#if (MAC_START_REQUEST_CONFIRM == 1)

/**
 * Holds the contents of the beacon payload.
 */
uint8_t macBeacon_Payload[aMaxBeaconPayloadLength];
#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))

/**
 * Holds the values of all security related PIB attributes.
 */
MAC_SecPib_t macSecPib;
#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

/**
 * Holds the mlme request buffer pointer, used to give the respective
 * confirmation in scan, poll and association.
 */
uint8_t *macConfBufPtr;

#if (MAC_SCAN_SUPPORT == 1)

/**
 * Stores the original channel before start of scanning.
 */
uint8_t macScanOrigChannel;

/**
 * Stores the original channel page before start of scanning.
 */
uint8_t macScanOrigPage;

#if ((MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1))

/**
 * Stores the original PAN-Id before start of scanning.
 */
uint16_t macScanOrigPanid;
#endif /* ((MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1) ||
        *(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1)) */

/**
 * Holds the buffer pointer which is used to send scan command.
 */
uint8_t *macScanCmdBufPtr;
#endif /* (MAC_SCAN_SUPPORT == 1) */

/**
 * MAC busy state, indicates whether MAC can process any
 * request from NHLE.
 */
bool macBusy;

/**
 * NHLE to MAC queue in which NHLE pushes all the requests to the MAC layer
 */
queue_t nhleMacQueue;

/**
 * Queue used by MAC for its internal operation. TAL pushes the incoming frames
 * in this queue.
 */
queue_t phyMacQueue;

#if (MAC_INDIRECT_DATA_FFD == 1)

/**
 * Queue used by MAC layer to put in indirect data. Any indirect data given by
 * NHLE is placed here by MAC, until the device polls for the data.
 */
queue_t indirectDataQueue;
#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

MAC_Pib_t macPib;

MAC_FrameInfo_t *macFrameInfo;

PHY_FrameInfo_t phyTxFrameInfo;


/* === Prototypes =========================================================== */
#ifdef ENABLE_SLEEP
static void MAC_ReadyToDeepSleep(void);
static void MAC_WakeUpFromDeepSleep(void);
#endif
/* === Implementation ======================================================= */

/**
 * @brief Runs the MAC scheduler
 *
 * This function runs the MAC scheduler.
 *
 * MLME and MCPS queues are removed alternately, starting with MLME queue.
 *
 * @return true if event is dispatched, false if no event to dispatch.
 */
bool MAC_TaskHandler(void)
{
	uint8_t *event = NULL;
	bool processedEvent = false;

	if (!macBusy) {
		/* Check whether queue is empty */
		if (nhleMacQueue.size != 0) {
			event = (uint8_t *)qmm_queue_remove(&nhleMacQueue, NULL);

			/* If an event has been detected, handle it. */
			if (NULL != event) {
				/* Process event due to NHLE requests */
				DispatchEvent(event);
				processedEvent = true;
			}
		}
	}

	/*
	 * Internal event queue should be dispatched
	 * irrespective of the dispatcher state.
	 */
	/* Check whether queue is empty */
	if (phyMacQueue.size != 0) {
		event = (uint8_t *)qmm_queue_remove(&phyMacQueue, NULL);

		/* If an event has been detected, handle it. */
		if (NULL != event) {
			DispatchEvent(event);
			processedEvent = true;
		}
	}

	return processedEvent;
}

/**
 * @brief Checks if the mac stack is ready to sleep
 *
 * Checks if the mac stack is in inactive state for beacon support
 *
 * or idle in case of no beacon support.
 *
 * @return  32bit time duration in microseconds for which the mac is ready to
 * sleep
 *
 * For No beacon support 1 if stack is idle,0 if it is busy
 */

uint32_t MAC_ReadyToSleep(void)
{
	uint32_t sleepTime = 0;
    PHY_TrxStatus_t trxStatus = PHY_GetTrxStatus();
	if (macBusy ||
			(macNhleQueue.size != 0) ||
			(nhleMacQueue.size != 0) ||
			(phyMacQueue.size != 0) ||

			(trxStatus != PHY_TRX_SLEEP)
			) {
		sleepTime = 0;
	} else {
		sleepTime = READY_TO_SLEEP;
#ifdef ENABLE_SLEEP
        MAC_ReadyToDeepSleep();
#endif
	}
	return sleepTime;
}

PHY_Retval_t sendFrame(MAC_FrameInfo_t *macFrame, uint8_t csmaType, bool frameRetry)
{
    PHY_Retval_t phyStatus;
    
    phyTxFrameInfo.buffer_header = macFrame->buffer_header;
    phyTxFrameInfo.mpdu = macFrame->mpdu;
    
    phyStatus = PHY_TxFrame(&phyTxFrameInfo, (PHY_CSMAMode_t)csmaType, frameRetry);
    
    if (phyStatus == PHY_SUCCESS)
    {
         macFrameInfo = macFrame;
    }
    
    return phyStatus;
}


#ifdef ENABLE_SLEEP

typedef __PACKED_STRUCT mac_ds_param
{
    uint64_t mac_CoordExtAddr;  
    uint64_t mac_ieee_addr;      
    uint16_t mac_CoordShtAddr; 
    uint16_t mac_short_addr;
    uint16_t panid;
    uint16_t dummy;
    uint16_t mac_max_frame_total_wait_time;
    uint16_t mac_response_wait_time; 
    uint8_t mac_state;  
    uint8_t mac_radio_sleep_state; 
    uint8_t mac_poll_state; 
    uint8_t mac_associated_PAN_coord;   
    uint8_t mac_auto_request; 
	uint8_t mac_batt_life_ext_periods; 
	uint8_t mac_dsn; 
    uint8_t phy_current_channel;
}MAC_Ds_Param_t;

static uint16_t    macshortaddr; 
static uint64_t    macieeeaddr;
static uint16_t    panid;

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
MAC_SecPib_t __attribute__ ((persistent)) macSecPibBackup;
#endif

MAC_Ds_Param_t __attribute__((persistent)) mdsParam;

static void memcpy4ByteAligned(void* outbuf, void* inbuf, uint16_t length)
{
  static uint16_t mod_size;
  static uint16_t size;
  static uint16_t k;
  uint32_t* src = (uint32_t* )inbuf;
  uint32_t* dst = (uint32_t* )outbuf;

  mod_size = (length % 4);
  // total_length is in multiple of 4
  if (mod_size !=0)
    size  = length + 4 - mod_size; 
  else 
    size  = length;

  size  = size >> 2;
  for (k = 0; k < size; k++)
  {
      *dst = *src;
      src++;
      dst++;
  }
}

static void MAC_ReadyToDeepSleep(void)
{  
    MAC_Ds_Param_t param; 
        
    param.mac_CoordExtAddr = macPib.mac_CoordExtendedAddress;
    param.mac_CoordShtAddr = macPib.mac_CoordShortAddress;
    param.mac_max_frame_total_wait_time = macPib.mac_MaxFrameTotalWaitTime;
    param.mac_response_wait_time = macPib.mac_ResponseWaitTime;    
    param.mac_state = macState;      
    param.mac_poll_state = macPollState;
    param.mac_radio_sleep_state = macRadioSleepState;    
    param.mac_associated_PAN_coord = macPib.mac_AssociatedPANCoord;
    param.mac_auto_request = macPib.mac_AutoRequest;      
    param.mac_batt_life_ext_periods = macPib.mac_BattLifeExtPeriods;
    param.mac_dsn = macPib.mac_DSN;
            
    PHY_PibGet(macPANId, (uint8_t *)&panid);
    param.panid = (uint32_t)panid;
    
    PHY_PibGet(macShortAddress, (uint8_t *)&macshortaddr);
    param.mac_short_addr = (uint32_t)macshortaddr;
    
    PHY_PibGet(macIeeeAddress, (uint8_t *)&macieeeaddr);
    param.mac_ieee_addr = macieeeaddr;
    
    uint8_t channelBeforeSleep;
    PHY_PibGet(phyCurrentChannel, &channelBeforeSleep);
    param.phy_current_channel = (uint32_t)channelBeforeSleep; 
        
    memcpy4ByteAligned(&mdsParam,&param,sizeof(mdsParam)); 
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))   
    memcpy4ByteAligned(&macSecPibBackup, &macSecPib, sizeof(macSecPib) );
#endif
} 

static void MAC_WakeUpFromDeepSleep(void)
{   
    MAC_Ds_Param_t param1;
    uint8_t channelAfterSleep;
        
    memset (&param1, 0, sizeof(param1)); 
    memcpy4ByteAligned(&param1,&mdsParam,sizeof(mdsParam));
    
    macPib.mac_CoordExtendedAddress = param1.mac_CoordExtAddr;
    macieeeaddr = param1.mac_ieee_addr;    
    macPib.mac_CoordShortAddress = param1.mac_CoordShtAddr;
    macshortaddr = param1.mac_short_addr;
    macPib.mac_MaxFrameTotalWaitTime = param1.mac_max_frame_total_wait_time;
    macPib.mac_ResponseWaitTime = param1.mac_response_wait_time;   
    macState = param1.mac_state;      
    macPollState = param1.mac_poll_state ;
    macRadioSleepState = param1.mac_radio_sleep_state;   
    macPib.mac_AssociatedPANCoord = param1.mac_associated_PAN_coord;
    macPib.mac_AutoRequest = param1.mac_auto_request;       
    macPib.mac_BattLifeExtPeriods = param1.mac_batt_life_ext_periods;
    macPib.mac_DSN = param1.mac_dsn;      
    channelAfterSleep = param1.phy_current_channel;
    panid = param1.panid;
    
    PibValue_t pibValue;
       
    pibValue.pib_value_8bit = channelAfterSleep;
    SetTalPibInternal(phyCurrentChannel, &pibValue);
    
    pibValue.pib_value_16bit = macshortaddr;
    SetTalPibInternal(macShortAddress, &pibValue);   
    
    pibValue.pib_value_64bit = macieeeaddr;
    SetTalPibInternal(macIeeeAddress, &pibValue);
    
    pibValue.pib_value_16bit = panid;
    SetTalPibInternal(macPANId, &pibValue);
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))   
    memcpy4ByteAligned(&macSecPib, &macSecPibBackup, sizeof(macSecPibBackup) );
#endif

}

/*
 * @brief MAC Wakeup Callback Function from application
 *
 */
void MAC_Wakeup(void)
{
    /* Retrieve MAC Parameters from Retention RAM after Deepsleep wakeup*/
    MAC_WakeUpFromDeepSleep();
}

#endif


/* EOF */
