## What this is

A single proposed file: `out/e3-controlled-20260918/finalize_delivery.py` — Python 3, standard library only, no subprocesses at all (no board, no SD write, no network, no git). It is derived from `out/e3-followup-20260918/finalize_delivery.py` (lines 1‑91) with the clean-ups the task demands:

| Prior helper (follow‑up profile) | New helper (controlled profile) |
|---|---|
| `assert` everywhere (defeated by `-O`) | `require()` → explicit `RuntimeError` |
| `dest.mkdir(exist_ok=True)`, copy in place | temp sibling dir + `os.rename`, never overwrites `delivery/` |
| only required the BIT after hashing all manifest entries | software: payload/Image/rootfs/dtb; fpga: BIT; cross‑action entries recorded, not failed |
| rewrote links inside copied docs (lines 53‑59) | docs copied verbatim; `docs/e3-queue-results-review.md` keeps its relative position so `E3_CONTROLLED.md`'s `docs/...` link resolves |
| kernel release hard‑coded in README | read from `out/linux/include/config/kernel.release` |
| `SHA256SUMS` flat, filename only | recursive, relative POSIX paths, sorted, self‑excluded, re‑verified, completeness‑checked |
| build‑status written with `write_text` | `os.replace` atomic write, only after every validation |
| no MCS‑vs‑manifest handling | MCS non‑empty, hashed separately, `listed_in_a_manifest` recorded |

Path facts taken from `scripts/platform.py`: stage tree `out/{profile}/fpga-src` (line 152‑158), BIT/MCS at `corev_apu/fpga/work-fpga/` (lines 179‑182), payload at `out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin` (line 228), bench binary `out/bench/idma_e3_controlled` (line 86), manifest artifact path list (lines 227‑229), `BUILD_CONTEXT` fields (lines 252‑256).

````python
#!/usr/bin/env python3
"""Finalize the migration-4dma-e3-controlled (E3 v6 controlled) delivery.

Proposal only: this file has NOT been executed.  Run it once the software and
FPGA builds have really finished and the root agent has recorded the completion
facts in out/e3-controlled-20260918/build-status.json.

Guarantees implemented here:

* host side only -- no board access, no SD-card write, no network, no git, no
  subprocess of any kind;
* every precondition is validated before a single artifact is copied;
* the delivery tree is assembled in a temporary sibling directory and renamed
  atomically to out/e3-controlled-20260918/delivery; an existing destination is
  refused and never touched;
* all hashes are streamed from the files on disk at run time, so no test count,
  throughput, version or token number is invented here;
* build-status.json is updated atomically only after all validation passed and
  keeps the root agent's fpga/software/exit-code facts.
"""

from datetime import datetime, timezone
from pathlib import Path
import hashlib
import json
import os
import shutil
import sys
import traceback
import uuid

ROOT = Path(__file__).resolve().parents[2]
PROFILE = 'migration-4dma-e3-controlled'
OUT = ROOT / 'out' / 'e3-controlled-20260918'
DEST = OUT / 'delivery'
BUILD_STATUS = OUT / 'build-status.json'
VALIDATION = OUT / 'validation.json'
SOURCES_LOCK = ROOT / 'sources.lock.json'
PROFILE_FILE = ROOT / 'configs' / f'{PROFILE}.json'
STAGE = ROOT / 'out' / PROFILE / 'fpga-src'
FPGA_WORK = STAGE / 'cva6' / 'corev_apu' / 'fpga' / 'work-fpga'
BIT = FPGA_WORK / 'ariane_xilinx.bit'
MCS = FPGA_WORK / 'ariane_xilinx.mcs'
BIT_REL = f'out/{PROFILE}/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit'
PAYLOAD = ROOT / 'out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin'
KERNEL_IMAGE = ROOT / 'out/linux/arch/riscv/boot/Image'
PLATFORM_DTB = ROOT / 'out/linux/platform.dtb'
ROOTFS = ROOT / 'out/rootfs.cpio'
KERNEL_RELEASE_FILE = ROOT / 'out/linux/include/config/kernel.release'
IDMA_UAPI = ROOT / 'linux/include/uapi/linux/idma.h'
IDMA_E3_CONTROLLED = ROOT / 'out/bench/idma_e3_controlled'
E3_CONTROLLED_MD = ROOT / 'E3_CONTROLLED.md'
QUEUE_REVIEW_MD = ROOT / 'docs/e3-queue-results-review.md'
MANIFESTS = {
    'software': ROOT / f'out/manifests/software-{PROFILE}.json',
    'fpga': ROOT / f'out/manifests/fpga-{PROFILE}.json',
}

# Artifacts each action is responsible for.  A manifest may omit entries that
# belong to the other action (the software manifest is written before the FPGA
# build exists, so it never lists the BIT); the FPGA manifest may list software
# artifacts that were current when it was written.  Only these required entries
# are hard failures -- cross-action entries are reported, not failed.
REQUIRED_ARTIFACTS = {
    'software': [
        'out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin',
        'out/linux/arch/riscv/boot/Image',
        'out/rootfs.cpio',
        'out/linux/platform.dtb',
    ],
    'fpga': [BIT_REL],
}

# The platform builder rewrites these two top files on purpose (DS delays and
# IOTLB entries), so they are excluded from the staged byte-for-byte comparison.
DELIBERATELY_MODIFIED = {
    'cva6/corev_apu/fpga/src/ariane_xilinx.sv',
    'cva6/corev_apu/tb/ariane_soc_pkg.sv',
}

LINKED_PROTOCOL_DOCS = ('E3_QUEUE_PAIR.md', 'E3_SERVICE.md', 'E3_DIAGNOSTIC.md')
BOARD_NOT_RUN = {None, 'not_run', 'not_started', 'never_run', 'skipped', 'n/a'}

# --------------------------------------------------------------------- helpers


def require(condition, message):
    """Raise unless ``condition`` holds; never silently pass."""
    if not condition:
        raise RuntimeError(message)


def sha256_of(path, chunk=1024 * 1024):
    """Streaming SHA256 so large BIT/MCS files never load fully into memory."""
    digest = hashlib.sha256()
    with Path(path).open('rb') as handle:
        for block in iter(lambda: handle.read(chunk), b''):
            digest.update(block)
    return digest.hexdigest()


def read_json(path):
    with Path(path).open('r', encoding='utf-8') as handle:
        return json.load(handle)


def write_json_atomic(path, data):
    path = Path(path)
    tmp = path.with_name(f'{path.name}.tmp-{os.getpid()}-{uuid.uuid4().hex[:8]}')
    try:
        tmp.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
        os.replace(tmp, path)
    finally:
        if tmp.exists():
            tmp.unlink()


def is_zero_int(value):
    return isinstance(value, int) and not isinstance(value, bool) and value == 0


def rel_or_abs(path):
    path = Path(path)
    try:
        return path.relative_to(ROOT).as_posix()
    except ValueError:
        return str(path)


def dirty_label(value):
    if value is True:
        return 'True（有未提交改动）'
    if value is False:
        return 'False（无未提交改动）'
    return f'未知（{value!r}）'


def write_sha256sums(directory):
    """Write and verify SHA256SUMS: recursive, relative, sorted, excluding itself."""
    directory = Path(directory)
    rows = []
    for path in sorted((p for p in directory.rglob('*') if p.is_file()), key=lambda p: p.as_posix()):
        rel = path.relative_to(directory).as_posix()
        if rel == 'SHA256SUMS':
            continue
        rows.append((rel, sha256_of(path), path.stat().st_size))
    require(rows, f'no files to checksum under {directory}')
    (directory / 'SHA256SUMS').write_text(
        ''.join(f'{digest}  {rel}\n' for rel, digest, _ in rows), encoding='utf-8')
    listed = {rel for rel, _, _ in rows}
    actual = {p.relative_to(directory).as_posix() for p in directory.rglob('*') if p.is_file()}
    require(actual - {'SHA256SUMS'} == listed,
            'SHA256SUMS does not cover exactly the delivered files')
    for rel, digest, size in rows:
        path = directory / rel
        require(sha256_of(path) == digest, f'SHA256SUMS verification failed: {rel}')
        require(path.stat().st_size == size, f'SHA256SUMS size verification failed: {rel}')
    return rows


def resolve_runner():
    """Board runner (E3_CONTROLLED.md section 13); the delivered name is fixed."""
    candidates = [
        ROOT / 'tests/board-e3-controlled.sh',
        ROOT / 'platform-e3-controlled',
        OUT / 'platform-e3-controlled',
        ROOT / 'out/bench/platform-e3-controlled',
    ]
    found = [path for path in candidates if path.is_file() and path.stat().st_size > 0]
    require(found, 'board runner not found; expected tests/board-e3-controlled.sh '
                   '(E3_CONTROLLED.md section 13)')
    return found[0], candidates


def resolve_server_review():
    """The frozen v5 independent server review that docs/e3-queue-results-review.md cites."""
    known = ROOT / 'out/mac-e3-queue-20260918-130545-run-d01lc9wp/server-review-v5.json'
    if known.is_file() and known.stat().st_size > 0:
        return known, [known]
    found = sorted((p for p in (ROOT / 'out').glob('*/server-review-v5.json') if p.is_file()),
                   key=lambda p: p.as_posix())
    require(found, f'server-review-v5.json not found under {ROOT / "out"}')
    require(len(found) == 1,
            'cannot choose server-review-v5.json unambiguously: '
            + ', '.join(str(path) for path in found))
    return found[0], found


# ------------------------------------------------------------------------ main


def main():
    # ------------------------------------------------------------ 0. layout
    require(OUT.is_dir(), f'missing output directory: {OUT}')
    require(not DEST.exists(), f'destination already exists, refusing to overwrite: {DEST}')

    runner, runner_candidates = resolve_runner()
    server_review, server_review_candidates = resolve_server_review()

    inputs = {
        'sources.lock.json': SOURCES_LOCK,
        'profile config': PROFILE_FILE,
        'validation.json': VALIDATION,
        'build-status.json': BUILD_STATUS,
        'software manifest': MANIFESTS['software'],
        'fpga manifest': MANIFESTS['fpga'],
        'ariane_xilinx.bit': BIT,
        'ariane_xilinx.mcs': MCS,
        'fw_payload.bin': PAYLOAD,
        'Image': KERNEL_IMAGE,
        'platform.dtb': PLATFORM_DTB,
        'rootfs.cpio': ROOTFS,
        'kernel.release': KERNEL_RELEASE_FILE,
        'idma.h': IDMA_UAPI,
        'idma_e3_controlled': IDMA_E3_CONTROLLED,
        'E3_CONTROLLED.md': E3_CONTROLLED_MD,
        'docs/e3-queue-results-review.md': QUEUE_REVIEW_MD,
        'server-review-v5.json': server_review,
        'platform-e3-controlled': runner,
    }
    for label, path in inputs.items():
        require(path.is_file(), f'missing input {label}: {path}')
        require(path.stat().st_size > 0, f'empty input {label}: {path}')

    protocol_text = E3_CONTROLLED_MD.read_text(encoding='utf-8')
    linked_docs = []
    for name in LINKED_PROTOCOL_DOCS:
        if name not in protocol_text:
            continue
        path = ROOT / name
        require(path.is_file() and path.stat().st_size > 0,
                f'E3_CONTROLLED.md links {name} but the file is missing or empty: {path}')
        linked_docs.append((name, path))

    # ------------------------------------------- 1. root completion facts
    status = read_json(BUILD_STATUS)
    require(status.get('profile') == PROFILE,
            f"build-status profile is {status.get('profile')!r}, expected {PROFILE!r}")
    for key in ('software', 'fpga'):
        require(status.get(key) == 'passed',
                f'build-status {key} must be "passed" before finalizing, got {status.get(key)!r}')
    for key in ('software_exit_code', 'fpga_exit_code'):
        require(is_zero_int(status.get(key)),
                f'build-status {key} must be the integer 0, got {status.get(key)!r}')
    board_state = status.get('board')
    require(board_state in BOARD_NOT_RUN,
            f'build-status board={board_state!r}: this delivery states the v6 image was not board tested')

    # ------------------------------------- 2. lock, profile, manifests
    lock_sha = sha256_of(SOURCES_LOCK)
    profile_sha = sha256_of(PROFILE_FILE)
    profile_cfg = read_json(PROFILE_FILE)
    require(profile_cfg.get('ds_delay_input') == 50 and profile_cfg.get('ds_delay_output') == 50,
            f'{PROFILE_FILE.name} is not the fixed DS 50/50 profile')
    require(profile_cfg.get('num_dma') == 4 and profile_cfg.get('device_ids') == [10, 11, 12, 13],
            f'{PROFILE_FILE.name} is not the four-DMA DID 10..13 profile')

    lock = read_json(SOURCES_LOCK)
    components = {}
    for item in lock.get('components', []):
        components[item.get('path')] = item.get('commit')
    versions = {}
    for component in ('cva6', 'cva6/corev_apu/riscv-iommu', 'linux'):
        commit = components.get(component)
        require(isinstance(commit, str) and len(commit) == 40,
                f'sources.lock.json component {component!r} has no usable commit: {commit!r}')
        versions[component] = commit

    manifests = {action: read_json(path) for action, path in MANIFESTS.items()}
    artifact_report, cross_notes, manifest_platform, dirty_flags = {}, {}, {}, {}
    for action in ('software', 'fpga'):
        manifest = manifests[action]
        require(manifest.get('action') == action,
                f'{action} manifest action field is {manifest.get("action")!r}')
        require(manifest.get('status') == 'passed',
                f'{action} manifest status is {manifest.get("status")!r}, expected "passed"')
        require(manifest.get('sources_lock_sha256') == lock_sha,
                f'{action} manifest sources_lock_sha256 does not match the current sources.lock.json')
        require(manifest.get('profile') == profile_cfg,
                f'{action} manifest profile does not match the current {PROFILE_FILE.name}')
        start = manifest.get('build_start') or {}
        require(start.get('sources_lock_sha256') == lock_sha,
                f'{action} build_start.sources_lock_sha256 does not match the current sources.lock.json')
        require(start.get('profile_sha256') == profile_sha,
                f'{action} build_start.profile_sha256 does not match the current profile config')
        require(start.get('platform_commit') or manifest.get('platform_commit'),
                f'{action} manifest/build_start records no platform commit')

        artifacts = manifest.get('artifacts') or {}
        verified, drifted, missing = [], [], []
        for rel in sorted(artifacts):
            expected = artifacts[rel]
            source = ROOT / rel
            if not source.is_file():
                missing.append(rel)
                continue
            actual = sha256_of(source)
            if actual == expected:
                verified.append(rel)
            else:
                drifted.append({'path': rel, 'manifest_sha256': expected, 'current_sha256': actual})
        for rel in REQUIRED_ARTIFACTS[action]:
            require(rel in artifacts,
                    f'{action} manifest does not list its required artifact {rel}')
            require(rel in verified,
                    f'{action} manifest artifact is missing or stale on disk: {rel}')
        extra = [rel for rel in missing + [entry['path'] for entry in drifted]
                 if rel not in REQUIRED_ARTIFACTS[action]]
        if extra:
            cross_notes[action] = {
                'note': ('entries owned by the other action (or an earlier build of it) drifted or '
                         'disappeared after this manifest was written; recorded, not treated as a failure'),
                'entries': extra,
            }
        artifact_report[action] = {'verified': verified, 'drifted': drifted, 'missing': missing}
        manifest_platform[action] = {
            'manifest_platform_commit': manifest.get('platform_commit'),
            'build_start_platform_commit': start.get('platform_commit'),
        }
        dirty_flags[action] = {
            'manifest_platform_dirty': manifest.get('platform_dirty'),
            'build_start_platform_dirty': start.get('platform_dirty'),
        }

    # ------------------------------------- 3. BIT / MCS / payload facts
    bit_sha = sha256_of(BIT)
    mcs_sha = sha256_of(MCS)
    require(manifests['fpga']['artifacts'].get(BIT_REL) == bit_sha,
            'fpga manifest BIT hash does not match the BIT on disk')
    mcs_rel = MCS.relative_to(ROOT).as_posix()
    mcs_listed = False
    for action, manifest in manifests.items():
        listed = (manifest.get('artifacts') or {}).get(mcs_rel)
        if listed is None:
            continue
        mcs_listed = True
        require(listed == mcs_sha,
                f'{action} manifest MCS entry does not match the MCS on disk')
    payload_sha = sha256_of(PAYLOAD)
    image_sha = sha256_of(KERNEL_IMAGE)
    dtb_sha = sha256_of(PLATFORM_DTB)

    # ------------------------------------------- 4. validation.json
    validation = read_json(VALIDATION)
    require(validation.get('status') == 'passed',
            f"validation.json status is {validation.get('status')!r}")
    require(validation.get('board_tested') is False,
            'validation.json board_tested must be false')
    checks = validation.get('checks') or []
    require(checks, 'validation.json contains no checks')
    for check in checks:
        require(check.get('name'), f'validation check without a name: {check!r}')
        require(is_zero_int(check.get('exit_code')),
                f"validation check {check.get('name')!r} exit_code={check.get('exit_code')!r}")
    sources = validation.get('sources_sha256') or {}
    require(sources, 'validation.json contains no sources_sha256')

    # untested-source drift, excluding the two deliberately rewritten top files
    for rel in sorted(sources):
        if rel in DELIBERATELY_MODIFIED:
            continue
        source = ROOT / rel
        require(source.is_file(), f'validation source missing: {rel}')
        require(sha256_of(source) == sources[rel], f'post-test source change: {rel}')

    staged_checked, deliberate = [], {}
    for rel in sorted(sources):
        expected = sources[rel]
        if rel in DELIBERATELY_MODIFIED:
            staged = STAGE / rel
            repo_copy = ROOT / rel
            repo_ok = repo_copy.is_file() and sha256_of(repo_copy) == expected
            staged_ok = staged.is_file() and sha256_of(staged) == expected
            require(repo_ok or staged_ok,
                    f'validation hash for {rel} matches neither the repository copy nor the staged '
                    'copy (the builder rewrites this file on purpose)')
            deliberate[rel] = {'repo_copy_matches_validation': repo_ok,
                              'staged_copy_matches_validation': staged_ok}
            continue
        if not rel.startswith('cva6/'):
            continue
        staged = STAGE / rel
        require(staged.is_file(), f'staged source missing: {staged}')
        require(sha256_of(staged) == expected,
                f'staged source does not match the tested source: {rel}')
        staged_checked.append(rel)
    require(staged_checked or deliberate,
            'validation.json lists no cva6/* source that could be cross-checked against the staged '
            'FPGA tree')

    # ------------------------------------------- 5. kernel release
    kernel_release = KERNEL_RELEASE_FILE.read_text(encoding='utf-8').strip()
    require(kernel_release, 'kernel.release is empty')
    require('\n' not in kernel_release,
            f'kernel.release should hold a single line: {kernel_release!r}')
    require(kernel_release != '00087', 'kernel.release still contains the placeholder 00087')
    kernel_release_suffix_matches = None
    if '-g' in kernel_release:
        suffix = kernel_release.rsplit('-g', 1)[1].strip().split('-dirty')[0]
        require(suffix and versions['linux'].startswith(suffix),
                f'kernel.release {kernel_release!r} does not carry the locked linux commit '
                f'{versions["linux"]}')
        kernel_release_suffix_matches = True

    # ------------------------------------------- 6. delivery entries
    entries = [
        ('ariane_xilinx.bit', BIT),
        ('ariane_xilinx.mcs', MCS),
        ('fw_payload.bin', PAYLOAD),
        ('Image', KERNEL_IMAGE),
        ('platform.dtb', PLATFORM_DTB),
        ('idma_e3_controlled', IDMA_E3_CONTROLLED),
        ('platform-e3-controlled', runner),
        ('idma.h', IDMA_UAPI),
        ('E3_CONTROLLED.md', E3_CONTROLLED_MD),
        ('docs/e3-queue-results-review.md', QUEUE_REVIEW_MD),
        ('sources.lock.json', SOURCES_LOCK),
        ('profile.json', PROFILE_FILE),
        ('software-manifest.json', MANIFESTS['software']),
        ('fpga-manifest.json', MANIFESTS['fpga']),
        ('validation.json', VALIDATION),
        ('server-review-v5.json', server_review),
    ] + linked_docs
    names = [name for name, _ in entries]
    require(len(names) == len(set(names)), f'duplicate delivery entry names: {sorted(names)}')
    required_names = set(names) | {'build-status.json', 'README.md', 'delivery.json'}

    # ------------------------------------------- 7. build in a temp sibling
    tmp = OUT / f'.delivery-tmp-{os.getpid()}-{uuid.uuid4().hex[:8]}'
    require(not tmp.exists(), f'stale temporary directory, remove it first: {tmp}')
    try:
        tmp.mkdir()
        written = {}
        for name, source in entries:
            target = tmp / name
            target.parent.mkdir(parents=True, exist_ok=True)
            source_sha = sha256_of(source)
            shutil.copy2(source, target)
            require(sha256_of(target) == source_sha, f'copied bytes differ from source: {name}')
            require(target.stat().st_size == source.stat().st_size,
                    f'copied size differs from source: {name}')
            written[name] = {'source': rel_or_abs(source), 'sha256': source_sha,
                             'size': target.stat().st_size}

        # ---- build-status: atomic, only facts already validated above ----
        finalized_at = datetime.now(timezone.utc).isoformat()
        new_status = dict(status)
        new_status.update({
            'kernel_release': kernel_release,
            'kernel_release_source': rel_or_abs(KERNEL_RELEASE_FILE),
            'bit_sha256': bit_sha,
            'mcs_sha256': mcs_sha,
            'payload_sha256': payload_sha,
            'image_sha256': image_sha,
            'platform_dtb_sha256': dtb_sha,
            'sources_lock_sha256': lock_sha,
            'profile_sha256': profile_sha,
            'artifact_hash_verification': 'passed',
            'critical_staged_sources_match_tested_sources': True,
            'validation_status': 'passed',
            'delivery_dir': DEST.relative_to(ROOT).as_posix(),
            'finalized_at': finalized_at,
            'updated': finalized_at,
        })
        write_json_atomic(BUILD_STATUS, new_status)
        build_status_sha = sha256_of(BUILD_STATUS)
        shutil.copy2(BUILD_STATUS, tmp / 'build-status.json')
        require(sha256_of(tmp / 'build-status.json') == build_status_sha,
                'build-status.json copy verification failed')
        written['build-status.json'] = {'source': rel_or_abs(BUILD_STATUS),
                                       'sha256': build_status_sha,
                                       'size': (tmp / 'build-status.json').stat().st_size}

        # ---- README (Chinese, concise) ----
        sw_dirty = dirty_label(dirty_flags['software']['build_start_platform_dirty'])
        fpga_dirty = dirty_label(dirty_flags['fpga']['build_start_platform_dirty'])
        sw_manifest_dirty = dirty_label(dirty_flags['software']['manifest_platform_dirty'])
        fpga_manifest_dirty = dirty_label(dirty_flags['fpga']['manifest_platform_dirty'])
        sw_start = manifest_platform['software']['build_start_platform_commit']
        sw_commit = manifest_platform['software']['manifest_platform_commit']
        fpga_start = manifest_platform['fpga']['build_start_platform_commit']
        fpga_commit = manifest_platform['fpga']['manifest_platform_commit']
        linked_names = ', '.join(f'`{name}`' for name, _ in linked_docs) or '（本轮无被引用的外部协议文档）'
        excluded_names = '、'.join(f'`{name}`' for name in sorted(DELIBERATELY_MODIFIED))

        readme = f"""# E3 v6 独立受控有限实验：构建与主机验证交付

本目录由 `out/e3-controlled-20260918/finalize_delivery.py` 在主机侧生成；下文全部哈希为本次实际计算结果，未编造测试数量、吞吐或 token 用量。

**必须同时使用本目录的新 BIT 和 SD payload（同一实验的产物），不可与其他轮次的 BIT/payload 混用。**

- `ariane_xilinx.bit`：observer 外层 magic v6（受控 `0x49444d414f425306`），E3 bank v2 不变；受控窗口只把 DMA0/DMA1 的 `0xb0..0xff` 接入 launcher。
- `fw_payload.bin`：内核 `{kernel_release}`，已内置 rootfs；在旧系统上只替换 CLI 不够。
- `ariane_xilinx.mcs`：非空；标准 manifest 只记录 BIT、未列 MCS，故其 SHA256 单独记录。
- `idma_e3_controlled`、`platform-e3-controlled`（板卡 runner）、`idma.h`：新程序与配套接口。
- [E3_CONTROLLED.md](E3_CONTROLLED.md)：本轮协议、寄存器、CLI 与固定矩阵。
- [docs/e3-queue-results-review.md](docs/e3-queue-results-review.md)：v5 队列配对（13 对 / 26 子轮）上板复核，须与本轮 v6 结果分开阅读。
- 本轮被引用的协议文档 {linked_names}；以上文档均按原文复制，未改写其中链接。

设计要点：本轮为独立受控——每轮 formal 只发一份 64 B 的 V 副本（有 A 时再发一份 64 B 的 A 副本），各一次读、一次写，由实际计数器核对；launcher 在硬件内做受控时序，CPU 只做**有限完成轮询**（非中断）。固定 DS 50/50、4 DMA、IOTLB 16、50 MHz、无 endless A、无 CPU refill。

板端建议命令：

```
platform-e3-controlled all /tmp/e3-controlled-<新目录名>
```

runner 逐 case 打印标记，把 CLI stdout JSON 存文件并 `cat` 到 UART、stderr 存文件并 `cat` 到 stderr；CLI 非零立即停止，并用 `trap` 始终保存 dmesg 与 taint 终态（保留原始退出码）。禁止烧写 SD、联网、系统升级，禁止重试 ioctl，禁止隐藏失败。UART 在 Mac 上捕获、保存并记录版本后回传 Dell（常设做法）。

矩阵：短验收 3 对 + 正式 8 对 = **11 对、22 子轮**；若每轮仍为 19 条观测，合计**预期 418 条 UART 观测——本轮尚未测量**，不得写成已得结果。实际出错立即停止，不自动重试、不扩矩阵、不补采。

SD 更新仍按**原有原始分区流程**刷写 payload（写入既有 payload 分区）。本目录不提供、也不发明新的写卡步骤或分区偏移。

版本与完整性（全部取自本机文件，无估计值）：

- 构建起点平台提交：软件 `{sw_start}`，FPGA `{fpga_start}`；manifest 记录的平台提交：软件 `{sw_commit}`，FPGA `{fpga_commit}`（文档提交可能使二者不同）。
- 组件：CVA6 `{versions['cva6']}`，IOMMU `{versions['cva6/corev_apu/riscv-iommu']}`，Linux `{versions['linux']}`（取自 `sources.lock.json`）。
- BIT `{bit_sha}`；MCS `{mcs_sha}`；payload `{payload_sha}`；Image `{image_sha}`；`platform.dtb` `{dtb_sha}`；`sources.lock.json` `{lock_sha}`。
- 工作树脏状态如实记录：软件构建起点 {sw_dirty}；FPGA 构建起点 {fpga_dirty}；manifest 记录：软件 {sw_manifest_dirty}、FPGA {fpga_manifest_dirty}。原文档脏改动原样保留，本交付**不声称工作树干净**。
- 主机验证：`validation.json` 全部检查 `exit_code=0`、`board_tested=false`；其 `sources_sha256` 逐项重新核对，`cva6/*` 与 `out/{PROFILE}/fpga-src/` 中实际导出源码逐项比对（构建器有意改写的顶部文件 {excluded_names} 按策略不计入 staged 逐字节比对，清单见 `delivery.json`）。
- `SHA256SUMS` 递归列出本目录全部文件（不含自身），生成后已逐项复核；`delivery.json` 记录版本、哈希、逐项验证结论、跨动作清单项漂移说明与 runner 来源。

边界与说明：**新 v6 尚未上板**；构建与主机测试成功**不等于硬件正确**；时钟/CDC/IO 审查边界维持原样，本轮**未新增时序签核**；成功日志只保存、**未审查**。分析文中的原始数据链接指向原工程路径，**原始 v5 数据与 `v5-reviewed-facts.json` 仍在 Dell，未随本目录复制**；协议文档保持原文（含其原有的 `../` 相对链接），未做改写。

署名：DeepSeek 整理原始证据并起草 CLI/桩/测试与文档；Codex 独立复核原始数据与关键代码，负责 RTL/驱动、设计、关键修复、应用审查提案、测试与编译。
"""
        (tmp / 'README.md').write_text(readme, encoding='utf-8')
        written['README.md'] = {'source': 'generated by finalize_delivery.py',
                                'sha256': sha256_of(tmp / 'README.md'),
                                'size': (tmp / 'README.md').stat().st_size}

        # ---- delivery.json ----
        delivery = {
            'schema': 'e3-controlled.delivery.v1',
            'profile': PROFILE,
            'generated_by': rel_or_abs(Path(__file__)),
            'recorded_at': finalized_at,
            'software_exit_code': status.get('software_exit_code'),
            'fpga_exit_code': status.get('fpga_exit_code'),
            'root_build_status': {'software': status.get('software'),
                                  'fpga': status.get('fpga'),
                                  'board': board_state},
            'board_tested': False,
            'successful_build_logs_read': False,
            'timing_report_reviewed': False,
            'kernel_release': kernel_release,
            'kernel_release_source': rel_or_abs(KERNEL_RELEASE_FILE),
            'kernel_release_carries_locked_linux_commit': kernel_release_suffix_matches,
            'platform': manifest_platform,
            'platform_dirty': dirty_flags,
            'versions': versions,
            'hashes': {
                'ariane_xilinx.bit': bit_sha,
                'ariane_xilinx.mcs': mcs_sha,
                'fw_payload.bin': payload_sha,
                'Image': image_sha,
                'platform.dtb': dtb_sha,
                'sources.lock.json': lock_sha,
                'profile': profile_sha,
            },
            'artifact_hash_verification': 'passed',
            'artifact_verification': artifact_report,
            'cross_action_manifest_notes': cross_notes,
            'mcs': {
                'source_path': mcs_rel,
                'listed_in_a_manifest': mcs_listed,
                'note': ('the standard platform manifest records the BIT only; the MCS is non-empty '
                         'and its SHA256 is recorded separately here'),
            },
            'critical_staged_sources': {
                'checked_against_validation': staged_checked,
                'policy_exclusions': sorted(DELIBERATELY_MODIFIED),
                'validation_entries_for_excluded_files': deliberate,
                'note': ('the builder rewrites the excluded top files on purpose; every other cva6/* '
                         'source listed in validation.json matches the staged export byte for byte'),
            },
            'validation': {
                'path': rel_or_abs(VALIDATION),
                'status': validation.get('status'),
                'scope': validation.get('scope'),
                'checks': len(checks),
                'sources_verified': len(sources),
                'board_tested': validation.get('board_tested'),
            },
            'expected_matrix': {
                'short_pairs': 3,
                'formal_pairs': 8,
                'pairs': 11,
                'rounds': 22,
                'expected_uart_lines_if_19_per_round': 418,
                'measured': False,
                'note': 'anticipated, not measured; never report as a result',
            },
            'recommended_board_command': 'platform-e3-controlled all /tmp/e3-controlled-<newname>',
            'runner': {
                'delivered_as': 'platform-e3-controlled',
                'source': rel_or_abs(runner),
                'candidates_checked': [rel_or_abs(path) for path in runner_candidates],
            },
            'server_review': {
                'delivered_as': 'server-review-v5.json',
                'source': rel_or_abs(server_review),
                'candidates_checked': [rel_or_abs(path) for path in server_review_candidates],
                'sha256': sha256_of(server_review),
            },
            'files': written,
            'sha256sums': ('see SHA256SUMS (it excludes itself); README.md and delivery.json are '
                           'generated by this script'),
            'notes': [
                'Software and FPGA builds may have started at different platform commits; component '
                'commits come from sources.lock.json and the tested critical sources were re-verified.',
                'Existing dirty working-tree changes (documentation included) are reported as recorded '
                'and were not cleaned; this delivery never claims a clean tree.',
                'Raw v5 run data and v5-reviewed-facts.json remain in the original project (Dell) and '
                'are not copied here; protocol/review documents keep their original relative links.',
                'No board access, SD-card write, network access or git command is performed by this '
                'script.',
            ],
        }
        (tmp / 'delivery.json').write_text(
            json.dumps(delivery, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')

        # ---- SHA256SUMS + completeness, then atomic rename ----
        rows = write_sha256sums(tmp)
        present = {path.relative_to(tmp).as_posix() for path in tmp.rglob('*') if path.is_file()}
        require(required_names <= present,
                f'delivery is missing required files: {sorted(required_names - present)}')
        require('SHA256SUMS' in present, 'SHA256SUMS was not written')
        total_bytes = sum(path.stat().st_size for path in tmp.rglob('*') if path.is_file())

        require(not DEST.exists(),
                f'destination appeared while finalizing, refusing to overwrite: {DEST}')
        os.rename(tmp, DEST)

        for rel, digest, _size in rows:
            require(sha256_of(DEST / rel) == digest, f'post-rename verification failed: {rel}')
        delivered = sorted(p.relative_to(DEST).as_posix() for p in DEST.rglob('*') if p.is_file())
        require(len(delivered) == len(present),
                'delivered file set differs from the verified temporary tree')

        print(json.dumps({
            'status': 'passed',
            'delivery': DEST.relative_to(ROOT).as_posix(),
            'files': len(present),
            'bytes': total_bytes,
            'kernel_release': kernel_release,
            'hashes': {
                'ariane_xilinx.bit': bit_sha,
                'ariane_xilinx.mcs': mcs_sha,
                'fw_payload.bin': payload_sha,
                'Image': image_sha,
                'platform.dtb': dtb_sha,
                'sources.lock.json': lock_sha,
                'build-status.json': build_status_sha,
                'SHA256SUMS': sha256_of(DEST / 'SHA256SUMS'),
            },
            'board_tested': False,
            'successful_build_logs_read': False,
            'expected_uart_lines_unmeasured': 418,
        }, ensure_ascii=False, indent=2))

    except BaseException:
        # Only ever remove the temporary tree this run created; DEST and the
        # sources are never touched.  Set E3_FINALIZE_KEEP_TMP=1 to keep it.
        if os.environ.get('E3_FINALIZE_KEEP_TMP') == '1' and tmp.exists():
            keep = OUT / f'delivery-failed-{datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")}'
            os.replace(tmp, keep)
            print(f'kept the temporary tree for inspection: {keep}', file=sys.stderr)
        elif tmp.exists():
            shutil.rmtree(tmp, ignore_errors=True)
            if tmp.exists():
                print(f'could not remove the temporary tree: {tmp}', file=sys.stderr)
        raise


if __name__ == '__main__':
    try:
        main()
    except Exception as error:          # explicit failure exit, never a false pass
        traceback.print_exc()
        print(json.dumps({'status': 'failed',
                          'error': f'{type(error).__name__}: {error}'},
                         ensure_ascii=False, indent=2))
        sys.exit(1)
````

## Assumptions (flag to Codex)

1. **Runner source path.** `tests/board-e3-controlled.sh` is the first candidate (per `E3_CONTROLLED.md` §13, line 321); `platform-e3-controlled`, `out/e3-controlled-20260918/platform-e3-controlled` and `out/bench/platform-e3-controlled` are fallbacks. It is always delivered **as `platform-e3-controlled`**, matching the recommended board command. The chosen source is recorded in `delivery.json`; if the real script lives elsewhere, edit `resolve_runner()`.
2. **`server-review-v5.json`.** Primary path is the one cited by `docs/e3-queue-results-review.md` line 24 (`out/mac-e3-queue-20260918-130545-run-d01lc9wp/server-review-v5.json`); otherwise exactly one `out/*/server-review-v5.json` must exist, else the script fails rather than guessing.
3. **`idma_e3_controlled`** is taken from `out/bench/idma_e3_controlled` (`scripts/platform.py` line 86).
4. **`profile.json`** keeps the follow-up helper's name for `configs/migration-4dma-e3-controlled.json`.
5. **validation.json path keys** are root-relative; the critical-source mapping assumes `cva6/…` → `out/{profile}/fpga-src/cva6/…` (as in the reference helper lines 25‑26). If validation names files differently, the explicit `require` fails loudly instead of passing silently.
6. **Deliberately modified top files** are accepted only for the staged comparison; if validation lists them, the hash must match *either* the repo copy *or* the staged copy (recorded per file). Non-listed == nothing to exclude.
7. **`kernel.release` cross-check**: only enforced when the string contains `-g`; the suffix must prefix-match the locked Linux commit. If the tree is built without `LOCALVERSION_AUTO`, the check is skipped (recorded as `null`), never guessed.
8. **Board state**: `build-status.json`'s `board` field must be a not-run value (missing/`not_run`/`not_started`/`never_run`/`skipped`/`n/a`), because the generated README asserts v6 was not board tested.
9. **Executable bit**: this proposal cannot set mode. Root should `chmod 0755 out/e3-controlled-20260918/finalize_delivery.py`.
10. **No execution performed here** — no assertion in this answer claims any test, hash, or build result.

## Unresolved / needs Codex or root

- Whether `E3_QUEUE_PAIR.md` / `E3_SERVICE.md` / `E3_DIAGNOSTIC.md` exist at repo root (the protocol links them as `../…`); the script fails if a linked doc is absent.
- Whether the FPGA manifest really lists the software artifacts as still-current, i.e. which entries land in `cross_action_manifest_notes`.
- Whether root records `fpga_exit_code` / `software_exit_code` at top level of `build-status.json` exactly as assumed.
- `platform.py` `manifest()` uses `platform_commit`/`platform_dirty` for root; the script never invokes git, so current-HEAD drift is only visible through those recorded fields.

## Suggested acceptance checks (for root to run)

```bash
python3 -m py_compile out/e3-controlled-20260918/finalize_delivery.py   # syntax only
ls -l out/e3-controlled-20260918/finalize_delivery.py                   # then chmod 0755

# dry expectations before running for real
python3 - <<'PY'
import json,pathlib
p=pathlib.Path('out/e3-controlled-20260918')
print(json.loads((p/'build-status.json').read_text()))       # fpga/software == passed, exit codes 0
print(json.loads((p/'validation.json').read_text())['status'])
PY

# real run (only after both builds truly finished)
./out/e3-controlled-20260918/finalize_delivery.py
# then, from inside the delivery:
cd out/e3-controlled-20260918/delivery && sha256sum -c SHA256SUMS
```

Expected post-conditions: stdout JSON with `"status": "passed"`; `delivery/` created atomically with `README.md`, `delivery.json`, `SHA256SUMS`, `docs/e3-queue-results-review.md` and all listed artifacts; no second `delivery/` ever created; `build-status.json` keeps `fpga`/`software` = `passed` plus the new hash fields.