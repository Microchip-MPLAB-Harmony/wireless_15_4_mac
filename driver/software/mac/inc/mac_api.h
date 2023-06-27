/*******************************************************************************
  MAC API Header
  
  File Name:
    mac_api.h

  Summary:
    This file contains MAC Callback's,Confirmation Callback's,Indication Callback's
	declarations

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

#ifndef MAC_API_H
#define MAC_API_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************


#include "mac.h"
#include "mac_config.h"

#ifdef MAC_SECURITY_ZIP
#include "app_config.h"
#endif

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/* === Macros =============================================================== */

/* === Externals ============================================================ */

// *****************************************************************************
// *****************************************************************************
//Section: Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* MAC Address type
 
   Summary:
    This contain MAC address type information
 
   Description:
    None
   Remarks:
    None 
 */

/**
 * MAC Address type
 */

typedef union {
	uint16_t shortAddress;
	uint64_t longAddress;
} AddressField_t;


// *****************************************************************************
/* device address specification
 
   Summary:
    This structure contain device address specification information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct wpan_addr_spec_tag {
	/**
	 * Address mode (@ref WPAN_ADDRMODE_NONE, @ref WPAN_ADDRMODE_SHORT, or
	 *@ref WPAN_ADDRMODE_LONG)
	 */
	uint8_t AddrMode;

	/**
	 * The 16 bit PAN identifier.
	 */
	uint16_t PANId;

	/**
	 * Device address. If AddrMode is @ref WPAN_ADDRMODE_SHORT, it is
	 * interpreted as a
	 * 16 bit address.
	 */
	AddressField_t Addr;
} WPAN_AddrSpec_t;

// *****************************************************************************
/* PAN descriptor
 
   Summary:
    This structure contain PAN descriptor information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct wpan_pandescriptor_tag {
	/**Coordinator address specification in received beacon frame */
	WPAN_AddrSpec_t CoordAddrSpec;
	/**The current logical channel used by the network */
	uint8_t LogicalChannel;
	/**The current channel page occupied by the network */
	uint8_t ChannelPage;
	/**Superframe specification in received beacon frame */
	uint16_t SuperframeSpec;
	/**Set to true if the beacon is from a PAN coordinator accepting GTS requests */
	bool GTSPermit;
	/**LQI at which the beacon was received.Lower values represent poorer link quality. */
	uint8_t LinkQuality;

#ifdef ENABLE_TSTAMP

	/**
	 * Time at which the beacon frame was received, in symbol counts.  This
	 * quantity
	 * shall be interpreted as only 24-bits, with the most significant
	 * 8-bits entirely ignored.
	 */
	uint32_t TimeStamp;
#endif  /* ENABLE_TSTAMP */
} WPAN_Pandescriptor_t;

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))

// *****************************************************************************
/* Device Descriptor
 
   Summary:
    This structure contain Device Descriptor information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_device_desc_tag {
	/** The 16-bit PAN identifier of the device in this DeviceDescriptor. */
	uint16_t PANId;

	/**
	 * The 16-bit short address of the device in this DeviceDescriptor.
	 * A value of 0xfffe indicates that this device is using only its
	 * extended address. A value of 0xffff indicates that this value is
	 * unknown.
	 */
	uint16_t ShortAddress;

	/**
	 * The 64-bit IEEE extended address of the device in this
	 * DeviceDescriptor.
	 * This element is also used in unsecuring operations on incoming
	 * frames.
	 */
	uint64_t ExtAddress;

	/**
	 * The incoming frame counter of the device in this DeviceDescriptor.
	 * This value is used to ensure sequential freshness of frames.
	 */
	uint32_t FrameCounter;

	/**
	 * Indication of whether the device may override the minimum security
	 * level settings defined in Table 92.
	 */
	bool Exempt;
} MAC_DeviceDesc_t;

// *****************************************************************************
/* KeyUsage Descriptor
 
   Summary:
    This structure contain KeyUsage Descriptor information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_key_usage_tag {
	/** See 7.2.1.1.1. */
	uint8_t Frametype;

	/** See Table 82. */
	uint8_t CommandFrameIdentifier;
} MAC_KeyUsage_t;

// *****************************************************************************
/* KeyDevice Descriptor
 
   Summary:
    This structure contain KeyDevice Descriptor information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_key_device_desc_tag {
	/** Handle to the DeviceDescriptor corresponding to the device. */
	uint8_t DeviceDescriptorHandle;

	/**
	 * Indication of whether the device indicated by DeviceDescriptorHandle
	 * is uniquely associated with the KeyDescriptor, i.e., it is a link
	 * key as opposed to a group key.
	 */
	bool UniqueDevice;

	/**
	 * Indication of whether the device indicated by DeviceDescriptorHandle
	 * previously communicated with this key prior to the exhaustion of the
	 * frame counter. If TRUE, this indicates that the device shall not use
	 * this key further because it exhausted its use of the frame counter
	 * used
	 * with this key.
	 */
	bool BlackListed;
} MAC_KeyDeviceDesc_t;

// *****************************************************************************
/* KeyIdLookup Descriptor
 
   Summary:
    This structure contain KeyIdLookup Descriptor information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_key_id_lookup_desc_tag {
	/** Data used to identify the key. */
	uint8_t LookupData[9];

	/**
	 * A value of 0x00 indicates a set of 5 octets, a value of 0x01
	 * indicates
	 * a set of 9 octets.
	 */
	uint8_t LookupDataSize;
} MAC_KeyIdLookupDesc_t;

// *****************************************************************************
/* macKeyTable
 
   Summary:
    This structure contain KeyIdLookup Descriptor information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_key_table_tag {
	/** List of KeyIdLookupDescriptor entries. */
	MAC_KeyIdLookupDesc_t KeyIdLookupList[
		MAC_ZIP_MAX_KEY_ID_LOOKUP_LIST_ENTRIES];
	/** The number of entries in KeyIdLookupList. */
	uint8_t KeyIdLookupListEntries;

	/** List of KeyDeviceDescriptor entries. */
	MAC_KeyDeviceDesc_t KeyDeviceList[MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES];
	/** The number of entries in KeyDeviceList. */
	uint8_t KeyDeviceListEntries;

	/** List of KeyUsageDescriptor entries. */
	MAC_KeyUsage_t KeyUsageList[MAC_ZIP_MAX_KEY_USAGE_LIST_ENTRIES];
	/** The number of entries in KeyUsageList. */
	uint8_t KeyUsageListEntries;

	/** Set of 16 octets - the actual value of the key. */
	uint8_t Key[16];
} MAC_KeyTable_t;

// *****************************************************************************
/* macSecurityLevelTable
 
   Summary:
    This structure contain macSecurityLevelTable information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_sec_lvl_table_tag {
	/** See 7.2.1.1.1. */
	uint8_t FrameType;

	/** See Table 82. */
	uint8_t CommandFrameIdentifier;

	/**
	 * The minimal required/expected security level for incoming
	 * MAC frames with the indicated frame type and, if present,
	 * command frame type (see Table 95 in 7.6.2.2.1).
	 */
	uint8_t SecurityMinimum;

	/**
	 * Indication of whether originating devices for which the Exempt flag
	 * is set may override the minimum security level indicated by the
	 * SecurityMinimum element.
	 * If TRUE, this indicates that for originating devices with Exempt
	 * status,
	 * the incoming security level zero is acceptable, in addition to the
	 * incoming security levels meeting the minimum expected security level
	 * indicated by the SecurityMinimum element.
	 */
	bool DeviceOverrideSecurityMinimum;
} MAC_SecLvlTable_t;

// *****************************************************************************
/* macDeviceTable
 
   Summary:
    This structure contain macDeviceTable information
 
   Description:
    None
   Remarks:
    None 
 */

typedef struct mac_dev_table_tag {
	MAC_DeviceDesc_t DeviceDescriptor[1];
} MAC_DevTable_t;

// *****************************************************************************
/* MAC Security related PIB attributes
 
   Summary:
    This structure contain the MAC Security related PIB attributes information
 
   Description:
    None
   Remarks:
    None 
 */
typedef struct mac_sec_pib_tag {
	/**
	 * Holds a table of KeyDescriptor entries, each containing keys and
	 * related
	 * information required for secured communications.
	 */
	MAC_KeyTable_t KeyTable[MAC_ZIP_MAX_KEY_TABLE_ENTRIES];

	/**
	 * Holds the number of entries in macKeyTable.
	 */
	uint8_t KeyTableEntries;

	/**
	 * Holds a table of DeviceDescriptor entries, each indicating a remote
	 * device
	 * with which this device securely communicates.
	 */
	MAC_DevTable_t DeviceTable[MAC_ZIP_MAX_DEV_TABLE_ENTRIES];

	/**
	 * Holds the number of entries in macDeviceTable.
	 */
	uint16_t DeviceTableEntries;

	/**
	 * Holds a table of SecurityLevelDescriptor entries, each with
	 * information
	 * about the minimum security level expected depending on incoming frame
	 * type
	 * and subtype.
	 */
	MAC_SecLvlTable_t SecurityLevelTable[MAC_ZIP_MAX_SEC_LVL_TABLE_ENTRIES
	];

	/**
	 * Holds the number of entries in macSecurityLevelTable.
	 */
	uint8_t SecurityLevelTableEntries;

	/**
	 * Holds the outgoing frame counter for this device.
	 */
	uint32_t FrameCounter;

	/**
	 * Holds the originator of the default key used for key identifier mode
	 * 0x01.
	 */
	uint8_t DefaultKeySource[8];

	/**
	 * Holds the 64-bit extended address of the PAN coordinator
	 */
	uint8_t PANCoordExtendedAddress[8];

	/**
	 * Holds the 16-bit short address of the PAN coordinator
	 */
	uint16_t PANCoordShortAddress;
} MAC_SecPib_t;
#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* MAC NHLE QUEUE CAPACITY
 
   Summary:
    This macro defines Capacity of queue between MAC and Next Higher Layer
   Description:
    None
   Remarks:
    None 
 */

#define MAC_NHLE_QUEUE_CAPACITY         255U

// *****************************************************************************
// *****************************************************************************
// Section: Symbolic constants for MAC API
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* WPAN_ADDRMODE_NONE
 
   Summary:
    Value for the address mode, where no address is given
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_ADDRMODE_NONE              (0x00)

// *****************************************************************************
/* WPAN_ADDRMODE_SHORT
 
   Summary:
    Value for the address mode, where a 16 bit short address is given
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_ADDRMODE_SHORT             (0x02)

// *****************************************************************************
/* WPAN_ADDRMODE_LONG
 
   Summary:
    Value for the address mode, where a 64 bit long address is given
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_ADDRMODE_LONG              (0x03)

// *****************************************************************************
/* WPAN_CAP_ALTPANCOORD
 
   Summary:
    Flag value for capability information field
	The alternate PAN coordinator subfield shall be set if the device is
    capable of becoming a PAN coordinator. Otherwise, the alternate 
	PAN coordinator subfield shall be set to 0.
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_CAP_ALTPANCOORD            (0x01)

// *****************************************************************************
/* WPAN_CAP_FFD
 
   Summary:
    Flag value for capability information field
	The device type subfield shall be set if the device is an FFD. Otherwise,
    the device type subfield shall be set to 0 to indicate an RFD.
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_CAP_FFD                    (0x02)

// *****************************************************************************
/* WPAN_CAP_PWRSOURCE
 
   Summary:
    Flag value for capability information field
	The power source subfield shall be set if the device is receiving power
    from the alternating current mains. Otherwise, the power source subfield
    shall be set to 0
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_CAP_PWRSOURCE              (0x04)

// *****************************************************************************
/* WPAN_CAP_RXONWHENIDLE
 
   Summary:
    Flag value for capability information field
	The receiver on when idle subfield shall be set if the device does not
    disable its receiver to conserve power during idle periods. Otherwise, the
    receiver on when idle subfield shall be set to 0.
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_CAP_RXONWHENIDLE           (0x08)

// *****************************************************************************
/* WPAN_CAP_ALLOCADDRESS
 
   Summary:
    Flag value for capability information field
	The allocate address subfield shall be set if the device wishes the
    coordinator to allocate a short address as a result of the association
    procedure. If this subfield is set to 0, the special short address of
    0xfffe shall be allocated to the device and returned through the
    association response command. In this case, the device shall communicate
    on the PAN using only its 64 bit extended address.
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_CAP_ALLOCADDRESS           (0x80)

// *****************************************************************************
/* WPAN_DISASSOC_BYPARENT
 
   Summary:
    Flag value for capability information field
	Symbolic constant for disassociate reason - initiated by parent(WPAN_MLME_DisassociateReq()).
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_DISASSOC_BYPARENT          (0x01)

// *****************************************************************************
/* WPAN_DISASSOC_BYCHILD
 
   Summary:
    Flag value for capability information field
	Symbolic constant for disassociate reason - initiated by child(WPAN_MLME_DisassociateReq()).
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_DISASSOC_BYCHILD           (0x02)

// *****************************************************************************
/* WPAN_NUM_SHORT_ADDR_PENDING
 
   Summary:
    This macro defines extract size of short address list in PAN descriptor
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_NUM_SHORT_ADDR_PENDING(x)      ((x) & 0x7)

// *****************************************************************************
/* WPAN_NUM_EXTENDED_ADDR_PENDING
 
   Summary:
    This macro defines extract size of extended address list in PAN descriptor
   Description:
    None
   Remarks:
    None 
 */

#define WPAN_NUM_EXTENDED_ADDR_PENDING(x)   (((x) >> 4) & 0x7)

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************

/**
 * @brief The stack initialization function.
 *
 * This function initializes all resources, which are used from the stack.
 * It has to be called before any other function of the stack is called.
 *
 * @ingroup group_mac_gen
 */
MAC_Retval_t WPAN_Init(void);

/**
 * @brief The stack task function called by the application.
 *
 * This function should be called as frequently as possible by the application
 * in order to provide a permanent execution of the protocol stack.
 *
 * @return Boolean true if an event was processed otherwise false.
 * @ingroup group_mac_gen
 */
bool WPAN_Task(void);

/*--------------------------------------------------------------------*/

/*
 * These functions have to be called from the application
 * in order to initiate an action in the communication
 * stack at the MAC level
 */

/**
 * Initiate MCPS-DATA.request service and have it placed in the MCPS-SAP queue.
 *
 * @param SrcAddrMode   Address Mode of the source address.
 * @param DstAddrSpec   Pointer to WPAN_AddrSpec_t structure for destination.
 * @param msduHandle    Handle (identification) of the MSDU.
 * @param TxOptions     Bitmap for transmission options. Valid values:
 *                      - @ref WPAN_TXOPT_OFF,
 *                      - @ref WPAN_TXOPT_ACK,
 *                      - @ref WPAN_TXOPT_INDIRECT,
 *                      - @ref WPAN_TXOPT_INDIRECT_ACK.
 *                      - @ref WPAN_TXOPT_GTS.
 *                      - @ref WPAN_TXOPT_GTS_ACK.
 * @param msdu          Pointer to the data to be transmitted.
 * @param msduLength    Length of the data to be transmitted.
 *
 * @param SecurityLevel Used security level; this parameter is only available
 *                      if MAC security is enabled via MAC_SECURITY_ZIP
 * @param KeyIdMode     Used mode to identify the key; this parameter is only
 * available
 *                      if MAC security is enabled via MAC_SECURITY_ZIP
 * @param KeyIndex      Used index of the key; this parameter is only available
 *                      if MAC security is enabled via MAC_SECURITY_ZIP
 *
 * @return true - success; false - buffer not available or queue full.
 */
#if (defined(MAC_SECURITY_ZIP) || defined(MAC_SECURITY_2006))
bool WPAN_MCPS_Datareq(uint8_t SrcAddrMode,
		WPAN_AddrSpec_t *DstAddrSpec,
		uint8_t msduLength,
		uint8_t *msdu,
		uint8_t msduHandle,
		uint8_t TxOptions,
		uint8_t SecurityLevel,
		uint8_t *KeySource,
		uint8_t KeyIdMode,
		uint8_t KeyIndex);

#else   /* No MAC_SECURITY */
bool WPAN_MCPS_Datareq(uint8_t SrcAddrMode,
		WPAN_AddrSpec_t *DstAddrSpec,
		uint8_t msduLength,
		uint8_t *msdu,
		uint8_t msduHandle,
		uint8_t TxOptions);

#endif

#if ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))

/**
 * Initiate MCPS-PURGE.request service and have it placed in the MCPS-SAP queue.
 *
 * @param msduHandle    Handle of MSDU to be purged.
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MCPS_PurgeReq(const uint8_t msduHandle);

#endif  /* ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))
        **/

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) 

/**
 * Initiate MLME-ASSOCIATE.request service and have it placed in the MLME-SAP
 * queue.
 *
 * @param LogicalChannel        The logical channel on which to attempt
 * association.
 * @param ChannelPage           The channel page on which to attempt
 * association.
 * @param CoordAddrSpec         Pointer to WPAN_AddrSpec_t structure for
 * coordinator.
 * @param CapabilityInformation Bitmap that describes the nodes capabilities.
 *                              (@ref WPAN_CAP_ALTPANCOORD |
 *                               @ref WPAN_CAP_FFD |
 *                               @ref WPAN_CAP_PWRSOURCE |
 *                               @ref WPAN_CAP_RXONWHENIDLE |
 *                               @ref WPAN_CAP_ALLOCADDRESS)
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_AssociateReq(uint8_t LogicalChannel,
		uint8_t ChannelPage,
		WPAN_AddrSpec_t *CoordAddrSpec,
		uint8_t CapabilityInformation);

#endif  /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)

/**
 * Initiate MLME-ASSOCIATE.response service and place it in the MLME-SAP queue.
 *
 * @param DeviceAddress      Extended address for device requesting association.
 * @param AssocShortAddress  Short address allocated on successful association.
 * @param status             Status of the association attempt. Valid values:
 *                           - @ref ASSOCIATION_SUCCESSFUL,
 *                           - @ref PAN_AT_CAPACITY,
 *                           - @ref PAN_ACCESS_DENIED,
 *                           - @ref ASSOCIATION_RESERVED.
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_resp
 */
bool WPAN_MLME_AssociateResp(uint64_t DeviceAddress,
		uint16_t AssocShortAddress,
		uint8_t status);

#endif  /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) 

/**
 * Inititate MLME-DISASSOCIATE.request service and have it placed in the
 * MLME-SAP queue.
 *
 * @param DeviceAddrSpec     Pointer to WPAN_AddrSpec_t structure for device
 *                           to which to send the disassociation notification
 *                           command.
 * @param DisassociateReason Reason for disassociation. Valid values:
 *                           - @ref WPAN_DISASSOC_BYPARENT,
 *                           - @ref WPAN_DISASSOC_BYCHILD.
 * @param TxIndirect         TRUE if the disassociation notification command
 *                           is to be sent indirectly
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_DisassociateReq(WPAN_AddrSpec_t *DeviceAddrSpec,
		uint8_t DisassociateReason,
		bool TxIndirect);

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

#if (MAC_GET_SUPPORT == 1) 

/**
 * Initiate MLME-GET.request service and have it placed in the MLME-SAP queue.
 *
 * @param PIBAttribute     PIB attribute to be retrieved.
 * @param PIBAttributeIndex Index of the PIB attribute to be read.
 *
 * @return true - success; false - buffer not availability or queue full.
 * @ingroup group_mac_req
 */
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
bool WPAN_MLME_GetReq(uint8_t PIBAttribute, uint8_t PIBAttributeIndex);

#else
bool WPAN_MLME_GetReq(uint8_t PIBAttribute);

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */
#endif  /* (MAC_GET_SUPPORT == 1) */

#if (MAC_ORPHAN_INDICATION_RESPONSE == 1)

/**
 * Initiate MLME-ORPHAN.response service and have it placed in MLME_SAP queue.
 *
 * @param OrphanAddress    Address of orphaned device.
 * @param ShortAddress     Short address allocated to orphaned device.
 * @param AssociatedMember Boolean true if the orphaned device is associated.
 *
 * @return true - success; false - buffer not availability or queue full.
 * @ingroup group_mac_resp
 */
bool WPAN_MLME_OrphanResp(uint64_t OrphanAddress,
		uint16_t ShortAddress,
		bool AssociatedMember);

#endif  /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) */

#if (MAC_INDIRECT_DATA_BASIC == 1)

/**
 * Initiate MLME-POLL.request service and have it placed in the MLME-SAP queue.
 *
 * @param CoordAddrSpec   Pointer to WPAN_AddrSpec_t structure for the
 * coordinator.
 *
 * @return true - success; false - buffer not availability or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_PollReq(WPAN_AddrSpec_t *CoordAddrSpec);

#endif  /* (MAC_INDIRECT_DATA_BASIC == 1) */

/**
 * Initiate MLME-RESET.request service and have it placed in the MLME-SAP queue.
 *
 * @param SetDefaultPib  Boolean to set all PIB values to their respective
 * defaults.
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_ResetReq(bool SetDefaultPib);

/**
 * Initiate MLME-SET.request service and have it placed in MLME_SAP queue.
 *
 * @param PIBAttribute      PIB attribute to be set.
 * @param PIBAttributeIndex Index of the PIB attribute to be set.
 * @param PIBAttributeValue Pointer to new PIB attribute value.
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
#if (defined MAC_SECURITY_ZIP) || (defined MAC_SECURITY_2006)
bool WPAN_MLME_SetReq(uint8_t PIBAttribute,
		uint8_t PIBAttributeIndex,
		void *PIBAttributeValue);

#else
bool WPAN_MLME_SetReq(uint8_t PIBAttribute,
		void *PIBAttributeValue);

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006)  */

#if (MAC_RX_ENABLE_SUPPORT == 1) 

/**
 * Initiate MLME-RX-ENABLE.request service and have it placed in the MLME-SAP
 * queue.
 *
 * @param DeferPermit     Set to true if receiver enable can be deferred until
 * next
 *                        superframe if requested time has already passed.
 * @param RxOnTime        Number of symbols from start of superframe before
 * receiver
 *                        is enabled.
 * @param RxOnDuration    Number of symbols for which the receiver is enabled,
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_RxEnableReq(bool DeferPermit,
		uint32_t RxOnTime,
		uint32_t RxOnDuration);

#endif  /* (MAC_RX_ENABLE_SUPPORT == 1) */

#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1)      ||	\
	(MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1)  || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1)) 

/**
 * Initiate MLME-SCAN.request service and have it placed in the MLME-SAP queue.
 *
 * @param ScanType      Type of scan to perform. Valid values:
 *                      - @ref MLME_SCAN_TYPE_ED,
 *                      - @ref MLME_SCAN_TYPE_ACTIVE,
 *                      - @ref MLME_SCAN_TYPE_PASSIVE,
 *                      - @ref MLME_SCAN_TYPE_ORPHAN.
 * @param ScanChannels  Channels to be scanned.
 * @param ScanDuration  Duration of each scan.
 * @param ChannelPage   The channel page on which to perform the scan.
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_ScanReq(uint8_t ScanType,
		uint32_t ScanChannels,
		uint8_t ScanDuration,
		uint8_t ChannelPage);

#endif

#if (MAC_START_REQUEST_CONFIRM == 1)

/**
 * Initiate MLME-START service and have it placed in the MLME-SAP queue.
 *
 * @param PANId                 PAN identifier to be used by device.
 * @param LogicalChannel        The logical channel on which to start
 *                              using the new superframe configuration.
 * @param ChannelPage           The channel page on which to begin
 *                              using the new superframe configuration.
 * @param BeaconOrder           Beacon transmission interval.
 * @param SuperframeOrder       Duration of active portion of superframe.
 * @param PANCoordinator        Indicates whether node is PAN coordinator of
 * PAN.
 * @param BatteryLifeExtension  Boolean true disables receiver of beaconing
 * device
 *                              for a period after interframe spacing of beacon
 * frame.
 * @param CoordRealignment      Boolean to transmit Coordinator Realignment
 * command
 *                              prior to changing to new superframe
 * configuration.
 *
 * @return true - success; false - buffer not available or queue full.
 * @ingroup group_mac_req
 */
bool WPAN_MLME_StartReq(uint16_t PANId,
		uint8_t LogicalChannel,
		uint8_t ChannelPage,
		uint8_t BeaconOrder,
		uint8_t SuperframeOrder,
		bool PANCoordinator,
		bool BatteryLifeExtension,
		bool CoordRealignment
		);

#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */

/*@}*//* apiMacReq */

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MCPS-DATA.confirm.
 *
 * @param msduHandle  Handle of MSDU handed over to MAC earlier.
 * @param status      Result for requested data transmission request.
 * @param Timestamp   The time, in symbols, at which the data were transmitted;
 *                    this parameter is only available if timestamping is
 * enabled
 *                    via compile switch ENABLE_TSTAMP.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
#if defined(ENABLE_TSTAMP)
void USR_MCPS_DataConf(uint8_t msduHandle,
		uint8_t status,
		uint32_t Timestamp);

#else
void USR_MCPS_DataConf(uint8_t msduHandle,
		uint8_t status);

#endif  /* ENABLE_TSTAMP */

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MCPS-DATA.indication.
 *
 * @param SrcAddrSpec      Pointer to source address specification.
 * @param DstAddrSpec      Pointer to destination address specification.
 * @param msduLength       Number of octets contained in MSDU.
 * @param msdu             Pointer to MSDU.
 * @param mpduLinkQuality  LQI measured during reception of the MPDU.
 * @param DSN              The DSN of the received data frame.
 * @param Timestamp        The time, in symbols, at which the data were
 * received;
 *                         this parameter is only available if timestamping is
 * enabled
 *                         via compile switch ENABLE_TSTAMP.
 *
 * @param SecurityLevel    Used security level; this parameter is only available
 *                         if MAC security is enabled via MAC_SECURITY_ZIP
 * @param KeyIdMode        Used mode to identify the key; this parameter is only
 * available
 *                         if MAC security is enabled via MAC_SECURITY_ZIP
 * @param KeyIndex         Used index of the key; this parameter is only
 * available
 *                         if MAC security is enabled via MAC_SECURITY_ZIP
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
#if (defined(MAC_SECURITY_ZIP) || defined(MAC_SECURITY_2006))
void USR_MCPS_DataInd(WPAN_AddrSpec_t * SrcAddrSpec,
WPAN_AddrSpec_t * DstAddrSpec,
uint8_t msduLength,
uint8_t * msdu,
uint8_t mpduLinkQuality,
uint8_t DSN,
    #if defined(ENABLE_TSTAMP)
uint32_t Timestamp,
    #endif  /* ENABLE_TSTAMP */
uint8_t SecurityLevel,
uint8_t KeyIdMode,
uint8_t KeyIndex);
#else   /* No MAC_SECURITY */
void USR_MCPS_DataInd(WPAN_AddrSpec_t * SrcAddrSpec,
WPAN_AddrSpec_t * DstAddrSpec,
uint8_t msduLength,
uint8_t * msdu,
uint8_t mpduLinkQuality,
    #ifdef ENABLE_TSTAMP
uint8_t DSN,
uint32_t Timestamp);
    #else
uint8_t DSN);
    #endif  /* ENABLE_TSTAMP */
#endif  /* (MAC_SECURITY_ZIP) || (MAC_SECURITY_2006) */

#if ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MCPS-PURGE.confirm.
 *
 * @param msduHandle           Handle (id) of MSDU to be purged.
 * @param status               Result of requested purge operation.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MCPS_PurgeConf(uint8_t msduHandle,
		uint8_t status);

#endif  /* ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))
        **/

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-ASSOCIATE.confirm.
 *
 * @param AssocShortAddress    Short address allocated by the coordinator.
 * @param status               Result of requested association operation.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_AssociateConf(uint16_t AssocShortAddress,
		uint8_t status);

#endif  /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-ASSOCIATE.indication.
 *
 * @param DeviceAddress         Extended address of device requesting
 * association.
 * @param CapabilityInformation Capabilities of device requesting association.
 *                              (@ref WPAN_CAP_ALTPANCOORD |
 *                               @ref WPAN_CAP_FFD |
 *                               @ref WPAN_CAP_PWRSOURCE |
 *                               @ref WPAN_CAP_RXONWHENIDLE |
 *                               @ref WPAN_CAP_ALLOCADDRESS)
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
void USR_MLME_AssociateInd(uint64_t DeviceAddress,
		uint8_t CapabilityInformation);

#endif  /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */

#if (MAC_BEACON_NOTIFY_INDICATION == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-BEACON-NOTIFY.indication.
 *
 * @param BSN            Beacon sequence number.
 * @param PANDescriptor  Pointer to PAN descriptor for received beacon.
 * @param PendAddrSpec   Pending address specification in received beacon.
 * @param AddrList       List of addresses of devices the coordinator has
 * pending data.
 * @param sduLength      Length of beacon payload.
 * @param sdu            Pointer to beacon payload.
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
void USR_MLME_BeaconNotifyInd(uint8_t BSN,
		WPAN_Pandescriptor_t *PANDescriptor,
		uint8_t PendAddrSpec,
		uint8_t *AddrList,
		uint8_t sduLength,
		uint8_t *sdu);

#endif  /* (MAC_BEACON_NOTIFY_INDICATION == 1) */

#if ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || \
	(MAC_ASSOCIATION_INDICATION_RESPONSE == 1))

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-COMM-STATUS.indication.
 *
 * @param SrcAddrSpec      Pointer to source address specification.
 * @param DstAddrSpec      Pointer to destination address specification.
 * @param status           Result for related response operation.
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
void USR_MLME_CommStatusInd(WPAN_AddrSpec_t *SrcAddrSpec,
		WPAN_AddrSpec_t *DstAddrSpec,
		uint8_t status);

#endif  /* ((MAC_ORPHAN_INDICATION_RESPONSE == 1) ||
         *(MAC_ASSOCIATION_INDICATION_RESPONSE == 1)) */

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-DISASSOCIATE.confirm.
 *
 * @param status             Result of requested disassociate operation.
 * @param DeviceAddrSpec     Pointer to WPAN_AddrSpec_t structure for device
 *                           that has either requested disassociation or been
 *                           instructed to disassociate by its coordinator.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_DisassociateConf(uint8_t status,
		WPAN_AddrSpec_t *DeviceAddrSpec);

#endif /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) || defined(__DOXYGEN__) */

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-DISASSOCIATE.indication.
 *
 * @param DeviceAddress        Extended address of device which initiated the
 *                             disassociation request.
 * @param DisassociateReason   Reason for the disassociation. Valid values:
 *                           - @ref WPAN_DISASSOC_BYPARENT,
 *                           - @ref WPAN_DISASSOC_BYCHILD.
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
void USR_MLME_DisassociateInd(uint64_t DeviceAddress,
		uint8_t DisassociateReason);

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) || defined(__DOXYGEN__) */

#if (MAC_GET_SUPPORT == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-GET.confirm.
 *
 * @param status            Result of requested PIB attribute get operation.
 * @param PIBAttribute      Retrieved PIB attribute.
 * @param PIBAttributeIndex Index of the PIB attribute to be read.
 * @param PIBAttributeValue Pointer to data containing retrieved PIB attribute,
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_GetConf(uint8_t status,
uint8_t PIBAttribute,
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
uint8_t PIBAttributeIndex,
#endif  /* ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006)) */
void *PIBAttributeValue);
#endif  /* (MAC_GET_SUPPORT == 1) */

#if (MAC_ORPHAN_INDICATION_RESPONSE == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-ORPHAN.indication.
 *
 * @param OrphanAddress     Address of orphaned device.
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
void USR_MLME_OrphanInd(uint64_t OrphanAddress);

#endif  /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) */

#if (MAC_INDIRECT_DATA_BASIC == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-POLL.confirm.
 *
 * @param status           Result of requested poll operation.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_PollConf(uint8_t status);

#endif  /* (MAC_INDIRECT_DATA_BASIC == 1) */

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-RESET.confirm.
 *
 * @param status           Result of requested reset operation.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_ResetConf(uint8_t status);

#if (MAC_RX_ENABLE_SUPPORT == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-RX-ENABLE.confirm.
 *
 * @param status           Result of requested receiver enable operation.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_RxEnableConf(uint8_t status);

#endif  /* (MAC_RX_ENABLE_SUPPORT == 1) */

#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1)      ||	\
	(MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1)  || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1))

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-SCAN.confirm.
 *
 * @param status            Result of requested scan operation.
 * @param ScanType          Type of scan performed.
 * @param ChannelPage       The channel page on which the scan was performed.
 * @param UnscannedChannels Bitmap of unscanned channels
 * @param ResultListSize    Number of elements in ResultList.
 * @param ResultList        Pointer to array of scan results .
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_ScanConf(uint8_t status,
		uint8_t ScanType,
		uint8_t ChannelPage,
		uint32_t UnscannedChannels,
		uint8_t ResultListSize,
		void *ResultList);

#endif

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-SET.confirm.
 *
 * @param status        Result of requested PIB attribute set operation.
 * @param PIBAttribute  Updated PIB attribute.
 * @param PIBAttributeIndex  Index of updated PIB attribute.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
void USR_MLME_SetConf(uint8_t status,
		uint8_t PIBAttribute,
		uint8_t PIBAttributeIndex);

#else
void USR_MLME_SetConf(uint8_t status,
		uint8_t PIBAttribute);

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

#if (MAC_START_REQUEST_CONFIRM == 1)

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-START.confirm.
 *
 * @param status        Result of requested start operation.
 *
 * @return void
 *
 * @ingroup group_mac_conf
 */
void USR_MLME_StartConf(uint8_t status);

#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */

/**
 * Callback function that must be implemented by application (NHLE) for MAC
 * service
 * MLME-SYNC-LOSS.indication.
 *
 * @param LossReason     Reason for synchronization loss.
 * @param PANId          The PAN identifier with which the device lost
 *                       synchronization or to which it was realigned.
 * @param LogicalChannel The logical channel on which the device lost
 *                       synchronization or to which it was realigned.
 * @param ChannelPage    The channel page on which the device lost
 *                       synchronization or to which it was realigned.
 *
 * @return void
 *
 * @ingroup group_mac_ind
 */
void USR_MLME_SyncLossInd(uint8_t LossReason,
		uint16_t PANId,
		uint8_t LogicalChannel,
		uint8_t ChannelPage);

uint8_t MAC_GetPibAttributeSize(uint8_t pibAttributeId);

/**
 * @brief Checks if the mac stack is ready to sleep
 * \ingroup group_mac_gen_int
 */
uint32_t MAC_ReadyToSleep(void);

/*
 * @brief MAC Wakeup Callback Function from application for Synchronizing beacon
 * timing after Wakeup
 *
 * This function Handles the residual time for Beacon Synchronization after
 * Wakeup
 * @param res_time remaining time to be synchronized with next beacon timing.
 */
#if  (defined ENABLE_SLEEP)
void MAC_Wakeup(void);

#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* MAC_API_H */
/* EOF */
