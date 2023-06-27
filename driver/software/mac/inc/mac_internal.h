/*******************************************************************************
  MAC Header
  
  File Name:
    mac_internal.h

  Summary:
    Declares MAC internal functions, globals, and macros.

  Description:
    This file contains MAC internal functions, globals, and macros.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef MAC_INTERNAL_H
#define MAC_INTERNAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include "pal.h"
#include "phy.h"
#include "ieee_mac_const.h"
#include "mac_data_structures.h"
#include "mac.h"
#include "mac_config.h"

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
#include "mac_msg_types.h"
#endif

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************

#define INDEX_0                      (0)  
#define INDEX_4                      (4)     
#define INDEX_8                      (8)
#define BYTE_MASK                    (0xFF)

/*
 * Beacon order used as timer interval for checking the expiration of indirect
 * transactions in a nonbeacon-enabled network
 */
#define BO_USED_FOR_MAC_PERS_TIME       (0)

/*
 * Final CAP slot in standard superframe without GTS
 */
#define FINAL_CAP_SLOT_DEFAULT          (0x0F)

#define macMaxFrameTotalWaitTime_def    (1986)

// *****************************************************************************
// *****************************************************************************
// Types
// *****************************************************************************
// *****************************************************************************

/**
 * MAC state type.
 */

typedef enum mac_state_tag {
	/*
	 * IEEE 802.15.4-defined MAC states.
	 */

	/**
	 * Node is idle,
	 * i.e. it is neither associated nor has started its own network
	 */
	MAC_IDLE = 0,

	/* Device has successfully associated */
	MAC_ASSOCIATED = 1,

	/**
	 * Coordinator successfully associated with PAN Coordinator
	 * and successfully started network with same PAN-Id
	 * (not as PAN Coordinator)
	 */
	MAC_COORDINATOR = 2,

	/** PAN coordinator successfully started */
	MAC_PAN_COORD_STARTED = 3
} MAC_State_t;

/**
 * MAC state type.
 */
typedef enum mac_superframe_state_tag {
	/*
	 * IEEE 802.15.4-defined Superframe states.
	 */
	MAC_ACTIVE_CAP = 0,
	MAC_INACTIVE,
	MAC_NOBEACON
} MAC_SuperframeState_t;

/**
 * MAC poll states.
 * These states describe the current status of the MAC for polling
 * for devices or coordinators, not for PAN coordinator.
 */
typedef enum mac_poll_state_tag {
	/**
	 * No polling ongoing.
	 */
	MAC_POLL_IDLE = 0,

	/**
	 * Successful transmission of association request frame,
	 * wait for Association response.
	 */
	MAC_AWAIT_ASSOC_RESPONSE,

	/**
	 * Explicit poll ongoing (MLME_POLL.request),
	 * Ack after Data request frame transmission received,
	 * awaiting data response. */
	MAC_POLL_EXPLICIT,

	/**
	 * Implicit poll ongoing (more pending data detected, either in beacon
	 * or
	 * data frame),
	 * awaiting data response, */
	MAC_POLL_IMPLICIT
} MAC_PollState_t;

/**
 * Device or coordinator scan states.
 */
typedef enum mac_scan_state_tag {
	/**
	 * No scanning ongoing.
	 */
	MAC_SCAN_IDLE = 0,

	/* Scanning in progress. */
	/** ED scan ongoing */
	MAC_SCAN_ED,
	/** Active scan proceeding */
	MAC_SCAN_ACTIVE,
	/** Orphan scan proceeding */
	MAC_SCAN_ORPHAN,
	/** Passive scan proceeding */
	MAC_SCAN_PASSIVE
} MAC_ScanState_t;

/**
 * Device or coordinator sync states.
 */
typedef enum mac_sync_state_tag {
	/** Do not track beacons */
	MAC_SYNC_NEVER = 0,
	/** Track the next beacon */
	MAC_SYNC_ONCE,
	/** Track beacons continuously */
	MAC_SYNC_TRACKING_BEACON,

	/**
	 * Track beacons continuously before being associated in order to
	 * obtain
	 * synchronization with desired network
	 */
	MAC_SYNC_BEFORE_ASSOC
} MAC_SyncState_t;

/**
 * MAC sleep state type.
 */
typedef enum mac_radio_sleep_state_tag {
	/**< Radio is awake */
	RADIO_AWAKE = 0,
	/**< Radio is in sleep mode */
	RADIO_SLEEPING
} MAC_RadioSleepState_t;

/* ! @} */
typedef void (*Handler_t)(uint8_t *);

/**
 * \addtogroup group_mac_ds
 * @{
 */

/* Structure implementing the MAC PIBs */
typedef struct mac_pib_tag {
	/**
	 * Holds the 64 bit address of the coordinator with which the
	 * device is associated.
	 */
	uint64_t mac_CoordExtendedAddress;

#if (MAC_INDIRECT_DATA_FFD == 1)

	/**
	 * Holds the maximum time (in superframe periods) that a indirect
	 * transaction
	 * is stored by a PAN coordinator.
	 */
	uint16_t mac_TransactionPersistenceTime;
#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

	/**
	 * Holds the 16 bit short address of the coordinator with which the
	 * device is
	 * associated. A value of 0xfffe indicates that the coordinator is only
	 * using
	 * its 64 bit extended address. A value of 0xffff indicates that this
	 * value is unknown.
	 */
	uint16_t mac_CoordShortAddress;

#if (MAC_INDIRECT_DATA_BASIC == 1)

	/**
	 * The maximum number of CAP symbols in a beaconenabled PAN, or symbols
	 * in a
	 * nonbeacon-enabled PAN, to wait either for a frame intended as a
	 * response to
	 * a data request frame or for a broadcast frame following a beacon with
	 * the
	 * Frame Pending subfield set to one.
	 * This attribute, which shall only be set by the next higher layer, is
	 * dependent upon macMinBE, macMaxBE, macMaxCSMABackoffs and the number
	 * of
	 * symbols per octet. See 7.4.2 for the formula relating the attributes.
	 * Maximum values:
	 * O-QPSK (2.4 GHz and 900 MHz for Channel page 2): 25766
	 * BPSK (900 MHz for Channel page 0): 26564
	 * Both values are valid for
	 * macMinBE = 8
	 * macMaxBE = 8
	 * macMaxCSMABackoffs = 5
	 *
	 * This PIB attribute is only used if basic indirect data transmission
	 * is used
	 * or if beacon enabled network is enabled.
	 */
	uint16_t mac_MaxFrameTotalWaitTime;
#endif  /* ((MAC_INDIRECT_DATA_BASIC == 1)) */

	/**
	 * The maximum time, in multiples of aBaseSuperframeDuration, a device
	 * shall
	 * wait for a response command frame to be available following a request
	 * command frame.
	 */
	uint16_t mac_ResponseWaitTime;

#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)

	/**
	 * Holds the value which states whether a coordinator is currently
	 * allowing
	 * association. A value of true indicates that association is permitted.
	 */
	uint8_t mac_AssociationPermit;
#endif /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */

#if (MAC_START_REQUEST_CONFIRM == 1)

	/**
	 * Holds the length, in octets, of the beacon payload.
	 */
	uint8_t mac_BeaconPayloadLength;

	/**
	 * Holds the sequence number added to the transmitted beacon frame.
	 */
	uint8_t mac_BSN;
#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)

	/**
	 * Indication of whether the device is associated to the PAN through the
	 * PAN
	 * coordinator. A value of TRUE indicates the device has associated
	 * through the
	 * PAN coordinator. Otherwise, the value is set to FALSE.
	 */
	uint8_t mac_AssociatedPANCoord;
#endif /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

	/**
	 * Holds the value which states whether a device automatically sends a
	 * data
	 * request command if its address is listed in the beacon frame. A value
	 * of true
	 * indicates that the data request command is automatically sent.
	 */
	uint8_t mac_AutoRequest;

	/**
	 * Holds the value which states the number of backoff periods during
	 * which the
	 * receiver is enabled following a beacon in battery life extension
	 * mode.
	 * This value is dependent on the currently selected logical channel.
	 */
	uint8_t mac_BattLifeExtPeriods;

	/**
	 * Holds the sequence number of the transmitted data or command frame.
	 */
	uint8_t mac_DSN;

	/**
	 * Holds the value which states whether the MAC sublayer is to enable
	 * its
	 * receiver during idle periods.
	 */
	bool mac_RxOnWhenIdle;

	/**
	 * Indication of whether the MAC sublayer has security enabled. A value
	 * of
	 * TRUE indicates that security is enabled, while a value of FALSE
	 * indicates
	 * that security is disabled.
	 */
	bool mac_SecurityEnabled;

#ifdef TEST_HARNESS
	/* Private MAC PIB variables, only valid for testing purposes */

	/**
	 * Holds the private MAC PIB attribute to generate a frame with an
	 * illegale
	 * frame type.
	 */
	uint8_t privateIllegalFrameType;

	/**
	 * Holds the private MAC PIB attribute which suppresses the initiation
	 * of a
	 * data request frame after association request.
	 */
	uint8_t privateNoDataAfterAssocReq;

	/**
	 * Holds the private MAC PIB attribute to pretend virtual Beacon-enabled
	 * PANs.
	 */
	uint8_t privateVirtualPANs;
#endif /* TEST_HARNESS */
} MAC_Pib_t;

/* === Externals ============================================================ */

/* Global data variables */
extern uint8_t *macConfBufPtr;

#if (MAC_SCAN_SUPPORT == 1)
extern uint8_t *macScanCmdBufPtr;
extern uint8_t macScanOrigChannel;
extern uint8_t macScanOrigPage;
#if ((MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1))
extern uint16_t macScanOrigPanid;
#endif  /* ((MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1) ||
         *(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1)) */
#endif /* (MAC_SCAN_SUPPORT == 1) */

#if (MAC_START_REQUEST_CONFIRM == 1)
extern uint8_t macBeacon_Payload[];
#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */

extern queue_t phyMacQueue;

#if (MAC_INDIRECT_DATA_FFD == 1)
extern queue_t indirectDataQueue;
#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

/* === Externals ============================================================ */
extern uint8_t macLastDsn;
extern uint64_t macLastSrcAddr;
extern Parse_t macParseData;
extern MAC_RadioSleepState_t macRadioSleepState;
extern bool macBusy;
extern bool macRxEnabled;
extern MAC_State_t macState;
extern MAC_ScanState_t macScanState;
extern MAC_SyncState_t macSyncState;
extern MAC_PollState_t macPollState;
extern MAC_Pib_t macPib;

/* === Prototypes =========================================================== */

/**
 * \ingroup group_mac
 * \defgroup group_mcl  MAC Core Layer(MCL)
 *
 * The MAC Core Layer (MCL) abstracts and implements IEEE 802.15.4-2006
 * compliant behavior for non-beacon enabled and beacon-enabled network support.
 *
 */
/*@{*/

/* MAC-internal functions -- please keep them alphabetically sorted. */

#if (MAC_START_REQUEST_CONFIRM == 1)
void MAC_BuildAndTxBeacon(bool beaconEnabled,
		buffer_t *beaconBufferHeader);
#endif /* (MAC_START_REQUEST_CONFIRM == 1) */

#if (MAC_INDIRECT_DATA_BASIC == 1)
bool MAC_BuildAndTxDataReq(bool explPoll,
		bool forceOwnLongAddr,
		uint8_t explDestAddrMode,
		AddressField_t *explDestAddr,
		uint16_t explDestPanId);

#endif /* (MAC_INDIRECT_DATA_BASIC == 1) */

#if (MAC_START_REQUEST_CONFIRM == 1)
void MAC_CoordRealignmentCommandTxSuccess(uint8_t txStatus,
		buffer_t *bufPtr);

#endif /* (MAC_START_REQUEST_CONFIRM == 1) */

#ifdef ENABLE_TSTAMP
void MAC_GenMCPSDataConf(buffer_t *bufPtr,
		uint8_t status,
		uint8_t handle,
		uint32_t timestamp);

#else
void MAC_GenMCPSDataConf(buffer_t *bufPtr,
		uint8_t status,
		uint8_t handle);

#endif  /* ENABLE_TSTAMP */

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)
void MAC_GenMLMEAssociateConf(buffer_t *bufPtr,
		uint8_t status,
		uint16_t assocShortAddr);

#endif /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

#if (MAC_INDIRECT_DATA_FFD == 1)
void MAC_HandleTxNullDataFrame(void);

#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

void MAC_IdleTrans(void);

#if (MAC_COMM_STATUS_INDICATION == 1)
void MAC_MLME_CommStatus(uint8_t status,
		buffer_t *bufPtr);

#endif /* (MAC_COMM_STATUS_INDICATION == 1) */

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)
void MAC_PrepDisassocConf(buffer_t *bufPtr,
		uint8_t status);

#endif /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)
void MAC_ProcessAssociateRequest(buffer_t *bufPtr);

#endif /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)
void MAC_ProcessAssociateResponse(buffer_t *bufPtr);

#endif /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

#if (MAC_SCAN_SUPPORT == 1)
void MAC_ProcessBeaconFrame(buffer_t *msg);

#endif /* (MAC_SCAN_SUPPORT == 1) */

#if (MAC_START_REQUEST_CONFIRM == 1)
void MAC_ProcessBeaconRequest(buffer_t *bufPtr);

#endif /* (MAC_START_REQUEST_CONFIRM == 1) */

#if (MAC_SYNC_LOSS_INDICATION == 1)
void MAC_ProcessCoordRealign(buffer_t *bufPtr);
void MAC_SyncLoss(uint8_t lossReason);

#endif /* (MAC_SYNC_LOSS_INDICATION == 1) */

#if (MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1)
void MAC_ProcessOrphanRealign(buffer_t *bufPtr);

#endif /* (MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1) */

#if (MAC_INDIRECT_DATA_FFD == 1)
void MAC_ProcessDataRequest(buffer_t *bufPtr);

#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

#if (MAC_INDIRECT_DATA_BASIC == 1)
void MAC_ProcessDataResponse(void);

#endif /* (MAC_INDIRECT_DATA_BASIC == 1) */

void MAC_ProcessDataFrame(buffer_t *bufPtr);

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)
void MAC_ProcessDisassociateNotification(buffer_t *bufPtr);

#endif /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

#if (MAC_ORPHAN_INDICATION_RESPONSE == 1)
void MAC_ProcessOrphanNotification(buffer_t *bufPtr);

#endif /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) */

void MAC_ProcessTalDataInd(uint8_t *msg);

void MAC_SleepTrans(void);

#if ((MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1))
void MAC_ScanSendComplete(MAC_Retval_t status);

#endif /* ((MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1) ||
        *(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1)) */

#if (MAC_INDIRECT_DATA_FFD == 1)
void MAC_StartPersistenceTimer(void);

#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)
void MAC_Timer_AssocresponsetimeCb(void *callbackParameter);

#endif /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

#if ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || (MAC_START_REQUEST_CONFIRM == 1))
bool MAC_TxCoordRealignmentCommand(FrameMsgtype_t cmdType,
		buffer_t *bufPtr,
		uint16_t newPanid,
		uint8_t newChannel,
		uint8_t newPage);

#endif /* ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || (MAC_START_REQUEST_CONFIRM
        *== 1)) */

void MAC_TrxInitSleep(void);
void MAC_TrxWakeup(void);

/* Timer callbacks */
#if (MAC_INDIRECT_DATA_BASIC == 1)
void MAC_Timer_PollWaitTimeCb(void *callbackParameter);

#endif /* (MAC_INDIRECT_DATA_BASIC == 1) */
#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)
void MAC_Timer_ResponseWaitCb(void *callbackParameter);

#endif /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

void DispatchEvent(uint8_t *event);

MAC_Retval_t SetTalPibInternal(uint8_t attribute, PibValue_t *attributeValue);

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
MAC_Retval_t MAC_BuildAuxSecHeader(uint8_t **framePtr, MCPS_DataReq_t *pmdr,
		uint8_t *frameLen);
MAC_Retval_t MAC_Secure(MAC_FrameInfo_t *frame, uint8_t *macPayloadPtr,
		MCPS_DataReq_t *pmdr);
MAC_Retval_t MAC_Unsecure(Parse_t *macParseDataBuf, uint8_t *mpdu,
		uint8_t *macPayload, uint8_t *payloadIndex);

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

/*
 * Function:
 *  PHY_Retval_t sendFrame(MAC_FrameInfo_t *txFrame, PHY_CSMAMode_t csmaType,
 *                            bool frameRetry)
 *
 * Summary:
 *  Request to transmit MAC frame
 */

PHY_Retval_t sendFrame(MAC_FrameInfo_t *macFrame, uint8_t csmaType, bool frameRetry);
/*@}*/

MAC_Retval_t MAC_TimersInit(void);

MAC_Retval_t MAC_TimersStop(void);

#if (MAC_INDIRECT_DATA_FFD == 1)

/*
 * @brief Checks whether the Persistence timer needs to be started
 *
 * If an FFD does have pending data, the MAC persistence timer
 * needs to be started.
 */
static inline void MAC_CheckPersistenceTimer(void)
{
	if (!PAL_TimerIsRunning(Timer_DataPersistence)) {
		MAC_StartPersistenceTimer();
	}
}

#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

/**
 * This macro sets the MAC to busy
 */

#define MAKE_MAC_BUSY()             do { \
		macBusy = true; \
} \
	while (0)


/**
 * This macro sets the MAC to not busy
 */

#define MAKE_MAC_NOT_BUSY()         do { \
		macBusy = false; \
} \
	while (0)

/**
 * @brief Converts a 64-Bit value into  a 8 Byte array
 *
 * @param[in] value 64-Bit value
 * @param[out] data Pointer to the 8 Byte array to be updated with 64-Bit value
 * @ingroup apiPalApi
 */
static inline void convert_64_bit_to_byte_array(uint64_t value, uint8_t *data)
{
    uint8_t index = INDEX_0;

    while (index < INDEX_8)
    {
        data[index++] = value & BYTE_MASK;
        value = value >> INDEX_8;
    }
}

/**
 * @brief Converts a 16-Bit value into  a 2 Byte array
 *
 * @param[in] value 16-Bit value
 * @param[out] data Pointer to the 2 Byte array to be updated with 16-Bit value
 * @ingroup apiPalApi
 */
static inline void convert_16_bit_to_byte_array(uint16_t value, uint8_t *data)
{
    data[0] = value & BYTE_MASK;
    data[1] = (value >> INDEX_8) & BYTE_MASK;
}

/* Converts a 16-Bit value into a 2 Byte array */
static inline void convert_spec_16_bit_to_byte_array(uint16_t value, uint8_t *data)
{
    data[0] = value & BYTE_MASK;
    data[1] = (value >> INDEX_8) & BYTE_MASK;
}

/* Converts a 16-Bit value into a 2 Byte array */
static inline void convert_16_bit_to_byte_address(uint16_t value, uint8_t *data)
{
    data[0] = value & BYTE_MASK;
    data[1] = (value >> INDEX_8) & BYTE_MASK;
}

/*
 * @brief Converts a 2 Byte array into a 16-Bit value
 *
 * @param data Specifies the pointer to the 2 Byte array
 *
 * @return 16-Bit value
 * @ingroup apiPalApi
 */
static inline uint16_t convert_byte_array_to_16_bit(uint8_t *data)
{
    return (data[0] | ((uint16_t)data[1] << INDEX_8));
}

/**
 * @brief Converts a 8 Byte array into a 64-Bit value
 *
 * @param data Specifies the pointer to the 8 Byte array
 *
 * @return 64-Bit value
 * @ingroup apiPalApi
 */
static inline uint64_t convert_byte_array_to_64_bit(uint8_t *data)
{
    union
    {
        uint64_t u64;
        uint8_t u8[8];
    } long_addr;

    uint8_t index;

    for (index = INDEX_0; index < INDEX_8; index++)
    {
        long_addr.u8[index] = *data++;
    }

    return long_addr.u64;
}

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* MAC_INTERNAL_H */
/* EOF */
