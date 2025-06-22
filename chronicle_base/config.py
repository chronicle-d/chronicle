from contextvars import ContextVar
from fastapi import Depends, Request

# Paths
DEVICES_BASE = "/home/noam/Work_spaces/Personal/chronicle/chronicle_core/bin/devices/"

# Default values
## Device
CHRONICLE_CONFIG_DEFAULT_KEX_METHODS = "curve25519-sha256@libssh.org,ecdh-sha2-nistp384,ecdh-sha2-nistp256,diffie-hellman-group14-sha256"
CHRONICLE_CONFIG_DEFAULT_HOSTKEYS = "ssh-ed25519,ecdsa-sha2-nistp521,ecdsa-sha2-nistp384,ecdsa-sha2-nistp256,rsa-sha2-512,rsa-sha2-256"
CHRONICLE_CONFIG_DEFAULT_PORT = 22
CHRONICLE_CONFIG_DEFAULT_USER = "chronicle-runner"
CHRONICLE_CONFIG_DEFAULT_VERBOSITY = 0

## Chronicle settings
CHRONICLE_CONFIG_DEFAULT_SSH_IDLE_TIMEOUT = 1000
CHRONICLE_CONFIG_DEFAULT_SSH_TOTAL_TIMEOUT = 10000

# Const vars
__error_context: ContextVar[str] = ContextVar("__error_context", default="Unknown error")
def get_error_context() -> str:
    return __error_context.get()
def set_error_context(msg: str):
    return __error_context.set(msg)
def reset_error_context(token):
    __error_context.reset(token)
def set_error_context_dep(msg_template: str):
    async def _set(request: Request):
        params = {**request.path_params, **dict(request.query_params)}
        try:
            msg = msg_template.format(**params)
        except KeyError:
            msg = msg_template
        set_error_context(msg)
    return _set
