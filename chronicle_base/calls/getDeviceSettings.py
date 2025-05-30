# Function to return a given device settings
from flask import Blueprint, request
from chronicle import ChronicleException, getErrorMsg
from core.response import makeResponse
from core.chronicle import getDeviceSettings

my_blueprint = Blueprint("getdevicesettings", __name__)

@my_blueprint.route("/getDeviceSettings", methods=["GET"])
def get_device_settings_route():
    deviceNickname = request.args.get("name")
    if not deviceNickname :
        return makeResponse(
            False,
            "Missing 'name' query parameter, you must supply a device name. (name=my_device)",
            {},
            400
        )
    try:
        deviceSettings = getDeviceSettings(deviceNickname)
        return makeResponse(
            True,
            "Fetched device settings succesfuly",
            {
                "name": deviceNickname,
                "ssh": {
                    "kexMethods": deviceSettings.kexMethods,
                    "hostkeyAlgorithms": deviceSettings.hostkeyAlgorithms,
                    "user": deviceSettings.user,
                    "host": deviceSettings.host,
                    "port": deviceSettings.port,
                    "password": deviceSettings.password
                },
                "device": {
                    "vendorId": deviceSettings.vendor,
                    "vendorName": deviceSettings.vendorName,
                    "deviceId": deviceSettings.device,
                    "deviceName": deviceSettings.deviceName
                }
            },
            200
        )
    except (ChronicleException, Exception) as e:
        error = {}
        if isinstance(e, ChronicleException):
            error.update({"code": e.code})
            error.update({"codeMessage": getErrorMsg(e.code)})
            error.update({"details": e.details})
            error.update({"exception": str(e)})
        else:
            error.update({"details": "Non chronicle exception"})
            error.update({"exception": str(e)})

        return makeResponse(
            False,
            "Failed to fetch device settings",
            {"error": error},
            500
        )
