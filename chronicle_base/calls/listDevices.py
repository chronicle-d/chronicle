# List all devices
from flask import Blueprint, request
from core.response import makeResponse
from utils.paramHandler import validateParams
from config.settings import API_ROUTE
from chronicle import ChronicleDB, ChronicleException, getErrorMsg
import json

my_blueprint = Blueprint("listdevices", __name__)

@my_blueprint.route(API_ROUTE + "/listDevices", methods=["GET"])
def list_devices_route():
    try:
        raw_response = ChronicleDB().listDevices()
        response = []

        for r in raw_response:
            response.append(json.loads(r))

        return makeResponse(
            True,
            "Fetched devices list succefuly.",
            {
                "devices": response,
                "total": len(response)
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
            "Failed to fetch devices list.",
            {"error": error},
            500
        )
