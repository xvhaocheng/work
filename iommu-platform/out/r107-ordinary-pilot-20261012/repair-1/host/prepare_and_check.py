#!/usr/bin/env python3
"""Copy frozen data, build host/static RV64 CLI, record current host checks.

This wrapper never opens a device and never calls an old replay/model function.
"""
import datetime
import hashlib
import json
import shutil
import subprocess
import sys
import time
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
PLATFORM = ROOT.parents[1]
OLD = PLATFORM/'out/r107-bounded-decision-20261012/instance'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    start = time.monotonic_ns()
    (ROOT/'data').mkdir(exist_ok=True)
    (ROOT/'validation').mkdir(exist_ok=True)
    expected = json.loads((OLD/'raw-manifest.json').read_text())
    files = []
    for entry in expected:
        source = OLD/entry['file'];target=ROOT/'data'/source.name
        if not target.exists():
            shutil.copyfile(source, target)
        if target.stat().st_size != entry['bytes'] or sha(target) != entry['sha256']:
            raise RuntimeError(f'frozen asset disagreement: {target.name}')
        files.append(dict(file=target.name, source=str(source), bytes=entry['bytes'], sha256=entry['sha256']))
    manifest = dict(schema='r107-ordinary-frozen-data-v1', origin_stage=108,
                    copied_only=True, old_checker_executed=False, files=files,
                    origin_manifest=str(OLD/'raw-manifest.json'), origin_manifest_sha256=sha(OLD/'raw-manifest.json'))
    (ROOT/'data/raw-manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    (ROOT/'data/SHA256SUMS').write_text(''.join(x['sha256']+'  '+x['file']+'\n' for x in files))
    commands = [
        ['gcc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', 'host/idma_finite_cli.c', '-o', 'host/idma_finite_cli.host'],
        ['riscv64-linux-gnu-gcc', '-std=c11', '-O2', '-static', '-Wall', '-Wextra', '-Werror', 'host/idma_finite_cli.c', '-o', 'host/idma_finite_cli.rv64'],
        ['host/idma_finite_cli.host', '--describe'],
        ['host/idma_finite_cli.host', '--check-data', '--data-dir', 'data'],
        ['file', 'host/idma_finite_cli.host', 'host/idma_finite_cli.rv64'],
        [sys.executable, '-B', 'host/test_finite_receipt.py'],
        [sys.executable, '-B', 'host/validate_finite_pilot.py', 'validation/receipt-validator-tests/receipt-synthetic.json'],
    ]
    records = []
    for i, command in enumerate(commands):
        begin = time.monotonic_ns()
        result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True, check=False)
        end = time.monotonic_ns()
        output = ROOT/'validation'/f'check-{i:02d}.stdout'
        error = ROOT/'validation'/f'check-{i:02d}.stderr'
        output.write_text(result.stdout);error.write_text(result.stderr)
        records.append(dict(command=command, returncode=result.returncode, wall_ns=end-begin,
                            stdout=str(output.relative_to(ROOT)), stderr=str(error.relative_to(ROOT))))
        if result.returncode:
            (ROOT/'validation/host-check-ledger.json').write_text(json.dumps(dict(commands=records, completed=False), indent=2)+'\n')
            print(result.stderr or result.stdout)
            return result.returncode
    ledger = dict(schema='r107-finite-host-check-ledger-v1', actor='Codex gpt-6.1-sol ultra',
                  timestamp_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                  commands=records, completed=True, total_wall_ns=time.monotonic_ns()-start,
                  files={str(path.relative_to(ROOT)): sha(path) for path in [
                      ROOT/'host/idma_finite_cli.c', ROOT/'host/validate_finite_pilot.py',
                      ROOT/'host/test_finite_receipt.py', ROOT/'host/idma_finite_cli.host',
                      ROOT/'host/idma_finite_cli.rv64', ROOT/'source/linux/include/uapi/linux/idma.h',
                      ROOT/'design/root-implementation-contract.json', ROOT/'data/raw-manifest.json']},
                  usage='Codex-only requested; no DeepSeek/Claude/Astra call; no separate per-seat token meter',
                  board_execution=False, device_ioctls=False, old_stage106_matrix=False,
                  old_stage108_checker=False, source_cpu_replay=False,
                  earlier_diagnostics=[dict(command=commands[0], returncode=1,
                      diagnostic='initial host compilation rejected misleading indentation and run-only unused functions; corrected before final build',
                      source_before_revision_preserved=False),
                      dict(command=commands[0], returncode=0, diagnostic='intermediate host compilation passed; final build rerun after new ABI static assertions'),
                      dict(command=commands[1], returncode=0, diagnostic='intermediate RV64 static compilation passed; final build rerun after new ABI static assertions')])
    (ROOT/'validation/host-check-ledger.json').write_text(json.dumps(ledger, indent=2)+'\n')
    print(json.dumps(dict(completed=True, commands=len(records), total_wall_ns=ledger['total_wall_ns'],
                          rv64_sha256=ledger['files']['host/idma_finite_cli.rv64']), indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
