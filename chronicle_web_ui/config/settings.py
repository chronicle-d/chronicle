# Defines global variables
from dataclasses import dataclass
CONFIG_PATH = "/etc/chronicle/chronicle.ini"
DEVICES_BASE = "/home/noam/Work_spaces/Personal/chronicle/chronicle_core/bin/devices/"

@dataclass
class ChronicleSettings:
    sshIdleTimeout:  int
    sshTotalTimeout: int

@dataclass
class DeviceSettings:
    kexMethods:             str
    hostkeyAlgorithms:      str
    vendor:                 int
    vendorName:             str
    device:                 int
    deviceName:             str
    user:                   str
    password:               str
    host:                   str
    port:                   int
    verbosity:              int
