"""
A simple Python interface for calling resymgen commands via subprocess.
Requires cargo to be installed and available in the runtime environment.

Example usage:
```
from resymgen import resymgen
resymgen.help([]).check_returncode()
resymgen.fmt(["--check", "<path/to/symbol/file>"]).check_returncode()
```
"""

import os
import subprocess


class Resymgen:
    MANIFEST_PATH = os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "Cargo.toml"
    )
    CARGO_MANIFEST_PATH_ARG = f"--manifest-path={MANIFEST_PATH}"

    def __init__(self):
        # Eagerly build resymgen so it doesn't happen lazily on the first run
        # (this also confirms that cargo is available in the user environment)
        subprocess.run(
            ["cargo", "build", "--release", "--quiet", Resymgen.CARGO_MANIFEST_PATH_ARG]
        ).check_returncode()

    def __getattr__(self, command):
        """Attributes are passed straight to resymgen to be interpreted as commands

        Args:
            command (str): resymgen command

        Returns:
            A function that behaves just like subprocess.run, except the args
            list will be passed to resymgen.
        """

        def run_command(args, **kwargs):
            """Same API as subprocess.run"""
            subprocess_args = [
                "cargo",
                "run",
                "--release",
                Resymgen.CARGO_MANIFEST_PATH_ARG,
                "--",
                command,
            ]
            if type(args) == list:
                subprocess_args += args
            else:
                subprocess_args.append(args)
            return subprocess.run(subprocess_args, **kwargs)

        return run_command


resymgen = Resymgen()
