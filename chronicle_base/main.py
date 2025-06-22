import os
import errno
from fastapi import FastAPI, Request, HTTPException, Response
from fastapi.exceptions import RequestValidationError
from fastapi.responses import JSONResponse
from fastapi.middleware.cors import CORSMiddleware
from chronicle import ChronicleDB, ChronicleException, getErrorMsg
from chronicle_base.routers import devices, settings
import logging
import logging.config
from chronicle_base.uvicorn_log_config import log_config

logging.config.dictConfig(log_config)
logger = logging.getLogger("uvicorn.error")


def create_app() -> FastAPI:
    app = FastAPI()

    @app.exception_handler(RequestValidationError)
    async def validation_exception_handler(
        request: Request, exc: RequestValidationError
    ):
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

    app.add_middleware(
        CORSMiddleware,
        allow_origins=["*"],
        allow_credentials=True,
        allow_methods=["*"],
        allow_headers=["*"],
    )

    @app.on_event("startup")
    async def startup_event():
        try:
            ChronicleDB().connect()
            ChronicleDB().initDB()
        except (ChronicleException, Exception) as e:
            if isinstance(e, ChronicleException):
                errorMsg = getErrorMsg(e.code)
                if e.code == 10001:
                    logger.error(
                        f"Error {e.code}, {errorMsg} - Make sure that MongoDB is up."
                    )
                    logger.error(f"{e.details}")
                else:
                    logger.error(f"Error {e.code}, {errorMsg} => {e.details}")
            else:
                logger.error(f"ChronicleDB init failed: {e}")

            logger.error("ChronicleDB unavailable, shutting down.")

            os._exit(errno.EINTR)

    return app


app = create_app()

app.include_router(devices.router)
app.include_router(settings.router)
