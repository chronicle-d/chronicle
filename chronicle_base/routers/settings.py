"""
This file contains all Chronicle API endpoints to handle chronicle settings.
"""

from fastapi import APIRouter, Depends, Response
from chronicle_base.responses import makeResponse, APIResponse, makeResponseDoc, responseExceptionHandler
from chronicle_base.dependencies import getCdb, safeDep
from chronicle_base.config import set_error_context_dep
from chronicle import ChronicleDB
import json

router = APIRouter(prefix="/settings", tags=["settings"],)
E = set_error_context_dep

# Get settings
@router.get(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "Fetched chronicle settings successfully."))
)
@responseExceptionHandler()
def get_settings(response: Response, cdb: ChronicleDB = Depends(safeDep(getCdb)), _: None = Depends(E("Could not fetch chronicle settings"))):
    settings = cdb.getSettings()
    return makeResponse(response, True, "Fetched chronicle settings successfully.", json.loads(settings), 200)

# Update settings
@router.post(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "Updated chronicle settings successfully."))
)
@responseExceptionHandler()
def update_settings(
        response: Response,
        sshIdleTimeout: int | None = None,
        sshTotalTimeout: int | None = None,
        cdb: ChronicleDB = Depends(safeDep(getCdb)),
        _: None = Depends(E("Could not update chronicle settings"))
):

    cdb.updateSettings(
        sshIdleTimeout = sshIdleTimeout,
        sshTotalTimeout = sshTotalTimeout
    )

    return makeResponse(response, True, "Updated chronicle settings successfully.", {}, 200)
