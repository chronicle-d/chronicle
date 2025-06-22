"""
This file contains all Chronicle API endpoints to handle devices.
"""

from fastapi import APIRouter, Response
from chronicle import ChronicleDB, getConnectionInfo, loadDeviceOps, getConfig
from chronicle_base.responses import makeResponse, APIResponse, makeResponseDoc, responseExceptionHandler
from chronicle_base.config import (
    DEVICES_BASE,
    CHRONICLE_CONFIG_DEFAULT_HOSTKEYS,
    CHRONICLE_CONFIG_DEFAULT_KEX_METHODS,
    CHRONICLE_CONFIG_DEFAULT_PORT,
    CHRONICLE_CONFIG_DEFAULT_USER,
    CHRONICLE_CONFIG_DEFAULT_VERBOSITY
)
import json
import os

router = APIRouter(prefix="/devices", tags=["devices"],)
cdb = ChronicleDB()

# Delete a device
@router.delete(
    "/{deviceNickname}",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "Device was deleted successfully."))
)
@responseExceptionHandler("Device could not be deleted")
def delete_device(response: Response, deviceNickname: str):
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
@responseExceptionHandler("Device could not be fetched")
def get_device(response: Response, deviceNickname: str):
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
@responseExceptionHandler("Device configuration could not be fetched")
async def get_device_config(response: Response, deviceNickname: str):
    deviceSettings = getConnectionInfo(deviceNickname)
    deviceMapPath = os.path.join(DEVICES_BASE, deviceSettings.vendorName, deviceSettings.deviceName + ".cld")
    DeviceOperationalMap = loadDeviceOps(deviceMapPath, deviceSettings.device, deviceSettings.vendor)
    deviceConfig = await getConfig(deviceSettings, DeviceOperationalMap.ops.getConfig)
    
    return makeResponse(response, True, f"Fetched device '{deviceNickname}' configuration successfully.", deviceConfig, 200)

# Get list of devices
@router.get(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Fetched list of devices successfully"),
    )
)
@responseExceptionHandler("Could not fetch list of devices")
def list_devices(response: Response):
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
@responseExceptionHandler("Device could not be created")
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
    user: str = CHRONICLE_CONFIG_DEFAULT_USER):
    
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
@responseExceptionHandler("Device could not be modified")
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
    user: str | None = None):
    
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
