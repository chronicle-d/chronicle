from flask import jsonify, make_response as flask_response
from flask import Response
from chronicle import getConnectionInfo

def makeResponse(success: bool, message: str, data, status: int, deviceNickname: str="") -> Response:
    body = {}
    data = data or {}

    if deviceNickname:
        try:
            deviceSettings = getConnectionInfo(deviceNickname)
            data.update({
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
            })
        except Exception as e:
            body.update({"success": 0})
            body.update({"message": str(e)})
            data.update({"name": deviceNickname})
            body.update({"data": data})

    body.update({"success": int(success)})
    body.update({"message": message})
    body.update(({"data": data}))

    return flask_response(jsonify(body), status)
