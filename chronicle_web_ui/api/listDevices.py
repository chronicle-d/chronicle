# Return a list of availbale device names
import chronicle
from .initChronicleConfig import initChronicleConfig


def listDevices() -> list[str]:
    if not hasattr(chronicle, "config") or not chronicle.config.Sections():
        initChronicleConfig()
    sections = chronicle.config.Sections()
    filtered = [s for s in sections if s.lower() != "chroniclesettings"]
    return filtered
