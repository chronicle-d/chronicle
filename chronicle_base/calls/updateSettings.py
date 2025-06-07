# Function to update the current chronicle settings
from flask import Blueprint, request
from core.response import makeResponse
from utils.paramHandler import validateParams
from chronicle import ChronicleException, ChronicleDB, getErrorMsg
from config.settings import API_ROUTE


my_blueprint = Blueprint("updatesettings", __name__)

GLOBAL_SCHEMA = {
    "sshIdleTimeout": {
        "required": False,
        "type": int,
        "range": (1, 10000),
        "value_error": "SSH max idle timeout can only be between '1' and '10000'."
    },
    "sshTotalTimeout": {
        "required": False,
        "type": int,
        "range": (1, 10000),
        "value_error": "SSH total timeout can only be between '1' and '10000'."
    }
}

@my_blueprint.route(API_ROUTE + "/updateSettings", methods=["POST"])
def update_settings_route():
    global_error, global_data = validateParams(request.args, GLOBAL_SCHEMA)
    if global_error:
        return global_error

    assert global_data is not None, "Expected validated params, got None"

    sshIdleTimeout = global_data.get("sshIdleTimeout")
    sshTotalTimeout = global_data.get("sshTotalTimeout")

    try:
        ChronicleDB().updateSettings(
            sshIdleTimeout = sshIdleTimeout,
            sshTotalTimeout = sshTotalTimeout
        )
        return makeResponse(
            True,
            "Updated Chronicle settings succesfuly",
            {},
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
            "Failed to update chronicle settings",
            {"error": error},
            500
        )
