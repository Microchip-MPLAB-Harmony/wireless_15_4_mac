/*******************************************************************************
  SAL Header
  
  File Name:
    sal.h

  Summary:
    This file contains declarations for low-level security API

  Description:
   The IEEE standard 802.15.4 MAC Layer defines the protocol and compatible
   interconnection for data communication devices using low data rate, low 
   power and low complexity, short-range radio frequency (RF) transmissions in a
   wireless personal area network (WPAN).

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

#ifndef SAL_H
#define SAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "sal_types.h"
#include "sal_generic.h"
#include "stdbool.h"
#include <stdint.h>

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

/*
 * Values for SPI based systems are defined in the corresponding header files
 * for these transceivers.
 */

/* Values for SR_AES_DIR */
#ifndef AES_DIR_ENCRYPT

/**
 * Defines AES direction as encryption
 */
#define AES_DIR_ENCRYPT              (0)
#endif
#ifndef AES_DIR_DECRYPT

/**
 * Defines AES direction as decryption
 */
#define AES_DIR_DECRYPT              (1)
#endif

/* Values for SR_AES_MODE */
#ifndef AES_MODE_ECB

/**
 * Defines AES mode as ECB
 */
#define AES_MODE_ECB                 (0)
#endif


#ifndef AES_MODE_CBC
/**
 * Defines AES mode as CBC
 */
#define AES_MODE_CBC                 (1)
#endif

/** AES core operation direction: Decryption (ECB) */
#define AES_DIR_DECRYPT   (1)

/** AES core operation direction: Encryption (ECB, CBC) */
#define AES_DIR_ENCRYPT   (0)

/** Set CBC mode */
#define AES_MODE_CBC   (1)

/** Set ECB mode */
#define AES_MODE_ECB   (0)

/** Set CTR mode */
#define AES_MODE_CTR    (2)

/* Nonce size. */
#define NONCE_SIZE              (13)



// *****************************************************************************
// *****************************************************************************
// Types
// *****************************************************************************
// *****************************************************************************
/**
 * AES status values
 */
typedef enum {
	/** AES CCM en/decryption OK */
	AES_SUCCESS = 0,
	/** AES failure */
	AES_FAILURE,

} SAL_AesStatus_t;
/* === Externals ========================================================== */

// *****************************************************************************
// *****************************************************************************
// Prototypes
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialization of SAL.
 *
 * This functions initializes the SAL.
 *
 * @ingroup group_SalApi
 */
void SAL_Init(void);

/**
 * @brief aes set key for SAL.
 *
 * This function set the aes set key and key length for SAL.
 *
 * @param[in] key The key to be used; if NULL, use the current key
 * @param[in] key_len Length of key
 */

SAL_AesStatus_t SAL_AesSetKey(uint8_t *key, uint8_t key_len);

/**
 * @brief aes ccm secure for SAL.
 *
 * This function for aes secure with CCM.
 *
 * @param[in]  buffer Input data
 * @param[in]  nonce   The nonce: Initialization Vector (IV) as used in
 *                     cryptography
 * @param[in] key The key to be used; if NULL, use the current key
 * @param[in] hdr_len Length of plaintext header (will not be encrypted)
 * @param[in] pld_len Length of payload to be encrypted; if 0, then only MIC
 *                    authentication implies
 * @param[in] aes_dir AES_DIR_ENCRYPT if secure, AES_DIR_DECRYPT if unsecure
 */

SAL_AesStatus_t SAL_AesCcmSecure(uint8_t *buffer,
		uint8_t nonce[AES_BLOCKSIZE],
		uint8_t hdr_len,
		uint8_t pld_len,
		uint8_t aes_dir, uint8_t mic_len, uint8_t enc_flag);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* SAL_H */
/* EOF */
