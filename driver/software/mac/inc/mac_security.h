/*******************************************************************************
    IEEE MAC Security 

  File Name:
    mac_security.h
 Summary:
    MAC security definitions and macros
  Description:
    This header holds MAC security related definitions and macros.

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

#ifndef MAC_SECURITY_H
#define MAC_SECURITY_H

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "mac_api.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/**
 * Default value for PIB macKeyTableEntries
 */
#define macKeyTableEntriesDef              (0)

/**
 * Default value for PIB macDeviceTableEntries
 */
#define macDeviceTableDef                  (0)

/**
 * Default value for PIB macSecurityLevelTableEntries
 */
#define macSecurityLevelTableDef           (0)

/**
 * Default value for PIB macFrameCounter
 */
#define macFrameCounterDef                 (0x00000000)

/**
 * Default value for PIB macDefaultKeySource
 */
#define macDefaultKeySourceDef             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
					     0xFF, 0xFF}

/**
 * Default value for KeyIdLookupListEntries
 */
#define KeyIdLookupListEntriesDef          (0)

/**
 * Default value for KeyDeviceListEntries
 */
#define KeyDeviceListEntriesDef            (0)

/**
 * Default value for KeyUsageListEntries
 */
#define KeyUsageListEntriesDef             (0)

// *****************************************************************************
// *****************************************************************************
// Section: Externals
// *****************************************************************************
// *****************************************************************************
        
extern MAC_SecPib_t macSecPib;

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************

/* Finding the Key Identifier Length field */
uint8_t GetKeyIdFieldLen(uint8_t keyIdMode);

bool BuildSecMcpsDataFrame(MCPS_DataReq_t *mpdr, MAC_FrameInfo_t *mframe);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

#endif /* MAC_SECURITY_H */
/* EOF */
