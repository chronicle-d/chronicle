# Function to return a given device settings
import chronicle
from config.settings import DeviceSettings

def getDeviceSettings(deviceNicname: str) -> DeviceSettings:
    ds = chronicle.getConnectionInfo(deviceNicname)

    if not hasattr(chronicle, "config") or not chronicle.config.Sections():
        initChronicleConfig()

    deviceName = chronicle.config.Get(deviceNicname, "DeviceName", "")
    vendorName = chronicle.config.Get(deviceNicname, "Vendor", "")

    return DeviceSettings(
        kexMethods=ds.kex_methods,
        hostkeyAlgorithms=ds.hostkey_algorithms,
        vendor=ds.vendor,
        vendorName=vendorName,
        deviceName=deviceName,
        device=ds.device,
        user=ds.user,
        password=ds.password,
        host=ds.host,
        port=ds.port,
        verbosity=ds.verbosity,
    )
