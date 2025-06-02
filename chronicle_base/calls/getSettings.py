# Function to return the current chronicle settings
from flask import Blueprint
from core.response import makeResponse
from core.chronicle import getSettings
from chronicle import ChronicleException
from config.settings import API_ROUTE

my_blueprint = Blueprint("getsettings", __name__)

@my_blueprint.route(API_ROUTE + "/getSettings", methods=["GET"])
def get_settings_route():
    try:
        chronicleSettings = getSettings()
        return makeResponse(
            True,
            "Fetched Chronicle settings succesfuly",
            {
                "ssh": {
                    "sshIdleTimeout": chronicleSettings.sshIdleTimeout,
                    "sshTotalTimeout": chronicleSettings.sshTotalTimeout
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
            "Failed to fetch chronicle settings",
            {"error": error},
            500,
            deviceNickname
        )
