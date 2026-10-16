#!/usr/bin/env python3
"""Capture final patched SOFTWARE build; does not build/run/load anything."""
import datetime
import difflib
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

P = Path(__file__).resolve().parent.parent
ROOT = P.parent.parent


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write(path, value):
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False) + '\n')


stamp = datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%SZ')
patch_dir = P / 'patches'
kernel_dir = P / 'kernel'
manifest = json.loads((patch_dir / 'source-manifest.json').read_text())
patches = {'linux-a2.patch': [], 'platform-a2.patch': []}
for directory, names in (
    (patch_dir, ['source-manifest.json', 'linux-a2.patch', 'platform-a2.patch',
                 'patch-checks.json', 'sha256.json']),
    (kernel_dir, ['build-lock.json', 'final-output-manifest.json',
                  'linux-full-working-tree.patch', 'current-build-inputs.json']),
):
    archive = directory / ('before-final-mlock-' + stamp)
    archive.mkdir()
    for name in names:
        shutil.copy2(directory / name, archive / name)

for entry in manifest:
    source = ROOT / entry['path']
    entry['after_sha256'] = digest(source)
    linux = entry['path'].startswith('linux/')
    rel = entry['path'][6:] if linux else entry['path']
    key = 'linux-a2.patch' if linux else 'platform-a2.patch'
    if source.suffix == '.bin':
        proc = subprocess.run(['git', 'diff', '--no-index', '--binary', '--',
                               '/dev/null', str(source)], capture_output=True)
        assert proc.returncode == 1, proc.stderr
        patch = proc.stdout.decode().replace('a' + str(source), 'a/' + rel)
        patches[key].append(patch.replace('b' + str(source), 'b/' + rel))
        continue
    before = (Path(entry['before_source']).read_text()
              if entry['before_sha256'] else '')
    header = ['diff --git a/' + rel + ' b/' + rel + '\n']
    if entry['before_sha256'] is None:
        header.append('new file mode 100644\n')
    patches[key].append(''.join(header) + ''.join(difflib.unified_diff(
        before.splitlines(True), source.read_text().splitlines(True),
        fromfile='a/' + rel if entry['before_sha256'] else '/dev/null',
        tofile='b/' + rel)))
write(patch_dir / 'source-manifest.json', manifest)
checks = []
for name, parts in patches.items():
    target = patch_dir / name
    target.write_text(''.join(parts))
    cwd = ROOT / 'linux' if name.startswith('linux') else ROOT
    argv = ['git', 'apply', '--check', '--reverse', str(target)]
    proc = subprocess.run(argv, cwd=cwd, text=True, capture_output=True)
    checks.append(dict(argv=argv, cwd=str(cwd), exit_code=proc.returncode,
                       stderr=proc.stderr))
    assert proc.returncode == 0, proc.stderr
write(patch_dir / 'patch-checks.json', checks)
write(patch_dir / 'sha256.json', {n: digest(patch_dir / n) for n in patches})

lock = json.loads((kernel_dir / 'build-lock.json').read_text())
lock['outputs'] = [dict(path=e['path'], bytes=(ROOT / e['path']).stat().st_size,
                        sha256=digest(ROOT / e['path'])) for e in lock['outputs']]
lock['sources'] = manifest
lock['kernel_commands'] += ['mlock-driver-build.json', 'mlock-rootfs-build.json',
                            'mlock-image-build.json']
lock['profile']['borrowed_mm_cpu_PTE_stability'] = (
    'mlock the same8 pages before initialization, VM_LOCKED required; '
    'LRU_GEN/IDLE_PAGE_TRACKING/THP/KSM/NUMA_BALANCING/DAMON builds rejected; '
    'no external proc clear_refs/ptrace/mm writer. Ordinary independent '
    'I/O page tables do not inherit these extra restrictions or mlock cost.')
overlay = subprocess.run(['git', 'diff', '--binary', 'HEAD'], cwd=ROOT / 'linux',
                         check=True, stdout=subprocess.PIPE).stdout
(kernel_dir / 'linux-full-working-tree.patch').write_bytes(overlay)
lock['Linux']['overlay_sha256'] = digest(kernel_dir / 'linux-full-working-tree.patch')
for rel in lock['Linux']['untracked_files']:
    target = kernel_dir / 'linux-untracked' / rel
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(ROOT / 'linux' / rel, target)
lock['Linux']['untracked_manifest'] = [
    dict(path=f, sha256=digest(kernel_dir / 'linux-untracked' / f))
    for f in lock['Linux']['untracked_files']]
for copy in lock['final_artifact_copies']:
    target = P / copy['path']
    original = next(ROOT / e['path'] for e in lock['outputs']
                    if Path(e['path']).name == target.name)
    shutil.copy2(original, target)
    copy['sha256'] = digest(target)
write(kernel_dir / 'build-lock.json', lock)
write(kernel_dir / 'final-output-manifest.json', lock['outputs'])
write(kernel_dir / 'current-build-inputs.json', dict(
    schema='a2-final-build-inputs-v1',
    sources=[dict(path=e['path'], sha256=e['after_sha256'],
                  bytes=(ROOT / e['path']).stat().st_size) for e in manifest],
    config_sha256=digest(ROOT / 'out/linux/.config'),
    outputs='final-output-manifest.json'))
print(json.dumps(dict(sources=len(manifest), reverse_checks=2,
                     image_sha256=digest(ROOT / 'out/linux/arch/riscv/boot/Image'))))
