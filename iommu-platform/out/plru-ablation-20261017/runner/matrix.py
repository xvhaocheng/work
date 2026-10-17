#!/usr/bin/env python3
"""Run the explicitly accepted 18 configurations and six R8 repeats, once each."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

TASK = Path(__file__).resolve().parents[1]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--variants', nargs='+', choices=('v0', 'v1', 'v2'), default=['v0', 'v1', 'v2'])
    args = ap.parse_args()
    for variant in args.variants:
        executable = TASK / 'builds' / f'{variant}-build01/obj/Vnonblock_actual_dma_system_top'
        for mode in ('p', 'pn'):
            for ring in (1, 8, 16):
                case = f'{variant}-{mode}-r{ring}'
                required = 2 if ring == 8 else 1
                existing = sorted((TASK / 'runs' / case).glob('attempt*/receipt.json'))
                for path in existing:
                    receipt = json.loads(path.read_text())
                    if receipt.get('state') != 'completed' or receipt.get('runner_exit_code') != 0:
                        raise RuntimeError(f'Existing failed attempt requires explicit review; no automatic retry: {path}')
                if len(existing) > required:
                    raise RuntimeError(f'Unexpected extra attempts: {case}')
                for _ in range(required - len(existing)):
                    command = [sys.executable, str(TASK / 'runner/execute.py'), case, '--executable', str(executable)]
                    completed = subprocess.run(command, check=False)
                    if completed.returncode:
                        return completed.returncode
    return 0


if __name__ == '__main__':
    sys.exit(main())
