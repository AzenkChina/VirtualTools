#-------------------------------------------------
#
# Project created by QtCreator 2020-05-18T17:07:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = DLMSProgrammer
TEMPLATE = app

win32 {
    LIBS += -lws2_32 -lWinmm
}
unix {
    LIBS += -lpthread -lrt
    QMAKE_RPATHDIR=$ORIGIN/lib
    QMAKE_LFLAGS += -fno-pie -no-pie
}

VERSION = 1.0.0.0
QMAKE_TARGET_COMPANY = azenk
QMAKE_TARGET_PRODUCT = "DLMSProgrammer"
QMAKE_TARGET_DESCRIPTION = "DLMS Programmer"
QMAKE_TARGET_COPYRIGHT = azenk

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    communication.cpp \
    src/GXAdjacentCell.cpp \
    src/GXAPDU.cpp \
    src/GXApplicationContextName.cpp \
    src/GXAuthenticationMechanismName.cpp \
    src/GXBitString.cpp \
    src/GXBytebuffer.cpp \
    src/GXChargePerUnitScaling.cpp \
    src/GXChargeTable.cpp \
    src/GXCipher.cpp \
    src/GXCommodity.cpp \
    src/GXCreditChargeConfiguration.cpp \
    src/GXCurrency.cpp \
    src/GXDateTime.cpp \
    src/GXDLMS.cpp \
    src/GXDLMSAccount.cpp \
    src/GXDLMSActionItem.cpp \
    src/GXDLMSActionSchedule.cpp \
    src/GXDLMSActionSet.cpp \
    src/GXDLMSActivityCalendar.cpp \
    src/GXDLMSAssociationLogicalName.cpp \
    src/GXDLMSAssociationShortName.cpp \
    src/GXDLMSAutoAnswer.cpp \
    src/GXDLMSAutoConnect.cpp \
    src/GXDLMSCaptureObject.cpp \
    src/GXDLMSCertificateInfo.cpp \
    src/GXDLMSCharge.cpp \
    src/GXDLMSClient.cpp \
    src/GXDLMSClock.cpp \
    src/GXDLMSCompactData.cpp \
    src/GXDLMSContextType.cpp \
    src/GXDLMSConverter.cpp \
    src/GXDLMSCredit.cpp \
    src/GXDLMSData.cpp \
    src/GXDLMSDayProfile.cpp \
    src/GXDLMSDayProfileAction.cpp \
    src/GXDLMSDemandRegister.cpp \
    src/GXDLMSDisconnectControl.cpp \
    src/GXDLMSEmergencyProfile.cpp \
    src/GXDLMSExtendedRegister.cpp \
    src/GXDLMSGPRSSetup.cpp \
    src/GXDLMSGSMCellInfo.cpp \
    src/GXDLMSGSMDiagnostic.cpp \
    src/GXDLMSHdlcSetup.cpp \
    src/GXDLMSIECOpticalPortSetup.cpp \
    src/GXDLMSImageActivateInfo.cpp \
    src/GXDLMSImageTransfer.cpp \
    src/GXDLMSIp4Setup.cpp \
    src/GXDLMSIp4SetupIpOption.cpp \
    src/GXDLMSLimiter.cpp \
    src/GXDLMSLimits.cpp \
    src/GXDLMSLlcSscsSetup.cpp \
    src/GXDLMSLNCommandHandler.cpp \
    src/GXDLMSLNParameters.cpp \
    src/GXDLMSMacAddressSetup.cpp \
    src/GXDLMSMBusClient.cpp \
    src/GXDLMSMBusMasterPortSetup.cpp \
    src/GXDLMSMBusSlavePortSetup.cpp \
    src/GXDLMSMd5.cpp \
    src/GXDLMSMessageHandler.cpp \
    src/GXDLMSModemConfiguration.cpp \
    src/GXDLMSModemInitialisation.cpp \
    src/GXDLMSMonitoredValue.cpp \
    src/GXDLMSNotify.cpp \
    src/GXDLMSObject.cpp \
    src/GXDLMSObjectCollection.cpp \
    src/GXDLMSObjectDefinition.cpp \
    src/GXDLMSObjectFactory.cpp \
    src/GXDLMSParameterMonitor.cpp \
    src/GXDLMSPppSetup.cpp \
    src/GXDLMSPppSetupIPCPOption.cpp \
    src/GXDLMSPppSetupLcpOption.cpp \
    src/GXDLMSPrimeNbOfdmPlcApplicationsIdentification.cpp \
    src/GXDLMSPrimeNbOfdmPlcMacCounters.cpp \
    src/GXDLMSPrimeNbOfdmPlcMacFunctionalParameters.cpp \
    src/GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData.cpp \
    src/GXDLMSPrimeNbOfdmPlcMacSetup.cpp \
    src/GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters.cpp \
    src/GXDLMSProfileGeneric.cpp \
    src/GXDLMSPushObject.cpp \
    src/GXDLMSPushSetup.cpp \
    src/GXDLMSQualityOfService.cpp \
    src/GXDLMSRegister.cpp \
    src/GXDLMSRegisterActivation.cpp \
    src/GXDLMSRegisterMonitor.cpp \
    src/GXDLMSSapAssignment.cpp \
    src/GXDLMSSchedule.cpp \
    src/GXDLMSScheduleEntry.cpp \
    src/GXDLMSScript.cpp \
    src/GXDLMSScriptAction.cpp \
    src/GXDLMSScriptTable.cpp \
    src/GXDLMSSeasonProfile.cpp \
    src/GXDLMSSecureClient.cpp \
    src/GXDLMSSecureServer.cpp \
    src/GXDLMSSecuritySetup.cpp \
    src/GXDLMSServer.cpp \
    src/GXDLMSSettings.cpp \
    src/GXDLMSSha1.cpp \
    src/GXDLMSSha256.cpp \
    src/GXDLMSSNCommandHandler.cpp \
    src/GXDLMSSNParameters.cpp \
    src/GXDLMSSpecialDay.cpp \
    src/GXDLMSSpecialDaysTable.cpp \
    src/GXDLMSTarget.cpp \
    src/GXDLMSTcpUdpSetup.cpp \
    src/GXDLMSTokenGateway.cpp \
    src/GXDLMSTranslator.cpp \
    src/GXDLMSTranslatorStructure.cpp \
    src/GXDLMSUtilityTables.cpp \
    src/GXDLMSValueEventArg.cpp \
    src/GXDLMSVariant.cpp \
    src/GXDLMSWeekProfile.cpp \
    src/GXHelpers.cpp \
    src/GXReplyData.cpp \
    src/GXSecure.cpp \
    src/GXSerialNumberCounter.cpp \
    src/GXSNInfo.cpp \
    src/GXStandardObisCode.cpp \
    src/GXStandardObisCodeCollection.cpp \
    src/GXTokenGatewayConfiguration.cpp \
    src/GXUnitCharge.cpp \
    src/GXXmlReader.cpp \
    src/GXXmlWriter.cpp \
    src/GXXmlWriterSettings.cpp \
    src/GXDLMSCommon.cpp \
    programmer.cpp

HEADERS += \
    communication.h \
    include/chipperingenums.h \
    include/enums.h \
    include/errorcodes.h \
    include/GXAdjacentCell.h \
    include/GXAPDU.h \
    include/GXApplicationContextName.h \
    include/GXAttributeCollection.h \
    include/GXAuthentication.h \
    include/GXAuthenticationMechanismName.h \
    include/GXBitString.h \
    include/GXBytebuffer.h \
    include/GXChargePerUnitScaling.h \
    include/GXChargeTable.h \
    include/GXCipher.h \
    include/GXCommodity.h \
    include/GXCreditChargeConfiguration.h \
    include/GXCurrency.h \
    include/GXDataInfo.h \
    include/GXDate.h \
    include/GXDateTime.h \
    include/GXDLMS.h \
    include/GXDLMSAccount.h \
    include/GXDLMSActionItem.h \
    include/GXDLMSActionSchedule.h \
    include/GXDLMSActionSet.h \
    include/GXDLMSActivityCalendar.h \
    include/GXDLMSAssociationLogicalName.h \
    include/GXDLMSAssociationShortName.h \
    include/GXDLMSAttribute.h \
    include/GXDLMSAutoAnswer.h \
    include/GXDLMSAutoConnect.h \
    include/GXDLMSCaptureObject.h \
    include/GXDLMSCertificateInfo.h \
    include/GXDLMSCharge.h \
    include/GXDLMSClient.h \
    include/GXDLMSClock.h \
    include/GXDLMSCompactData.h \
    include/GXDLMSConnectionEventArgs.h \
    include/GXDLMSContextType.h \
    include/GXDLMSConverter.h \
    include/GXDLMSCredit.h \
    include/GXDLMSData.h \
    include/GXDLMSDayProfile.h \
    include/GXDLMSDayProfileAction.h \
    include/GXDLMSDemandRegister.h \
    include/GXDLMSDisconnectControl.h \
    include/GXDLMSEmergencyProfile.h \
    include/GXDLMSExtendedRegister.h \
    include/GXDLMSGPRSSetup.h \
    include/GXDLMSGSMCellInfo.h \
    include/GXDLMSGSMDiagnostic.h \
    include/GXDLMSHdlcSetup.h \
    include/GXDLMSIECOpticalPortSetup.h \
    include/GXDLMSImageActivateInfo.h \
    include/GXDLMSImageTransfer.h \
    include/GXDLMSIp4Setup.h \
    include/GXDLMSIp4SetupIpOption.h \
    include/GXDLMSLimiter.h \
    include/GXDLMSLimits.h \
    include/GXDLMSLlcSscsSetup.h \
    include/GXDLMSLNCommandHandler.h \
    include/GXDLMSLNParameters.h \
    include/GXDLMSLongTransaction.h \
    include/GXDLMSMacAddressSetup.h \
    include/GXDLMSMBusClient.h \
    include/GXDLMSMBusMasterPortSetup.h \
    include/GXDLMSMBusSlavePortSetup.h \
    include/GXDLMSMd5.h \
    include/GXDLMSMessageHandler.h \
    include/GXDLMSModemConfiguration.h \
    include/GXDLMSModemInitialisation.h \
    include/GXDLMSMonitoredValue.h \
    include/GXDLMSNotify.h \
    include/GXDLMSObject.h \
    include/GXDLMSObjectCollection.h \
    include/GXDLMSObjectDefinition.h \
    include/GXDLMSObjectFactory.h \
    include/GXDLMSParameterMonitor.h \
    include/GXDLMSPppSetup.h \
    include/GXDLMSPppSetupIPCPOption.h \
    include/GXDLMSPppSetupLcpOption.h \
    include/GXDLMSPrimeNbOfdmPlcApplicationsIdentification.h \
    include/GXDLMSPrimeNbOfdmPlcMacCounters.h \
    include/GXDLMSPrimeNbOfdmPlcMacFunctionalParameters.h \
    include/GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData.h \
    include/GXDLMSPrimeNbOfdmPlcMacSetup.h \
    include/GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters.h \
    include/GXDLMSProfileGeneric.h \
    include/GXDLMSPushObject.h \
    include/GXDLMSPushSetup.h \
    include/GXDLMSQualityOfService.h \
    include/GXDLMSRegister.h \
    include/GXDLMSRegisterActivation.h \
    include/GXDLMSRegisterMonitor.h \
    include/GXDLMSSapAssignment.h \
    include/GXDLMSSchedule.h \
    include/GXDLMSScheduleEntry.h \
    include/GXDLMSScript.h \
    include/GXDLMSScriptAction.h \
    include/GXDLMSScriptTable.h \
    include/GXDLMSSeasonProfile.h \
    include/GXDLMSSecureClient.h \
    include/GXDLMSSecureServer.h \
    include/GXDLMSSecuritySetup.h \
    include/GXDLMSServer.h \
    include/GXDLMSSettings.h \
    include/GXDLMSSha1.h \
    include/GXDLMSSha256.h \
    include/GXDLMSSNCommandHandler.h \
    include/GXDLMSSNParameters.h \
    include/GXDLMSSpecialDay.h \
    include/GXDLMSSpecialDaysTable.h \
    include/GXDLMSTarget.h \
    include/GXDLMSTcpUdpSetup.h \
    include/GXDLMSTokenGateway.h \
    include/GXDLMSTranslator.h \
    include/GXDLMSTranslatorStructure.h \
    include/GXDLMSUtilityTables.h \
    include/GXDLMSValueEventArg.h \
    include/GXDLMSValueEventCollection.h \
    include/GXDLMSVariant.h \
    include/GXDLMSWeekProfile.h \
    include/GXHelpers.h \
    include/GXMacAvailableSwitch.h \
    include/GXMacDirectTable.h \
    include/GXMacMulticastEntry.h \
    include/GXMacPhyCommunication.h \
    include/GXReplyData.h \
    include/GXSecure.h \
    include/GXSerialNumberCounter.h \
    include/GXServerReply.h \
    include/GXSNInfo.h \
    include/GXStandardObisCode.h \
    include/GXStandardObisCodeCollection.h \
    include/GXTime.h \
    include/GXTokenGatewayConfiguration.h \
    include/GXUnitCharge.h \
    include/GXXmlReader.h \
    include/GXXmlWriter.h \
    include/GXXmlWriterSettings.h \
    include/IGXDLMSBase.h \
    include/OBiscodes.h \
    include/TranslatorGeneralTags.h \
    include/TranslatorSimpleTags.h \
    include/TranslatorStandardTags.h \
    include/TranslatorTags.h \
    include/GXDLMSCommon.h \
    programmer.h

FORMS += \
    programmer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
    RC_ICONS=Paint.ico
}
