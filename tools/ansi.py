"""
A simple, no-dependency Python module for printing with ANSI escape codes.

Example usage:
```
ansi.print(ansi.BOLD, "bold text")
ansi.print(ansi.BOLD + ansi.RED, "bold red text")
ansi.print(ansi.GREEN + ansi.CYAN_BG, "green text with cyan background")
```
"""

import os
import sys


class Code:
    """ANSI escape code sequence"""

    def __init__(self, params):
        self.params = params

    def __add__(self, other):
        return Code(f"{self.params};{other.params}")

    def __repr__(self):
        return f"\\033[{self.params}m"

    def __str__(self):
        return f"\033[{self.params}m"


# Commonly supported ANSI escape codes
RESET = Code("0")

BOLD = Code("1")

BLACK = Code("30")
RED = Code("31")
GREEN = Code("32")
YELLOW = Code("33")
BLUE = Code("34")
MAGENTA = Code("35")
CYAN = Code("36")
WHITE = Code("37")

BLACK_BG = Code("40")
RED_BG = Code("41")
GREEN_BG = Code("42")
YELLOW_BG = Code("43")
BLUE_BG = Code("44")
MAGENTA_BG = Code("45")
CYAN_BG = Code("46")
WHITE_BG = Code("47")

# This is how the termcolor Rust crate checks for ANSI support
ALLOWED_IN_ENV = bool(
    "NO_COLOR" not in os.environ
    and "TERM" in os.environ
    and os.environ["TERM"] != "dumb"
    and (os.name != "nt" or os.environ["TERM"] != "cygwin")
)


_builtin_print = print


def print(code, *args, **kwargs):
    """
    Print with ANSI escape codes if supported in the current environment.
    The end argument is printed normally, without ANSI escape codes.
    Arguments besides the code are passed to the built-in print() function.

    Args:
        code (Code): ANSI escape code sequence to use while printing
    """
    # Only print ANSI codes if they're supported in the environment and the
    # output file is a tty
    if ALLOWED_IN_ENV and kwargs.get("file", sys.stdout).isatty():
        flush = kwargs.get("flush", False)
        end = kwargs.get("end")
        kwargs["flush"] = False
        kwargs["end"] = ""
        _builtin_print(code, **kwargs)
        _builtin_print(*args, **kwargs)
        _builtin_print(RESET, end=end, file=kwargs.get("file"), flush=flush)
    else:
        # Ignore ANSI codes on non-ANSI terminals
        _builtin_print(*args, **kwargs)
