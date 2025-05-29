# Function to return the current chronicle settings
import chronicle
from config.settings import ChronicleSettings

def getSettings() -> ChronicleSettings:
    cs = chronicle.getChronicleSettings()
    
    return ChronicleSettings(
        sshIdleTimeout=cs.ssh_idle_timeout,
        sshTotalTimeout=cs.ssh_total_timeout,
    )
