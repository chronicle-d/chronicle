# Get a device configuration
from flask import Blueprint, request
from core.response import makeResponse
from utils.paramHandler import validateParams
from config.settings import API_ROUTE, DEVICES_BASE
from chronicle import ChronicleException, getErrorMsg, getConnectionInfo, loadDeviceOps, getConfig
import os

GLOBAL_SCHEMA = {
    "name": {
        "required": True,
        "required_error": "Missing 'name' (deviceNickname). Example: name=my_device"
    }
}

my_blueprint = Blueprint("getdeviceconfig", __name__)

@my_blueprint.route(API_ROUTE + "/getDeviceConfig", methods=["GET"])
def get_device_config_route():
    global_error, global_data = validateParams(request.args, GLOBAL_SCHEMA)
    if global_error:
        return global_error

    assert global_data is not None, "Expected validated params, got None"

    deviceNickname = global_data["name"]

    try:
        deviceSettings = getConnectionInfo(deviceNickname)
        deviceMapPath = os.path.join(
            DEVICES_BASE, deviceSettings.vendorName, deviceSettings.deviceName + ".cld"
        )

        DeviceOperationalMap = loadDeviceOps(
            deviceMapPath, deviceSettings.device, deviceSettings.vendor
        )

        deviceConfig = getConfig(
            deviceSettings, DeviceOperationalMap.ops.getConfig
        )

        return makeResponse(
            True,
            "Fetched device configuration succesfuly",
            {
                "config": deviceConfig
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
