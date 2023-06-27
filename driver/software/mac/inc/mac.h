/*******************************************************************************
  MAC Header
  
  File Name:
    mac.h

  Summary:
    This file contains MAC API function declarations

  Description:
   The IEEE standard 802.15.4 MAC Layer defines the protocol and compatible
   interconnection for data communication devices using low data rate, low 
   power and low complexity, short-range radio frequency (RF) transmissions in a
   wireless personal area network (WPAN).

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

#ifndef MAC_H
#define MAC_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "phy.h"
#include "qmm.h"
#include "mac_config.h"

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

#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1)      ||	\
	(MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1)  || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1))

// *****************************************************************************
/* Maximum allowed PAN Descriptors
 
   Summary:
    This macro defines the Maximum allowed PAN descriptors this is calculated 
	using the large buffer size
   Description:
    None
   Remarks:
    None 
 */
        
#define MAX_ALLOWED_PAN_DESCRIPTORS     ((LARGE_BUFFER_SIZE - \
	sizeof(MLME_ScanConf_t) / sizeof(WPAN_Pandescriptor_t)))

// *****************************************************************************
/* Maximum PAN Descriptors
 
   Summary:
    This macro defines the Active/passive scan: implementation-defined maximum 
	number of PANDescriptors that can be stored
   Description:
    None
   Remarks:
    None 
 */
        
#define MAX_PANDESCRIPTORS \
	(MAX_ALLOWED_PAN_DESCRIPTORS > 5 ? 5 : MAX_ALLOWED_PAN_DESCRIPTORS)
#endif

// *****************************************************************************
/* FCF address Mask
 
   Summary:
    This macro defines the mask for the FCF address mode
   Description:
    None
   Remarks:
    None 
 */
        
#define FCF_ADDR_MASK                   (3)

// *****************************************************************************
/* FCF source address mode
 
   Summary:
    This macro defines to get the source address mode
   Description:
    None
   Remarks:
    None 
 */
        
#define FCF_GET_SOURCE_ADDR_MODE(x) \
	(((x) >> FCF_SOURCE_ADDR_OFFSET) & FCF_ADDR_MASK)

// *****************************************************************************
/* FCF destination address mode
 
   Summary:
    This macro defines to get the destination address mode
   Description:
    None
   Remarks:
    None 
 */
        
#define FCF_GET_DEST_ADDR_MODE(x) \
	(((x) >> FCF_DEST_ADDR_OFFSET) & FCF_ADDR_MASK)

#define PGM_READ_BYTE(x) *(x)
#define PGM_READ_WORD(x) *(x)
#define PGM_READ_BLOCK(dst, src, len) memcpy((dst), (src), (len))
#define ADDR_COPY_DST_SRC_16(dst, src) ((dst) = (src))
#define ADDR_COPY_DST_SRC_64(dst, src) ((dst) = (src))


// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* MAC Return Values
 
   Summary:
    List of return status for the MAC functions
 
   Description:
    None
   Remarks:
    None 
 */
        
typedef enum mac_return_value_tag {
    /* General Success condition*/
    MAC_SUCCESS                 = 0x00,
	/* General failure condition*/
	FAILURE                 = 0x85,
	/* Invalid Frame counter value defined by 802.15.4 */
	MAC_COUNTER_ERROR           = 0xDB,
	/* Unallowed key in received frame defined by 802.15.4 */
	MAC_IMPROPER_KEY_TYPE       = 0xDC, 
	/* Required security level not applied within received frame defined by 802.15.4 */
	MAC_IMPROPER_SECURITY_LEVEL = 0xDD, 
	/* Received frame was secured using 802.15.4-2003 security */
	MAC_UNSUPPORTED_LEGACY      = 0xDE, 
	/* Not supported applied security defined by 802.15.4 */
	MAC_UNSUPPORTED_SECURITY    = 0xDF, 
	/* Loss of beacons defined by 802.15.4 */
	MAC_BEACON_LOSS             = 0xE0,
	/* Channel access failure defined by 802.15.4 */
	MAC_CHANNEL_ACCESS_FAILURE  = 0xE1, 
	/* GTS Request denied by PANC defined by 802.15.4 */
	MAC_DENIED                  = 0xE2, 
	/* Disabling of TRX failed defined by 802.15.4 */
	MAC_DISABLE_TRX_FAILURE     = 0xE3, 
	/* Failed cryptographic processing of the received secured frame defined by 802.15.4 */
	MAC_SECURITY_ERROR          = 0xE4, 
	/* Current frame is too long defined by 802.15.4 */
	MAC_FRAME_TOO_LONG          = 0xE5,
	/*< Invalid GTS defined by 802.15.4 */	
	MAC_INVALID_GTS             = 0xE6, 
	/*< Invalid handle defined by 802.15.4 */
	MAC_INVALID_HANDLE          = 0xE7, 
	/* Invalid Parameter defined by 802.15.4 */
	MAC_INVALID_PARAMETER       = 0xE8, 
	/* No ACK defined by 802.15.4 */
	MAC_NO_ACK                  = 0xE9, 
	/* No beacon defined by 802.15.4 */
	MAC_NO_BEACON               = 0xEA, 
	/* NO data defined by 802.15.4 */
	MAC_NO_DATA                 = 0xEB, 
	/* No valid short address defined by 802.15.4 */
	MAC_NO_SHORT_ADDRESS        = 0xEC,
	/* Out of CA defined by 802.15.4-2003, deprecated in 802.15.4-2006 */
	MAC_OUT_OF_CAP              = 0xED,
	/* PAN ID conflict defined by 802.15.4 */
	MAC_PAN_ID_CONFLICT         = 0xEE,
	/* Realignment defined by 802.15.4 */
	MAC_REALIGNMENT             = 0xEF,
	/* Transaction expired defined by 802.15.4 */
	MAC_TRANSACTION_EXPIRED     = 0xF0,
	/* Transaction overflow defined by 802.15.4 */	
	MAC_TRANSACTION_OVERFLOW    = 0xF1, 
	/* Tx active defined by 802.15.4 */
	MAC_TX_ACTIVE               = 0xF2, 
	/* Unavailable key or blacklisted device defined by 802.15.4 */
	MAC_UNAVAILABLE_KEY         = 0xF3, 
	/* Unsupported attribute defined by 802.15.4 */
	MAC_UNSUPPORTED_ATTRIBUTE   = 0xF4,
	/* Invalid address defined by 802.15.4 */	
	MAC_INVALID_ADDRESS         = 0xF5, 
	/* Receiver could not be enabled as defined by 802.15.4 */
	MAC_PAST_TIME               = 0xF7, 
	/* Invalid index in table of MAC PIB attribute defined by 802.15.4 */
	MAC_INVALID_INDEX           = 0xF9, 
	/* Scan operation terminated prematurely defined by 802.15.4 */
	MAC_LIMIT_REACHED           = 0xFA,
	/* SET request issued for read only attribute defined by 802.15.4 */	
	MAC_READ_ONLY               = 0xFB, 
	/* Scan operation failed because ofongoing scan defined by 802.15.4 */
	MAC_SCAN_IN_PROGRESS        = 0xFC, 
	/* Frame pending at PHY */
    MAC_FRAME_PENDING           = 0xFD                          
            
} MAC_Retval_t;

// *****************************************************************************
/* MAC Message types
 
   Summary:
    Enumeration for MAC Message types
 
   Description:
    None
   Remarks:
    None 
 */

typedef enum {
	/* MAC Command Frames (table 67) */
	/* Command Frame Identifier for Association Request */
	ASSOCIATIONREQUEST          = (0x01),
	/* Command Frame Identifier for Association Response */
	ASSOCIATIONRESPONSE,
	/* Command Frame Identifier for Disassociation Notification */
	DISASSOCIATIONNOTIFICATION,
	/* Command Frame Identifier for Data Request */
	DATAREQUEST,
	/* Command Frame Identifier for PANID Conflict Notification */
	PANIDCONFLICTNOTIFICAION,
	/* Command Frame Identifier for Orphan Notification */
	ORPHANNOTIFICATION,
	/* Command Frame Identifier for Beacon Request */
	BEACONREQUEST,
	/* Command Frame Identifier for Coordinator Realignment */
	COORDINATORREALIGNMENT,
    /* Command Frame Identifier for gts request */
	GTSREQUEST,

	/*
	 * These are not MAC command frames but listed here as they are needed
	 * in the msgtype field
	 */
	/* Message is a directed orphan realignment command */
	ORPHANREALIGNMENT,
	/* Message is a beacon frame (in response to a beacon request cmd) */
	BEACON_MESSAGE,
	/* Message type field value for implicite poll without request */
	DATAREQUEST_IMPL_POLL,
	/* Message type field value for Null frame */
	NULL_FRAME,
	/* Message type field value for MCPS message */
	MCPS_MESSAGE

} FrameMsgtype_t;

// *****************************************************************************
/* MAC Frame information structure
 
   Summary:
    MAC_FrameInfo_t holds the data to be transmitted and frame information structure
 
   Description:
    None
 
   Remarks:
    None 
 */

typedef struct mac_frame_info_tag
{
	/** Message type of frame */
	FrameMsgtype_t msgType;
	/** Pointer to buffer header of frame */
	buffer_t *buffer_header;
	/** MSDU handle */
	uint8_t msduHandle;
	/** Indirect frame persistence time */
	uint16_t persistenceTime;
	/** Indirect frame transmission ongoing */
	bool indirectInTransit;
#ifdef MAC_SECURITY_ZIP
	/** MAC Payload Pointer */
	uint8_t *macPayload;
#endif

#ifdef ENABLE_TSTAMP

	/** Timestamp information of frame
	 * The time stamping is only required for beaconing networks
	 * or if time stamping is explicitly enabled.
	 */
	uint32_t timeStamp;
#endif  /* ENABLE_TSTAMP */
	/** Pointer to MPDU */
	uint8_t *mpdu;
} MAC_FrameInfo_t;
// *****************************************************************************
// *****************************************************************************
// Externals
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* externals
   Summary:
    Queue used by MAC for communication to next higher layer 
   Description:
    None
   Remarks:
    None 
 */
extern queue_t macNhleQueue;

extern queue_t nhleMacQueue;

/* === Prototypes =========================================================== */

/* 802.15.4 MAC layer entries */

/**
 * @brief Builds the data frame for transmission
 *
 * This function builds the data frame for transmission.
 * The NWK layer has supplied the parameters.
 * The MAC_FrameInfo_t data type is constructed and filled in.
 * Also the FCF is constructed based on the parameters passed.
 *
 * \ingroup group_mac_req_int
 * @param msg Pointer to the MCPS-DATA.request parameter
 */

void MAC_MCPS_DataRequest(uint8_t *msg);

/**
 * @brief Wrapper function for messages of type MCPS_DataConf_t
 *
 * This function is a callback for mcps data confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */

void MAC_MCPS_DataConf(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MCPS_DataInd_t
 *
 * This function is a callback for mcps data indication
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */

void MAC_MCPS_DataInd(uint8_t *m);

#if ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))

/**
 * @brief Processes a MCPS-PURGE.request primitive
 *
 * This functions processes a MCPS-PURGE.request from the NHLE.
 * The MCPS-PURGE.request primitive allows the next higher layer
 * to purge an MSDU from the transaction queue.
 * On receipt of the MCPS-PURGE.request primitive, the MAC sublayer
 * attempts to find in its transaction queue the MSDU indicated by the
 * msduHandle parameter. If an MSDU matching the given handle is found,
 * the MSDU is discarded from the transaction queue, and the MAC
 * sublayer issues the MCPSPURGE. confirm primitive with a status of
 * MAC_SUCCESS. If an MSDU matching the given handle is not found, the MAC
 * sublayer issues the MCPS-PURGE.confirm primitive with a status of
 * INVALID_HANDLE.
 *
 * \ingroup group_mac_req_int
 * @param msg Pointer to the MCPS-PURGE.request parameter
 */
void MAC_MCPS_PurgeRequest(uint8_t *msg);

/**
 * @brief Wrapper function for messages of type MCPS_PurgeConf_t
 *
 * This function is a callback for mcps purge confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MCPS_PurgeConf(uint8_t *m);

#endif  /* ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))
        **/

#if (MAC_GET_SUPPORT == 1)

/**
 * @brief Handles an MLME-GET.request
 *
 * This function handles an MLME-GET.request.
 * The MLME-GET.request primitive requests information about a
 * given PIB attribute.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the request structure
 */

void MAC_MLME_GetRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_GetConf_t
 *
 * This function is a callback for mlme get confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_GetConf(uint8_t *m);

#endif  /* (MAC_GET_SUPPORT == 1) */

/**
 * @brief Resets the MAC layer
 *
 * The MLME-RESET.request primitive allows the next higher layer to request
 * that the MLME performs a reset operation.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the MLME_RESET.request given by the NHLE
 */
void MAC_MLME_ResetRequest(uint8_t *m);

#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1) || (MAC_SCAN_ACTIVE_REQUEST_CONFIRM == \
	1)  || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1))

/**
 * @brief The MLME-SCAN.request primitive makes a request for a node to
 * start a scan procedure.
 *
 * 802.15.4. Section 7.1.11.1.
 *
 * The MLME-SCAN.request primitive is used to initiate a channel scan over a
 * given list of channels. A device can use a channel scan to measure the
 * energy on the channel, search for the coordinator with which it associated,
 * or search for all coordinators transmitting beacon frames within the
 * POS of the scanning device.
 *
 * The MLME-SCAN.request primitive is generated by the next higher layer and
 * issued to its MLME to initiate a channel scan to search for activity within
 * the POS of the device. This primitive can be used to perform an ED scan to
 * determine channel usage, an active or passive scan to locate beacon frames
 * containing any PAN identifier, or an orphan scan to locate a PAN to which
 * the device is currently associated.
 *
 * ED or active scans can be performed before an FFD starts operation as a
 * PAN coordinator. Active or passive scans can be performed prior to selecting
 * a PAN for association. Orphan scans can be performed to attempt to locate a
 * specific coordinator with which communication has been lost.
 *
 * All devices shall be capable of performing passive scans and orphan scans;
 * ED scans and active scans are optional for an RFD.
 *
 * When the MLME receives the MLME-SCAN.request primitive, it initiates a scan
 * in all channels specified in the ScanChannels parameter. The MLME suspends
 * all beacon transmissions for the duration of the scan. During a scan, the
 * MAC sublayer only accepts frames received over the PHY data service that are
 * relevant to the scan being performed (see 7.5.2.1).
 *
 * An ED scan allows a device to obtain a measure of the peak energy in each
 * requested channel. The ED scan is performed on each channel by the MLMEs
 * repeatedly issuing the PLME-ED.request primitive to the PHY until
 * [aBaseSuperframeDuration * (2n + 1)] symbols, where n is the value of the
 * ScanDuration parameter, have elapsed. The MLME notes the maximum energy
 * measurement and moves on to the next channel in the channel list. A device
 * will be able to store between one and an implementation-specified maximum
 * number of channel ED measurements. The ED scan terminates when the number
 * of channel ED measurements stored equals this implementation-specified
 * maximum or when every channel specified in the channel list has been scanned.
 *
 * An active scan is used by an FFD to locate all coordinators transmitting
 * beacon frames within its POS. The active scan is performed on each channel
 * by the MLMEs first sending a beacon request command (see 7.3.2.4). The MLME
 * then enables the receiver and records the information contained in each
 * received beacon in a PAN descriptor structure (see Table 41 in 7.1.5.1.1).
 * A device will be able to store between one and an implementation-specified
 * maximum number of PAN descriptors. The active scan on a particular channel
 * terminates when the number of PAN descriptors stored equals this
 * implementation-specified maximum or when [aBaseSuperframeDuration*(2n + 1)]
 * symbols, where n is the value of the ScanDuration parameter, have elapsed.
 * If the latter condition has been satisfied, the channel is considered to
 * have been scanned. Where possible, the scan is repeated on each channel and
 * terminates when the number of PAN descriptors stored equals the
 * implementation-specified maximum or when every channel in the set of
 *  available channels has been scanned.
 *
 * A passive scan, like an active scan, is used to locate all coordinators
 * transmitting beacon frames within the POS of the scanning device; the
 * difference is that the passive scan is a receive-only operation and does not
 * transmit the beacon request command. The passive scan is performed on each
 * channel by the MLMEs enabling its receiver and recording the information
 * contained in each received beacon in a PAN descriptor structure
 * (see Table 41 in 7.1.5.1.1). A device will be able to store between one and
 * an implementation-specified maximum number of PAN descriptors. The passive
 * scan on a particular channel terminates when the number of PAN descriptors
 * stored equals this implementation-specified maximum or when
 * [aBaseSuperframeDuration * (2n + 1)] symbols, where n is the value of the
 * ScanDuration parameter, have elapsed. If the latter condition has been
 * satisfied, the channel is considered to have been scanned. Where possible,
 * the scan is repeated on each channel and terminates when the number of PAN
 * descriptors stored equals the implementation-specified maximum or when
 * every channel in the set of available channels has been scanned.
 *
 * An orphan scan is used to locate the coordinator with which the scanning
 * device had previously associated. The orphan scan is performed on each
 * channel by the MLME first sending an orphan notification command
 * (see 7.3.2.3). The MLME then enables its receiver for at most
 * aResponseWaitTime symbols. If the device receives a coordinator realignment
 * command within this time, the MLME will disable its receiver. Otherwise, the
 * scan is repeated on the next channel in the channel list. The scan
 * terminates when the device receives a coordinator realignment command
 * (see 7.3.2.5) or when every channel in the set of available channels has
 * been scanned.
 *
 * The results of an ED scan are recorded in a list of ED values and reported
 * to the next higher layer through the MLME-SCAN.confirm primitive with a
 * status of MAC_SUCCESS. The results of an active or passive scan are recorded
 * in a set of PAN descriptor values and reported to the next higher layer
 * through the MLME-SCAN.confirm primitive. If no beacons were found, the
 * MLME-SCAN.confirm primitive will contain a null set of PAN descriptor
 * values and a status of NO_BEACON. Otherwise, the MLME-SCAN.confirm primitive
 * will contain the set of PANDescriptor values found, a list of unscanned
 * channels, and a status of MAC_SUCCESS.
 *
 * The results of an orphan scan are reported to the next higher layer through
 * the MLME-SCAN.confirm primitive. If successful, the MLME-SCAN.confirm
 * primitive will contain a status of MAC_SUCCESS. If the device did not receive
 * a
 * coordinator realignment command, MLME-SCAN.confirm primitive will contain
 * a status of NO_BEACON. In either case, the PANDescriptorList and
 * EnergyDetectList parameters of the MLMESCAN.confirm primitive are null.
 *
 * If any parameter in the MLME-SCAN.request primitive is not supported or is
 * out of range, the MAC sublayer will issue the MLME-SCAN.confirm primitive
 * with a status of MAC_INVALID_PARAMETER.
 *
 * \ingroup group_mac_req_int
 * @param m The MLME_SCAN.request message
 */

void MAC_MLME_ScanRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_ScanConf_t
 *
 * This function is a callback for mlme scan confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_ScanConf(uint8_t *m);

#endif

#if (MAC_START_REQUEST_CONFIRM == 1)

/**
 * @brief The MLME-START.request primitive makes a request for the device to
 * start using a new superframe configuration
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to MLME_START.request message issued by the NHLE
 */
void MAC_MLME_StartRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_StartConf_t
 *
 * This function is a callback for mlme start confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_StartConf(uint8_t *m);

#endif /* (MAC_START_REQUEST_CONFIRM == 1) */

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)

/**
 * @brief Handles the MLME associate request command from the NWK layer
 *
 * The MLME associate request primitive is generated by the next higher layer
 * of an unassociated device and issued to its MAC to request an association
 * with a coordinator.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to MLME association request parameters
 */

void MAC_MLME_AssociateRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_AssociateConf_t
 *
 * This function is a callback for mlme associate confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_AssociateConf(uint8_t *m);

#endif  /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */
#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)

/**
 * @brief Entry point from the stack for MLME associate response
 *
 * The MLME associate response primitive is used to initiate a response to a
 * MLME association indication primitive.
 *
 * \ingroup group_mac_resp_int
 * @param m Pointer to association response parameters
 */
void MAC_MLME_AssociateResponse(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_AssociateInd_t
 *
 * This function is a callback for mlme associate indication.
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */
void MAC_MLME_AssociateInd(uint8_t *m);

#endif /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */
#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/**
 * @brief Handles the MLME disassociate request command from the NWK layer
 *
 * The MLME-DISASSOCIATE.request primitive is generated by the next
 * higher layer of an associated device and issued to its MLME to
 * request disassociation from the PAN. It is also generated by the
 * next higher layer of the coordinator and issued to its MLME to
 * instruct an associated device to leave the PAN.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the MLME-DISASSOCIATION.Request message passed by NHLE
 */
void MAC_MLME_DisassociateRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_DisassociateConf_t
 *
 * This function is a callback for mlme disassociate confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_DisassociateConf(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_DisassociateInd_t
 *
 * This function is a callback for mlme disassociate indication.
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */
void MAC_MLME_DisassociateInd(uint8_t *m);

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */
#if (MAC_ORPHAN_INDICATION_RESPONSE == 1)

/**
 * @brief Implements the MLME-ORPHAN.response
 *
 * The MLME-ORPHAN.response primitive allows the next higher layer
 * of a coordinator to respond to the MLME-ORPHAN.indication primitive.
 * The MLME-ORPHAN.response primitive is generated by the next higher
 * layer and issued to its MLME when it reaches a decision about whether
 * the orphaned device indicated in the MLME-ORPHAN.indication primitive
 * is associated.
 *
 * \ingroup group_mac_resp_int
 * @param m Pointer to the message.
 */
void MAC_MLME_OrphanResponse(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_OrphanInd_t
 *
 * This function is a callback for mlme orphan indication.
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */
void MAC_MLME_OrphanInd(uint8_t *m);

#endif /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) */
#if (MAC_INDIRECT_DATA_BASIC == 1)

/**
 * @brief Implements MLME-POLL.request
 *
 * This function handles an MLME-POLL.request primitive.
 * The MLME-POLL.request primitive is generated by the next
 * higher layer and issued to its MLME when data are to be
 * requested from a coordinator.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the message
 */
void MAC_MLME_PollRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_PollConf_t
 *
 * This function is a callback for mlme poll confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_PollConf(uint8_t *m);

#endif  /* (MAC_INDIRECT_DATA_BASIC == 1) */
#if (MAC_RX_ENABLE_SUPPORT == 1)

/**
 * @brief Implement the MLME-RX-ENABLE.request primitive.
 *
 * The MLME-RX-ENABLE.request primitive is generated by the next
 * higher layer and issued to MAC to enable the receiver for a
 * fixed duration, at a time relative to the start of the current or
 * next superframe on a beacon-enabled PAN or immediately on a
 * nonbeacon-enabled PAN. The receiver is enabled exactly once per
 * primitive request.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the MLME-RX-ENABLE.request message
 */
void MAC_MLME_RxEnableRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_RxEnableConf_t
 *
 * This function is a callback for mlme rx enable confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_RxEnableConf(uint8_t *m);

#endif  /* (MAC_RX_ENABLE_SUPPORT == 1) */

/**
 * @brief Implements the MLME-SYNC request.
 *
 * The MLME-SYNC.request primitive requests to synchronize with the
 * coordinator by acquiring and, if specified, tracking its beacons.
 * The MLME-SYNC.request primitive is generated by the next higher layer of a
 * device on a beacon-enabled PAN and issued to its MLME to synchronize with
 * the coordinator.
 *
 * Enable receiver and search for beacons for at most an interval of
 * [aBaseSuperframeDuration * ((2 ^ (n))+ 1)] symbols where n is the value of
 * macBeaconOrder. If a beacon frame containing the current PAN identifier of
 * the device is not received, the MLME shall repeat this search. Once the
 * number of missed beacons reaches aMaxLostBeacons, the MLME shall notify
 * the next higher layer by issuing the MLME-SYNC-LOSS.indication primitive
 * with a loss reason of BEACON_LOSS.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the MLME sync request parameters.
 */
void MAC_MLME_SyncRequest(uint8_t *m);

#if (MAC_BEACON_NOTIFY_INDICATION == 1)

/**
 * @brief Wrapper function for messages of type MLME_BeaconNotifyInd_t
 *
 * This function is a callback for mlme beacon notify indication.
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */

void MAC_MLME_BeaconNotifyInd(uint8_t *m);

#endif  /* (MAC_BEACON_NOTIFY_INDICATION == 1) */
#if ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || \
	(MAC_ASSOCIATION_INDICATION_RESPONSE == 1))

/**
 * @brief Wrapper function for messages of type MLME_CommStatusInd_t
 *
 * This function is a callback for mlme comm status indication.
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */
void MAC_MLME_CommStatusInd(uint8_t *m);

#endif  /* ((MAC_ORPHAN_INDICATION_RESPONSE == 1) ||
         *(MAC_ASSOCIATION_INDICATION_RESPONSE == 1)) */

/**
 * @brief Wrapper function for messages of type MLME_ResetConf_t
 *
 * This function is a callback for mlme reset confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */
void MAC_MLME_ResetConf(uint8_t *m);

/**
 * @brief Wrapper function for messages of type MLME_SyncLossInd_t
 *
 * This function is a callback for mlme sync loss indication.
 *
 * \ingroup group_mac_ind_int
 * @param m Pointer to message structure
 */
void MAC_MLME_SyncLossInd(uint8_t *m);

/**
 * @brief Handles an MLME-SET.request primitive
 *
 * This function handles the MLME-SET.request. The MLME-SET.request primitive
 * attempts to write the given value to the indicated PIB attribute.
 *
 * \ingroup group_mac_req_int
 * @param m Pointer to the request structure
 */
void MAC_MLME_SetRequest(uint8_t *m);

/**
 * @brief Wrapper function for messages of type mlme_set_conf_t
 *
 * This function is a callback for mlme set confirm.
 *
 * \ingroup group_mac_conf_int
 * @param m Pointer to message structure
 */

void MAC_MLME_SetConf(uint8_t *m);

#if (defined MAC_SECURITY_ZIP) || (defined MAC_SECURITY_2006)

/**
 * @brief Setting of MAC PIB attributes via functional access
 *
 * In case the highest stack layer is above MAC (e.g. NWK or even
 * higher), it is not efficient to change PIB attributes using
 * the standard request / confirm primitive concept via the NHLE_MAC
 * queue. In order to allow a more efficient way to change PIB attributes
 * residing in MAC or TAL, this function replaces the standard primitive
 * access via a functional interface.
 *
 * An additional parameter allows for forcing the transceiver back to sleep
 * after PIB setting. Otherwise the transceiver will stay awake (if it has been
 * woken up before).
 * This enables the higher layer to change several PIB attributes without
 * waking up the transceiver and putting it back to sleep several times.
 *
 * @param attribute PIB attribute to be set
 * @param attribute_index Index of the PIB attribute to be set
 * @param attribute_value Attribute value to be set
 * @param set_trx_to_sleep Set TRX back to sleep after this PIB access if it was
 *        before starting this TRX access. Otherwise the transceiver state will
 *        remain as it is, i.e. in case the transceiver was woken up, it will
 * stay
 *        awake.
 *        The default value for just changing one variable is true, i.e. the
 *        transceiver will be put back to sleep if it has been woken up.
 *
 * @return Status of the attempt to set the TAL PIB attribute:
 *         MAC_UNSUPPORTED_ATTRIBUTE if the PIB attribute was not found
 *         MAC_SUCCESS if the attempt to set the PIB attribute was successful
 *         TAL_BUSY if the TAL is not in an idle state to change PIB attributes
 * \ingroup group_mac_gen_int
 */

MAC_Retval_t MAC_MLME_Set(uint8_t attribute, uint8_t attributeIndex,
		PibValue_t *attributeValue, bool setTrxToSleep);

#else
MAC_Retval_t MAC_MLME_Set(uint8_t attribute, PibValue_t *attributeValue,
		bool setTrxToSleep);

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006)  */

#if (defined MAC_SECURITY_ZIP) || (defined MAC_SECURITY_2006)

/**
 * \ingroup group_mac_gen_int
 */
MAC_Retval_t MAC_MLME_Get(uint8_t attribute, PibValue_t *attributeValue,
		uint8_t attributeIndex);

#else
MAC_Retval_t MAC_MLME_Get(uint8_t attribute, PibValue_t *attributeValue);

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006)  */

/**
 * @brief Initializes the MAC sublayer
 *
 * \ingroup group_mac_gen_int
 * @return MAC_SUCCESS  if TAL is intialized successfully else FAILURE
 */
MAC_Retval_t MAC_Init(void);

/**
 * @brief Runs the MAC scheduler
 *
 * This function runs the MAC scheduler.
 *
 * MLME and MCPS queues are removed alternately, starting with MLME queue.
 *
 * \ingroup group_mac_gen_int
 * @return true if event is dispatched, false if no event to dispatch.
 */
bool MAC_TaskHandler(void);

/*
 * @brief Helper function to extract the complete address information
 *        of the received frame
 *
 * @param frame_ptr Pointer to first octet of Addressing fields of received
 * frame
 *        (See IEEE 802.15.4-2006 Figure 41)
 *
 * \ingroup group_mac_gen_int
 * @return bool Length of Addressing fields
 */
uint8_t MAC_ExtractMhrAddrInfo(uint8_t *framePtr);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* MAC_H */
/* EOF */
