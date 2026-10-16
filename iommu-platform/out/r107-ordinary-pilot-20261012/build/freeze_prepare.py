#!/usr/bin/env python3
"""One-time stage111 source/input freeze. Never invokes a compiler."""
import difflib
import hashlib
import json
import os
from pathlib import Path
import shutil
import stat
import subprocess
import tarfile
import time
from datetime import datetime, timezone

PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
STAGE = PLATFORM / 'out/r107-ordinary-pilot-20261012'
SOURCE, BUILD = STAGE / 'source', STAGE / 'build'
OLD = PLATFORM / 'out/r107-platform-identity-recovery-20261012/kernel'
REPRO = OLD / 'reproduction'
BASE_LINUX = REPRO / 'linux-source'
BASE_MANIFEST = REPRO / 'frozen-source-manifest.json'
OPEN_PROD = PLATFORM / 'opensbi'
EXPECTED = {
    'rootfs.cpio': 'a3075bc621c712bb78ad8234576700988149f057caff3aeffbc98fb8c8f8a705',
    'Image-embedded.config': 'b9af38b79a9f0d7c5348bb70ea4641ac0f7e21a4e44119c63a1e653639c2933d',
    'platform.dtb': 'aa9cdf4cb28700c83374205caca5ac0085a527f03aaeae49f7e19e8d2d3d90af',
}
OPEN_COMMIT = 'e8d21bfdc2dd753314627968e496c3a1188f6dfb'
OPEN_OVERLAY = 'platform/fpga/ariane/platform.c'
OPEN_OVERLAY_SHA = '7f87031601f04345456cb78ac5d6364c5eade71dd9e49e6d87c344f10397b659'
ledger = []


def now():
    return datetime.now(timezone.utc).isoformat()


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def save(name, data):
    (BUILD / name).write_text(json.dumps(data, indent=2) + '\n')


def command(argv, cwd, stdout_file=None):
    start, tick = now(), time.monotonic()
    p = subprocess.run(argv, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                       env=dict(os.environ, LC_ALL='C', TZ='Asia/Tokyo'))
    if stdout_file:
        stdout_file.write_bytes(p.stdout)
    ledger.append({'argv': argv, 'cwd': str(cwd), 'started_at_utc': start,
                   'finished_at_utc': now(), 'wall_seconds': time.monotonic() - tick,
                   'exit_code': p.returncode,
                   'stdout_path': str(stdout_file) if stdout_file else None,
                   'stdout': None if stdout_file else p.stdout.decode(errors='replace'),
                   'stderr': p.stderr.decode(errors='replace')})
    save('freeze-command-ledger.json', ledger)
    if p.returncode:
        raise RuntimeError(f'command failed: {argv!r}; {p.stderr.decode(errors="replace")}')
    return p.stdout


def entry(root, relative):
    p = root / relative
    st = p.lstat()
    if stat.S_ISLNK(st.st_mode):
        return {'path': relative, 'kind': 'symlink', 'target': os.readlink(p)}
    if not stat.S_ISREG(st.st_mode):
        raise RuntimeError(f'unexpected file kind: {p}')
    return {'path': relative, 'kind': 'file', 'sha256': sha(p),
            'bytes': st.st_size, 'mode': oct(stat.S_IMODE(st.st_mode))}


def paths(root):
    found = []
    for directory, names, files in os.walk(root, followlinks=False):
        for n in names[:]:
            p = Path(directory) / n
            if p.is_symlink():
                found.append(p.relative_to(root).as_posix())
                names.remove(n)
        found.extend((Path(directory) / n).relative_to(root).as_posix() for n in files)
    return sorted(found)


def main():
    begin, tick = now(), time.monotonic()
    if any(SOURCE.iterdir()):
        raise RuntimeError('source directory is not empty; refuse overwriting')
    frozen = json.loads(BASE_MANIFEST.read_text())
    assert frozen['base_commit'] == 'c0f5e13ea81fa30d78f3bb1d6389e2397321e040'
    assert len(frozen['files']) == 81122
    expected_paths = sorted(x['path'] for x in frozen['files'])
    if paths(BASE_LINUX) != expected_paths:
        raise RuntimeError('old frozen Linux inventory differs from complete manifest')
    save('freeze-state.json', {'status': 'copying', 'started_at_utc': begin,
                              'kernel_compiler_invoked': False, 'firmware_compiler_invoked': False})
    shutil.copy2(BASE_MANIFEST, BUILD / 'linux-parent-frozen-source-manifest.json')
    linux = SOURCE / 'linux'
    shutil.copytree(BASE_LINUX, linux, symlinks=True, copy_function=shutil.copy2)
    if paths(linux) != expected_paths:
        raise RuntimeError('new frozen Linux inventory differs from complete manifest')
    actual = []
    for old in frozen['files']:
        rel = old['path']
        source_entry, dest_entry = entry(BASE_LINUX, rel), entry(linux, rel)
        if source_entry != old or dest_entry != old:
            raise RuntimeError(f'Linux source hash/mode/target mismatch: {rel}')
        if old['kind'] == 'file':
            source_stat, dest_stat = (BASE_LINUX / rel).stat(), (linux / rel).stat()
            if dest_stat.st_nlink != 1 or (source_stat.st_dev, source_stat.st_ino) == (dest_stat.st_dev, dest_stat.st_ino):
                raise RuntimeError(f'Linux destination is not independent: {rel}')
        actual.append(dest_entry)
    linux_manifest = {'observed_at_utc': now(), 'parent_manifest_path': str(BASE_MANIFEST),
                      'parent_manifest_sha256': sha(BASE_MANIFEST),
                      'base_commit': frozen['base_commit'],
                      'base_archive_sha256': frozen['base_archive_sha256'],
                      'tracked_patch_sha256': frozen['tracked_patch_sha256'],
                      'overlays': frozen['overlays'], 'files': actual}
    save('linux-baseline-source-manifest.json', linux_manifest)

    inputs = SOURCE / 'frozen-inputs'
    inputs.mkdir()
    input_origins = {'rootfs.cpio': REPRO / 'frozen-inputs/rootfs.cpio',
                     'Image-embedded.config': OLD / 'Image-embedded.config',
                     'platform.dtb': PLATFORM / 'out/composition-clean-20261006/software/delivery/platform.dtb'}
    input_receipts = []
    for name, origin in input_origins.items():
        assert sha(origin) == EXPECTED[name], name
        destination = inputs / name
        shutil.copy2(origin, destination)
        assert sha(destination) == EXPECTED[name], name
        assert destination.stat().st_ino != origin.stat().st_ino or destination.stat().st_dev != origin.stat().st_dev
        input_receipts.append({'name': name, 'original': str(origin), 'frozen': str(destination),
                               'sha256': EXPECTED[name], 'bytes': destination.stat().st_size})
    baseline = (inputs / 'Image-embedded.config').read_text()
    config = baseline.replace('CONFIG_LOCALVERSION=""\n', 'CONFIG_LOCALVERSION="-r107-ordinary-stage111"\n')
    config = config.replace('CONFIG_LOCALVERSION_AUTO=y\n', '# CONFIG_LOCALVERSION_AUTO is not set\n')
    old_init = 'CONFIG_INITRAMFS_SOURCE="/home/xvhaocheng/work/iommu-platform/out/rootfs.cpio"\n'
    assert old_init in config
    config = config.replace(old_init, f'CONFIG_INITRAMFS_SOURCE="{inputs / "rootfs.cpio"}"\n')
    (inputs / 'stage111.config').write_text(config)
    (BUILD / 'config-pre-olddefconfig.diff').write_text(''.join(difflib.unified_diff(
        baseline.splitlines(keepends=True), config.splitlines(keepends=True),
        fromfile=str(inputs / 'Image-embedded.config'), tofile=str(inputs / 'stage111.config'))))

    head_before = command(['git', 'rev-parse', 'HEAD'], OPEN_PROD).decode().strip()
    status_before = command(['git', '--no-optional-locks', 'status', '--short'], OPEN_PROD).decode()
    assert head_before == OPEN_COMMIT
    changed = command(['git', 'diff', '--name-only', 'HEAD'], OPEN_PROD).decode().splitlines()
    assert changed == [OPEN_OVERLAY], changed
    assert not command(['git', 'ls-files', '--others', '--exclude-standard'], OPEN_PROD).strip()
    assert sha(OPEN_PROD / OPEN_OVERLAY) == OPEN_OVERLAY_SHA
    archive = BUILD / 'opensbi-base-e8d21bfd.tar'
    command(['git', 'archive', '--format=tar', OPEN_COMMIT], OPEN_PROD, archive)
    overlay = BUILD / 'opensbi-tracked-overlay.patch'
    command(['git', 'diff', '--binary', 'HEAD', '--', OPEN_OVERLAY], OPEN_PROD, overlay)
    opensbi = SOURCE / 'opensbi'
    opensbi.mkdir()
    with tarfile.open(archive, 'r:') as tar:
        for member in tar.getmembers():
            assert not Path(member.name).is_absolute() and '..' not in Path(member.name).parts
        tar.extractall(opensbi)
    # This export is inside a containing Git repository. git apply can silently
    # skip paths outside that repository's cwd prefix; preserve exact overlay
    # bytes instead, with the captured binary patch as provenance evidence.
    shutil.copy2(OPEN_PROD / OPEN_OVERLAY, opensbi / OPEN_OVERLAY)
    assert sha(opensbi / OPEN_OVERLAY) == OPEN_OVERLAY_SHA
    open_manifest = {'observed_at_utc': now(), 'source_repository': str(OPEN_PROD),
                     'base_commit': OPEN_COMMIT, 'archive_sha256': sha(archive),
                     'tracked_overlay_sha256': sha(overlay),
                     'overlay': {'path': OPEN_OVERLAY, 'sha256': OPEN_OVERLAY_SHA},
                     'files': [entry(opensbi, rel) for rel in paths(opensbi)]}
    save('opensbi-frozen-source-manifest.json', open_manifest)
    assert command(['git', 'rev-parse', 'HEAD'], OPEN_PROD).decode().strip() == head_before
    assert command(['git', '--no-optional-locks', 'status', '--short'], OPEN_PROD).decode() == status_before
    assert sha(OPEN_PROD / OPEN_OVERLAY) == OPEN_OVERLAY_SHA

    loader = SOURCE / 'lloader'
    loader.mkdir()
    for name in ('Makefile', 'rv64.S', 'loader_rv64.ld'):
        shutil.copy2(linux / 'tools/lloader' / name, loader / name)
    save('lloader-frozen-source-manifest.json', {'files': [entry(loader, rel) for rel in paths(loader)]})
    tools = {}
    for name in ('riscv64-linux-gnu-gcc', 'riscv64-linux-gnu-ld', 'riscv64-linux-gnu-readelf',
                 'riscv64-unknown-elf-gcc', 'riscv64-unknown-elf-objcopy', 'make', 'python3', 'gzip', 'flex', 'bison'):
        resolved = Path(shutil.which(name)).resolve()
        version = command([name, '--version'], BUILD).decode(errors='replace').splitlines()[:2]
        tools[name] = {'resolved_path': str(resolved), 'sha256': sha(resolved), 'version': version}
    save('toolchain-fingerprints.json', tools)
    save('frozen-input-manifest.json', {'inputs': input_receipts,
                                      'stage111_config': entry(inputs, 'stage111.config')})
    save('freeze-state.json', {'status': 'prepared_not_compiled', 'started_at_utc': begin,
                              'finished_at_utc': now(), 'wall_seconds': time.monotonic() - tick,
                              'linux_entries': len(actual),
                              'linux_parent_and_copy_all_entries_verified': True,
                              'all_regular_files_independent_no_hardlinks': True,
                              'symlinks_and_file_modes_preserved': True,
                              'linux_source': str(linux), 'opensbi_source': str(opensbi),
                              'inputs': str(inputs), 'kernel_compiler_invoked': False,
                              'firmware_compiler_invoked': False,
                              'kernel_build_timestamp': 'not assigned until first authorized actual build start'})
    print(json.dumps({'status': 'prepared_not_compiled', 'linux_source': str(linux),
                      'linux_entries_verified': len(actual), 'wall_seconds': time.monotonic() - tick}))


if __name__ == '__main__':
    try:
        main()
    except Exception as exc:
        save('freeze-failure.json', {'observed_at_utc': now(), 'error': repr(exc)})
        raise
