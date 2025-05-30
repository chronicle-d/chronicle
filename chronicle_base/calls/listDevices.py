# Return a list of availbale device names
import chronicle
from core.chronicle import initChronicleConfig
from flask import Blueprint
from core.response import makeResponse

my_blueprint = Blueprint("listdevices", __name__)

def listDevices() -> list[str]:
    if not hasattr(chronicle, "config") or not chronicle.config.Sections():
        initChronicleConfig()
    sections = chronicle.config.Sections()
    filtered = [s for s in sections if s.lower() != "chroniclesettings"]
    return filtered

@my_blueprint.route("/listDevices", methods=["GET"])
def list_devices_route():
    devices = listDevices()
    return makeResponse(
        True,
        "Fetched list of devices succesfuly.",
        {
            "devices": devices,
            "total": len(devices)
        },
        200
    )
