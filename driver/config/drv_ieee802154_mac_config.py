# coding: utf-8
##############################################################################
# Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
###############################################################################

#
# Interface
#

# ========== Device type Configuration for MAC ====================

global maclibraryGen
maclibraryGen = ieee802154mac.createKeyValueSetSymbol("MAC_LIBRARY_GENERATION",None)
maclibraryGen.setLabel("Mac Generation")
maclibraryGen.addKey("Source","Source","Source")
maclibraryGen.addKey("Library","Library","Library")
maclibraryGen.setDefaultValue(0)
maclibraryGen.setVisible(True)
maclibraryGen.setDependencies(libGenConfiguration,["MAC_LIBRARY_GENERATION"])

macDeviceConfig = ieee802154mac.createMenuSymbol("MAC_DEVICE_CONFIG", None)
macDeviceConfig.setLabel("Device Type Configuration")
macDeviceConfig.setVisible(True)

macDeviceType = ieee802154mac.createKeyValueSetSymbol("MAC_DEVICE_TYPE", macDeviceConfig)
macDeviceType.setLabel("MAC Device Type")
macDeviceType.addKey("FFD", "0", "FFD")
macDeviceType.addKey("RFD", "1", "RFD")
macDeviceType.setDefaultValue(0)
macDeviceType.setOutputMode("Value")
macDeviceType.setDisplayMode("Description")
macDeviceType.setDescription("MAC Device Type of the application")
macDeviceType.setDependencies(DeviceTypeConfiguration,["MAC_DEVICE_TYPE"])

# ========== Sleep Configuration =====================================

global DeepSleepEnable
DeepSleepEnable = ieee802154mac.createBooleanSymbol('DEEP_SLEEP_ENABLE', macDeviceConfig)
DeepSleepEnable.setLabel('Enable Deep Sleep')
DeepSleepEnable.setDefaultValue(False)
DeepSleepEnable.setVisible(False)
DeepSleepEnable.setDependencies(SleepConfiguration, ["DEEP_SLEEP_ENABLE"])

# ========== Security Configuration for MAC ===========================

macSecurityConfig = ieee802154mac.createMenuSymbol("MAC_SECURITY_CONFIG", None)
macSecurityConfig.setLabel("Security Configuration")
macSecurityConfig.setVisible(True)

SecurityConfig = ieee802154mac.createKeyValueSetSymbol("MAC_SECURITY_OPTION", macSecurityConfig)
SecurityConfig.setLabel("MAC Security Config")
SecurityConfig.addKey("DISABLED", "0", "DISABLED")
SecurityConfig.addKey("ENABLED", "1", "ENABLED")
SecurityConfig.setDefaultValue(0)
SecurityConfig.setOutputMode("Value")
SecurityConfig.setDisplayMode("Description")
SecurityConfig.setDescription("MAC Security option of the application")
SecurityConfig.setDependencies(SecurityConfiguration,["MAC_SECURITY_OPTION"])

# ========== Buffer Configuration for MAC ================================

macMenuBmm = ieee802154mac.createMenuSymbol("MAC_MENU_BMM", None)
macMenuBmm.setLabel("MAC Buffer Configuration")
macMenuBmm.setVisible(True)

global macIntegerBmmLargeBuffers
macIntegerBmmLargeBuffers = ieee802154mac.createIntegerSymbol("MAC_INTEGER_BMMLARGEBUFFERS", macMenuBmm)
macIntegerBmmLargeBuffers.setLabel("Large Buffers")
macIntegerBmmLargeBuffers.setMin(6)
macIntegerBmmLargeBuffers.setMax(50)
macIntegerBmmLargeBuffers.setDefaultValue(10)


global macIndirectIntegerBmmLargeBuffers
macIndirectIntegerBmmLargeBuffers = ieee802154mac.createIntegerSymbol("MAC_INDIRECT_INTEGER_BMMLARGEBUFFERS", macIntegerBmmLargeBuffers)
macIndirectIntegerBmmLargeBuffers.setLabel("Indirect Buffers")
macIndirectIntegerBmmLargeBuffers.setVisible(True)
macIndirectIntegerBmmLargeBuffers.setMin(2)
macIndirectIntegerBmmLargeBuffers.setMax(25)
macIndirectIntegerBmmLargeBuffers.setDefaultValue(2)

global macCommentBmmLargeBuffers
macCommentBmmLargeBuffers = ieee802154mac.createCommentSymbol("MAC_COMMENT_BMMLARGEBUFFERS", macIntegerBmmLargeBuffers)
macCommentBmmLargeBuffers.setVisible(True)
timersTotalMem = macConstLargeBufferSize * macIntegerBmmLargeBuffers.getValue()
macCommentBmmLargeBuffers.setLabel("Memory occupied: ~%d bytes" %timersTotalMem)
macCommentBmmLargeBuffers.setDependencies(macCommentBmmLargeBuffersDepend, ["MAC_INTEGER_BMMLARGEBUFFERS"])


global macIntegerBmmSmallBuffers
macIntegerBmmSmallBuffers = ieee802154mac.createIntegerSymbol("MAC_INTEGER_BMMSMALLBUFFERS", macMenuBmm)
macIntegerBmmSmallBuffers.setLabel("Small Buffers")
macIntegerBmmSmallBuffers.setMin(3)
macIntegerBmmSmallBuffers.setMax(50)
macIntegerBmmSmallBuffers.setDefaultValue(3)

global macCommentBmmSmallBuffers
macCommentBmmSmallBuffers = ieee802154mac.createCommentSymbol("MAC_COMMENT_BMMSMALLBUFFERS", macIntegerBmmSmallBuffers)
macCommentBmmSmallBuffers.setVisible(True)
timersTotalMem = macConstSmallBufferSize * macIntegerBmmSmallBuffers.getValue()
macCommentBmmSmallBuffers.setLabel("Memory occupied: ~%d bytes" %timersTotalMem)
macCommentBmmSmallBuffers.setDependencies(macCommentBmmSmallBuffersDepend, ["MAC_INTEGER_BMMSMALLBUFFERS"])


