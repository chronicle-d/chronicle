import sys
from fastapi import FastAPI, Request, HTTPException, Response
from fastapi.exceptions import RequestValidationError
from fastapi.responses import JSONResponse
from chronicle import ChronicleDB, ChronicleException, getErrorMsg
from chronicle_base.responses import makeResponse, APIResponse
from chronicle_base.routers import devices, settings


def create_app() -> FastAPI:
    try:
        ChronicleDB().connect()
        ChronicleDB().initDB()
    except (ChronicleException, Exception) as e:
        if isinstance(e, ChronicleException):
            errorMsg = getErrorMsg(e.code)
            if (e.code == 10001):
                sys.stderr.write(f"Error {e.code}, {errorMsg} => Make sure that MongoD is up. ({e.details})\n")
            else:
                sys.stderr.write(f"Error {e.code}, {errorMsg} => {e.details}\n")
        else:
            sys.stderr.write(f"Pythonic error: {e}\n")
        sys.exit(1)

    app = FastAPI()

    @app.exception_handler(RequestValidationError)
    async def validation_exception_handler(request: Request, exc: RequestValidationError):
        return JSONResponse(
            status_code=422,
            content={
                "success": 0,
                "description": "Validation error",
                "data": exc.errors(),
            },
        )

    @app.exception_handler(HTTPException)
    async def http_exception_handler(request: Request, exc: HTTPException):
        return JSONResponse(
            status_code=exc.status_code,
            content={
                "success": 0,
                "description": exc.detail,
                "data": {},
            },
        )
    return app

app = create_app()

app.include_router(devices.router)
app.include_router(settings.router)
