from chronicle import ChronicleDB, connectionInfo, deviceOperations, getConnectionInfo, loadDeviceOps, ChronicleException, getErrorMsg
from chronicle_base.config import DEVICES_BASE
from fastapi import Path, Depends, HTTPException
from inspect import iscoroutinefunction
from functools import wraps
from chronicle_base.config import get_error_context
import os

def safeDep(fn, msg: str | None = None):
    if iscoroutinefunction(fn):
        @wraps(fn)
        async def async_wrapper(*args, **kwargs):
            try:
                return await fn(*args, **kwargs)
            except HTTPException:
                raise
            except ChronicleException as ce:
                context = msg or get_error_context()
                raise HTTPException(status_code=500, detail=f"{context}: {getErrorMsg(ce.code)}, {ce.details}")
            except Exception as e:
                context = msg or get_error_context()
                raise HTTPException(status_code=500, detail=f"{context}: {str(e)}")
        return async_wrapper
    else:
        @wraps(fn)
        def sync_wrapper(*args, **kwargs):
            try:
                return fn(*args, **kwargs)
            except HTTPException:
                raise
            except ChronicleException as ce:
                context = msg or get_error_context()
                raise HTTPException(status_code=500, detail=f"{context}: {getErrorMsg(ce.code)}, {ce.details}")
            except Exception as e:
                context = msg or get_error_context()
                raise HTTPException(status_code=500, detail=f"{context}: {str(e)}")
        return sync_wrapper

def getCdb() -> ChronicleDB:
    return ChronicleDB()

def getDeviceConnectionInfo(deviceNickname: str = Path(...)) -> connectionInfo:
    return getConnectionInfo(deviceNickname)

def getDeviceOperationalMap(deviceSettings: connectionInfo = Depends(getDeviceConnectionInfo), deviceNickname: str = Path(...)) -> deviceOperations:
    deviceMapPath = os.path.join(DEVICES_BASE, deviceSettings.vendorName, deviceSettings.deviceName + ".cld")
    if os.path.exists(deviceMapPath):
        return loadDeviceOps(deviceMapPath, deviceSettings.device, deviceSettings.vendor)
    else:
        raise Exception(f"Device map '{deviceSettings.vendorName}/{deviceSettings.deviceName}.cld' could not be found, are you sure it is installed?")
