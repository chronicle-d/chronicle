from fastapi import HTTPException, Response
from typing import Any
from functools import wraps
from pydantic import BaseModel
from chronicle import ChronicleException, getErrorMsg
from inspect import iscoroutinefunction
from chronicle_base.config import get_error_context, set_error_context, reset_error_context

class APIResponse(BaseModel):
    success: int
    description: str
    data: Any

def makeResponse(response: Response, success: bool, message: str, data: Any = None, status: int = 200) -> dict:
    response.status_code = status
    return {
        "success": int(success),
        "description": message,
        "data": data or {}
    }

def makeResponseDoc(*docs: tuple[int, str]) -> dict:

    docs_with_defaults = list(docs)

    existing_statuses = [status for status, _ in docs_with_defaults]

    if 500 not in existing_statuses:
        docs_with_defaults.append((500, "An unexpected error occurred."))

    if 422 not in existing_statuses:
        docs_with_defaults.append((422, "Validation error"))

    return {
        status: {
            "description": desc,
            "content": {
                "application/json": {
                    "example": {
                        "success": 1 if status == 200 else 0,
                        "description": desc,
                        "data": {
                            "Foo": "Bar"
                        }
                    }
                }
            }
        }
        for status, desc in docs_with_defaults
    }

def responseExceptionHandler():
    def decorator(func):
        if iscoroutinefunction(func):
            @wraps(func)
            async def async_wrapper(*args, **kwargs):
                try:
                    return await func(*args, **kwargs)
                except HTTPException:
                    raise
                except ChronicleException as ce:
                    context = get_error_context()
                    raise HTTPException(status_code=500, detail=f"{context}: {getErrorMsg(ce.code)}, {ce.details}")
                except Exception as e:
                    context = get_error_context()
                    raise HTTPException(status_code=500, detail=f"{context} (Python error): {str(e)}")
            return async_wrapper
        else:
            @wraps(func)
            def sync_wrapper(*args, **kwargs):
                try:
                    return func(*args, **kwargs)
                except HTTPException:
                    raise
                except ChronicleException as ce:
                    context = get_error_context()
                    raise HTTPException(status_code=500, detail=f"{context}: {getErrorMsg(ce.code)}, {ce.details}")
                except Exception as e:
                    context = get_error_context()
                    raise HTTPException(status_code=500, detail=f"{context}: {str(e)}")
            return sync_wrapper
    return decorator
