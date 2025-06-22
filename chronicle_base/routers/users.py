"""
This file contains all Chronicle API endpoints to handle users.
"""

from fastapi import APIRouter, Response, Depends
from chronicle import ChronicleDB
from chronicle_base.responses import makeResponse, APIResponse, makeResponseDoc, responseExceptionHandler
from chronicle_base.dependencies import getCdb, safeDep
from chronicle_base.config import set_error_context_dep
import json

router = APIRouter(prefix="/users", tags=["users"],)
E = set_error_context_dep

# Delete a user
@router.delete(
    "/{username}",
    response_model=APIResponse,
    responses=makeResponseDoc((200, "User was deleted successfully."))
)
@responseExceptionHandler()
def delete_user(
    response: Response,
    username: str,
    _: None = Depends(E("User '{username}' could not be deleted")),
    cdb: ChronicleDB = Depends(safeDep(getCdb))
):

    cdb.deleteUser(username=username)
    
    return makeResponse(response, True, f"User '{username}' successfully.", {}, 200)

# Get a user
@router.get(
    "/{username}",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "User was fetched successfully."),
        (404, "User was not found.")
    )
)
@responseExceptionHandler()
def get_user(response: Response, username: str, _: None = Depends(E("User '{username}' could not be fetched")), cdb: ChronicleDB =  Depends(safeDep(getCdb))):

    userData = cdb.getUser(username=username)

    if not userData:
        return makeResponse(response, False, f"User '{username}' does not exist.", {}, 404)
    
    return makeResponse(response, True, f"User '{username}' fetched successfully.", json.loads(userData), 200)

# Get list of users
@router.get(
    "/",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "Fetched list of users successfully"),
    )
)
@responseExceptionHandler()
def list_users(response: Response, _: None = Depends(E("Could not fetch list of users")), cdb: ChronicleDB =  Depends(safeDep(getCdb))):
    users = cdb.listUsers()
    formatted_users = [json.loads(d) for d in users]
    
    return makeResponse(
        response,
        True,
        "Fetched list of users successfully.",
        {
            "users": formatted_users,
            "total": len(formatted_users)},
        200
    )

# Create user
@router.post(
    "/create/{username}",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "User created successfully."),
    )
)
@responseExceptionHandler()
def create_user(
    response: Response,
    username: str,
    password: str,
    connected: bool,
    _: None = Depends(E("User '{username}' could not be created")),
    cdb: ChronicleDB =  Depends(safeDep(getCdb))
):

    cdb.addUser(
        username=username,
        password=password,
        connected=connected
    )

    return makeResponse(response, True, f"Created user '{username}' successfully.", {}, 200)

# Modify user
@router.post(
    "/modify/{username}",
    response_model=APIResponse,
    responses=makeResponseDoc(
        (200, "User modified successfully."),
    )
)
@responseExceptionHandler()
def modify_user(
    response: Response,
    username: str,
    password: str | None = None,
    connected: bool | None = None,
    _: None = Depends(E("User '{username}' could not be modified")),
    cdb: ChronicleDB =  Depends(safeDep(getCdb))
):

    cdb.modifyUser(
        username=username,
        password=password,
        connected=connected
    )

    return makeResponse(response, True, f"modified user '{username}' successfully.", {}, 200)
