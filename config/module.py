# coding: utf-8
##############################################################################
# Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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
##############################################################################

def loadModule():
    print("Load Module: Harmony IEEE 802.15.4 MAC Standalone")

    pic32cx_bz2_family = {'PIC32CX1012BZ25048',
                          'PIC32CX1012BZ25032',
                          'PIC32CX1012BZ24032',
                          'WBZ451',
                          'WBZ450',
                          'WBZ451H'
                          }
    pic32cx_bz3_family = {'PIC32CX5109BZ31048',
                          'PIC32CX5109BZ31032',
                          'WBZ351',
                          'WBZ350'
                          }
    processor = Variables.get('__PROCESSOR') 
    
    print('processor={}'.format(processor))

    ieee802154mac  = Module.CreateComponent('IEEE_802154_MAC', 'IEEE 802.15.4 MAC', 'Wireless/Drivers/IEEE 802.15.4', 'driver/config/drv_ieee802154_mac.py')
    ieee802154mac.setDisplayType('Standalone MAC Driver')
    ieee802154mac.addDependency('ieee802154phyDependency', 'IEEE 802.15.4 PHY', 'IEEE 802.15.4 PHY', True, True)
    ieee802154mac.addDependency('HarmonyCoreDependency', 'Core Service', 'Core', True, True)
    ieee802154mac.addDependency('SysTimeDependency', 'SYS_TIME', 'SYS_TIME', True, True)
    ieee802154mac.addDependency('FreeRtosDependency', 'RTOS', 'RTOS', True, True)
    ieee802154mac.addDependency('DeviceSupportDependency', 'Device_Support', 'Device_Support', True, True)
    ieee802154mac.addCapability('ieee802154mac_Capability', 'IEEE 802.15.4 MAC', True)
