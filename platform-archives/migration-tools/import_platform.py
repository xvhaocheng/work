#!/usr/bin/env python3
"""One-time, read-only capture of the legacy tree, then independent local clones."""
import concurrent.futures
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tarfile

OLD = Path('/home/xvhaocheng/riscv-iommu-demo-refact')
NEW = Path('/home/xvhaocheng/work/iommu-platform')
ARCHIVE = Path('/home/xvhaocheng/work/platform-archives/legacy-20260908')
ENV = dict(os.environ, GIT_OPTIONAL_LOCKS='0')

def git(path, *args):
    return subprocess.check_output(['git', '-C', str(path), *args], env=ENV)

def run(*args):
    subprocess.run(list(map(str, args)), check=True, env=ENV)

def discover(path, rel=''):
    yield rel, path
    if not (path / '.gitmodules').exists():
        return
    data = git(path, 'config', '-f', '.gitmodules', '--get-regexp', r'^submodule\..*\.path$').decode()
    for line in data.splitlines():
        _, child = line.split(' ', 1)
        sub = path / child
        if (sub / '.git').exists():
            yield from discover(sub, str(Path(rel) / child))

def snapshot(item):
    rel, path = item
    key = rel.replace('/', '__') or 'demo'
    target = ARCHIVE / key
    target.mkdir(parents=True, exist_ok=False)
    data = {'path': rel, 'head': git(path, 'rev-parse', 'HEAD').decode().strip(),
            'remotes': git(path, 'remote', '-v').decode(),
            'status': git(path, 'status', '--short').decode(),
            'branches': git(path, 'branch', '-avv').decode(), 'archive_key': key}
    for name, args in [('working.patch', ('diff', '--binary', '--ignore-submodules=all')),
                       ('staged.patch', ('diff', '--cached', '--binary', '--ignore-submodules=all'))]:
        (target / name).write_bytes(git(path, *args))
    files = [x.decode() for x in git(path, 'ls-files', '--others', '--exclude-standard', '-z').split(b'\0') if x]
    with tarfile.open(target / 'untracked.tar', 'w') as tar:
        for filename in files:
            tar.add(path / filename, arcname=filename, recursive=False)
    data['untracked'] = files
    (target / 'metadata.json').write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n')
    with (target / 'bundle.log').open('w') as log:
        subprocess.run(['git', '-C', str(path), '-c', 'pack.threads=2', 'bundle', 'create',
                        str(target / 'history.bundle'), '--all', 'HEAD'], check=True, env=ENV, stdout=log, stderr=log)
        subprocess.run(['git', '-C', str(path), 'bundle', 'verify', str(target / 'history.bundle')],
                       check=True, env=ENV, stdout=log, stderr=log)
    print('Captured', rel or 'demo', flush=True)
    return data

def commit(path, message):
    if git(path, 'status', '--porcelain', '--untracked-files=no').strip():
        run('git', '-C', path, 'add', '-u')
        run('git', '-C', path, 'commit', '-m', message)

def main():
    if NEW.exists():
        raise SystemExit('Destination already exists; refusing to overwrite.')
    if (ARCHIVE / 'inventory.json').exists():
        records = json.loads((ARCHIVE / 'inventory.json').read_text())
    else:
        ARCHIVE.mkdir(parents=True)
        items = list(discover(OLD))
        with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
            records = list(pool.map(snapshot, items))
    extra = ARCHIVE / 'artifacts'
    extra.mkdir(exist_ok=True)
    artifacts = ['linux/build/.config', 'linux/build/arch/riscv/boot/Image',
                 'opensbi/build/platform/fpga/ariane/firmware/fw_payload.bin',
                 'cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit',
                 'cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt',
                 'cva6/corev_apu/fpga/work-fpga/ariane_xilinx_utilization_placed.rpt']
    for rel in artifacts:
        if (OLD / rel).is_file():
            dest = extra / rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(OLD / rel, dest)
    (ARCHIVE / 'inventory.json').write_text(json.dumps(records, indent=2, ensure_ascii=False) + '\n')
    hashes = {}
    for p in sorted(ARCHIVE.rglob('*')):
        if p.is_file():
            with p.open('rb') as f:
                digest = hashlib.sha256()
                for chunk in iter(lambda: f.read(1024 * 1024), b''):
                    digest.update(chunk)
                hashes[str(p.relative_to(ARCHIVE))] = digest.hexdigest()
    (ARCHIVE / 'SHA256.json').write_text(json.dumps(hashes, indent=2) + '\n')
    print('Archive verified; cloning independently.', flush=True)
    NEW.mkdir()
    run('git', 'init', '-b', 'main', NEW)
    for record in records:
        rel = record['path']
        if not rel:
            continue
        dest = NEW / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        run('git', 'clone', '--quiet', '--no-hardlinks', '--no-checkout', OLD / rel, dest)
        run('git', '-C', dest, 'switch', '-c', 'platform', record['head'])
        # Local clones must not retain a dependency on the old working directory.
        run('git', '-C', dest, 'remote', 'remove', 'origin')
        lines = record['remotes'].splitlines()
        urls = [s.split()[1] for s in lines if '(fetch)' in s and s.split()[1].startswith(('https://', 'git@'))]
        if urls:
            run('git', '-C', dest, 'remote', 'add', 'upstream', urls[0])
        for patch in ['staged.patch', 'working.patch']:
            source = ARCHIVE / record['archive_key'] / patch
            if source.stat().st_size:
                run('git', '-C', dest, 'apply', '--binary', source)
        for filename in record['untracked']:
            source = OLD / rel / filename
            target = dest / filename
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target, follow_symlinks=False)
            run('git', '-C', dest, 'add', '--', filename)
        print('Restored', rel, flush=True)
    for record in reversed(records):
        if record['path']:
            commit(NEW / record['path'], 'platform: preserve local integration and measurement changes')
    for name, url in [('cva6','https://github.com/zero-day-labs/cva6.git'),
                      ('linux','https://github.com/malejo97/linux.git'),
                      ('opensbi','https://github.com/riscv-software-src/opensbi.git')]:
        run('git', '-C', NEW, 'config', '-f', '.gitmodules', f'submodule.{name}.path', name)
        run('git', '-C', NEW, 'config', '-f', '.gitmodules', f'submodule.{name}.url', url)
        run('git', '-C', NEW, 'add', name)
    run('git', '-C', NEW, 'add', '.gitmodules')
    run('git', '-C', NEW, 'commit', '-m', 'platform: import pinned Linux-only IOMMU platform')
    print('Independent platform imported:', NEW, flush=True)

if __name__ == '__main__':
    main()
