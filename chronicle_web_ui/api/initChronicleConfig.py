import chronicle
from config.settings import CONFIG_PATH

# Returns a given device configuration
def initChronicleConfig() -> None:
    if not hasattr(chronicle, "config") or not chronicle.config.Sections():
        chronicle.readChronicleConfig(CONFIG_PATH)
