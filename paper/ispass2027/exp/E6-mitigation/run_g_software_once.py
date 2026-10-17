#!/usr/bin/env python3
"""One Root-authorized prepare/finalize sequence; no retry or hardware action."""
import hashlib
from pathlib import Path
import subprocess
import sys

HERE = Path(__file__).resolve().parent
BUILDER = HERE/'build_g_software.py'
OUT = HERE/'artifacts/g-software01'
EXPECTED = 'cf478f749eb6b92f7d21404d08e5119efa7ccd6aec88164ebae2eeb145a3f6e1'

if __name__ == '__main__':
    assert hashlib.sha256(BUILDER.read_bytes()).hexdigest() == EXPECTED
    assert not OUT.exists(), 'one fresh G software package only'
    for action in ['prepare', 'finalize']:
        subprocess.run([sys.executable, BUILDER, action, '--out', OUT, '--jobs', '8'], check=True)
