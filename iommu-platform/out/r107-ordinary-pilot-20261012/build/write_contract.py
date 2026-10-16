#!/usr/bin/env python3
"""Record prepared, uncompiled staging contract; no compiler invocation."""
from pathlib import Path
import hashlib
import json
import os
import stat

STAGE = Path('/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012')
BUILD, SOURCE = STAGE / 'build', STAGE / 'source'
PARENT = Path('/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/linux-source')


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


directories = []
for directory, names, _ in os.walk(PARENT, followlinks=False):
    for name in names[:]:
        p = Path(directory) / name
        if p.is_symlink():
            names.remove(name)
            continue
        relative = p.relative_to(PARENT)
        mode = stat.S_IMODE(p.stat().st_mode)
        assert mode == stat.S_IMODE((SOURCE / 'linux' / relative).stat().st_mode)
        directories.append({'path': str(relative), 'mode': oct(mode)})
root_mode = stat.S_IMODE(PARENT.stat().st_mode)
assert root_mode == stat.S_IMODE((SOURCE / 'linux').stat().st_mode)
(BUILD / 'linux-directory-mode-preservation.json').write_text(json.dumps({
    'root_mode': oct(root_mode), 'directory_count': len(directories),
    'source_and_copy_modes_verified': True, 'directories': directories}, indent=2) + '\n')

contract = {
    'status': 'prepared_not_compiled', 'stage': 'stage111',
    'source_edit_ownership': 'root exclusively owns ordinary driver/UAPI changes after the initial Linux freeze',
    'linux_parent_source': str(PARENT), 'linux_source': str(SOURCE / 'linux'),
    'linux_entries': 81122,
    'copy_policy': 'independent copy2 with symlinks; all old/new file hashes, sizes, modes and targets matched complete parent manifest; no hardlinks; all directory modes checked',
    'opensbi_freeze': 'read-only e8d21bfd base archive plus independent exact-byte copy of sole platform.c tracked overlay; binary patch retained',
    'opensbi_overlay_sha256': '7f87031601f04345456cb78ac5d6364c5eade71dd9e49e6d87c344f10397b659',
    'initramfs_policy': 'same exact frozen rootfs.cpio; never repack or build seven historical benchmarks',
    'config_baseline_sha256': 'b9af38b79a9f0d7c5348bb70ea4641ac0f7e21a4e44119c63a1e653639c2933d',
    'initial_config_changes': ['CONFIG_INITRAMFS_SOURCE=frozen exact old rootfs', 'CONFIG_LOCALVERSION=-r107-ordinary-stage111', 'CONFIG_LOCALVERSION_AUTO=n'],
    'additional_config_symbols': 'only exact root-approved symbols; complete olddefconfig delta checked',
    'review_gate': str(BUILD / 'reviewed-build.json'),
    'review_gate_workflow': [
        'root completes and directly reviews ordinary driver/UAPI changes',
        'python3 build/build_once.py --inspect writes full source manifest and exact review proposal; no compiler',
        'root copies the proposal to reviewed-build.json with status=reviewed-build and reviewed_by=root, retaining exact source/config digest fields',
        'only after root reviewed-build notification: python3 build/build_once.py'],
    'compile_commands_not_executed': [
        'make -C source/linux O=build/linux ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- olddefconfig',
        'make -C source/linux O=build/linux ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- -j12 Image',
        'make -C build/lloader ARCH=rv64 CROSS_COMPILE=riscv64-unknown-elf- TARGET=linux-rv64-cva6 IMAGE=new-Image DTB=frozen-platform.dtb -j12',
        'make -C source/opensbi O=build/opensbi CROSS_COMPILE=riscv64-linux-gnu- PLATFORM=fpga/ariane FW_TEXT_START=0x80000000 FW_PAYLOAD=y FW_PAYLOAD_OFFSET=0x200000 FW_PAYLOAD_PATH=new-linux-rv64-cva6.bin -j12'],
    'output_policy': 'new empty build/linux,build/lloader,build/opensbi; no old objects; wrapper refuses nonempty output or repeated attempt',
    'build_environment': {
        'LC_ALL': 'C', 'TZ': 'Asia/Tokyo', 'KBUILD_BUILD_USER': 'xvhaocheng',
        'KBUILD_BUILD_HOST': 'dell', 'KBUILD_BUILD_VERSION': '1',
        'KBUILD_BUILD_TIMESTAMP': 'assigned once at actual first authorized build start before olddefconfig; never Oct6'},
    'artifact_acceptance': [
        'new Image IKCONFIG exact bytes equal final .config',
        'release ends with -r107-ordinary-stage111',
        'full approved frozen sources and inputs unchanged during build',
        'Image/vmlinux/System.map/config/release/notes/loader/payload hashes recorded',
        'OpenSBI fw_payload.bin <=33554432 bytes'],
    'metadata_sha256': {name: sha(BUILD / name) for name in (
        'linux-parent-frozen-source-manifest.json', 'linux-baseline-source-manifest.json',
        'opensbi-frozen-source-manifest.json', 'lloader-frozen-source-manifest.json',
        'frozen-input-manifest.json', 'toolchain-fingerprints.json', 'build_once.py')},
    'kernel_builds': 0, 'firmware_builds': 0, 'device_actions': 0, 'network_actions': 0,
    'production_source_or_output_writes': 0, 'old_stage110_writes': 0,
}
(BUILD / 'build-contract.json').write_text(json.dumps(contract, indent=2) + '\n')
print(json.dumps({'status': contract['status'], 'linux_directory_modes_verified': len(directories),
                  'build_contract': str(BUILD / 'build-contract.json'), 'wrapper_sha256': sha(BUILD / 'build_once.py')}))
