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

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))

/**
 * Holds the values of all security related PIB attributes.
 */
MAC_SecPib_t macSecPib;
#endif  /* (MAC_SECURITY_ZIP || MAC_SECURITY_2006) */

#if (MAC_INDIRECT_DATA_FFD == 1)

/**
 * Queue used by MAC layer to put in indirect data. Any indirect data given by
 * NHLE is placed here by MAC, until the device polls for the data.
 */
queue_t indirectDataQueue;
#endif /* (MAC_INDIRECT_DATA_FFD == 1) */

MAC_Pib_t macPib;


/* === Prototypes =========================================================== */
typedef __PACKED_STRUCT mac_ds_param
{
    uint64_t mac_CoordExtAddr;  
    uint64_t mac_ieee_addr; 
    uint32_t mac_CoordShtAddr; 
    uint32_t mac_short_addr;
    uint32_t panid;
    uint32_t mac_max_frame_total_wait_time;
    uint32_t mac_response_wait_time; 
    uint32_t mac_state;  
    uint32_t mac_radio_sleep_state; 
    uint32_t mac_poll_state; 
    uint32_t mac_associated_PAN_coord;   
    uint32_t mac_auto_request; 
    uint32_t mac_batt_life_ext_periods; 
    uint32_t mac_dsn; 
    uint32_t phy_current_channel;
}MAC_Ds_Param_t;

static uint16_t    macshortaddr; 
static uint64_t    macieeeaddr;
static uint16_t    panid;
static MAC_Ds_Param_t param1;

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))
static MAC_SecPib_t __attribute__ ((persistent)) macSecPibBackup;
#endif

static MAC_Ds_Param_t __attribute__((persistent)) mdsParam;
/* === Implementation ======================================================= */
static void memcpy4ByteAligned(void* outbuf, void* inbuf, uint16_t length)
{
  static uint16_t mod_size;
  static uint16_t size;
  static uint16_t k;
  uint32_t* src = (uint32_t* )inbuf;
  uint32_t* dst = (uint32_t* )outbuf;

  mod_size = (length % 4U);
  // total_length is in multiple of 4
  if (mod_size != 0U)
  {
    size  = length + 4U - mod_size; 
  }
  else 
  {
    size  = length;
  }

  size  = size >> 2;
  for (k = 0; k < size; k++)
  {
      *dst = *src;
      src++;
      dst++;
  }
}

void MAC_ReadyToDeepSleep(void)
{  
    MAC_Ds_Param_t param;
    PHY_Retval_t pibStatus = PHY_FAILURE;
    uint64_t attributeValue = 0x00;    
    MAC_MLME_Get(macCoordExtendedAddress,(PibValue_t*)&attributeValue);
    param.mac_CoordExtAddr = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macCoordShortAddress,(PibValue_t*)&attributeValue);
    param.mac_CoordShtAddr = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macMaxFrameTotalWaitTime,(PibValue_t*)&attributeValue);
    param.mac_max_frame_total_wait_time = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macResponseWaitTime,(PibValue_t*)&attributeValue);
    param.mac_response_wait_time = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macAssociatedPANCoord,(PibValue_t*)&attributeValue);
    param.mac_associated_PAN_coord = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macAutoRequest,(PibValue_t*)&attributeValue);
    param.mac_auto_request = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macBattLifeExtPeriods,(PibValue_t*)&attributeValue);
    param.mac_batt_life_ext_periods = attributeValue;
    
    attributeValue = 0x00;
    MAC_MLME_Get(macDSN,(PibValue_t*)&attributeValue);
    param.mac_dsn = attributeValue;
            
    pibStatus = PHY_PibGet(macPANId, (uint8_t *)&panid);
    param.panid = (uint32_t)panid;
    
    pibStatus = PHY_PibGet(macShortAddress, (uint8_t *)&macshortaddr);
    param.mac_short_addr = (uint32_t)macshortaddr;
    
    pibStatus = PHY_PibGet(macIeeeAddress, (uint8_t *)&macieeeaddr);
    param.mac_ieee_addr = macieeeaddr;
    
    uint8_t channelBeforeSleep;
    pibStatus = PHY_PibGet(phyCurrentChannel, &channelBeforeSleep);
    param.phy_current_channel = (uint32_t)channelBeforeSleep; 
        
    memcpy4ByteAligned(&mdsParam,&param,((uint16_t)sizeof(mdsParam))); 
#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))   
    memcpy4ByteAligned(&macSecPibBackup, &macSecPib, ((uint16_t)sizeof(macSecPib)) );
#endif
    (void)pibStatus;
    
} 

static void MAC_WakeUpFromDeepSleep(void)
{   
    uint8_t channelAfterSleep;
    memcpy4ByteAligned(&param1,&mdsParam,((uint16_t)sizeof(mdsParam)));
    uint64_t attributeValue = 0x00;
    attributeValue = param1.mac_CoordExtAddr;
    MAC_MLME_Set(macCoordExtendedAddress,(PibValue_t*)&attributeValue,false);
    
    attributeValue = 0x00;    
    attributeValue = param1.mac_ieee_addr;
    MAC_MLME_Set(macIeeeAddress,(PibValue_t*)&attributeValue,false);
    
    attributeValue = 0x00;
    attributeValue = (uint16_t)param1.mac_CoordShtAddr;
    MAC_MLME_Set(macCoordShortAddress,(PibValue_t*)&attributeValue,false);

    attributeValue = 0x00;    
    attributeValue = (uint16_t)param1.mac_max_frame_total_wait_time;
    MAC_MLME_Set(macMaxFrameTotalWaitTime,(PibValue_t*)&attributeValue,false);

    attributeValue = 0x00;
    attributeValue = (uint16_t)param1.mac_response_wait_time;
    MAC_MLME_Set(macResponseWaitTime,(PibValue_t*)&attributeValue,false);

    attributeValue = 0x00;    
    attributeValue = (uint16_t)param1.mac_associated_PAN_coord;
    MAC_MLME_Set(macAssociatedPANCoord,(PibValue_t*)&attributeValue,false);

    attributeValue = 0x00;
    attributeValue = (uint16_t)param1.mac_auto_request;
    MAC_MLME_Set(macAutoRequest,(PibValue_t*)&attributeValue,false);

    attributeValue = 0x00;    
    attributeValue = (uint16_t)param1.mac_batt_life_ext_periods;
    MAC_MLME_Set(macBattLifeExtPeriods,(PibValue_t*)&attributeValue,false);

    attributeValue = 0x00;
    attributeValue = (uint16_t)param1.mac_dsn;
    MAC_MLME_Set(macDSN,(PibValue_t*)&attributeValue,false);
    

    macshortaddr = (uint16_t)param1.mac_short_addr;
    channelAfterSleep = (uint8_t)param1.phy_current_channel;
    panid = (uint16_t)param1.panid;
    
    PibValue_t pibValue;
    PHY_Retval_t pibStatus = PHY_FAILURE;
       
    pibValue.pib_value_8bit = channelAfterSleep;
    pibStatus = SetPhyPibInternal(phyCurrentChannel, &pibValue);
    
    pibValue.pib_value_16bit = macshortaddr;
    pibStatus = SetPhyPibInternal(macShortAddress, &pibValue);  
    
    pibValue.pib_value_64bit = macieeeaddr;
    pibStatus = SetPhyPibInternal(macIeeeAddress, &pibValue);
    
    pibValue.pib_value_16bit = panid;
    pibStatus = SetPhyPibInternal(macPANId, &pibValue);

#if ((defined MAC_SECURITY_ZIP)  || (defined MAC_SECURITY_2006))   
    memcpy4ByteAligned(&macSecPib, &macSecPibBackup, ((uint16_t)sizeof(macSecPibBackup)) );
#endif
    (void)pibStatus;

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



/* EOF */
