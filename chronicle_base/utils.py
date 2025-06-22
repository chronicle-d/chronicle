import re

ansi_escape_re = re.compile(
    r'''
    \x1B   # ESC
    (?:    # 7-bit C1 Fe (except CSI)
        [@-Z\\-_]
    |      # or [ for CSI
        \[
        [0-?]*  # Parameter bytes
        [ -/]*  # Intermediate bytes
        [@-~]   # Final byte
    |      # or ] for OSC, terminated by BEL or ST
        \]
        .*?     # Anything up to...
        (?:\x07|\x1B\\)  # BEL or ST
    )
    ''',
    re.VERBOSE
)

def stripAnsi(line: str) -> str:
    return ansi_escape_re.sub('', line)
