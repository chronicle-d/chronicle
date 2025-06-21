"""
This file contains all Chronicle API endpoints to handle chronicle settings.
"""

from fastapi import APIRouter, Response
from chronicle import ChronicleDB
from chronicle_base.responses import makeResponse, APIResponse, makeResponseDoc, responseExceptionHandler
import json

router = APIRouter(prefix="/settings", tags=["settings"],)
cdb = ChronicleDB()

# Get settings
@router.get(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "Fetched chronicle settings successfully."))
)
@responseExceptionHandler("Could not fetch chronicle settings")
def get_settings(response: Response):
    settings = cdb.getSettings()
    return makeResponse(response, True, "Fetched chronicle settings successfully.", json.loads(settings), 200)

# Update settings
@router.post(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "Updated chronicle settings successfully."))
)
@responseExceptionHandler("Could not update chronicle settings")
def update_settings(response: Response, sshIdleTimeout: int | None = None, sshTotalTimeout: int | None = None):

    cdb.updateSettings(
        sshIdleTimeout = sshIdleTimeout,
        sshTotalTimeout = sshTotalTimeout
    )

    return makeResponse(response, True, "Updated chronicle settings successfully.", {}, 200)
