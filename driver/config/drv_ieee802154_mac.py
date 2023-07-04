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

#-------------------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ IEEE 802.15.4 MAC CONFIGURATIONS ~~~~~~~~~~~~~~~~
#-------------------------------------------------------------------------------

def instantiateComponent(ieee802154mac):
    print("IEEE 802.15.4 MAC Standlone library driver component")
    configName = Variables.get("__CONFIGURATION_NAME")
    print configName
    # === Activate required components automatically
    global requiredComponents
    requiredComponents = [
        "IEEE_802154_PHY",
        "HarmonyCore",
        "sys_time",
        "pic32cx_bz2_devsupport",
        "RTOS",
        "trng"
    ]
    
    conditionAlwaysInclude = [True, None, []]
    condSecurity = [False, SecurityFilesConfig, ['MAC_SECURITY_OPTION']]

    global macConstLargeBufferSize
    macConstLargeBufferSize = 172
    global macConstSmallBufferSize
    macConstSmallBufferSize = 60
    
    res = Database.activateComponents(requiredComponents)
    
    # === Interfaces
 
    
    # === Radio menu
    execfile(Module.getPath() + "/driver/config/drv_ieee802154_mac_config.py")
    
    # === Header Files
    
    includeMac = [
        ["mac/inc/ieee_mac_const.h", conditionAlwaysInclude],
        ["mac/inc/mac.h", conditionAlwaysInclude],
        ["mac/inc/mac_api.h", conditionAlwaysInclude],
        ["mac/inc/mac_config.h", conditionAlwaysInclude],
        ["mac/inc/mac_data_structures.h", conditionAlwaysInclude],
        ["mac/inc/mac_internal.h", conditionAlwaysInclude],
        ["mac/inc/mac_msg_const.h", conditionAlwaysInclude],
        ["mac/inc/mac_msg_type_data_ind.h", conditionAlwaysInclude],
        ["mac/inc/mac_msg_types.h", conditionAlwaysInclude],
        ["mac/inc/mac_security.h", conditionAlwaysInclude],
        ["mac/inc/mac_tasks.h", conditionAlwaysInclude],
    ]
    
    includeStb = [
        ["stb/inc/stb.h", condSecurity],
        ["stb/inc/stb_generic.h", condSecurity],
    ]
    includeServices = [
        ["services/sal/inc/sal.h", condSecurity],
        ["services/sal/inc/sal_generic.h", condSecurity],
        ["services/sal/inc/sal_types.h", condSecurity],
    ]
    

    # === Import the header files
    
    for incFileEntry in includeMac:
        importIncFile(ieee802154mac, configName, incFileEntry)
    for incFileEntry in includeStb:
        importIncFile(ieee802154mac, configName, incFileEntry)
    for incFileEntry in includeServices:
        importIncFile(ieee802154mac, configName, incFileEntry)

    # # === Source files
    
    srcMac = [
        ["mac/src/mac.c", conditionAlwaysInclude],
        ["mac/src/mac_api.c", conditionAlwaysInclude],
        ["mac/src/mac_associate.c", conditionAlwaysInclude],
        ["mac/src/mac_beacon.c", conditionAlwaysInclude],
        ["mac/src/mac_callback_wrapper.c", conditionAlwaysInclude],
        ["mac/src/mac_data_extract_mhr.c", conditionAlwaysInclude],
        ["mac/src/mac_data_ind.c", conditionAlwaysInclude],
        ["mac/src/mac_data_req.c", conditionAlwaysInclude],
        ["mac/src/mac_disassociate.c", conditionAlwaysInclude],
        ["mac/src/mac_dispatcher.c", conditionAlwaysInclude],
        ["mac/src/mac_mcps_data.c", conditionAlwaysInclude],
        ["mac/src/mac_misc.c", conditionAlwaysInclude],
        ["mac/src/mac_orphan.c", conditionAlwaysInclude],
        ["mac/src/mac_pib.c", conditionAlwaysInclude],
        ["mac/src/mac_poll.c", conditionAlwaysInclude],
        ["mac/src/mac_process_beacon_frame.c", conditionAlwaysInclude],
        ["mac/src/mac_process_phy_tx_frame_status.c", conditionAlwaysInclude],
        ["mac/src/mac_rx_enable.c", conditionAlwaysInclude],
        ["mac/src/mac_scan.c", conditionAlwaysInclude],
        ["mac/src/mac_security.c", conditionAlwaysInclude],
        ["mac/src/mac_start.c", conditionAlwaysInclude],
        ["mac/src/mac_sync.c", conditionAlwaysInclude],
        ["mac/src/mac_task.c", conditionAlwaysInclude],
        ["mac/src/mac_tx_coord_realignment_command.c", conditionAlwaysInclude],
        ["mac/src/usr_mcps_data_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mcps_data_ind.c", conditionAlwaysInclude],
        ["mac/src/usr_mcps_purge_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_associate_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_associate_ind.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_beacon_notify_ind.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_comm_status_ind.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_disassociate_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_disassociate_ind.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_get_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_orphan_ind.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_poll_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_reset_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_rx_enable_conf.c", conditionAlwaysInclude],  
        ["mac/src/usr_mlme_scan_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_set_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_start_conf.c", conditionAlwaysInclude],
        ["mac/src/usr_mlme_sync_loss_ind.c", conditionAlwaysInclude],         
        
    ]
    srcStb = [
        ["stb/src/stb.c", condSecurity]
    ]

    srcServices = [
        ["services/sal/pic32cx_bz2/src/sal.c", condSecurity]
    ]

    # === Import the source files
    for srcFileEntry in srcMac:
        importSrcFile(ieee802154mac, configName, srcFileEntry)
    for srcFileEntry in srcStb:
        importSrcFile(ieee802154mac, configName, srcFileEntry)
    for srcFileEntry in srcServices:
        importSrcFile(ieee802154mac, configName, srcFileEntry)


    # === Include path setting
    includePathsMac = [
        ["/IEEE_802154_MAC/mac/inc", conditionAlwaysInclude]
    ]
    
    for incPathEntry in includePathsMac:
        setIncPath(ieee802154mac, configName, incPathEntry)
    
    includePathsMacSecurity = [
        ["/IEEE_802154_MAC/stb/inc/", condSecurity],
        ["/IEEE_802154_MAC/services/sal/inc/", condSecurity]
    ]
    
    for incPathEntry in includePathsMacSecurity:
        setIncPath(ieee802154mac, configName, incPathEntry)
        
    global preprocessorCompiler
    # === Compiler macros
    preprocessorCompiler = ieee802154mac.createSettingSymbol("IEEE802154MAC_XC32_PREPRECESSOR", None)
    preprocessorCompiler.setCategory("C32")
    preprocessorCompiler.setKey("preprocessor-macros")
    preprocessorCompiler.setValue("ENABLE_TSTAMP;FFD")
         

    # === File templates processing
    macConfHeader = ieee802154mac.createFileSymbol("MAC_CONF_HEADER", None)
    macConfHeader.setSourcePath('/driver/templates/stack_config.h.ftl')
    macConfHeader.setOutputName('stack_config.h')
    macConfHeader.setDestPath('../../')
    macConfHeader.setProjectPath('')
    macConfHeader.setType('HEADER')
    macConfHeader.setOverwrite(True)
    macConfHeader.setMarkup(True)
    
    macConfHeader = ieee802154mac.createFileSymbol("MAC_APP_CONF_HEADER", None)
    macConfHeader.setSourcePath("/driver/templates/app_config.h.ftl")
    macConfHeader.setOutputName("app_config.h")
    macConfHeader.setDestPath('../../')
    macConfHeader.setProjectPath('')
    macConfHeader.setType("HEADER")
    macConfHeader.setOverwrite(True)
    macConfHeader.setMarkup(True)

    macDefinitionsH = ieee802154mac.createFileSymbol('IEEE802154MAC_DEFINITIONS_H', None)
    macDefinitionsH.setType('STRING')
    macDefinitionsH.setOutputName('core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES')
    macDefinitionsH.setSourcePath('driver/templates/system/system_definitions.h.ftl')
    macDefinitionsH.setMarkup(True)
    
    macTasksC = ieee802154mac.createFileSymbol('IEEE802154MAC_TASKS_C', None)
    macTasksC.setType('STRING')
    macTasksC.setOutputName('core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS')
    macTasksC.setSourcePath('driver/templates/system/system_tasks.c.ftl')
    macTasksC.setMarkup(True)
    
    macTasksDefC = ieee802154mac.createFileSymbol('IEEE802154MAC_TASK_INITIALIZATION_C', None)
    macTasksDefC.setType('STRING')
    macTasksDefC.setOutputName('core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS')
    macTasksDefC.setSourcePath('driver/templates/system/system_tasks_def.c.ftl')
    macTasksDefC.setMarkup(True)
    
    macInitC = ieee802154mac.createFileSymbol('IEEE802154MAC_INITIALIZATION_C', None)
    macInitC.setType('STRING')
    macInitC.setOutputName('core.LIST_SYSTEM_INIT_C_APP_INITIALIZE_DATA')
    macInitC.setSourcePath('driver/templates/system/system_initialize_middleware.c.ftl')
    macInitC.setMarkup(True)

    # === Treat warnings as errors
    macWarnAsErr = ieee802154mac.createSettingSymbol("IEEE802154MAC_GCC_WARN_ERROR", None)
    macWarnAsErr.setValue("false")
    macWarnAsErr.setCategory("C32")
    macWarnAsErr.setKey("make-warnings-into-errors")

    # === Set optimization level
    macOptLevel = ieee802154mac.createSettingSymbol("PET_LEVEL", None)
    macOptLevel.setValue("-O2")
    macOptLevel.setCategory("C32")
    macOptLevel.setKey("optimization-level")

    # === Library

# end instantiateComponent

def finalizeComponent(ieee802154mac):
    pass
#end finalizeComponent

def importIncFile(component, configName, incFileEntry, firmwarePath = None):
    incFilePath  = incFileEntry[0]
    isEnabled    = incFileEntry[1][0]
    callback     = incFileEntry[1][1]
    dependencies = incFileEntry[1][2]

    incFilePathTup = incFilePath.rsplit("/", 1)

    if len(incFilePathTup) == 1:
        secName = ""
        incFile = incFilePathTup[0]
    else :
        secName = incFilePathTup[0]
        incFile = incFilePathTup[1]

    symName = incFile.replace(".", "_").upper()
    secSName = secName + "/"
    secDName = secSName
    print("importIncFile: ", secDName)

    incFileSym = component.createFileSymbol(symName, None)
    incFileSym.setSourcePath("driver/software/" + secSName + "/" + incFile)
    incFileSym.setOutputName(incFile)
    incFileSym.setDestPath("IEEE_802154_MAC/" + secDName + "")
    incFileSym.setProjectPath("config/" + configName + "/IEEE_802154_MAC/"+ secDName + "")
    incFileSym.setType("HEADER")
    incFileSym.setOverwrite(True)
    incFileSym.setEnabled(isEnabled)

    if callback and dependencies:
        incFileSym.setDependencies(callback, dependencies)
#end importIncFile

def importSrcFile(component, configName, srcFileEntry, firmwarePath = None):
    srcFilePath  = srcFileEntry[0]
    isEnabled    = srcFileEntry[1][0]
    callback     = srcFileEntry[1][1]
    dependencies = srcFileEntry[1][2]

    srcFilePathTup = srcFilePath.rsplit("/", 1)

    if len(srcFilePathTup) == 1:
        secName = ""
        srcFile = srcFilePathTup[0]
    else:
        secName = srcFilePathTup[0]
        srcFile = srcFilePathTup[1]

    srcFilePrefix   = ""
    symName = srcFile.replace(".", "_").upper()
    secSName = secName + "/"
    secDName = secSName
    print("******", secDName)

    srcFileSym = component.createFileSymbol(symName, None)
    srcFileSym.setSourcePath("driver/software/" + secSName + srcFile)
    srcFileSym.setOutputName(srcFile.rsplit("/", 1)[-1])
    srcFileSym.setDestPath("IEEE_802154_MAC/" + secDName + "")
    srcFileSym.setProjectPath("config/" + configName + "/IEEE_802154_MAC/"+ secDName + "")
    srcFileSym.setType("SOURCE")
    srcFileSym.setEnabled(isEnabled)

    if callback and dependencies:
        srcFileSym.setDependencies(callback, dependencies)
#end importSrcFile

def setIncPath(component, configName, incPathEntry):
    incPath      = incPathEntry[0]
    isEnabled    = incPathEntry[1][0]
    callback     = incPathEntry[1][1]
    dependencies = incPathEntry[1][2]
    incPathSym = component.createSettingSymbol("IEEE_802154_MAC_INC_PATH" + incPath.replace(".", "_").replace("/", "_").upper(), None)
    incPathSym.setValue("../src/config/" + configName + incPath + ";")
    incPathSym.setCategory("C32")
    incPathSym.setKey("extra-include-directories")
    incPathSym.setAppend(True, ";")
    incPathSym.setEnabled(isEnabled)
    incPathSym.setDependencies(callback, dependencies)
    
    
    incPathSymCpp = component.createSettingSymbol("IEEE_802154_MAC_INC_PATH_CPP" + incPath.replace(".", "_").replace("/", "_").upper(), None)
    incPathSymCpp.setValue("../src/config/" + configName + incPath + ";")
    incPathSymCpp.setCategory("C32CPP")
    incPathSymCpp.setKey("extra-include-directories")
    incPathSymCpp.setAppend(True, ";")
    incPathSymCpp.setEnabled(isEnabled)
    incPathSymCpp.setDependencies(callback, dependencies)
#end setIncPath

#-------------------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DEPENDENCY CALLBACKS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#-------------------------------------------------------------------------------
def DeviceTypeConfiguration(symbol,event):
    setDeviceType = event['value']
    if setDeviceType == 0:#FFD
        preprocessorMacro = preprocessorCompiler.getValue()
        preprocessorMacro = preprocessorMacro + ";FFD"
        preprocessorCompiler.setValue(preprocessorMacro)
        preprocessorCompiler.setEnabled(True)
        macIndirectIntegerBmmLargeBuffers.setVisible(True)       
        EnableSleep.setVisible(False)
        EnableSleep.setValue(False)
        HandleSleep(False)
        
        
    if setDeviceType == 1:#RFD
        preprocessorMacro = preprocessorCompiler.getValue()
        preprocessorMacro = preprocessorMacro.replace(";FFD","")
        preprocessorCompiler.setValue(preprocessorMacro)
        preprocessorCompiler.setEnabled(True)  
        macIndirectIntegerBmmLargeBuffers.setVisible(False)
        EnableSleep.setVisible(True)
        EnableSleep.setValue(False)  
       
def SleepConfiguration(symbol,event):
    sleepEnable = event['value']
    HandleSleep(sleepEnable)
    
def HandleSleep(sleepEnable):
    if sleepEnable == True:
        preprocessorSleepMacro = preprocessorCompiler.getValue()
        preprocessorSleepMacro = preprocessorSleepMacro + ";ENABLE_SLEEP"
        preprocessorCompiler.setValue(preprocessorSleepMacro)
        preprocessorCompiler.setEnabled(True)
        Database.setSymbolValue("pic32cx_bz2_devsupport", "ENABLE_DEEP_SLEEP", True)
        Database.setSymbolValue("pic32cx_bz2_devsupport", "SYSTEM_ENABLE_PMUMODE_SETTING", True)
        
    if sleepEnable == False:
        preprocessorSleepMacro = preprocessorCompiler.getValue()
        preprocessorSleepMacro = preprocessorSleepMacro.replace(";ENABLE_SLEEP","")
        preprocessorCompiler.setValue(preprocessorSleepMacro)
        preprocessorCompiler.setEnabled(True) 
        Database.setSymbolValue("pic32cx_bz2_devsupport", "ENABLE_DEEP_SLEEP", False)
        Database.setSymbolValue("pic32cx_bz2_devsupport", "SYSTEM_ENABLE_PMUMODE_SETTING", False) 
     

def SecurityConfiguration(symbol,event):
    Securityoption = event['value']
    if Securityoption == 1:#Enabled
        preprocessorSecurity = preprocessorCompiler.getValue()
        preprocessorSecurity = preprocessorSecurity + ";MAC_SECURITY_ZIP;MAC_SECURITY_2006;STB_ON_SAL"
        preprocessorCompiler.setValue(preprocessorSecurity)
        preprocessorCompiler.setEnabled(True)      
        Database.activateComponents(['lib_wolfcrypt']) 
        Database.setSymbolValue("core", "AES_CLOCK_ENABLE", True)
                  
    if Securityoption == 0:#Disabled
        preprocessorSecurity = preprocessorCompiler.getValue()
        preprocessorSecurity = preprocessorSecurity.replace(";MAC_SECURITY_ZIP;MAC_SECURITY_2006;STB_ON_SAL","")
        preprocessorCompiler.setValue(preprocessorSecurity)
        preprocessorCompiler.setEnabled(True)
        Database.deactivateComponents(['lib_wolfcrypt'])
        Database.setSymbolValue("core", "AES_CLOCK_ENABLE", False)
        

def SecurityFilesConfig(symbol,event):
    Securityoption = event['value']
    if Securityoption == 1:#Enabled
        symbol.setEnabled(True)        
    if Securityoption == 0:#Disabled
        symbol.setEnabled(False) 

def macCommentBmmLargeBuffersDepend(sourceSymbol, event):
    totalMem = macConstLargeBufferSize * macIntegerBmmLargeBuffers.getValue()
    macCommentBmmLargeBuffers.setLabel("Memory occupied: ~%d bytes" %totalMem)

def macCommentBmmSmallBuffersDepend(sourceSymbol, event):
    totalMem = macConstSmallBufferSize * macIntegerBmmSmallBuffers.getValue()
    macCommentBmmSmallBuffers.setLabel("Memory occupied: ~%d bytes" %totalMem)

#end dependency callbacks

def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]
    if  connectID == 'ieee802154phyDependency':   
          remoteComponent.getSymbolByID("CREATE_PHY_RTOS_TASK").setValue(False)
          remoteComponent.getSymbolByID("CREATE_PHY_RTOS_TASK").setReadOnly(True)  

    remoteComponent = Database.getComponentByID("trng")
    if (remoteComponent):
          print('Printing TRNG remoteComponent Value')
          remoteComponent.getSymbolByID("trngEnableInterrupt").setReadOnly(True)
          remoteComponent.getSymbolByID("trngEnableEvent").setReadOnly(True)
          remoteComponent.getSymbolByID("TRNG_STANDBY").setReadOnly(True)         
  
    
def onAttachmentDisconnected(source, target):

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    if  connectID == 'ieee802154phyDependency':   
          remoteComponent.getSymbolByID("CREATE_PHY_RTOS_TASK").setValue(True)
          remoteComponent.getSymbolByID("CREATE_PHY_RTOS_TASK").setReadOnly(False)
          
def destroyComponent(ieee802154mac):
    Database.deactivateComponents(requiredComponents)