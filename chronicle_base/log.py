import logging
import sys
import threading
import os
from contextlib import contextmanager

logger = logging.getLogger("uvicorn.error")

@contextmanager
def captureStderrToLogger(source: str):
    stderr_fd = sys.stderr.fileno()
    old_stderr = os.dup(stderr_fd)

    r_fd, w_fd = os.pipe()
    os.dup2(w_fd, stderr_fd)
    os.close(w_fd)

    def log_stderr_lines():
        with os.fdopen(r_fd) as r:
            for line in r:
                logger.info(f"<{source}> %s", line.rstrip())

    thread = threading.Thread(target=log_stderr_lines, daemon=True)
    thread.start()

    try:
        yield
    finally:
        os.dup2(old_stderr, stderr_fd)
        os.close(old_stderr)
