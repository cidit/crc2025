#!/usr/bin/python3

# ---------------

from utils.test_environment import test_conda_environment
import sys, subprocess


HELP_TEXT = """
    #------------------------------------------------------------#
     WORKSPACE TOOLS
        This file/tool helps with managing a multi user workspace
        by making a number of commands available for unintuitive 
        operations.
        REQUIRES CONDA.
    #------------------------------------------------------------#

    AVAILABLE COMMANDS:
    ===================
    - env create: 
        creates the environment
    - env test: 
        test the environment
    - env record: 
        overwrites the conda_env.yml file to reflect the current environment
    - env sync: 
        updates the current environment to the stnadards of the conda_env.yml file
    - run: 
        runs the app. (WIP)
"""

def exec_command(argv: list[str]):
    match argv:
        case [_, "env", "test"]:
            test_conda_environment()
        case [_, "env", "record"]:
            out = subprocess.run("conda env export".split(" "), capture_output=True)
            interesting_lines = [x for x in out.stdout.decode().splitlines() if not any(["prefix:" in x, "name:" in x])]
            with open("conda_env.yml", "w") as f:
                f.write("\n".join(interesting_lines))
        case [_, "env", "sync"]:
            subprocess.run("conda env update --file conda_env.yml  --prune".split(" "))
        case [_, "run"]:
            pass
        case _:
            print(HELP_TEXT)
           
    
if __name__ == "__main__":
    exec_command(sys.argv)
