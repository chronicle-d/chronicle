# Manage a device
from flask import Blueprint, request
from core.response import makeResponse
from utils.paramHandler import validateParams
from config.settings import (
    CHRONICLE_CONFIG_DEFAULT_HOSTKEYS,
    CHRONICLE_CONFIG_DEFAULT_KEX_METHODS,
    CHRONICLE_CONFIG_DEFAULT_PORT,
    CHRONICLE_CONFIG_DEFAULT_USER,
    CHRONICLE_CONFIG_DEFAULT_VERBOSITY,
    API_ROUTE
)
from chronicle import ChronicleDB, ChronicleException, getErrorMsg

GLOBAL_SCHEMA = {
    "action": {
        "required": True,
        "required_error": "Missing 'action'. Must be one of: create, modify.",
        "allowed": {"create", "modify"},
        "value_error": "Invalid 'action'. Must be 'create' or 'modify'."
    }
}

CREATE_DEVICE_SCHEMA = {
    "name": {
        "required": True,
        "required_error": "Missing 'name' (deviceNickname). Example: name=my_device"
    },
    "device": {
        "required": True,
        "required_error": "Missing 'device'. Example: device=SRX"
    },
    "vendor": {
        "required": True,
        "required_error": "Missing 'vendor'. Example: vendor=Cisco"
    },
    "user": {
        "required": False,
    },
    "password": {
        "required": True,
        "required_error": "Missing 'password'. Example: password=1234"
    },
    "host": {
        "required": True,
        "required_error": "Missing 'host'. Example: host=192.168.1.1"
    },
    "port": {
        "required": False,
        "type": int,
        "range": (1, 65535),
        "value_error": "Port must be an integer between 1 and 65535"
    },
    "ssh_verbosity": {
        "required": False,
        "type": int,
        "range": (0, 4),
        "value_error": "ssh_verbosity must be between 0 and 4"
    },
    "ssh_kex_methods": {
        "required": False,
    },
    "ssh_hostkey_algorithms": {
        "required": False,
    }
}

MODIFY_DEVICE_SCHEMA = {
    "name": {
        "required": True,
        "required_error": "Missing 'name' (deviceNickname). Example: name=my_device"
    },
    "device": {
        "required": False
    },
    "vendor": {
        "required": False,
    },
    "user": {
        "required": False
    },
    "password": {
        "required": False,

    },
    "host": {
        "required": False,
    },
    "port": {
        "required": False,
        "type": int,
        "range": (1, 65535),
        "value_error": "Port must be an integer between 1 and 65535"
    },
    "ssh_verbosity": {
        "required": False,
        "type": int,
        "range": (0, 4),
        "value_error": "ssh_verbosity must be between 0 and 4"
    },
    "ssh_kex_methods": {
        "required": False,
    },
    "ssh_hostkey_algorithms": {
        "required": False,
    }
}

my_blueprint = Blueprint("managedevice", __name__)

@my_blueprint.route(API_ROUTE + "/manageDevice", methods=["POST"])
def manage_device_route():
    global_error, global_data = validateParams(request.args, GLOBAL_SCHEMA)
    if global_error:
        return global_error

    assert global_data is not None, "Expected validated params, got None"

    action = global_data["action"]

    if action == "create":
        error, data = validateParams(request.args, CREATE_DEVICE_SCHEMA)
        if error:
            return error

        assert data is not None, "Expected validated params, got None"

        deviceNickname = data["name"]
        deviceName = data["device"]
        vendor = data["vendor"]
        user = data.get("user", CHRONICLE_CONFIG_DEFAULT_USER)
        password = data["password"]
        host = data["host"]
        port = data.get("port", CHRONICLE_CONFIG_DEFAULT_PORT)
        sshVerbosity = data.get("ssh_verbosity", CHRONICLE_CONFIG_DEFAULT_VERBOSITY)
        kexMethods = data.get("ssh_kex_methods", CHRONICLE_CONFIG_DEFAULT_KEX_METHODS)
        hostkeyAlgorithms = data.get("ssh_hostkey_algorithms", CHRONICLE_CONFIG_DEFAULT_HOSTKEYS)

        try:
            ChronicleDB().addDevice(
                deviceNickname=deviceNickname,
                deviceName=deviceName,
                vendor=vendor,
                user=user,
                password=password,
                host=host,
                port=port,
                sshVerbosity=sshVerbosity,
                kexMethods=kexMethods,
                hostkeyAlgorithms=hostkeyAlgorithms
            )
            return makeResponse(
                True,
                f"Added device {deviceNickname} succefuly.",
                {},
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
                f"Failed to add device {deviceNickname}.",
                {"error": error},
                500,
                deviceNickname
            )
    elif action == "modify":
        error, data = validateParams(request.args, MODIFY_DEVICE_SCHEMA)
        if error:
            return error

        assert data is not None, "Expected validated params, got None"

        deviceNickname = data["name"]
        deviceName = data.get("device")
        vendor = data.get("vendor")
        user = data.get("user")
        password = data.get("password")
        host = data.get("host")
        port = data.get("port")
        sshVerbosity = data.get("ssh_verbosity")
        kexMethods = data.get("ssh_kex_methods")
        hostkeyAlgorithms = data.get("ssh_hostkey_algorithms")

        try:
            ChronicleDB().modifyDevice(
                deviceNickname=deviceNickname,
                deviceName=deviceName,
                vendor=vendor,
                user=user,
                password=password,
                host=host,
                port=port,
                sshVerbosity=sshVerbosity,
                kexMethods=kexMethods,
                hostkeyAlgorithms=hostkeyAlgorithms
            )
            return makeResponse(
                True,
                f"Modified device {deviceNickname} succefuly.",
                {},
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
                f"Failed to modify device {deviceNickname}.",
                {"error": error},
                500,
                deviceNickname
            )
    else:
        return makeResponse(
            False,
            "Action can only be 'create' or 'modify'.",
            {},
            400
        )
