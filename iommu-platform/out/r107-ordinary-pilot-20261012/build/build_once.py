#!/usr/bin/env python3
"""Reviewed, isolated, one-attempt kernel/loader/OpenSBI build for stage111."""
import argparse
import difflib
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import stat
import subprocess
import time
from datetime import datetime, timezone
from zoneinfo import ZoneInfo

STAGE = Path('/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012')
SOURCE, BUILD = STAGE / 'source', STAGE / 'build'
INPUTS = SOURCE / 'frozen-inputs'
ALLOWED_ADAPTER_PATHS = {
    'drivers/iommu/riscv/io_pgtable.c', 'include/linux/iommu-finite.h',
    'drivers/iommu/dma-iommu.c',
    'drivers/misc/idma.c', 'drivers/misc/idma_finite.h', 'include/uapi/linux/idma.h',
}
REQUIRED_CONFIG_SYMBOLS = {'CONFIG_INITRAMFS_SOURCE', 'CONFIG_LOCALVERSION', 'CONFIG_LOCALVERSION_AUTO'}
ledger = []
build_env = None


def now():
    return datetime.now(timezone.utc).isoformat()


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':')).encode()).hexdigest()


def save(name, value):
    (BUILD / name).write_text(json.dumps(value, indent=2) + '\n')


def inventory(root):
    paths = []
    for directory, names, files in os.walk(root, followlinks=False):
        for name in names[:]:
            p = Path(directory) / name
            if p.is_symlink():
                paths.append(p.relative_to(root).as_posix())
                names.remove(name)
        paths.extend((Path(directory) / name).relative_to(root).as_posix() for name in files)
    entries = []
    for relative in sorted(paths):
        p = root / relative
        st = p.lstat()
        if stat.S_ISLNK(st.st_mode):
            entries.append({'path': relative, 'kind': 'symlink', 'target': os.readlink(p)})
        elif stat.S_ISREG(st.st_mode):
            if st.st_nlink != 1:
                raise RuntimeError(f'source has hard-linked regular file: {p}')
            entries.append({'path': relative, 'kind': 'file', 'sha256': sha(p),
                            'bytes': st.st_size, 'mode': oct(stat.S_IMODE(st.st_mode))})
        else:
            raise RuntimeError(f'unsupported source type: {p}')
    return entries


def config_values(path):
    values = {}
    for line in path.read_text().splitlines():
        match = re.match(r'^(CONFIG_[A-Za-z0-9_]+)=(.*)$', line)
        disabled = re.match(r'^# (CONFIG_[A-Za-z0-9_]+) is not set$', line)
        if match:
            values[match[1]] = match[2]
        elif disabled:
            values[disabled[1]] = 'n'
    return values


def config_changes(config):
    before, after = config_values(INPUTS / 'Image-embedded.config'), config_values(config)
    return [{'symbol': key, 'before': before.get(key), 'after': after.get(key)}
            for key in sorted(set(before) | set(after)) if before.get(key) != after.get(key)]


def inspect_sources():
    baseline = json.loads((BUILD / 'linux-baseline-source-manifest.json').read_text())['files']
    current = inventory(SOURCE / 'linux')
    old_map, new_map = {x['path']: x for x in baseline}, {x['path']: x for x in current}
    changes = [{'path': p, 'before': old_map.get(p), 'after': new_map.get(p)}
               for p in sorted(set(old_map) | set(new_map)) if old_map.get(p) != new_map.get(p)]
    if any(c['path'] not in ALLOWED_ADAPTER_PATHS for c in changes):
        raise RuntimeError('source changes exceed the root-owned adapter scope: ' +
                           ', '.join(c['path'] for c in changes if c['path'] not in ALLOWED_ADAPTER_PATHS))
    if any(c['after'] is None or c['after']['kind'] != 'file' for c in changes):
        raise RuntimeError('adapter deletion/symlink change is outside this build contract')
    for name, root in [('opensbi', SOURCE / 'opensbi'), ('lloader', SOURCE / 'lloader')]:
        baseline_manifest = json.loads((BUILD / f'{name}-frozen-source-manifest.json').read_text())
        if inventory(root) != sorted(baseline_manifest['files'], key=lambda x: x['path']):
            raise RuntimeError(f'{name} frozen source was modified')
    frozen_inputs = json.loads((BUILD / 'frozen-input-manifest.json').read_text())
    for x in frozen_inputs['inputs']:
        p = Path(x['frozen'])
        if p.stat().st_size != x['bytes'] or sha(p) != x['sha256']:
            raise RuntimeError(f'frozen input changed: {p}')
    cfg = INPUTS / 'stage111.config'
    changes_cfg = config_changes(cfg)
    values = config_values(cfg)
    expected = {'CONFIG_INITRAMFS_SOURCE': f'"{INPUTS / "rootfs.cpio"}"',
                'CONFIG_LOCALVERSION': '"-r107-ordinary-stage111"', 'CONFIG_LOCALVERSION_AUTO': 'n'}
    for symbol, value in expected.items():
        if values.get(symbol) != value:
            raise RuntimeError(f'config contract mismatch: {symbol}')
    review = {'status': 'pending_root_review', 'reviewed_by': None,
              'approved_source_files_digest': digest(current),
              'approved_source_changes': changes,
              'config_sha256': sha(cfg), 'approved_config_changes': changes_cfg,
              'additional_approved_config_symbols': sorted(x['symbol'] for x in changes_cfg
                                                         if x['symbol'] not in REQUIRED_CONFIG_SYMBOLS),
              'source_entries': len(current),
              'build_intent': 'one_clean_kernel_one_loader_one_OpenSBI_no_old_benchmarks_no_devices'}
    save('linux-reviewed-source-manifest.json', {'observed_at_utc': now(), 'files_digest': digest(current), 'files': current})
    save('linux-source-delta.json', {'changes': changes})
    save('config-approved-input-delta.json', {'changes': changes_cfg})
    (BUILD / 'config-approved-input.diff').write_text(''.join(difflib.unified_diff(
        (INPUTS / 'Image-embedded.config').read_text().splitlines(keepends=True),
        cfg.read_text().splitlines(keepends=True), fromfile='baseline/Image-embedded.config',
        tofile='frozen-inputs/stage111.config')))
    save('source-review-proposal.json', review)
    return review, current


def run(name, argv, cwd):
    start, tick = now(), time.monotonic()
    stdout, stderr = BUILD / f'{name}.stdout.log', BUILD / f'{name}.stderr.log'
    with stdout.open('wb') as out, stderr.open('wb') as err:
        p = subprocess.run(argv, cwd=cwd, env=build_env, stdout=out, stderr=err)
    item = {'name': name, 'argv': argv, 'cwd': str(cwd), 'started_at_utc': start,
            'finished_at_utc': now(), 'wall_seconds': time.monotonic() - tick,
            'exit_code': p.returncode, 'stdout_path': str(stdout), 'stderr_path': str(stderr),
            'build_environment': {k: build_env[k] for k in (
                'LC_ALL', 'TZ', 'KBUILD_BUILD_TIMESTAMP', 'KBUILD_BUILD_USER',
                'KBUILD_BUILD_HOST', 'KBUILD_BUILD_VERSION')}}
    ledger.append(item)
    save('actual-command-ledger.json', ledger)
    if p.returncode:
        raise RuntimeError(f'{name} failed with exit {p.returncode}; see {stderr}')
    return stdout


def clean_output(name):
    p = BUILD / name
    if p.exists():
        if any(p.iterdir()):
            raise RuntimeError(f'output is not empty: {p}; refuse incremental/repeated build')
    else:
        p.mkdir()
    return p


def main():
    global build_env
    parser = argparse.ArgumentParser()
    parser.add_argument('--inspect', action='store_true', help='hash inputs and write root review proposal; no compiler')
    args = parser.parse_args()
    if args.inspect:
        review, _ = inspect_sources()
        print(json.dumps({'status': 'pending_root_review', 'source_entries': review['source_entries'],
                          'source_changes': [x['path'] for x in review['approved_source_changes']],
                          'proposal': str(BUILD / 'source-review-proposal.json')}))
        return
    gate_path = BUILD / 'reviewed-build.json'
    if not gate_path.is_file():
        raise RuntimeError('root reviewed-build gate is absent; compilation is not authorized')
    gate = json.loads(gate_path.read_text())
    if gate.get('status') != 'reviewed-build' or gate.get('reviewed_by') != 'root':
        raise RuntimeError('root reviewed-build gate is not accepted')
    if (BUILD / 'execution-state.json').exists():
        raise RuntimeError('an actual build was already attempted; no automatic retry is permitted')
    review, source_before = inspect_sources()
    for k in ('approved_source_files_digest', 'approved_source_changes', 'config_sha256',
              'approved_config_changes', 'additional_approved_config_symbols'):
        if review[k] != gate.get(k):
            raise RuntimeError(f'source/config no longer matches root approval: {k}')
    tools = json.loads((BUILD / 'toolchain-fingerprints.json').read_text())
    for name, record in tools.items():
        binary = Path(shutil.which(name)).resolve()
        if str(binary) != record['resolved_path'] or sha(binary) != record['sha256']:
            raise RuntimeError(f'tool fingerprint changed: {name}')
    kernel, loader, opensbi = [clean_output(x) for x in ('linux', 'lloader', 'opensbi')]
    start, tick = datetime.now(timezone.utc), time.monotonic()
    local = start.astimezone(ZoneInfo('Asia/Tokyo'))
    selected_env = {'LC_ALL': 'C', 'TZ': 'Asia/Tokyo',
                    'KBUILD_BUILD_TIMESTAMP': local.strftime('%a %b %d %H:%M:%S %Z %Y'),
                    'KBUILD_BUILD_USER': 'xvhaocheng', 'KBUILD_BUILD_HOST': 'dell',
                    'KBUILD_BUILD_VERSION': '1'}
    build_env = dict(os.environ, **selected_env)
    build_env.pop('LOCALVERSION', None)
    save('build-environment.json', {'actual_first_build_started_at_utc': start.isoformat(),
                                   'actual_first_build_started_at_local': local.isoformat(),
                                   'fixed_environment': selected_env,
                                   'timestamp_policy': 'assigned once at this actual clean build start; not historical Oct6'})
    save('execution-state.json', {'status': 'running', 'actual_first_build_started_at_utc': start.isoformat(),
                                  'root_gate_sha256': sha(gate_path), 'clean_output_directories': [str(x) for x in (kernel, loader, opensbi)]})
    shutil.copy2(INPUTS / 'stage111.config', kernel / '.config')
    common = ['make', '-C', str(SOURCE / 'linux'), f'O={kernel}', 'ARCH=riscv', 'CROSS_COMPILE=riscv64-linux-gnu-']
    run('kernel-olddefconfig', common + ['olddefconfig'], BUILD)
    final_config = kernel / '.config'
    (BUILD / 'config-after-olddefconfig.diff').write_text(''.join(difflib.unified_diff(
        (INPUTS / 'Image-embedded.config').read_text().splitlines(keepends=True),
        final_config.read_text().splitlines(keepends=True), fromfile='baseline/Image-embedded.config',
        tofile='build/linux/.config')))
    save('config-after-olddefconfig-delta.json', {'changes': config_changes(final_config)})
    if config_changes(final_config) != gate['approved_config_changes']:
        raise RuntimeError('olddefconfig changed symbols beyond the exact root-approved config delta')
    final_config_before_build = sha(final_config)
    run('kernel-Image', common + ['-j12', 'Image'], BUILD)
    image = kernel / 'arch/riscv/boot/Image'
    if sha(final_config) != final_config_before_build:
        raise RuntimeError('kernel build changed the reviewed final .config')
    embedded = run('kernel-extract-ikconfig', [str(SOURCE / 'linux/scripts/extract-ikconfig'), str(image)], BUILD)
    embedded_config = BUILD / 'Image-embedded.config'
    shutil.copy2(embedded, embedded_config)
    if embedded_config.read_bytes() != final_config.read_bytes():
        raise RuntimeError('new Image IKCONFIG bytes do not equal the final frozen .config')
    release = (kernel / 'include/config/kernel.release').read_text().strip()
    if not release.endswith('-r107-ordinary-stage111'):
        raise RuntimeError(f'new release has an unexpected suffix: {release}')
    run('kernel-ELF-notes', ['riscv64-linux-gnu-readelf', '-n', str(kernel / 'vmlinux')], BUILD)
    for name in ('Makefile', 'rv64.S', 'loader_rv64.ld'):
        shutil.copy2(SOURCE / 'lloader' / name, loader / name)
    run('lloader', ['make', '-C', str(loader), 'ARCH=rv64', 'CROSS_COMPILE=riscv64-unknown-elf-',
                    'TARGET=linux-rv64-cva6', f'IMAGE={image}', f'DTB={INPUTS / "platform.dtb"}', '-j12'], BUILD)
    loader_bin = loader / 'linux-rv64-cva6.bin'
    run('opensbi', ['make', '-C', str(SOURCE / 'opensbi'), f'O={opensbi}',
                    'CROSS_COMPILE=riscv64-linux-gnu-', 'PLATFORM=fpga/ariane',
                    'FW_TEXT_START=0x80000000', 'FW_PAYLOAD=y', 'FW_PAYLOAD_OFFSET=0x200000',
                    f'FW_PAYLOAD_PATH={loader_bin}', '-j12'], BUILD)
    firmware = opensbi / 'platform/fpga/ariane/firmware/fw_payload.bin'
    firmware_elf = firmware.with_suffix('.elf')
    if firmware.stat().st_size > 33554432:
        raise RuntimeError(f'payload exceeds 32MiB: {firmware.stat().st_size}')
    run('opensbi-ELF-notes', ['riscv64-linux-gnu-readelf', '-n', str(firmware_elf)], BUILD)
    after_review, source_after = inspect_sources()
    if source_after != source_before or after_review['config_sha256'] != gate['config_sha256']:
        raise RuntimeError('frozen source/input config changed during compilation')
    artifacts = [image, kernel / 'vmlinux', kernel / 'System.map', final_config,
                 embedded_config, kernel / 'include/config/kernel.release',
                 kernel / 'include/generated/utsrelease.h', kernel / 'include/generated/compile.h',
                 loader_bin, loader / 'linux-rv64-cva6.elf', firmware, firmware_elf,
                 INPUTS / 'rootfs.cpio', INPUTS / 'platform.dtb']
    save('artifact-manifest.json', {'observed_at_utc': now(), 'kernel_release': release,
                                  'kernel_source_files_digest': digest(source_before),
                                  'artifacts': [{'path': str(p), 'sha256': sha(p), 'bytes': p.stat().st_size} for p in artifacts],
                                  'payload_bytes': firmware.stat().st_size,
                                  'payload_limit_bytes': 33554432,
                                  'payload_headroom_bytes': 33554432 - firmware.stat().st_size,
                                  'image_ikconfig_equals_final_config': True,
                                  'frozen_source_and_inputs_unchanged_during_build': True})
    save('execution-state.json', {'status': 'built_not_deployed', 'actual_first_build_started_at_utc': start.isoformat(),
                                  'finished_at_utc': now(), 'wall_seconds': time.monotonic() - tick,
                                  'root_gate_sha256': sha(gate_path), 'kernel_release': release,
                                  'payload': str(firmware), 'payload_bytes': firmware.stat().st_size,
                                  'device_actions': 0, 'network_actions': 0, 'old_benchmark_builds': 0})
    print(json.dumps({'status': 'built_not_deployed', 'kernel_release': release,
                      'payload': str(firmware), 'payload_bytes': firmware.stat().st_size,
                      'wall_seconds': time.monotonic() - tick}))


if __name__ == '__main__':
    try:
        main()
    except Exception as exc:
        save('build-failure.json', {'observed_at_utc': now(), 'error': repr(exc),
                                   'automatic_retry': False, 'device_actions': 0})
        raise
