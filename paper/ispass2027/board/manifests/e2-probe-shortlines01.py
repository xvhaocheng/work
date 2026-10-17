#!/usr/bin/env python3
"""Preserve reviewed shell tokens while splitting long UART input lines."""
import hashlib
import json
from pathlib import Path
import shlex
import subprocess

ROOT = Path('/home/xvhaocheng/work-root-a')
RUN = ROOT / 'paper/ispass2027/board/runs/e2-timer-probe-20261018-01'


def main():
    original = RUN / 'plan.json'
    plan = json.loads(original.read_text())
    checks = []
    for step in plan['steps']:
        command = step['command']
        short = command.replace('; ', ';\n')
        assert shlex.split(short) == shlex.split(command), 'quoted token changed'
        width = max(map(len, short.splitlines()))
        assert width < 512, 'a target input line remains too long'
        subprocess.run(['bash', '-n'], input=short, text=True, check=True)
        step['command'] = short
        checks.append({'step': step['name'], 'tokens_unchanged': True,
                       'maximum_command_line_bytes': width, 'bash_n_exit': 0})
    plan['transport_adaptation'] = {
        'reason': 'preflight01 UART echoed a truncated incomplete single line; no BEGIN or DMA command executed',
        'source_plan': str(original),
        'source_plan_sha256': hashlib.sha256(original.read_bytes()).hexdigest(),
        'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'checks': checks,
        'experiment_cli_or_budget_changes': False,
    }
    output = RUN / 'plan-shortlines01.json'
    with output.open('x') as f:
        f.write(json.dumps(plan, indent=2) + '\n')
    print(json.dumps({'status': 'prepared_only', 'steps': len(checks),
                      'maximum_line_bytes': max(x['maximum_command_line_bytes'] for x in checks)}))


if __name__ == '__main__':
    main()
