# Defines global variables
from dataclasses import dataclass
CONFIG_PATH = "/etc/chronicle/chronicle.ini"
DEVICES_BASE = "/home/noam/Work_spaces/Personal/chronicle/chronicle_core/bin/devices/"

# Ddefault values
## Device
CHRONICLE_CONFIG_DEFUALT_KEX_METHODS = "curve25519-sha256@libssh.org,ecdh-sha2-nistp384,ecdh-sha2-nistp256,diffie-hellman-group14-sha256"
CHRONICLE_CONFIG_DEFUALT_HOSTKEYS = "ssh-ed25519,ecdsa-sha2-nistp521,ecdsa-sha2-nistp384,ecdsa-sha2-nistp256,rsa-sha2-512,rsa-sha2-256"
CHRONICLE_CONFIG_DEFUALT_PORT = 22
CHRONICLE_CONFIG_DEFUALT_USER = "chronicle-runner"
CHRONICLE_CONFIG_DEFUALT_VERBOSITY = 0

## Chronicle settings
CHRONICLE_CONFIG_DEFAULT_SSH_IDLE_TIMEOUT = 1000
CHRONICLE_CONFIG_DEFAULT_SSH_TOTAL_TIMEOUT = 10000

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
