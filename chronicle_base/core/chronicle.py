import chronicle
from config.settings import CONFIG_PATH, DeviceSettings, ChronicleSettings


# Returns a given device configuration
def initChronicleConfig() -> None:
    if not hasattr(chronicle, "config") or not chronicle.config.Sections():
        chronicle.readChronicleConfig(CONFIG_PATH)

# Returns a given device settings
def getDeviceSettings(deviceNickname: str) -> DeviceSettings:
    ds = chronicle.getConnectionInfo(deviceNickname)

    if not hasattr(chronicle, "config") or not chronicle.config.Sections():
        initChronicleConfig()

    deviceName = chronicle.config.Get(deviceNickname, "DeviceName", "")
    vendorName = chronicle.config.Get(deviceNickname, "Vendor", "")

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

def getSettings() -> ChronicleSettings:
    cs = chronicle.getChronicleSettings()
    
    return ChronicleSettings(
        sshIdleTimeout=cs.ssh_idle_timeout,
        sshTotalTimeout=cs.ssh_total_timeout,
    )
