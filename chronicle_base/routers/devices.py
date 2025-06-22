"""
This file contains all Chronicle API endpoints to handle devices.
"""

from fastapi import APIRouter, Response, Depends
from chronicle import ChronicleDB, deviceOperations, connectionInfo, getConfig
from chronicle_base.responses import makeResponse, APIResponse, makeResponseDoc, responseExceptionHandler
from chronicle_base.dependencies import getDeviceOperationalMap, getCdb, getDeviceConnectionInfo, safeDep
from chronicle_base.config import (
    CHRONICLE_CONFIG_DEFAULT_HOSTKEYS,
    CHRONICLE_CONFIG_DEFAULT_KEX_METHODS,
    CHRONICLE_CONFIG_DEFAULT_PORT,
    CHRONICLE_CONFIG_DEFAULT_USER,
    CHRONICLE_CONFIG_DEFAULT_VERBOSITY,
    set_error_context_dep
)
from chronicle_base.log import captureStderrToLogger
from chronicle_base.utils import stripAnsi
import json

router = APIRouter(prefix="/devices", tags=["devices"],)
E = set_error_context_dep

# Delete a device
@router.delete(
    "/{deviceNickname}",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "Device was deleted successfully."))
)
@responseExceptionHandler()
def delete_device(
    response: Response,
    deviceNickname: str,
    _: None = Depends(E("Device '{deviceNickname}' could not be deleted")),
    cdb: ChronicleDB = Depends(safeDep(getCdb))
):

    cdb.deleteDevice(deviceNickname=deviceNickname)
    
    return makeResponse(response, True, f"Deleted device '{deviceNickname}' successfully.", {}, 200)

# Get a device
@router.get(
    "/{deviceNickname}",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Device was fetched successfully."),
        (404, "Device was not found.")
    )
)
@responseExceptionHandler()
def get_device(response: Response, deviceNickname: str, _: None = Depends(E("Device '{deviceNickname}' could not be fetched")), cdb: ChronicleDB =  Depends(safeDep(getCdb))):

    deviceData = cdb.getDevice(deviceNickname=deviceNickname)

    if not deviceData:
        return makeResponse(response, False, f"Device '{deviceNickname}' does not exist.", {}, 404)
    
    return makeResponse(response, True, f"Fetched device '{deviceNickname}' successfully.", json.loads(deviceData), 200)

# Get a device configuration
@router.get(
    "/{deviceNickname}/config",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Device configuration was fetched successfully."),
    )
)
@responseExceptionHandler()
async def get_device_config(
    response: Response,
    deviceNickname: str,
    _: None = Depends(E("Device '{deviceNickname}' configuration could not be fetched")),
    deviceSettings: connectionInfo = Depends(safeDep(getDeviceConnectionInfo)),
    dom: deviceOperations = Depends(safeDep(getDeviceOperationalMap))
):

    with captureStderrToLogger("SSH - Get device config"):
        rawConfig = getConfig(deviceSettings, dom.ops.getConfig)
        deviceConfig = [stripAnsi(line) for line in rawConfig]
    
    return makeResponse(response, True, f"Fetched device '{deviceNickname}' configuration successfully.", deviceConfig, 200)

# Get list of devices
@router.get(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Fetched list of devices successfully"),
    )
)
@responseExceptionHandler()
def list_devices(response: Response, _: None = Depends(E("Could not fetch list of devices")), cdb: ChronicleDB =  Depends(safeDep(getCdb))):
    devices = cdb.listDevices()
    formatted_devices = [json.loads(d) for d in devices]
    
    return makeResponse(
        response,
        True,
        "Fetched list of devices successfully.",
        {
            "devices": formatted_devices,
            "total": len(formatted_devices)},
        200
    )

# Create device
@router.post(
    "/create/{deviceNickname}",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Device created successfully."),
    )
)
@responseExceptionHandler()
def create_device(
    response: Response,
    deviceNickname: str,
    deviceName: str,
    vendor: str,
    password: str,
    host: str,
    port: int = CHRONICLE_CONFIG_DEFAULT_PORT,
    sshVerbosity: int = CHRONICLE_CONFIG_DEFAULT_VERBOSITY,
    kexMethods: str = CHRONICLE_CONFIG_DEFAULT_KEX_METHODS,
    hostkeyAlgorithms: str = CHRONICLE_CONFIG_DEFAULT_HOSTKEYS, 
    user: str = CHRONICLE_CONFIG_DEFAULT_USER,
    _: None = Depends(E("Device '{deviceNickname}' could not be created")),
    cdb: ChronicleDB =  Depends(safeDep(getCdb))
):

    cdb.addDevice(
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

    return makeResponse(response, True, f"Created device '{deviceNickname}' successfully.", {}, 200)

# Modify device
@router.post(
    "/modify/{deviceNickname}",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Device modified successfully."),
    )
)
@responseExceptionHandler()
def modify_device(
    response: Response,
    deviceNickname: str,
    deviceName: str | None = None,
    vendor: str | None = None,
    password: str | None = None,
    host: str | None = None,
    port: int | None = None,
    sshVerbosity: int | None = None,
    kexMethods: str | None = None,
    hostkeyAlgorithms: str | None = None, 
    user: str | None = None,
    _: None = Depends(E("Device '{deviceNickname}' could not be modified")),
    cdb: ChronicleDB =  Depends(safeDep(getCdb))
):
    
    cdb.modifyDevice(
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

    return makeResponse(response, True, f"Modified device '{deviceNickname}' successfully.", {}, 200)
