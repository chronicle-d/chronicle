from flask import Blueprint, request
from config.settings import API_ROUTE, DEVICES_BASE
from core.chronicle import getDeviceSettings
from core.response import makeResponse
import chronicle
from chronicle import ChronicleException, getErrorMsg
import os

my_blueprint = Blueprint("getdeviceconfig", __name__)


# Returns a given device configuration
def getDeviceConfig(deviceNickname: str) -> list[str]:
    deviceSettings = getDeviceSettings(deviceNickname)
    deviceMapPath = os.path.join(
        DEVICES_BASE, deviceSettings.vendorName, deviceSettings.deviceName + ".cld"
    )
    DeviceConnectionInfo = chronicle.getConnectionInfo(deviceNickname)
    DeviceOperationalMap = chronicle.loadDeviceOps(
        deviceMapPath, DeviceConnectionInfo.device, DeviceConnectionInfo.vendor
    )
    deviceConfig = chronicle.getConfig(
        DeviceConnectionInfo, DeviceOperationalMap.ops.getConfig
    )

    return deviceConfig


@my_blueprint.route(API_ROUTE + "/getDeviceConfig", methods=["GET"])
def get_device_config_route():
    deviceNickname = request.args.get("name")
    if not deviceNickname :
        return makeResponse(
            False,
            "Missing 'name' query parameter, you must supply a device name. (name=my_device)",
            {},
            400
        )
    try:
        config = getDeviceConfig(deviceNickname )
        return makeResponse(
            True,
            "Fetched device configuration succesfuly",
            {
                "config": config
            },
            200,
            deviceNickname
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
            "Failed to fetch device configuration",
            {"error": error},
            500,
            deviceNickname
        )
