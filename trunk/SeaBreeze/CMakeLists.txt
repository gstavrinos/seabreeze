#
#
# This CMakeLists file has not been set up for Windows or Linux yet. It only runs on OS X
#
#


cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 98)
#set(CMAKE_BUILD_TYPE DEBUG)

project(SeaBreeze)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/")
include_directories(${PROJECT_SOURCE_DIR}/include)

set(COMMON_SOURCE_FILES
        include/api/seabreezeapi/AcquisitionDelayFeatureAdapter.h
        include/api/seabreezeapi/ContinuousStrobeFeatureAdapter.h
        include/api/seabreezeapi/DataBufferFeatureAdapter.h
        include/api/seabreezeapi/DeviceAdapter.h
        include/api/seabreezeapi/DHCPServerFeatureAdapter.h
        include/api/seabreezeapi/EEPROMFeatureAdapter.h
        include/api/seabreezeapi/EthernetConfigurationFeatureAdapter.h
        include/api/seabreezeapi/FastBufferFeatureAdapter.h
        include/api/seabreezeapi/FeatureAdapterInterface.h
        include/api/seabreezeapi/FeatureAdapterTemplate.h
        include/api/seabreezeapi/FeatureFamilies.h
        include/api/seabreezeapi/gpioFeatureAdapter.h
        include/api/seabreezeapi/I2CMasterFeatureAdapter.h
        include/api/seabreezeapi/IntrospectionFeatureAdapter.h
        include/api/seabreezeapi/IPv4FeatureAdapter.h
        include/api/seabreezeapi/IrradCalFeatureAdapter.h
        include/api/seabreezeapi/LightSourceFeatureAdapter.h
        include/api/seabreezeapi/MulticastFeatureAdapter.h
        include/api/seabreezeapi/NetworkConfigurationFeatureAdapter.h
        include/api/seabreezeapi/NonlinearityCoeffsFeatureAdapter.h
        include/api/seabreezeapi/OpticalBenchFeatureAdapter.h
        include/api/seabreezeapi/PixelBinningFeatureAdapter.h
        include/api/seabreezeapi/ProtocolFamilies.h
        include/api/seabreezeapi/RawUSBBusAccessFeatureAdapter.h
        include/api/seabreezeapi/RevisionFeatureAdapter.h
        include/api/seabreezeapi/SeaBreezeAPI.h
        include/api/seabreezeapi/SeaBreezeAPI_Impl.h
        include/api/seabreezeapi/SeaBreezeAPIConstants.h
        include/api/seabreezeapi/SerialNumberFeatureAdapter.h
        include/api/seabreezeapi/ShutterFeatureAdapter.h
        include/api/seabreezeapi/SpectrometerFeatureAdapter.h
        include/api/seabreezeapi/SpectrumProcessingFeatureAdapter.h
        include/api/seabreezeapi/StrayLightCoeffsFeatureAdapter.h
        include/api/seabreezeapi/StrobeLampFeatureAdapter.h
        include/api/seabreezeapi/TemperatureFeatureAdapter.h
        include/api/seabreezeapi/ThermoElectricCoolerFeatureAdapter.h
        include/api/seabreezeapi/WifiConfigurationFeatureAdapter.h
        include/api/DeviceFactory.h
        include/api/DllDecl.h
        include/api/SeaBreezeWrapper.h
        include/api/USBEndpointTypes.h
        include/common/buses/network/IPv4NetworkProtocol.h
        include/common/buses/network/IPv4SocketDeviceLocator.h
        include/common/buses/network/TCPIPv4SocketBus.h
        include/common/buses/network/TCPIPv4SocketTransferHelper.h
        include/common/buses/rs232/RS232DeviceLocator.h
        include/common/buses/rs232/RS232Interface.h
        include/common/buses/rs232/RS232TransferHelper.h
        include/common/buses/usb/USBDeviceLocator.h
        include/common/buses/usb/USBInterface.h
        include/common/buses/usb/USBTransferHelper.h
        include/common/buses/Bus.h
        include/common/buses/BusFamilies.h
        include/common/buses/BusFamily.h
        include/common/buses/DeviceLocationProberInterface.h
        include/common/buses/DeviceLocatorInterface.h
        include/common/buses/TransferHelper.h
        include/common/devices/Device.h
        include/common/exceptions/BusConnectException.h
        include/common/exceptions/BusException.h
        include/common/exceptions/BusTransferException.h
        include/common/exceptions/FeatureControlException.h
        include/common/exceptions/FeatureException.h
        include/common/exceptions/FeatureProtocolNotFoundException.h
        include/common/exceptions/IllegalArgumentException.h
        include/common/exceptions/NumberFormatException.h
        include/common/exceptions/ProtocolBusMismatchException.h
        include/common/exceptions/ProtocolException.h
        include/common/exceptions/ProtocolFormatException.h
        include/common/exceptions/ProtocolTransactionException.h
        include/common/features/Feature.h
        include/common/features/FeatureFamily.h
        include/common/features/FeatureImpl.h
        include/common/protocols/Exchange.h
        include/common/protocols/Protocol.h
        include/common/protocols/ProtocolFamily.h
        include/common/protocols/ProtocolHelper.h
        include/common/protocols/ProtocolHint.h
        include/common/protocols/Transaction.h
        include/common/protocols/Transfer.h
        include/common/ByteVector.h
        include/common/Data.h
        include/common/DoubleVector.h
        include/common/FloatVector.h
        include/common/globals.h
        include/common/Log.h
        include/common/SeaBreeze.h
        include/common/U32Vector.h
        include/common/UnitDescriptor.h
        include/common/UShortVector.h
        include/native/network/posix/NativeSocketPOSIX.h
        include/native/network/Inet4Address.h
        include/native/network/Socket.h
        include/native/network/SocketException.h
        include/native/network/SocketTimeoutException.h
        include/native/network/UnknownHostException.h
        include/native/rs232/NativeRS232.h
        include/native/rs232/RS232.h
        include/native/system/NativeSystem.h
        include/native/system/System.h
        include/native/usb/NativeUSB.h
        include/native/usb/USB.h
        include/native/usb/USBDiscovery.h
        include/vendors/OceanOptics/buses/network/FlameXTCPIPv4.h
        include/vendors/OceanOptics/buses/network/JazTCPIPv4.h
        include/vendors/OceanOptics/buses/rs232/OOIRS232Interface.h
        include/vendors/OceanOptics/buses/usb/ApexUSB.h
        include/vendors/OceanOptics/buses/usb/FlameNIRUSB.h
        include/vendors/OceanOptics/buses/usb/FlameXUSB.h
        include/vendors/OceanOptics/buses/usb/FlameXUSBTransferHelper.h
        include/vendors/OceanOptics/buses/usb/HR2000PlusUSB.h
        include/vendors/OceanOptics/buses/usb/HR2000USB.h
        include/vendors/OceanOptics/buses/usb/HR4000USB.h
        include/vendors/OceanOptics/buses/usb/JazUSB.h
        include/vendors/OceanOptics/buses/usb/Maya2000ProUSB.h
        include/vendors/OceanOptics/buses/usb/Maya2000USB.h
        include/vendors/OceanOptics/buses/usb/MayaLSLUSB.h
        include/vendors/OceanOptics/buses/usb/NIRQuest256USB.h
        include/vendors/OceanOptics/buses/usb/NIRQuest512USB.h
        include/vendors/OceanOptics/buses/usb/OOIUSB4KSpectrumTransferHelper.h
        include/vendors/OceanOptics/buses/usb/OOIUSBControlTransferHelper.h
        include/vendors/OceanOptics/buses/usb/OOIUSBEndpointMaps.h
        include/vendors/OceanOptics/buses/usb/OOIUSBInterface.h
        include/vendors/OceanOptics/buses/usb/OOIUSBProductID.h
        include/vendors/OceanOptics/buses/usb/OOIUSBSpectrumTransferHelper.h
        include/vendors/OceanOptics/buses/usb/OOIUSBTrivialTransferHelper.h
        include/vendors/OceanOptics/buses/usb/QE65000USB.h
        include/vendors/OceanOptics/buses/usb/QEProUSB.h
        include/vendors/OceanOptics/buses/usb/SparkUSB.h
        include/vendors/OceanOptics/buses/usb/STSUSB.h
        include/vendors/OceanOptics/buses/usb/TorusUSB.h
        include/vendors/OceanOptics/buses/usb/USB2000PlusUSB.h
        include/vendors/OceanOptics/buses/usb/USB2000USB.h
        include/vendors/OceanOptics/buses/usb/USB4000USB.h
        include/vendors/OceanOptics/buses/usb/VentanaUSB.h
        include/vendors/OceanOptics/devices/Apex.h
        include/vendors/OceanOptics/devices/FlameNIR.h
        include/vendors/OceanOptics/devices/FlameX.h
        include/vendors/OceanOptics/devices/HR2000.h
        include/vendors/OceanOptics/devices/HR2000Plus.h
        include/vendors/OceanOptics/devices/HR4000.h
        include/vendors/OceanOptics/devices/Jaz.h
        include/vendors/OceanOptics/devices/Maya2000.h
        include/vendors/OceanOptics/devices/Maya2000Pro.h
        include/vendors/OceanOptics/devices/MayaLSL.h
        include/vendors/OceanOptics/devices/NIRQuest256.h
        include/vendors/OceanOptics/devices/NIRQuest512.h
        include/vendors/OceanOptics/devices/QE65000.h
        include/vendors/OceanOptics/devices/QEPro.h
        include/vendors/OceanOptics/devices/Spark.h
        include/vendors/OceanOptics/devices/STS.h
        include/vendors/OceanOptics/devices/Torus.h
        include/vendors/OceanOptics/devices/USB2000.h
        include/vendors/OceanOptics/devices/USB2000Plus.h
        include/vendors/OceanOptics/devices/USB4000.h
        include/vendors/OceanOptics/devices/Ventana.h
        include/vendors/OceanOptics/features/acquisition_delay/AcquisitionDelayFeature.h
        include/vendors/OceanOptics/features/acquisition_delay/AcquisitionDelayFeature_FPGA.h
        include/vendors/OceanOptics/features/acquisition_delay/AcquisitionDelayFeatureInterface.h
        include/vendors/OceanOptics/features/acquisition_delay/STSAcquisitionDelayFeature.h
        include/vendors/OceanOptics/features/continuous_strobe/ContinuousStrobeFeature.h
        include/vendors/OceanOptics/features/continuous_strobe/ContinuousStrobeFeature_FPGA.h
        include/vendors/OceanOptics/features/continuous_strobe/ContinuousStrobeFeatureInterface.h
        include/vendors/OceanOptics/features/data_buffer/DataBufferFeatureBase.h
        include/vendors/OceanOptics/features/data_buffer/DataBufferFeatureInterface.h
        include/vendors/OceanOptics/features/data_buffer/FlameXDataBufferFeature.h
        include/vendors/OceanOptics/features/data_buffer/QEProDataBufferFeature.h
        include/vendors/OceanOptics/features/dhcp_server/DHCPServerFeature.h
        include/vendors/OceanOptics/features/dhcp_server/DHCPServerFeatureInterface.h
        include/vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeature.h
        include/vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeatureBase.h
        include/vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeatureInterface.h
        include/vendors/OceanOptics/features/eeprom_slots/NonlinearityEEPROMSlotFeature.h
        include/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeature.h
        include/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeature_MayaPro.h
        include/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeature_NIRQuest.h
        include/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeatureBase.h
        include/vendors/OceanOptics/features/eeprom_slots/SerialNumberEEPROMSlotFeature.h
        include/vendors/OceanOptics/features/eeprom_slots/StrayLightEEPROMSlotFeature.h
        include/vendors/OceanOptics/features/eeprom_slots/WavelengthEEPROMSlotFeature.h
        include/vendors/OceanOptics/features/eeprom_slots/WavelengthEEPROMSlotFeature_QE65000.h
        include/vendors/OceanOptics/features/ethernet_configuration/EthernetConfigurationFeature.h
        include/vendors/OceanOptics/features/ethernet_configuration/EthernetConfigurationFeatureInterface.h
        include/vendors/OceanOptics/features/fast_buffer/FastBufferFeatureBase.h
        include/vendors/OceanOptics/features/fast_buffer/FastBufferFeatureInterface.h
        include/vendors/OceanOptics/features/fast_buffer/FlameXFastBufferFeature.h
        include/vendors/OceanOptics/features/fpga_register/FPGARegisterFeature.h
        include/vendors/OceanOptics/features/fpga_register/FPGARegisterFeatureInterface.h
        include/vendors/OceanOptics/features/gpio/gpioFeature.h
        include/vendors/OceanOptics/features/gpio/gpioFeatureInterface.h
        include/vendors/OceanOptics/features/i2c_master/i2cMasterFeature.h
        include/vendors/OceanOptics/features/i2c_master/i2cMasterFeatureInterface.h
        include/vendors/OceanOptics/features/introspection/IntrospectionFeature.h
        include/vendors/OceanOptics/features/introspection/IntrospectionFeatureInterface.h
        include/vendors/OceanOptics/features/ipv4/IPv4Feature.h
        include/vendors/OceanOptics/features/ipv4/IPv4FeatureInterface.h
        include/vendors/OceanOptics/features/irradcal/IrradCalFeature.h
        include/vendors/OceanOptics/features/irradcal/IrradCalFeatureInterface.h
        include/vendors/OceanOptics/features/light_source/LightSourceFeatureBase.h
        include/vendors/OceanOptics/features/light_source/LightSourceFeatureImpl.h
        include/vendors/OceanOptics/features/light_source/LightSourceFeatureInterface.h
        include/vendors/OceanOptics/features/light_source/StrobeLampFeature.h
        include/vendors/OceanOptics/features/light_source/StrobeLampFeatureInterface.h
        include/vendors/OceanOptics/features/light_source/VentanaLightSourceFeature.h
        include/vendors/OceanOptics/features/multicast/MulticastFeature.h
        include/vendors/OceanOptics/features/multicast/MulticastFeatureInterface.h
        include/vendors/OceanOptics/features/network_configuration/NetworkConfigurationFeature.h
        include/vendors/OceanOptics/features/network_configuration/NetworkConfigurationFeatureInterface.h
        include/vendors/OceanOptics/features/nonlinearity/NonlinearityCoeffsFeature.h
        include/vendors/OceanOptics/features/nonlinearity/NonlinearityCoeffsFeatureInterface.h
        include/vendors/OceanOptics/features/optical_bench/OpticalBenchFeature.h
        include/vendors/OceanOptics/features/optical_bench/OpticalBenchFeatureInterface.h
        include/vendors/OceanOptics/features/pixel_binning/PixelBinningFeatureInterface.h
        include/vendors/OceanOptics/features/pixel_binning/STSPixelBinningFeature.h
        include/vendors/OceanOptics/features/raw_bus_access/RawUSBBusAccessFeature.h
        include/vendors/OceanOptics/features/raw_bus_access/RawUSBBusAccessFeatureInterface.h
        include/vendors/OceanOptics/features/revision/RevisionFeature.h
        include/vendors/OceanOptics/features/revision/RevisionFeatureInterface.h
        include/vendors/OceanOptics/features/serial_number/SerialNumberFeature.h
        include/vendors/OceanOptics/features/serial_number/SerialNumberFeatureInterface.h
        include/vendors/OceanOptics/features/shutter/ShutterFeature.h
        include/vendors/OceanOptics/features/shutter/ShutterFeatureInterface.h
        include/vendors/OceanOptics/features/spectrometer/ApexSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/FlameNIRSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/FlameXSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/GainAdjustedSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/HR2000PlusSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/HR2000SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/HR4000SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/JazSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/Maya2000ProSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/Maya2000SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/MayaLSLSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/NIRQuest256SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/NIRQuest512SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/NIRQuestSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/OOISpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/OOISpectrometerFeatureInterface.h
        include/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeature.h
        include/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureBase.h
        include/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureImpl.h
        include/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureInterface.h
        include/vendors/OceanOptics/features/spectrometer/QE65000SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/QEProSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/SparkSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/SpectrometerTriggerMode.h
        include/vendors/OceanOptics/features/spectrometer/STSSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/USB2000PlusSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/USB2000SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/USB4000SpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrometer/VentanaSpectrometerFeature.h
        include/vendors/OceanOptics/features/spectrum_processing/SpectrumProcessingFeature.h
        include/vendors/OceanOptics/features/spectrum_processing/SpectrumProcessingFeatureInterface.h
        include/vendors/OceanOptics/features/stray_light/StrayLightCoeffsFeature.h
        include/vendors/OceanOptics/features/stray_light/StrayLightCoeffsFeatureInterface.h
        include/vendors/OceanOptics/features/temperature/TemperatureFeature.h
        include/vendors/OceanOptics/features/temperature/TemperatureFeatureInterface.h
        include/vendors/OceanOptics/features/thermoelectric/QEProThermoElectricFeature.h
        include/vendors/OceanOptics/features/thermoelectric/ThermoElectricFeatureBase.h
        include/vendors/OceanOptics/features/thermoelectric/ThermoElectricFeatureInterface.h
        include/vendors/OceanOptics/features/thermoelectric/ThermoElectricQEFeature.h
        include/vendors/OceanOptics/features/thermoelectric/VentanaThermoElectricFeature.h
        include/vendors/OceanOptics/features/wavecal/WaveCalFeature.h
        include/vendors/OceanOptics/features/wavecal/WaveCalFeatureInterface.h
        include/vendors/OceanOptics/features/wifi_configuration/WifiConfigurationFeature.h
        include/vendors/OceanOptics/features/wifi_configuration/WifiConfigurationFeatureInterface.h
        include/vendors/OceanOptics/protocols/interfaces/AcquisitionDelayProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/ContinuousStrobeProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/DataBufferProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/DHCPServerProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/EEPROMProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/EthernetConfigurationProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/FastBufferProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/FPGARegisterProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/GPIOProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/I2CMasterProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/IntrospectionProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/IPv4ProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/IrradCalProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/LightSourceProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/MulticastProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/NetworkConfigurationProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/NonlinearityCoeffsProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/OpticalBenchProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/PixelBinningProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/ProgrammableSaturationProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/RevisionProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/SerialNumberProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/ShutterProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/SpectrometerProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/SpectrumProcessingProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/StrayLightCoeffsProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/StrobeLampProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/TemperatureProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/ThermoElectricProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/WaveCalProtocolInterface.h
        include/vendors/OceanOptics/protocols/interfaces/WifiConfigurationProtocolInterface.h
        include/vendors/OceanOptics/protocols/obp/constants/OBPMessageTypes.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPAddIPv4AddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPCommand.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPContinuousStrobeEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPContinuousStrobePeriodExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPDataBufferClearExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPDataBufferRemoveOldestExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPDeleteIPv4AddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetActivePixelRangesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetAllTemperaturesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetBoxcarWidthExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetConsecutiveSampleCountExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDataBufferCapacityExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDataBufferElementCountExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDataBufferMaximumCapacityExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDefaultPixelBinningExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDHCPServerAddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDHCPServerEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetElectricDarkPixelRangesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetEthernetConfigurationGbEEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetEthernetConfigurationMACAddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetFastBufferingEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetFirmwareRevisionExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionAvailableModesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionCurrentModeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionNumberOfPinsExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionOutputVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionValueExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIONumberOfPinsExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOOutputEnableVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOValueVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetHardwareRevisionExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetI2CMasterNumberOfBusesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4AddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4DefaultGatewayExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4DHCPEnableStateExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4NumberOfAddressesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIrradCalExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIrradCollectionAreaExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetMaxPixelBinningExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetMulticastEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNetworkInterfaceConnectionTypeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNetworkInterfaceEnableStateExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNonlinearityCoeffExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNonlinearityCoeffsCountExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNumberOfNetworkInterfacesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNumberOfPixelsExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchCoatingExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchFiberDiameterMicronsExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchFilterExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchGratingExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchIDExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchSerialNumberExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchSlitWidthMicronsExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalDarkPixelRangesExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetPixelBinningExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetSaturationExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetScansToAverageExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetSerialNumberExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetSerialNumberMaximumLengthExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetStrayLightCoeffExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetStrayLightCoeffsCountExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetTemperatureCountExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetTemperatureExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetThermoElectricTemperatureExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWaveCalExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWifiConfigurationModeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWifiConfigurationSecurityExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWifiConfigurationSSIDExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPIntegrationTimeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPLampEnableCommand.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceEnableCommand.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceEnabledQuery.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceIntensityCommand.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceIntensityQuery.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPMessage.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPQuery.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadI2CMasterBusExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadNumberOfRawSpectraWithMetadataExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadRawSpectrum32AndMetadataExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadRawSpectrumExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrum32AndMetadataExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrumExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrumWithGainExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestBufferedSpectrum32AndMetadataExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestNumberOfBufferedSpectraWithMetadataExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestRawSpectrumExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestSpectrumExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPRunNetworkInterfaceSelfTestExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSaveNetworkInterfaceConnectionSettingsExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetAcquisitionDelayExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetBoxcarWidthExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetConsecutiveSampleCountExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDataBufferCapacityExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDefaultPixelBinningExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDHCPServerAddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDHCPServerEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetEthernetConfigurationGbEEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetEthernetConfigurationMACAddressExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetFastBufferingEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionModeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionOutputVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionValueExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOOutputEnableVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOValueVectorExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIPv4DefaultGatewayExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIPv4DHCPEnableStateExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIrradCalExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIrradCollectionAreaExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetMulticastEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetNetworkInterfaceEnableStateExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetPixelBinningExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetScansToAverageExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetThermoElectricEnableExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetThermoElectricSetpointExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationModeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationPassPhraseExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationSecurityExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationSSIDExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPShutterExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPTransaction.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPTriggerModeExchange.h
        include/vendors/OceanOptics/protocols/obp/exchanges/OBPWriteI2CMasterBusExchange.h
        include/vendors/OceanOptics/protocols/obp/hints/OBPControlHint.h
        include/vendors/OceanOptics/protocols/obp/hints/OBPSpectrumHint.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPAcquisitionDelayProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPContinuousStrobeProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPDataBufferProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPDHCPServerProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPEthernetConfigurationProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPFastBufferProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPGPIOProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPI2CMasterProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPIntrospectionProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPIPv4Protocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPIrradCalProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPLightSourceProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPLightSourceProtocol_Ventana.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPMulticastProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPNetworkConfigurationProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPNonlinearityCoeffsProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPOpticalBenchProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPPixelBinningProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPProgrammableSaturationProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPRevisionProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPSerialNumberProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPShutterProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPSpectrometerProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPSpectrumProcessingProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPStrayLightCoeffsProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPStrobeLampProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPTemperatureProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPThermoElectricProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPWaveCalProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OBPWifiConfigurationProtocol.h
        include/vendors/OceanOptics/protocols/obp/impls/OceanBinaryProtocol.h
        include/vendors/OceanOptics/protocols/ooi/constants/FPGARegisterCodes.h
        include/vendors/OceanOptics/protocols/ooi/constants/OpCodes.h
        include/vendors/OceanOptics/protocols/ooi/constants/QETECConstants.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/FlameNIRSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/FPGARegisterReadExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/FPGARegisterWriteExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/FPGASpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/HRFPGASpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/IntegrationTimeExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/JazSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/MayaProSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/NIRQuestSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/OOI2KSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/OOIIrradCalExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/OOIReadIrradCalExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/OOIWriteIrradCalExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/QESpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/ReadEEPROMSlotExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/ReadSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/ReadTECQETemperatureExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/ReadTECTemperatureExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/RequestSpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/StrobeEnableExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/TECEnableExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/TECQEEnableExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/TriggerModeExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/USBFPGASpectrumExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/WriteEEPROMSlotExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/WriteTECQESetPointExchange.h
        include/vendors/OceanOptics/protocols/ooi/exchanges/WriteTECSetPointExchange.h
        include/vendors/OceanOptics/protocols/ooi/hints/ControlHint.h
        include/vendors/OceanOptics/protocols/ooi/hints/SpectrumHint.h
        include/vendors/OceanOptics/protocols/ooi/impls/FPGARegisterProtocol.h
        include/vendors/OceanOptics/protocols/ooi/impls/OOIEEPROMProtocol.h
        include/vendors/OceanOptics/protocols/ooi/impls/OOIIrradCalProtocol.h
        include/vendors/OceanOptics/protocols/ooi/impls/OOIProtocol.h
        include/vendors/OceanOptics/protocols/ooi/impls/OOISpectrometerProtocol.h
        include/vendors/OceanOptics/protocols/ooi/impls/OOIStrobeLampProtocol.h
        include/vendors/OceanOptics/protocols/ooi/impls/OOITECProtocol.h
        include/vendors/OceanOptics/utils/Polynomial.h
        src/api/seabreezeapi/AcquisitionDelayFeatureAdapter.cpp
        src/api/seabreezeapi/ContinuousStrobeFeatureAdapter.cpp
        src/api/seabreezeapi/DataBufferFeatureAdapter.cpp
        src/api/seabreezeapi/DeviceAdapter.cpp
        src/api/seabreezeapi/DHCPServerFeatureAdapter.cpp
        src/api/seabreezeapi/EEPROMFeatureAdapter.cpp
        src/api/seabreezeapi/EthernetConfigurationFeatureAdapter.cpp
        src/api/seabreezeapi/FastBufferFeatureAdapter.cpp
        src/api/seabreezeapi/FeatureFamilies.cpp
        src/api/seabreezeapi/gpioFeatureAdapter.cpp
        src/api/seabreezeapi/I2CMasterFeatureAdapter.cpp
        src/api/seabreezeapi/IntrospectionFeatureAdapter.cpp
        src/api/seabreezeapi/IPv4FeatureAdapter.cpp
        src/api/seabreezeapi/IrradCalFeatureAdapter.cpp
        src/api/seabreezeapi/LightSourceFeatureAdapter.cpp
        src/api/seabreezeapi/MulticastFeatureAdapter.cpp
        src/api/seabreezeapi/NetworkConfigurationFeatureAdapter.cpp
        src/api/seabreezeapi/NonlinearityCoeffsFeatureAdapter.cpp
        src/api/seabreezeapi/OpticalBenchFeatureAdapter.cpp
        src/api/seabreezeapi/PixelBinningFeatureAdapter.cpp
        src/api/seabreezeapi/ProtocolFamilies.cpp
        src/api/seabreezeapi/RawUSBBusAccessFeatureAdapter.cpp
        src/api/seabreezeapi/RevisionFeatureAdapter.cpp
        src/api/seabreezeapi/SeaBreezeAPI.cpp
        src/api/seabreezeapi/SeaBreezeAPI_Impl.cpp
        src/api/seabreezeapi/SerialNumberFeatureAdapter.cpp
        src/api/seabreezeapi/ShutterFeatureAdapter.cpp
        src/api/seabreezeapi/SpectrometerFeatureAdapter.cpp
        src/api/seabreezeapi/SpectrumProcessingFeatureAdapter.cpp
        src/api/seabreezeapi/StrayLightCoeffsFeatureAdapter.cpp
        src/api/seabreezeapi/StrobeLampFeatureAdapter.cpp
        src/api/seabreezeapi/TemperatureFeatureAdapter.cpp
        src/api/seabreezeapi/ThermoElectricCoolerFeatureAdapter.cpp
        src/api/seabreezeapi/WifiConfigurationFeatureAdapter.cpp
        src/api/DeviceFactory.cpp
        src/api/SeaBreezeWrapper.cpp
        src/common/buses/network/IPv4NetworkProtocol.cpp
        src/common/buses/network/IPv4SocketDeviceLocator.cpp
        src/common/buses/network/TCPIPv4SocketBus.cpp
        src/common/buses/network/TCPIPv4SocketTransferHelper.cpp
        src/common/buses/rs232/RS232DeviceLocator.cpp
        src/common/buses/rs232/RS232Interface.cpp
        src/common/buses/rs232/RS232TransferHelper.cpp
        src/common/buses/usb/USBDeviceLocator.cpp
        src/common/buses/usb/USBInterface.cpp
        src/common/buses/usb/USBTransferHelper.cpp
        src/common/buses/Bus.cpp
        src/common/buses/BusFamilies.cpp
        src/common/buses/BusFamily.cpp
        src/common/buses/DeviceLocationProberInterface.cpp
        src/common/buses/TransferHelper.cpp
        src/common/devices/Device.cpp
        src/common/exceptions/BusConnectException.cpp
        src/common/exceptions/BusException.cpp
        src/common/exceptions/BusTransferException.cpp
        src/common/exceptions/FeatureControlException.cpp
        src/common/exceptions/FeatureException.cpp
        src/common/exceptions/FeatureProtocolNotFoundException.cpp
        src/common/exceptions/IllegalArgumentException.cpp
        src/common/exceptions/NumberFormatException.cpp
        src/common/exceptions/ProtocolBusMismatchException.cpp
        src/common/exceptions/ProtocolException.cpp
        src/common/exceptions/ProtocolFormatException.cpp
        src/common/exceptions/ProtocolTransactionException.cpp
        src/common/features/FeatureFamily.cpp
        src/common/features/FeatureImpl.cpp
        src/common/protocols/Exchange.cpp
        src/common/protocols/Protocol.cpp
        src/common/protocols/ProtocolFamily.cpp
        src/common/protocols/ProtocolHelper.cpp
        src/common/protocols/ProtocolHint.cpp
        src/common/protocols/Transaction.cpp
        src/common/protocols/Transfer.cpp
        src/common/ByteVector.cpp
        src/common/Data.cpp
        src/common/DoubleVector.cpp
        src/common/FloatVector.cpp
        src/common/Log.cpp
        src/common/U32Vector.cpp
        src/common/UnitDescriptor.cpp
        src/common/UShortVector.cpp
        src/native/network/posix/NativeSocketPOSIX.cpp
        src/native/network/Inet4Address.cpp
        src/native/network/SocketException.cpp
        src/native/network/SocketTimeoutException.cpp
        src/native/network/UnknownHostException.cpp
        src/native/rs232/posix/NativeRS232POSIX.c
        src/native/rs232/RS232.cpp
        src/native/system/posix/NativeSystemPOSIX.c
        src/native/system/System.cpp
        src/native/usb/USB.cpp
        src/native/usb/USBDiscovery.cpp
        src/vendors/OceanOptics/buses/network/FlameXTCPIPv4.cpp
        src/vendors/OceanOptics/buses/network/JazTCPIPv4.cpp
        src/vendors/OceanOptics/buses/rs232/OOIRS232Interface.cpp
        src/vendors/OceanOptics/buses/usb/ApexUSB.cpp
        src/vendors/OceanOptics/buses/usb/FlameNIRUSB.cpp
        src/vendors/OceanOptics/buses/usb/FlameXUSB.cpp
        src/vendors/OceanOptics/buses/usb/FlameXUSBTransferHelper.cpp
        src/vendors/OceanOptics/buses/usb/HR2000PlusUSB.cpp
        src/vendors/OceanOptics/buses/usb/HR2000USB.cpp
        src/vendors/OceanOptics/buses/usb/HR4000USB.cpp
        src/vendors/OceanOptics/buses/usb/JazUSB.cpp
        src/vendors/OceanOptics/buses/usb/Maya2000ProUSB.cpp
        src/vendors/OceanOptics/buses/usb/Maya2000USB.cpp
        src/vendors/OceanOptics/buses/usb/MayaLSLUSB.cpp
        src/vendors/OceanOptics/buses/usb/NIRQuest256USB.cpp
        src/vendors/OceanOptics/buses/usb/NIRQuest512USB.cpp
        src/vendors/OceanOptics/buses/usb/OOIUSB4KSpectrumTransferHelper.cpp
        src/vendors/OceanOptics/buses/usb/OOIUSBControlTransferHelper.cpp
        src/vendors/OceanOptics/buses/usb/OOIUSBEndpointMaps.cpp
        src/vendors/OceanOptics/buses/usb/OOIUSBInterface.cpp
        src/vendors/OceanOptics/buses/usb/OOIUSBSpectrumTransferHelper.cpp
        src/vendors/OceanOptics/buses/usb/OOIUSBTrivialTransferHelper.cpp
        src/vendors/OceanOptics/buses/usb/QE65000USB.cpp
        src/vendors/OceanOptics/buses/usb/QEProUSB.cpp
        src/vendors/OceanOptics/buses/usb/SparkUSB.cpp
        src/vendors/OceanOptics/buses/usb/STSUSB.cpp
        src/vendors/OceanOptics/buses/usb/TorusUSB.cpp
        src/vendors/OceanOptics/buses/usb/USB2000PlusUSB.cpp
        src/vendors/OceanOptics/buses/usb/USB2000USB.cpp
        src/vendors/OceanOptics/buses/usb/USB4000USB.cpp
        src/vendors/OceanOptics/buses/usb/VentanaUSB.cpp
        src/vendors/OceanOptics/devices/Apex.cpp
        src/vendors/OceanOptics/devices/FlameNIR.cpp
        src/vendors/OceanOptics/devices/FlameX.cpp
        src/vendors/OceanOptics/devices/HR2000.cpp
        src/vendors/OceanOptics/devices/HR2000Plus.cpp
        src/vendors/OceanOptics/devices/HR4000.cpp
        src/vendors/OceanOptics/devices/Jaz.cpp
        src/vendors/OceanOptics/devices/Maya2000.cpp
        src/vendors/OceanOptics/devices/Maya2000Pro.cpp
        src/vendors/OceanOptics/devices/MayaLSL.cpp
        src/vendors/OceanOptics/devices/NIRQuest256.cpp
        src/vendors/OceanOptics/devices/NIRQuest512.cpp
        src/vendors/OceanOptics/devices/QE65000.cpp
        src/vendors/OceanOptics/devices/QEPro.cpp
        src/vendors/OceanOptics/devices/Spark.cpp
        src/vendors/OceanOptics/devices/STS.cpp
        src/vendors/OceanOptics/devices/Torus.cpp
        src/vendors/OceanOptics/devices/USB2000.cpp
        src/vendors/OceanOptics/devices/USB2000Plus.cpp
        src/vendors/OceanOptics/devices/USB4000.cpp
        src/vendors/OceanOptics/devices/Ventana.cpp
        src/vendors/OceanOptics/features/acquisition_delay/AcquisitionDelayFeature.cpp
        src/vendors/OceanOptics/features/acquisition_delay/AcquisitionDelayFeature_FPGA.cpp
        src/vendors/OceanOptics/features/acquisition_delay/STSAcquisitionDelayFeature.cpp
        src/vendors/OceanOptics/features/continuous_strobe/ContinuousStrobeFeature.cpp
        src/vendors/OceanOptics/features/continuous_strobe/ContinuousStrobeFeature_FPGA.cpp
        src/vendors/OceanOptics/features/data_buffer/DataBufferFeatureBase.cpp
        src/vendors/OceanOptics/features/data_buffer/FlameXDataBufferFeature.cpp
        src/vendors/OceanOptics/features/data_buffer/QEProDataBufferFeature.cpp
        src/vendors/OceanOptics/features/dhcp_server/DHCPServerFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeatureBase.cpp
        src/vendors/OceanOptics/features/eeprom_slots/NonlinearityEEPROMSlotFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeature_MayaPro.cpp
        src/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeature_NIRQuest.cpp
        src/vendors/OceanOptics/features/eeprom_slots/SaturationEEPROMSlotFeatureBase.cpp
        src/vendors/OceanOptics/features/eeprom_slots/SerialNumberEEPROMSlotFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/StrayLightEEPROMSlotFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/WavelengthEEPROMSlotFeature.cpp
        src/vendors/OceanOptics/features/eeprom_slots/WavelengthEEPROMSlotFeature_QE65000.cpp
        src/vendors/OceanOptics/features/ethernet_configuration/EthernetConfigurationFeature.cpp
        src/vendors/OceanOptics/features/fast_buffer/FastBufferFeatureBase.cpp
        src/vendors/OceanOptics/features/fast_buffer/FlameXFastBufferFeature.cpp
        src/vendors/OceanOptics/features/fpga_register/FPGARegisterFeature.cpp
        src/vendors/OceanOptics/features/gpio/gpioFeature.cpp
        src/vendors/OceanOptics/features/i2c_master/i2cFeature.cpp
        src/vendors/OceanOptics/features/introspection/IntrospectionFeature.cpp
        src/vendors/OceanOptics/features/ipv4/IPv4Feature.cpp
        src/vendors/OceanOptics/features/irradcal/IrradCalFeature.cpp
        src/vendors/OceanOptics/features/light_source/LightSourceFeatureBase.cpp
        src/vendors/OceanOptics/features/light_source/LightSourceFeatureImpl.cpp
        src/vendors/OceanOptics/features/light_source/StrobeLampFeature.cpp
        src/vendors/OceanOptics/features/light_source/VentanaLightSourceFeature.cpp
        src/vendors/OceanOptics/features/multicast/MulticastFeature.cpp
        src/vendors/OceanOptics/features/network_configuration/NetworkConfigurationFeature.cpp
        src/vendors/OceanOptics/features/nonlinearity/NonlinearityCoeffsFeature.cpp
        src/vendors/OceanOptics/features/optical_bench/OpticalBenchFeature.cpp
        src/vendors/OceanOptics/features/pixel_binning/PixelBinningFeatureInterface.cpp
        src/vendors/OceanOptics/features/pixel_binning/STSPixelBinningFeature.cpp
        src/vendors/OceanOptics/features/raw_bus_access/RawUSBBusAccessFeature.cpp
        src/vendors/OceanOptics/features/revision/RevisionFeature.cpp
        src/vendors/OceanOptics/features/serial_number/SerialNumberFeature.cpp
        src/vendors/OceanOptics/features/shutter/ShutterFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/ApexSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/FlameNIRSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/FlameXSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/GainAdjustedSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/HR2000PlusSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/HR2000SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/HR4000SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/JazSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/Maya2000ProSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/Maya2000SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/MayaLSLSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/NIRQuest256SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/NIRQuest512SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/NIRQuestSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/OOISpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureBase.cpp
        src/vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureImpl.cpp
        src/vendors/OceanOptics/features/spectrometer/QE65000SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/QEProSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/SparkSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/SpectrometerTriggerMode.cpp
        src/vendors/OceanOptics/features/spectrometer/STSSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/USB2000PlusSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/USB2000SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/USB4000SpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrometer/VentanaSpectrometerFeature.cpp
        src/vendors/OceanOptics/features/spectrum_processing/SpectrumProcessingFeature.cpp
        src/vendors/OceanOptics/features/stray_light/StrayLightCoeffsFeature.cpp
        src/vendors/OceanOptics/features/temperature/TemperatureFeature.cpp
        src/vendors/OceanOptics/features/thermoelectric/QEProThermoElectricFeature.cpp
        src/vendors/OceanOptics/features/thermoelectric/ThermoElectricFeatureBase.cpp
        src/vendors/OceanOptics/features/thermoelectric/ThermoElectricQEFeature.cpp
        src/vendors/OceanOptics/features/thermoelectric/VentanaThermoElectricFeature.cpp
        src/vendors/OceanOptics/features/wavecal/WaveCalFeature.cpp
        src/vendors/OceanOptics/features/wifi_configuration/WifiConfigurationFeature.cpp
        src/vendors/OceanOptics/protocols/interfaces/AcquisitionDelayProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/ContinuousStrobeProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/DataBufferProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/DHCPServerProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/EEPROMProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/EthernetConfigurationProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/FastBufferProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/FPGARegisterProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/GPIOProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/I2CMasterProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/IntrospectionProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/IPv4ProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/IrradCalProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/LightSourceProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/MulticastProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/NetworkConfigurationProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/NonlinearityCoeffsProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/OpticalBenchProtocolInteface.cpp
        src/vendors/OceanOptics/protocols/interfaces/PixelBinningProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/ProgrammableSaturationProtocolInteface.cpp
        src/vendors/OceanOptics/protocols/interfaces/RevisionProtocolInteface.cpp
        src/vendors/OceanOptics/protocols/interfaces/SerialNumberProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/ShutterProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/SpectrometerProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/SpectrumProcessingProtocolInteface.cpp
        src/vendors/OceanOptics/protocols/interfaces/StrayLightCoeffsProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/StrobeLampProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/TemperatureProtocolInteface.cpp
        src/vendors/OceanOptics/protocols/interfaces/ThermoElectricProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/WaveCalProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/interfaces/WifiConfigurationProtocolInterface.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPAddIPv4AddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPCommand.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPContinuousStrobeEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPContinuousStrobePeriodExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPDataBufferClearExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPDataBufferRemoveOldestExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPDeleteIPv4AddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetActivePixelRangesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetAllTemperaturesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetBoxcarWidthExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetConsecutiveSampleCountExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDataBufferCapacityExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDataBufferCountExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDataBufferMaximumCapacityExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDefaultPixelBinningExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDHCPServerAddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetDHCPServerEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetElectricDarkPixelRangesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetEthernetConfigurationGbEEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetEthernetConfigurationMACAddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetFastBufferingEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetFirmwareRevisionExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionAvailableModesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionCurrentModeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionNumberOfPinsExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionOutputVectorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOExtensionValueExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIONumberOfPinsExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOOutputEnableVectorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetGPIOValueVectorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetHardwareRevisionExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetI2CMasterNumberOfBusesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4AddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4DefaultGatewayExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4DHCPEnableStateExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIPv4NumberOfAddressesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIrradCalExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetIrradCollectionAreaExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetMaxPixelBinningExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetMulticastEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNetworkInterfaceConnectionTypeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNetworkInterfaceEnableStateExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNonlinearityCoeffExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNonlinearityCoeffsCountExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNumberOfNetworkInterfacesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetNumberOfPixelsExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchCoatingExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchFiberDiameterMicronsExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchFilterExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchGratingExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchIDExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchSerialNumberExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalBenchSlitWidthMicronsExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetOpticalDarkPixelRangesExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetPixelBinningExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetSaturationExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetScansToAverageExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetSerialNumberExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetSerialNumberMaximumLengthExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetStrayLightCoeffExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetStrayLightCoeffsCountExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetTemperatureCountExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetTemperatureExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetThermoElectricTemperatureExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWaveCalExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWifiConfigurationModeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWifiConfigurationSecurityExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPGetWifiConfigurationSSIDExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPIntegrationTimeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPLampEnableCommand.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceEnableCommand.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceEnabledQuery.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceIntensityCommand.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceIntensityQuery.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPMessage.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPQuery.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadI2CMasterBusExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadNumberOfRawSpectraWithMetadataExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadRawSpectrum32AndMetadataExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadRawSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrum32AndMetadataExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrumWithGainExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestBufferedSpectrum32AndMetadata.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestNumberOfBufferedSpectraWithMetadata.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestRawSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPRequestSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPRunNetworkInterfaceSelfTestExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSaveNetworkInterfaceConnectionSettingsExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetAcquisitionDelayExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetBoxcarWidthExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetConsecutiveSampleCountExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDataBufferCapacityExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDefaultPixelBinningFactorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDHCPServerAddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetDHCPServerEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetEthernetConfigurationGbEEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetEthernetConfigurationMACAddressExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetFastBufferingEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionModeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionOutputVectorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOExtensionValueExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOOutputEnableVectorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetGPIOValueVectorExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIPv4DefaultGatewayExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIPv4DHCPEnableStateExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIrradCalExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetIrradCollectionAreaExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetMulticastEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetNetworkInterfaceEnableStateExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetPixelBinningExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetScansToAverageExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetThermoElectricEnableExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetThermoElectricSetpointExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationModeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationPassPhraseExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationSecurityExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPSetWifiConfigurationSSIDExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPShutterExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPTransaction.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPTriggerModeExchange.cpp
        src/vendors/OceanOptics/protocols/obp/exchanges/OBPWriteI2CMasterBusExchange.cpp
        src/vendors/OceanOptics/protocols/obp/hints/OBPControlHint.cpp
        src/vendors/OceanOptics/protocols/obp/hints/OBPSpectrumHint.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPAcquisitionDelayProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPContinuousStrobeProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPDataBufferProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPDHCPServerProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPEthernetConfigurationProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPFastBufferProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPGPIOProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPI2CMasterProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPIntrospectionProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPIPv4Protocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPIrradCalProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPLightSourceProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPLightSourceProtocol_Ventana.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPMulticastProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPNetworkConfigurationProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPNonlinearityCoeffsProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPOpticalBenchProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPPixelBinningProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPProgrammableSaturationProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPRevisionProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPSerialNumberProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPShutterProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPSpectrometerProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPSpectrumProcessingProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPStrayLightCoeffsProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPStrobeLampProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPTemperatureProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPThermoElectricProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPWaveCalProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OBPWifiConfigurationProtocol.cpp
        src/vendors/OceanOptics/protocols/obp/impls/OceanBinaryProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/constants/FPGARegisterCodes.cpp
        src/vendors/OceanOptics/protocols/ooi/constants/OpCodes.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/FlameNIRSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/FPGARegisterReadExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/FPGARegisterWriteExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/FPGASpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/HRFPGASpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/IntegrationTimeExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/JazSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/MayaProSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/NIRQuestSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/OOI2KSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/OOIIrradCalExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/OOIReadIrradCalExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/OOIWriteIrradCalExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/QESpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/ReadEEPROMSlotExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/ReadSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/ReadTECQETemperatureExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/ReadTECTemperatureExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/RequestSpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/StrobeEnableExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/TECEnableExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/TECQEEnableExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/TriggerModeExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/USBFPGASpectrumExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/WriteEEPROMSlotExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/WriteTECQESetPointExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/exchanges/WriteTECSetPointExchange.cpp
        src/vendors/OceanOptics/protocols/ooi/hints/ControlHint.cpp
        src/vendors/OceanOptics/protocols/ooi/hints/SpectrumHint.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/FPGARegisterProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/OOIEEPROMProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/OOIIrradCalProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/OOIProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/OOISpectrometerProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/OOIStrobeLampProtocol.cpp
        src/vendors/OceanOptics/protocols/ooi/impls/OOITECProtocol.cpp
        )

if(WIN32)
    #add_compile_options(-std=c++0x)
    message("Compiling for Windows")
    set(PLATFORM_SOURCE_FILES
        test/VisualStudio2015/windows_api_test/stdafx.cpp
        test/VisualStudio2015/windows_api_test/stdafx.h
        test/VisualStudio2015/windows_api_test/targetver.h
        test/VisualStudio2015/windows_api_test/windows_api_test.cpp
        src/native/rs232/windows/NativeRS232Windows.c
        src/native/system/windows/NativeSystemWindows.c
        include/native/rs232/windows/NativeRS232Windows.h
        include/native/network/windows/NativeSocketWindows.h
        src/native/network/windows/NativeSocketWindows.cpp
        src/native/usb/winusb/NativeUSBWinUSB.c
        sample-code/CSharpDemo/Properties/AssemblyInfo.cs
        sample-code/CSharpDemo/Properties/Resources.Designer.cs
        sample-code/CSharpDemo/Properties/Resources.resx
        sample-code/CSharpDemo/Properties/Settings.Designer.cs
        sample-code/CSharpDemo/Properties/Settings.settings
        sample-code/CSharpDemo/Setup/Setup.vdproj
        sample-code/CSharpDemo/app.config
        sample-code/CSharpDemo/CSharpDemo.csproj
        sample-code/CSharpDemo/CSharpDemo.sln
        sample-code/CSharpDemo/Form1.cs
        sample-code/CSharpDemo/Form1.Designer.cs
        sample-code/CSharpDemo/Form1.resx
        sample-code/CSharpDemo/Logger.cs
        sample-code/CSharpDemo/OmniDriverSpectrometer.cs
        sample-code/CSharpDemo/Program.cs
        sample-code/CSharpDemo/README.TXT
        sample-code/CSharpDemo/SeaBreezeAPI.cs
        sample-code/CSharpDemo/SeaBreezeSpectrometer.cs
        sample-code/CSharpDemo/SeaBreezeWrapper.cs
        sample-code/CSharpDemo/Spectrometer.cs
        sample-code/VisualCppConsoleDemo/src/stdafx.cpp
        sample-code/VisualCppConsoleDemo/src/stdafx.h
        sample-code/VisualCppConsoleDemo/src/targetver.h
        sample-code/VisualCppConsoleDemo/src/VisualCppConsoleDemo.cpp
        os-support/windows/VisualStudio2010/VSProj/resource.h
        os-support/windows/VisualStudio2010/VSProj/stdafx.cpp
        os-support/windows/VisualStudio2010/VSProj/stdafx.h
        os-support/windows/VisualStudio2012/VSProj/resource.h
        os-support/windows/VisualStudio2012/VSProj/stdafx.cpp
        os-support/windows/VisualStudio2012/VSProj/stdafx.h
        os-support/windows/VisualStudio2013/VSProj/resource.h
        os-support/windows/VisualStudio2013/VSProj/stdafx.cpp
        os-support/windows/VisualStudio2013/VSProj/stdafx.h
        os-support/windows/VisualStudio2015/VSProj/resource.h
        os-support/windows/VisualStudio2015/VSProj/stdafx.cpp
        os-support/windows/VisualStudio2015/VSProj/stdafx.h
        os-support/windows/WinDDK_Includes/POPPACK.H
        os-support/windows/WinDDK_Includes/PSHPACK1.H
        os-support/windows/WinDDK_Includes/usb.h
        os-support/windows/WinDDK_Includes/usb100.h
        os-support/windows/WinDDK_Includes/usb200.h
        os-support/windows/WinDDK_Includes/winusb.h
        os-support/windows/WinDDK_Includes/winusbio.h
        include/native/usb/winusb/WindowsGUID.h
        )

    message("Building Windows")

    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -c -ggdb3 -Wall -Wunused -Werror -shared" )


    if(CYGWIN)
        add_executable(api_test_win WIN32 "test/seabreeze_test_posix.c")
    else()
        add_executable(api_test_win WIN32 "test/test/seabreeze_test_windows.c")
    endif()
    find_library(SEABREEZE_LIB libSeaBreeze.*)
    target_link_libraries(api_test_win ${SEABREEZE_LIB})

    set_target_properties("api_test_win" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/")

elseif(APPLE)

    message("Building SeaBreeze for OS X, Darwin")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ggdb -Wall -Wunused -Wmissing-include-dirs -Werror -O0 -fpic -fno-stack-protector" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ggdb ")

    set(PLATFORM_SOURCE_FILES
            src/native/usb/osx/NativeUSBMacOSX.c
            )

    add_library(SeaBreeze SHARED ${COMMON_SOURCE_FILES} ${PLATFORM_SOURCE_FILES})
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    target_link_libraries(SeaBreeze ${COREFOUNDATION_FRAMEWORK} ${IOKIT_FRAMEWORK})


    # build test applicaitons against the seabreeze api
    message("Building api test")


    add_executable(api_test_posix "test/seabreeze_test_posix.c" "test/spectral_correction.c")
    target_link_libraries(api_test_posix SeaBreeze)

    set_target_properties("api_test_posix" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/")



elseif(UNIX)
# need to copy os-support/linux/FilesToInstall/10-oceanoptics.rules to the rules folder

    message("Building for Unix-like platforms")
    #add_compile_options(-std=c++0x)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ggdb -Wall -Wunused -Wmissing-include-dirs -Werror -O0 -fpic -fno-stack-protector" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -gdb -pthread -lpthread")

    set(PLATFORM_SOURCE_FILES
        src/native/usb/linux/NativeUSBLinux.c
        )


    add_library(SeaBreeze SHARED ${COMMON_SOURCE_FILES} ${PLATFORM_SOURCE_FILES})
    target_link_libraries(SeaBreeze usb)

    # build test applicaitons against the seabreeze api
    message("Building api test")

    add_executable(api_test "test/api_test.c")
    target_link_libraries(api_test SeaBreeze)

    set_target_properties("api_test" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/")
endif(WIN32)

message("Building sample code for all platforms.")

add_executable(demo-averaging "sample-code/c/demo-averaging.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-averaging SeaBreeze)

add_executable(demo-named-spectrometer "sample-code/c/demo-named-spectrometer.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-named-spectrometer SeaBreeze)

add_executable(demo-pthreads "sample-code/c/demo-pthreads" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-pthreads SeaBreeze)

add_executable(demo-qepro-api "sample-code/c/demo-qepro-api.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-qepro-api SeaBreeze)

add_executable(demo-raw-usb-seabreezeapi "sample-code/c/demo-raw-usb-seabreezeapi.c"  "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-raw-usb-seabreezeapi SeaBreeze)

add_executable(demo-raw-usb "sample-code/c/demo-raw-usb.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-raw-usb SeaBreeze)

add_executable(demo-stability-scan "sample-code/c/demo-stability-scan.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-stability-scan SeaBreeze)

add_executable(demo-streaming-xml "sample-code/c/demo-streaming-xml.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-streaming-xml SeaBreeze)

add_executable(demo-strobe-sts "sample-code/c/demo-strobe-sts.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-strobe-sts SeaBreeze)

add_executable(demo-strobe "sample-code/c/demo-strobe.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-strobe SeaBreeze)

add_executable(demo-sts-api "sample-code/c/demo-sts-api.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-sts-api SeaBreeze)

add_executable(demo-sts-eeprom "sample-code/c/demo-sts-eeprom.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-sts-eeprom SeaBreeze)

add_executable(demo-timeout "sample-code/c/demo-timeout.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-timeout SeaBreeze)

add_executable(demo-timing "sample-code/c/demo-timing.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-timing SeaBreeze)

add_executable(demo-unformatted-spectrum "sample-code/c/demo-unformatted-spectrum.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(demo-unformatted-spectrum SeaBreeze)

add_executable(seabreeze-util "sample-code/c/seabreeze-util.c" "sample-code/c/util.h" "sample-code/c/util.c")
target_link_libraries(seabreeze-util SeaBreeze)

set(SAMPLE_CODE_C_TARGETS
        "demo-averaging"
        "demo-named-spectrometer"
        "demo-pthreads"
        "demo-qepro-api"
        "demo-raw-usb-seabreezeapi"
        "demo-raw-usb"
        "demo-stability-scan"
        "demo-streaming-xml"
        "demo-strobe-sts"
        "demo-strobe"
        "demo-sts-api"
        "demo-sts-eeprom"
        "demo-timeout"
        "demo-timing"
        "demo-unformatted-spectrum"
        "seabreeze-util"
        )

set_target_properties(${SAMPLE_CODE_C_TARGETS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/sample-code/c/")


set(SAMPLE_CODE_CPP_TARGETS data-collection)
add_executable(data-collection "sample-code/cpp/data-collection.cpp" "sample-code/cpp/util.h" "sample-code/cpp/util.cpp")
target_link_libraries(data-collection SeaBreeze)

set_target_properties(${SAMPLE_CODE_CPP_TARGETS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/sample-code/cpp/")



message("Building Ocean FX speed test for all platforms.")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/oceanfx_speed_test/")
add_executable(oceanfx_speed_test "oceanfx_speed_test/oceanfx_speed_test.c")
target_link_libraries(oceanfx_speed_test SeaBreeze)

set_target_properties("oceanfx_speed_test" PROPERTIES EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/oceanfx_speed_test/")

