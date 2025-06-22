from chronicle_base.colorful_formatter import ColorfulFormatter

log_config = {
    "version": 1,
    "formatters": {"colorful": {"()": ColorfulFormatter, "datefmt": "%H:%M:%S"}},
    "handlers": {
        "console": {
            "class": "logging.StreamHandler",
            "formatter": "colorful",
            "stream": "ext://sys.stdout",
        }
    },
    "loggers": {
        "uvicorn": {"handlers": ["console"], "level": "DEBUG", "propagate": False}
    },
    "root": {"handlers": ["console"], "level": "DEBUG"},
}
