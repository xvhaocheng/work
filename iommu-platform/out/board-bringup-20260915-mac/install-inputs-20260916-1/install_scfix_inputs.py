#!/usr/bin/env python3
"""Checked deployment: refuse concurrent edits, back up all replaced files first."""
import hashlib
import json
from pathlib import Path
import shutil

root = Path('/home/xvhaocheng/work/iommu-platform')
stage = Path(__file__).resolve().parent
backup = root/'out/board-bringup-20260915-mac/install-backup-20260916-1'
spec = [
    ('platform.py', root/'scripts/platform.py', '010e2d1505a3ba695b07f3806a4b33dc98c6e0a95f2e618ba7df59e21ecf2c6e'),
    ('sources.lock.json', root/'sources.lock.json', '18e89f740773f9ab18cfd47cc2bb7926852d352c251e3b101a3b2f05562685e5'),
    ('DELL_HANDOFF.md', root.parent/'HANDOFF.md', 'a9f0d67a040255866602d79e7be1cac7cb7b56e77704c80bab29295fdd8bad4e'),
    ('build_scfix.py', root/'scripts/build_scfix.py', None),
    ('migration-4dma-scfix.json', root/'configs/migration-4dma-scfix.json', None),
    ('SC_WORD_FIX.md', root/'SC_WORD_FIX.md', None),
    ('run_wt_sc_test.py', root/'tests/run_wt_sc_test.py', None),
    ('wt_sc_test_top.sv', root/'tests/wt_sc_test_top.sv', None),
    ('wt_sc_test.cpp', root/'tests/wt_sc_test.cpp', None),
]

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def check(target, expected):
    if expected is None:
        if target.exists() or target.is_symlink():
            raise RuntimeError(f'Refusing to replace a new target that already exists: {target}')
    elif target.is_symlink() or digest(target) != expected:
        raise RuntimeError(f'Concurrent edit or unexpected file: {target}')

for name, target, expected in spec:
    if not (stage/name).is_file():
        raise RuntimeError(f'Missing input: {name}')
    check(target, expected)
backup.mkdir(exist_ok=False)
records = []
for name, target, expected in spec:
    check(target, expected)
    if expected:
        shutil.copy2(target, backup/name)
        if digest(backup/name) != expected:
            raise RuntimeError(f'Backup verification failed: {target}')
    records.append({'target':str(target), 'previous_sha256':expected,
                    'new_sha256':digest(stage/name), 'input':str(stage/name)})
(backup/'manifest.json').write_text(json.dumps(records,indent=2)+'\n')
for name, target, expected in spec:
    check(target, expected)
    if expected:
        shutil.copy2(stage/name, target)
    else:
        with target.open('xb') as output:
            output.write((stage/name).read_bytes())
    if digest(target) != digest(stage/name):
        raise RuntimeError(f'Installed file mismatch: {target}')
print(f'Installed {len(spec)} checked files. Backups: {backup}')
