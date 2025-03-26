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
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

/*
 * Variable to hold the last received RSSI of the frame.
 * In PHY RX frame callback, this frame RSSI value will be updated, whenever a
 * PHY frame is received.
 * When next frame is received, this value will be overwritten with new frame 
 * RSSI value.
 */
uint8_t frameRSSI = UINT8_MAX;

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
		if (nhleMacQueue.size != 0U) {
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
	if (phyMacQueue.size != 0U) {
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
			(macNhleQueue.size != 0U) ||
			(nhleMacQueue.size != 0U) ||
			(phyMacQueue.size != 0U) ||

			(trxStatus != PHY_TRX_SLEEP)
			) {
		sleepTime = 0;
	} else {
		sleepTime = READY_TO_SLEEP;
        MAC_ReadyToDeepSleep();
	}
	return sleepTime;
}

PHY_Retval_t sendFrame(MAC_FrameInfo_t *macFrame, PHY_CSMAMode_t csmaType, bool frameRetry)
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

void __attribute__((weak)) MAC_ReadyToDeepSleep(void)
{
	
}

/* EOF */
