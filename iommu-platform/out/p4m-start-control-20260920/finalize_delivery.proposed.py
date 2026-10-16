#!/usr/bin/env python3
'''Finalize the P4M start-control delivery (software-only, reuse prior FPGA BIT).

SOFTWAREONLY change: FPGA RTL and the build profile are unchanged, so the prior
baseline deliverable's BIT/MCS are reused verbatim and no FPGA is compiled here.
Only the software payload (payload/Image/dtb/rootfs/CLI) and the new
platform-p4m-start runner are new.

The whole sources.lock is deliberately NOT required to equal the prior lock:
'linux' legitimately changed for the new payload.  Every other hardware-relevant
component must have exactly the same set and commits as the prior lock.
'''
from __future__ import annotations

import hashlib
import importlib.util
import json
import os
import shutil
import stat
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

OUT = ROOT / 'out' / 'p4m-start-control-20260920'
DELIVERY = OUT / 'delivery'
BUILD_STATUS = OUT / 'build-status.json'
VALIDATION = OUT / 'validation.json'
BASELINE = OUT / 'baseline.json'
PROTOCOL = ROOT / 'P4M_START_CONTROL.md'

PRIOR_DELIVERY_REL = 'out/p4m-baseline-20260920/delivery'
PRIOR_DELIVERY = ROOT / 'out' / 'p4m-baseline-20260920' / 'delivery'
PRIOR_DELIVERY_JSON = PRIOR_DELIVERY / 'delivery.json'
PRIOR_LOCK = PRIOR_DELIVERY / 'sources.lock.json'
PRIOR_BIT = PRIOR_DELIVERY / 'ariane_xilinx.bit'
PRIOR_MCS = PRIOR_DELIVERY / 'ariane_xilinx.mcs'
PRIOR_FPGA_MANIFEST_REL = 'manifests/fpga-migration-4dma-p4m.json'
PRIOR_FPGA_MANIFEST = PRIOR_DELIVERY / PRIOR_FPGA_MANIFEST_REL

PROFILE = ROOT / 'configs' / 'migration-4dma-p4m.json'
LOCK = ROOT / 'sources.lock.json'
SOFTWARE_MANIFEST = ROOT / 'out' / 'manifests' / 'software-migration-4dma-p4m.json'

PAYLOAD = ROOT / 'out' / 'opensbi' / 'platform' / 'fpga' / 'ariane' / 'firmware' / 'fw_payload.bin'
IMAGE = ROOT / 'out' / 'linux' / 'arch' / 'riscv' / 'boot' / 'Image'
DTB = ROOT / 'out' / 'linux' / 'platform.dtb'
ROOTFS = ROOT / 'out' / 'rootfs.cpio'
CLI = ROOT / 'out' / 'bench' / 'idma_experiment'
IDMA_H = ROOT / 'linux' / 'include' / 'uapi' / 'linux' / 'idma.h'
RUNNER = ROOT / 'tests' / 'board-p4m-start.sh'
KERNEL_RELEASE_FILE = ROOT / 'out' / 'linux' / 'include' / 'config' / 'kernel.release'
ROOTFS_SCRIPT = ROOT / 'scripts' / 'rootfs.py'

REQUIRED_SOFTWARE = {
    'rootfs.cpio': ROOTFS,
    'Image': IMAGE,
    'platform.dtb': DTB,
    'fw_payload.bin': PAYLOAD,
}
LOCK_COMPONENTS = (
    ('cva6', 'cva6_commit'),
    ('cva6/corev_apu/riscv-iommu', 'iommu_commit'),
    ('linux', 'linux_commit'),
)
HARDWARE_LOCK_EXCLUDE = ('linux',)
REUSE_BIT_SHA256 = '7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e'
REUSED_FPGA_MANIFEST_NAME = 'manifests/fpga-migration-4dma-p4m.reused.json'
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


def require_reused_fpga(entry):
    if not isinstance(entry, dict):
        fail('build-status missing fpga object')
    if entry.get('status') != 'reused' or entry.get('rebuilt', False) is not False:
        fail('build-status fpga must be status=reused with rebuilt=false')


def require_manifest(path, action):
    manifest = load_json(path)
    if manifest.get('status') != 'passed' or manifest.get('action') != action:
        fail(str(path) + ' is not a passed ' + action + ' manifest')
    return manifest


def check_profile(manifest, label, profile):
    got = manifest.get('profile')
    if not isinstance(got, dict):
        fail(label + ' profile missing')
    for key, value in profile.items():
        if got.get(key) != value:
            fail(label + ' profile mismatch: ' + key)


def lock_map(lock):
    components = lock.get('components')
    if not isinstance(components, list) or not components:
        fail('sources.lock components missing or empty')
    mapping = {}
    for entry in components:
        if not isinstance(entry, dict):
            fail('sources.lock component is not an object')
        path = entry.get('path')
        commit = entry.get('commit')
        if not isinstance(path, str) or not path:
            fail('sources.lock component path missing')
        if not isinstance(commit, str) or not commit:
            fail('sources.lock commit missing for ' + path)
        if path in mapping:
            fail('duplicate sources.lock component ' + path)
        mapping[path] = commit
    return mapping


def lock_commit(mapping, component):
    if component not in mapping:
        fail('sources.lock lacks component ' + component)
    return mapping[component]


def require_identical_hardware_lock(prior_map, current_map):
    prior_hw = {k: v for k, v in prior_map.items() if k not in HARDWARE_LOCK_EXCLUDE}
    current_hw = {k: v for k, v in current_map.items() if k not in HARDWARE_LOCK_EXCLUDE}
    missing = sorted(set(prior_hw) - set(current_hw))
    added = sorted(set(current_hw) - set(prior_hw))
    changed = sorted(k for k in set(prior_hw) & set(current_hw) if prior_hw[k] != current_hw[k])
    if missing or added or changed:
        fail('hardware-relevant sources.lock differs from prior baseline'
             ' (missing=' + repr(missing) + ' added=' + repr(added) + ' changed=' + repr(changed) + ')')


def read_kernel_release():
    if not KERNEL_RELEASE_FILE.is_file() or KERNEL_RELEASE_FILE.stat().st_size == 0:
        fail('missing or empty ' + str(KERNEL_RELEASE_FILE))
    text = KERNEL_RELEASE_FILE.read_text(encoding='utf-8').strip()
    if not text:
        fail('empty kernel release in ' + str(KERNEL_RELEASE_FILE))
    return text


def artifact_entry(manifest, rel, label):
    artifacts = manifest.get('artifacts')
    if not isinstance(artifacts, dict):
        fail(label + ' manifest artifacts missing')
    if rel not in artifacts:
        fail(label + ' manifest lacks artifact ' + rel)
    return rel, artifacts[rel]


def check_artifact(manifest, path, label):
    rel = str(path.relative_to(ROOT))
    key, expected = artifact_entry(manifest, rel, label)
    actual = sha256(path)
    if actual != expected:
        fail('artifact hash mismatch for ' + rel)
    return {'manifest_key': key, 'sha256': actual, 'bytes': path.stat().st_size}


def copy_file(src, dst, mode=None):
    if not src.is_file() or src.stat().st_size == 0:
        fail('missing or empty input: ' + str(src))
    src_sha = sha256(src)
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst)
    if mode is not None:
        os.chmod(dst, mode)
    if sha256(dst) != src_sha:
        fail('copy hash mismatch: ' + str(src) + ' -> ' + str(dst))
    return src_sha


def load_rootfs_reader():
    if not ROOTFS_SCRIPT.is_file():
        fail('missing ' + str(ROOTFS_SCRIPT))
    spec = importlib.util.spec_from_file_location('p4m_start_rootfs', ROOTFS_SCRIPT)
    if spec is None or spec.loader is None:
        fail('cannot import rootfs helper from ' + str(ROOTFS_SCRIPT))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    reader = getattr(module, 'read_newc', None)
    if not callable(reader):
        fail(str(ROOTFS_SCRIPT) + ' does not provide read_newc')
    return reader


def verify_rootfs_member(reader, rootfs_path, name, source):
    body = source.read_bytes()
    if not body:
        fail('empty source for rootfs member ' + name + ': ' + str(source))
    matches = [entry for entry in reader(rootfs_path.read_bytes()) if Path(entry[0]).name == name]
    if len(matches) != 1:
        fail('rootfs must contain exactly one ' + name + ' (found ' + str(len(matches)) + ')')
    fields, actual = matches[0][1], matches[0][2]
    if actual != body:
        fail('rootfs ' + name + ' content does not match current ' + str(source))
    if fields[1] != (stat.S_IFREG | 0o755):
        fail('rootfs ' + name + ' is not a regular 0755 file')


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
    tmp = OUT / ('.delivery-tmp-' + str(os.getpid()))
    if tmp.exists():
        fail('temporary delivery path exists: ' + str(tmp))

    build_status = load_json(BUILD_STATUS)
    validation = load_json(VALIDATION)
    baseline = load_json(BASELINE)
    profile = load_json(PROFILE)
    lock = load_json(LOCK)
    prior_lock = load_json(PRIOR_LOCK)
    prior_delivery = load_json(PRIOR_DELIVERY_JSON)
    software_manifest = require_manifest(SOFTWARE_MANIFEST, 'software')
    fpga_manifest = require_manifest(PRIOR_FPGA_MANIFEST, 'fpga')

    # gate: new software passed, FPGA reused (not rebuilt), board not run.
    require_passed('software', build_status.get('software'))
    require_reused_fpga(build_status.get('fpga'))
    board = build_status.get('board')
    if not isinstance(board, dict):
        fail('build-status missing board object')
    if board.get('status') != 'not_run' or board.get('board_tested') is not False:
        fail('build-status board must be not_run with board_tested=false')
    if validation.get('board_tested') is not False:
        fail('validation board_tested must be false')

    checks = validation.get('checks')
    if not isinstance(checks, list) or not checks:
        fail('validation checks missing or empty')
    for check in checks:
        if not isinstance(check, dict):
            fail('validation check is not an object')
        for key in ('name', 'log'):
            if not isinstance(check.get(key), str) or not check[key]:
                fail('validation check missing ' + key)
        if check.get('exit_code') != 0:
            fail('validation check not exit_code=0: ' + check['name'])
        log = ROOT / check['log']
        if not log.is_file() or log.stat().st_size == 0:
            fail('missing or empty validation log: ' + check['log'])
    notes = validation.get('notes')
    if not isinstance(notes, list):
        fail('validation notes must be a list')
    sources = validation.get('sources_sha256')
    if not isinstance(sources, dict) or not sources:
        fail('validation sources_sha256 missing or empty')
    for rel, expected in sources.items():
        if not isinstance(rel, str) or not rel:
            fail('validation sources_sha256 has an invalid key')
        if sha256(ROOT / rel) != expected:
            fail('current source hash mismatch: ' + rel)

    # hardware identity: everything except linux must match the prior lock exactly.
    current_map = lock_map(lock)
    prior_map = lock_map(prior_lock)
    require_identical_hardware_lock(prior_map, current_map)

    # profile unchanged across manifests/baseline and the current profile.
    check_profile(software_manifest, 'software manifest', profile)
    check_profile(fpga_manifest, 'reused fpga manifest', profile)
    check_profile(baseline, 'baseline.json', profile)

    # prior reuse anchors.
    base_versions = baseline.get('versions')
    if not isinstance(base_versions, dict):
        fail('baseline.json versions missing')
    base_bit = baseline.get('bit')
    if not isinstance(base_bit, dict) or base_bit.get('sha256') != REUSE_BIT_SHA256:
        fail('baseline.json does not anchor the reused BIT')

    prior_artifacts = prior_delivery.get('artifacts')
    if not isinstance(prior_artifacts, dict):
        fail('prior delivery.json artifacts missing')
    prior_bit = prior_artifacts.get('ariane_xilinx.bit')
    if not isinstance(prior_bit, dict) or prior_bit.get('sha256') != REUSE_BIT_SHA256:
        fail('prior delivery.json BIT hash does not match the reuse anchor')
    prior_mcs = prior_artifacts.get('ariane_xilinx.mcs')
    if not isinstance(prior_mcs, dict) or not prior_mcs.get('sha256'):
        fail('prior delivery.json MCS entry missing')
    prior_status = prior_delivery.get('build_status')
    if not isinstance(prior_status, dict) or prior_status.get('fpga', {}).get('status') != 'passed':
        fail('prior delivery.json does not record a passed FPGA build')
    prior_copied = prior_delivery.get('copied_sha256')
    if not isinstance(prior_copied, dict):
        fail('prior delivery.json copied_sha256 missing')
    if prior_copied.get(PRIOR_FPGA_MANIFEST_REL) != sha256(PRIOR_FPGA_MANIFEST):
        fail('prior FPGA manifest does not match the prior delivery listing')
    if sha256(PRIOR_BIT) != REUSE_BIT_SHA256:
        fail('prior delivered BIT file hash mismatch')
    if sha256(PRIOR_MCS) != prior_mcs['sha256']:
        fail('prior delivered MCS file hash mismatch')

    fpga_artifacts = fpga_manifest.get('artifacts')
    if not isinstance(fpga_artifacts, dict):
        fail('reused fpga manifest artifacts missing')
    bit_keys = [k for k, v in fpga_artifacts.items() if k.endswith('ariane_xilinx.bit')]
    if len(bit_keys) != 1 or fpga_artifacts[bit_keys[0]] != REUSE_BIT_SHA256:
        fail('reused fpga manifest does not record the reused BIT')
    if fpga_manifest.get('build_start', {}).get('profile_sha256') != sha256(PROFILE):
        fail('reused fpga manifest profile_sha256 mismatch')
    if fpga_manifest.get('sources_lock_sha256') != sha256(PRIOR_LOCK):
        fail('reused fpga manifest sources_lock_sha256 does not match the prior lock')
    if fpga_manifest.get('build_start', {}).get('sources_lock_sha256') != sha256(PRIOR_LOCK):
        fail('reused fpga manifest build_start sources_lock_sha256 mismatch')

    # new software manifest must match the current lock/profile and actual files.
    software_lock_sha = sha256(LOCK)
    software_profile_sha = sha256(PROFILE)
    if software_manifest.get('sources_lock_sha256') != software_lock_sha:
        fail('software manifest sources_lock_sha256 mismatch')
    build_start = software_manifest.get('build_start')
    if not isinstance(build_start, dict):
        fail('software manifest build_start missing')
    if build_start.get('sources_lock_sha256') != software_lock_sha:
        fail('software manifest build_start sources_lock_sha256 mismatch')
    if build_start.get('profile_sha256') != software_profile_sha:
        fail('software manifest build_start profile_sha256 mismatch')

    versions = {}
    for component, status_key in LOCK_COMPONENTS:
        commit = lock_commit(current_map, component)
        declared = build_status.get(status_key)
        if not isinstance(declared, str) or not declared:
            fail('build-status missing ' + status_key)
        if declared != commit:
            fail(status_key + ' mismatch: build-status=' + declared + ' lock=' + commit)
        versions[component] = commit
    for component in ('cva6', 'cva6/corev_apu/riscv-iommu'):
        if base_versions.get(component) != versions[component]:
            fail('baseline.json ' + component + ' does not match current lock')

    declared_release = build_status.get('kernel_release')
    if not isinstance(declared_release, str) or not declared_release.strip():
        fail('build-status missing kernel_release')
    kernel_release = read_kernel_release()
    if declared_release.strip() != kernel_release:
        fail('kernel_release mismatch: build-status=' + declared_release.strip() + ' kernel.release=' + kernel_release)

    reader = load_rootfs_reader()
    verify_rootfs_member(reader, ROOTFS, 'platform-p4m-start', RUNNER)
    verify_rootfs_member(reader, ROOTFS, 'idma_experiment', CLI)

    artifacts = {}
    for label, path in REQUIRED_SOFTWARE.items():
        artifacts[label] = check_artifact(software_manifest, path, 'software')
    artifacts['ariane_xilinx.bit'] = {
        'manifest_key': bit_keys[0],
        'reused': True,
        'sha256': REUSE_BIT_SHA256,
        'bytes': PRIOR_BIT.stat().st_size,
    }
    artifacts['ariane_xilinx.mcs'] = {
        'manifest_key': None,
        'reused': True,
        'sha256': prior_mcs['sha256'],
        'bytes': PRIOR_MCS.stat().st_size,
    }

    tmp.mkdir(parents=True)
    try:
        copy_file(PRIOR_BIT, tmp / 'ariane_xilinx.bit')
        copy_file(PRIOR_MCS, tmp / 'ariane_xilinx.mcs')
        copy_file(PAYLOAD, tmp / 'fw_payload.bin')
        copy_file(IMAGE, tmp / 'Image')
        copy_file(DTB, tmp / 'platform.dtb')
        copy_file(ROOTFS, tmp / 'rootfs.cpio')
        copy_file(CLI, tmp / 'idma_experiment')
        copy_file(IDMA_H, tmp / 'idma.h')
        copy_file(RUNNER, tmp / 'platform-p4m-start', mode=0o755)
        copy_file(LOCK, tmp / 'sources.lock.json')
        copy_file(PROFILE, tmp / PROFILE.name)
        copy_file(PROTOCOL, tmp / PROTOCOL.name)
        copy_file(VALIDATION, tmp / 'validation.json')
        copy_file(BUILD_STATUS, tmp / 'build-status.json')
        copy_file(BASELINE, tmp / 'baseline.json')
        copy_file(SOFTWARE_MANIFEST, tmp / 'manifests' / SOFTWARE_MANIFEST.name)
        copy_file(PRIOR_FPGA_MANIFEST, tmp / REUSED_FPGA_MANIFEST_NAME)

        copied_sha = {}
        for path in sorted(tmp.rglob('*')):
            if path.is_file() and path.name != 'SHA256SUMS':
                copied_sha[str(path.relative_to(tmp))] = sha256(path)

        delivery = {
            'schema': 'idma.p4m.start_control.delivery.v1',
            'profile': profile,
            'kernel_release': kernel_release,
            'versions': versions,
            'artifacts': artifacts,
            'reuse': {
                'fpga': 'reused',
                'rebuilt': False,
                'reason': ('SOFTWAREONLY change: FPGA RTL and profile are unchanged, so the '
                           'prior delivery BIT/MCS are reused verbatim and no FPGA was compiled.'),
                'prior_delivery': PRIOR_DELIVERY_REL,
                'prior_fpga_manifest': PRIOR_DELIVERY_REL + '/' + PRIOR_FPGA_MANIFEST_REL,
                'bit_sha256': artifacts['ariane_xilinx.bit']['sha256'],
                'mcs_sha256': artifacts['ariane_xilinx.mcs']['sha256'],
                'lock_comparison': {
                    'excluded_components': list(HARDWARE_LOCK_EXCLUDE),
                    'hardware_components_identical': True,
                    'note': ('Whole-lock equality is intentionally NOT required: linux changed for '
                             'the new payload, so only the non-linux components are compared and are '
                             'required to be identical in set and commit.'),
                },
            },
            'copied_sha256': copied_sha,
            'build_status': build_status,
            'validation': validation,
            'manifests': {
                'software': software_manifest,
                'fpga_reused': fpga_manifest,
            },
        }
        (tmp / 'delivery.json').write_text(json.dumps(delivery, indent=2, sort_keys=True) + NL, encoding='utf-8')

        readme = NL.join([
            '# P4M start-control 交付（软件-only，复用 FPGA BIT）',
            '',
            '本目录由 `out/p4m-start-control-20260920/finalize_delivery.py` 生成：software manifest 为 passed，build-status 中 software=passed/exit_code=0、fpga=reused/rebuilt=false、board=not_run，且 validation 检查全部 exit_code=0、board_tested=false。板上未运行，本目录不声明板卡通过，也不声称性能收益。',
            '',
            '- profile: `migration-4dma-p4m`（FPGA 相关硬件配置与上一次交付一致，未改）',
            '- kernel: `' + kernel_release + '`',
            '- FPGA BIT（复用上一次交付）: `ariane_xilinx.bit` SHA256 `' + artifacts['ariane_xilinx.bit']['sha256'] + '`',
            '- FPGA MCS（复用上一次交付）: `ariane_xilinx.mcs` SHA256 `' + artifacts['ariane_xilinx.mcs']['sha256'] + '`',
            '- payload（新）: `fw_payload.bin` SHA256 `' + artifacts['fw_payload.bin']['sha256'] + '`',
            '- Image（新）: `Image` SHA256 `' + artifacts['Image']['sha256'] + '`',
            '- dtb（新）: `platform.dtb` SHA256 `' + artifacts['platform.dtb']['sha256'] + '`',
            '- rootfs（新）: `rootfs.cpio` SHA256 `' + artifacts['rootfs.cpio']['sha256'] + '`（内含新的 `usr/bin/platform-p4m-start`，与本目录 `platform-p4m-start` 内容一致且权限为 0755）',
            '- CLI（新）: `idma_experiment` SHA256 `' + copied_sha['idma_experiment'] + '`',
            '',
            '## 为什么复用 BIT，不重编 FPGA',
            '本次是 SOFTWAREONLY 变更：FPGA RTL 与 profile 未改，`sources.lock.json` 中除 `linux` 外的全部组件（含 cva6、riscv-iommu 及其它 submodule）与上一次交付逐项 commit 完全一致（脚本逐项校验，要求组件集合与 commit 都相同）。因此不重编 FPGA，直接复用上一次交付的 BIT/MCS。新 lock 与旧 lock 的整体 hash 不同，只是因为 `linux` 已更新（新 payload 需要相应内核），不能据此判断硬件变化，也不要求整份 lock 相等。',
            '',
            '## 板卡运行',
            '仅在新 payload 启动后再执行；本目录保留当前 v7 BIT，不要与本 BIT 混用其它轮次的 payload：',
            '```',
            'platform-p4m-start all /tmp/p4m-start-01',
            '```',
            '- 4 次短验收：每次 64 transfers、warmup 8；',
            '- 另有 8 次 `BPPBBPPB` 序列，4096/256，chunk 4096；',
            '- 以上为工程检查，不是正式参数结论，也不构成性能收益声明；具体语义以 `platform-p4m-start` 脚本为准。',
            '',
            'SD 更新沿用既有原始分区流程；本目录不提供、不发明新的偏移或烧卡步骤，也不做自动写入。板上未运行；时钟/CDC/IO 审查边界沿用既有结论，无新增 timing signoff。成功构建只证明构建本身。',
            '',
            '本目录含 delivery.json、SHA256SUMS、validation.json、build-status.json、baseline.json、manifests、sources.lock.json、profile、P4M_START_CONTROL.md 及上述产物；SHA256SUMS 递归列出除自身外全部文件并已逐项复核。复用产物明确标注：FPGA manifest 以 `fpga-migration-4dma-p4m.reused.json` 命名，BIT/MCS 在 delivery.json 中标记 `reused: true`。',
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
