# Get a device
from flask import Blueprint, request
from core.response import makeResponse
from utils.paramHandler import validateParams
from config.settings import API_ROUTE
from chronicle import ChronicleDB, ChronicleException, getErrorMsg
import json

GLOBAL_SCHEMA = {
    "name": {
        "required": True,
        "required_error": "Missing 'name' (deviceNickname). Example: name=my_device"
    }
}

my_blueprint = Blueprint("getdevice", __name__)

@my_blueprint.route(API_ROUTE + "/getDevice", methods=["GET"])
def get_device_route():
    global_error, global_data = validateParams(request.args, GLOBAL_SCHEMA)
    if global_error:
        return global_error

    assert global_data is not None, "Expected validated params, got None"

    deviceNickname = global_data["name"]

    try:
        raw_response = ChronicleDB().getDevice(deviceNickname=deviceNickname)
        response = json.loads(raw_response)

        return makeResponse(
            True,
            f"Fetched device {deviceNickname} succefuly.",
            response,
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
            f"Failed to fetch device {deviceNickname}.",
            {"error": error},
            500,
            deviceNickname
        )
