#!/usr/bin/env python3
'''Finalize P4M delivery only after verified build-status and validation.'''
from __future__ import annotations

import hashlib
import json
import os
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'out' / 'p4m-baseline-20260920'
DELIVERY = OUT / 'delivery'
PROFILE = ROOT / 'configs' / 'migration-4dma-p4m.json'
LOCK = ROOT / 'sources.lock.json'
BUILD_STATUS = OUT / 'build-status.json'
VALIDATION = OUT / 'validation.json'
SOFTWARE_MANIFEST = ROOT / 'out' / 'manifests' / 'software-migration-4dma-p4m.json'
FPGA_MANIFEST = ROOT / 'out' / 'manifests' / 'fpga-migration-4dma-p4m.json'
BIT = ROOT / 'out' / 'migration-4dma-p4m' / 'fpga-src' / 'cva6' / 'corev_apu' / 'fpga' / 'work-fpga' / 'ariane_xilinx.bit'
MCS = BIT.with_suffix('.mcs')
PAYLOAD = ROOT / 'out' / 'opensbi' / 'platform' / 'fpga' / 'ariane' / 'firmware' / 'fw_payload.bin'
IMAGE = ROOT / 'out' / 'linux' / 'arch' / 'riscv' / 'boot' / 'Image'
DTB = ROOT / 'out' / 'linux' / 'platform.dtb'
ROOTFS = ROOT / 'out' / 'rootfs.cpio'
IDMA_H = ROOT / 'linux' / 'include' / 'uapi' / 'linux' / 'idma.h'
P4M_BASELINE = ROOT / 'P4M_BASELINE.md'
KERNEL_RELEASE_FILE = ROOT / 'out' / 'linux' / 'include' / 'config' / 'kernel.release'
# Exact host paths: no candidate list, no stale-binary fallback.
IDMA_EXPERIMENT = ROOT / 'out' / 'bench' / 'idma_experiment'
PLATFORM_P4M = ROOT / 'tests' / 'board-p4m.sh'
EXPORTED = ROOT / 'out' / 'migration-4dma-p4m' / 'fpga-src'
BUILDER_MODIFIED = {
    'cva6/corev_apu/fpga/src/ariane_xilinx.sv',
    'cva6/corev_apu/tb/ariane_soc_pkg.sv',
}
REQUIRED_SOFTWARE = {
    'rootfs.cpio': ROOTFS,
    'Image': IMAGE,
    'platform.dtb': DTB,
    'fw_payload.bin': PAYLOAD,
}
LOCK_COMPONENTS = (
    ('cva6', 'cva6_commit'),
    ('linux', 'linux_commit'),
    ('cva6/corev_apu/riscv-iommu', 'iommu_commit'),
)
BASELINE_LINK = 'out/p4m-baseline-20260920/build-status.json'
DELIVERY_LINK = 'build-status.json'
NL = chr(10)


def fail(message):
    raise SystemExit('finalize_delivery: ' + message)


def load_json(path):
    if not path.is_file():
        fail('missing JSON: ' + str(path))
    with path.open('r', encoding='utf-8') as f:
        return json.load(f)


def sha256(path):
    if not path.is_file() or path.stat().st_size == 0:
        fail('missing or empty file: ' + str(path))
    h = hashlib.sha256()
    with path.open('rb') as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()


def require_passed(label, entry):
    if not isinstance(entry, dict):
        fail('build-status missing object: ' + label)
    if entry.get('status') != 'passed' or entry.get('exit_code') != 0:
        fail(label + ' is not passed/exit_code=0')


def require_manifest(path, action):
    manifest = load_json(path)
    if manifest.get('status') != 'passed' or manifest.get('action') != action:
        fail(str(path) + ' is not a passed ' + action + ' manifest')
    return manifest


def check_profile(manifest, profile):
    got = manifest.get('profile')
    if not isinstance(got, dict):
        fail('manifest profile missing')
    for key, value in profile.items():
        if got.get(key) != value:
            fail('manifest profile mismatch: ' + key)


def artifact_entry(manifest, rel):
    artifacts = manifest.get('artifacts')
    if not isinstance(artifacts, dict):
        fail('manifest artifacts missing')
    if rel not in artifacts:
        fail('manifest lacks artifact ' + rel)
    return rel, artifacts[rel]


def check_artifact(manifest, path):
    rel = str(path.relative_to(ROOT))
    key, expected = artifact_entry(manifest, rel)
    actual = sha256(path)
    if actual != expected:
        fail('artifact hash mismatch for ' + rel)
    return {'manifest_key': key, 'sha256': actual, 'bytes': path.stat().st_size}


def lock_commit(lock, component):
    components = lock.get('components')
    if not isinstance(components, list):
        fail('sources.lock components missing or not a list')
    for entry in components:
        if isinstance(entry, dict) and entry.get('path') == component:
            commit = entry.get('commit')
            if not isinstance(commit, str) or not commit:
                fail('sources.lock commit missing for ' + component)
            return commit
    fail('sources.lock lacks component ' + component)


def read_kernel_release():
    if not KERNEL_RELEASE_FILE.is_file() or KERNEL_RELEASE_FILE.stat().st_size == 0:
        fail('missing or empty ' + str(KERNEL_RELEASE_FILE))
    text = KERNEL_RELEASE_FILE.read_text(encoding='utf-8').strip()
    if not text:
        fail('empty kernel release in ' + str(KERNEL_RELEASE_FILE))
    return text


def copy_file(src, dst):
    if not src.is_file() or src.stat().st_size == 0:
        fail('missing or empty input: ' + str(src))
    src_sha = sha256(src)
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst)
    dst_sha = sha256(dst)
    if src_sha != dst_sha:
        fail('copy hash mismatch: ' + str(src) + ' -> ' + str(dst))
    return dst_sha


def write_sha256sums(base):
    lines = []
    for path in sorted(base.rglob('*')):
        if path.is_file() and path.name != 'SHA256SUMS':
            lines.append(sha256(path) + '  ' + str(path.relative_to(base)))
    sums = base / 'SHA256SUMS'
    sums.write_text(NL.join(lines) + NL, encoding='utf-8')
    for line in lines:
        digest, rel = line.split('  ', 1)
        if sha256(base / rel) != digest:
            fail('SHA256SUMS verification failed: ' + rel)


def main():
    if DELIVERY.exists():
        fail('refusing to overwrite existing ' + str(DELIVERY))
    build_status = load_json(BUILD_STATUS)
    validation = load_json(VALIDATION)
    profile = load_json(PROFILE)
    lock = load_json(LOCK)
    software_manifest = require_manifest(SOFTWARE_MANIFEST, 'software')
    fpga_manifest = require_manifest(FPGA_MANIFEST, 'fpga')
    check_profile(software_manifest, profile)
    check_profile(fpga_manifest, profile)
    require_passed('software', build_status.get('software'))
    require_passed('fpga', build_status.get('fpga'))
    board = build_status.get('board')
    if not isinstance(board, dict):
        fail('build-status missing board object')
    if board.get('status') != 'not_run':
        fail('build-status board.status must be not_run')
    if board.get('board_tested', False) is not False:
        fail('build-status board.board_tested must be false')
    if validation.get('board_tested') is not False:
        fail('validation board_tested must be false')
    checks = validation.get('checks')
    if not isinstance(checks, list) or not checks:
        fail('validation checks missing or empty')
    for check in checks:
        if not isinstance(check, dict) or check.get('exit_code') != 0:
            fail('validation check not exit_code=0')
    notes = validation.get('notes')
    if not isinstance(notes, list):
        fail('validation notes must be a list')
    sources = validation.get('sources_sha256')
    if not isinstance(sources, dict) or not sources:
        fail('validation sources_sha256 missing or empty')
    for rel, expected in sources.items():
        current = ROOT / rel
        actual = sha256(current)
        if actual != expected:
            fail('current source hash mismatch: ' + rel)
        if rel.startswith('cva6/') and rel not in BUILDER_MODIFIED:
            exported = EXPORTED / rel
            if sha256(exported) != expected:
                fail('exported CVA6 source mismatch: ' + rel)
    lock_sha = sha256(LOCK)
    profile_sha = sha256(PROFILE)
    for label, manifest in (('software', software_manifest), ('fpga', fpga_manifest)):
        if manifest.get('sources_lock_sha256') != lock_sha:
            fail(label + ' top-level sources_lock_sha256 mismatch')
        build_start = manifest.get('build_start')
        if not isinstance(build_start, dict):
            fail(label + ' build_start missing')
        if build_start.get('sources_lock_sha256') != lock_sha:
            fail(label + ' build_start sources_lock_sha256 mismatch')
        if build_start.get('profile_sha256') != profile_sha:
            fail(label + ' build_start profile_sha256 mismatch')
    versions = {}
    for component, status_key in LOCK_COMPONENTS:
        commit = lock_commit(lock, component)
        declared = build_status.get(status_key)
        if not isinstance(declared, str) or not declared:
            fail('build-status missing ' + status_key + ' for ' + component)
        if declared != commit:
            fail(status_key + ' mismatch: build-status=' + declared + ' sources.lock=' + commit)
        versions[component] = commit
    declared_release = build_status.get('kernel_release')
    if not isinstance(declared_release, str) or not declared_release.strip():
        fail('build-status missing kernel_release')
    kernel_release = read_kernel_release()
    if declared_release.strip() != kernel_release:
        fail('kernel_release mismatch: build-status=' + declared_release.strip() + ' kernel.release=' + kernel_release)
    artifacts = {}
    for label, path in REQUIRED_SOFTWARE.items():
        artifacts[label] = check_artifact(software_manifest, path)
    artifacts['ariane_xilinx.bit'] = check_artifact(fpga_manifest, BIT)
    artifacts['ariane_xilinx.mcs'] = {'manifest_key': None, 'sha256': sha256(MCS), 'bytes': MCS.stat().st_size}
    tmp = OUT / ('.delivery-tmp-' + str(os.getpid()))
    if tmp.exists():
        fail('temporary delivery path exists: ' + str(tmp))
    tmp.mkdir(parents=True)
    try:
        copy_file(BIT, tmp / 'ariane_xilinx.bit')
        copy_file(MCS, tmp / 'ariane_xilinx.mcs')
        copy_file(PAYLOAD, tmp / 'fw_payload.bin')
        copy_file(IMAGE, tmp / 'Image')
        copy_file(DTB, tmp / 'platform.dtb')
        copy_file(ROOTFS, tmp / 'rootfs.cpio')
        copy_file(IDMA_EXPERIMENT, tmp / 'idma_experiment')
        copy_file(PLATFORM_P4M, tmp / 'board-p4m.sh')
        copy_file(IDMA_H, tmp / 'idma.h')
        copy_file(LOCK, tmp / 'sources.lock.json')
        copy_file(PROFILE, tmp / PROFILE.name)
        copy_file(VALIDATION, tmp / 'validation.json')
        copy_file(BUILD_STATUS, tmp / 'build-status.json')
        copy_file(SOFTWARE_MANIFEST, tmp / 'manifests' / SOFTWARE_MANIFEST.name)
        copy_file(FPGA_MANIFEST, tmp / 'manifests' / FPGA_MANIFEST.name)
        if not P4M_BASELINE.is_file() or P4M_BASELINE.stat().st_size == 0:
            fail('missing or empty input: ' + str(P4M_BASELINE))
        baseline_text = P4M_BASELINE.read_text(encoding='utf-8')
        if BASELINE_LINK not in baseline_text:
            fail('baseline doc does not reference ' + BASELINE_LINK)
        baseline_text = baseline_text.replace(BASELINE_LINK, DELIVERY_LINK)
        baseline_dst = tmp / 'P4M_BASELINE.md'
        baseline_dst.write_text(baseline_text, encoding='utf-8')
        if not baseline_dst.is_file() or baseline_dst.stat().st_size == 0:
            fail('failed to write delivered P4M_BASELINE.md')
        copied_sha = {}
        for path in sorted(tmp.rglob('*')):
            if path.is_file() and path.name != 'SHA256SUMS':
                copied_sha[str(path.relative_to(tmp))] = sha256(path)
        delivery = {
            'schema': 'idma.p4m.delivery.v1',
            'profile': profile,
            'kernel_release': kernel_release,
            'versions': versions,
            'artifacts': artifacts,
            'copied_sha256': copied_sha,
            'build_status': build_status,
            'validation': validation,
            'manifests': {
                'software': software_manifest,
                'fpga': fpga_manifest,
            },
        }
        (tmp / 'delivery.json').write_text(json.dumps(delivery, indent=2, sort_keys=True) + NL, encoding='utf-8')
        readme = NL.join([
            '# P4M 基线交付',
            '',
            '本交付只在 build-status 的 software/fpga 均为 passed、exit_code=0，board 为 not_run，且 validation 检查通过后由 finalize_delivery.py 生成；板上未运行，不声明板卡通过。',
            '',
            '- profile: `migration-4dma-p4m`',
            '- kernel: `' + kernel_release + '`',
            '- BIT: `ariane_xilinx.bit` SHA256 `' + artifacts['ariane_xilinx.bit']['sha256'] + '`',
            '- MCS: `ariane_xilinx.mcs` SHA256 `' + artifacts['ariane_xilinx.mcs']['sha256'] + '`（标准 manifest 只列 BIT，MCS 单独记录）',
            '- payload: `fw_payload.bin` SHA256 `' + artifacts['fw_payload.bin']['sha256'] + '`',
            '- Image: `Image` SHA256 `' + artifacts['Image']['sha256'] + '`',
            '- dtb: `platform.dtb` SHA256 `' + artifacts['platform.dtb']['sha256'] + '`',
            '- rootfs: `rootfs.cpio` SHA256 `' + artifacts['rootfs.cpio']['sha256'] + '`',
            '',
            '必须同时使用本目录的 BIT 与 payload；不要与其他轮次的 BIT/payload 混用。',
            '',
            '板上建议命令（本目录脚本 `board-p4m.sh`）：',
            '```',
            'board-p4m.sh short /tmp/p4m-v7-01',
            '```',
            '',
            '短验收 4 次，每次 64 transfers、warmup 8、4 KiB：Solo、A1 gap0、A8 gap0、A8 gap1000（仅工程检查，不是正式 gap 点）。脚本保留 JSON、stderr、dmesg 与 taint；错误、quarantine、timeout 或缺少命令时停止。',
            '',
            'SD 更新沿用既有原始分区流程，将 payload 写入既有 payload 分区；本目录不提供、不发明新的偏移或烧卡步骤。板上未运行；时钟/CDC/IO 审查边界沿用，无新增 timing signoff。成功构建只证明构建，主机模型不等于板卡验收。',
            '',
            '本目录包含 delivery.json、SHA256SUMS、validation.json、build-status.json、manifests、sources.lock.json、profile 与上述产物；SHA256SUMS 递归列出除自身外全部文件并已逐项复核。P4M_BASELINE.md 为交付副本，其 build-status 链接指向本目录的 build-status.json。',
        ]) + NL
        (tmp / 'README.md').write_text(readme, encoding='utf-8')
        write_sha256sums(tmp)
        os.replace(tmp, DELIVERY)
    except BaseException:
        if tmp.exists():
            shutil.rmtree(tmp)
        raise


if __name__ == '__main__':
    main()
