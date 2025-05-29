import chronicle
from config.settings import DEVICES_BASE
import os

from  .getDeviceSettings import getDeviceSettings

# Returns a given device configuration
def getDeviceConfig(deviceNicname: str) -> list[str]:

    deviceSettings = getDeviceSettings(deviceNicname)

    deviceMapPath = os.path.join(DEVICES_BASE, deviceSettings.vendorName, deviceSettings.deviceName + ".cld")

    DeviceConnectionInfo = chronicle.getConnectionInfo(deviceNicname)
    DeviceOperationalMap = chronicle.loadDeviceOps(
        deviceMapPath, DeviceConnectionInfo.device, DeviceConnectionInfo.vendor
    )

    deviceConfig = chronicle.getConfig(
        DeviceConnectionInfo, DeviceOperationalMap.ops.getConfig
    )
    
    return deviceConfig
