# Function to return the current chronicle settings
from flask import Blueprint
from core.response import makeResponse
from chronicle import ChronicleException, ChronicleDB, getErrorMsg
from config.settings import API_ROUTE
import json

my_blueprint = Blueprint("getsettings", __name__)
TRIED_INIT = False

@my_blueprint.route(API_ROUTE + "/getSettings", methods=["GET"])
def get_settings_route():
    global TRIED_INIT

    try:
        raw_response = ChronicleDB().getSettings()
        chronicleSettings = json.loads(raw_response)

        return makeResponse(
            True,
            "Fetched Chronicle settings succesfuly",
            {
                "settings": chronicleSettings
            },
            200
        )

    except (ChronicleException, Exception) as e:
        error = {}
        if isinstance(e, ChronicleException):

            if e.code == 15003 and not TRIED_INIT:
                TRIED_INIT = True
                ChronicleDB().initDB()
                return get_settings_route()

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
            500
        )
