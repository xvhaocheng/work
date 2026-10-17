#!/usr/bin/env python3
"""Freeze the bounded local sources used by the read-only generality check."""
from pathlib import Path
import datetime
import hashlib
import json
import subprocess

ROOT = Path('/home/xvhaocheng/work')
OUT = Path(__file__).resolve().parent
SOURCES = {
    'cva6_sv32': ('iommu-platform/cva6/core/mmu_sv32/cva6_tlb_sv32.sv', [(18, 57), (119, 139), (141, 216)]),
    'cva6_sv39': ('iommu-platform/cva6/core/mmu_sv39/tlb.sv', [(18, 57), (129, 150), (152, 228)]),
    'cva6_sv39x4': ('iommu-platform/cva6/core/mmu_sv39x4/cva6_tlb_sv39x4.sv', [(18, 78), (246, 267), (272, 348)]),
    'local_iotlb': ('iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv', [(22, 55), (103, 136), (303, 332), (336, 425)]),
    'linux_dma_iommu': ('iommu-platform/linux/drivers/iommu/dma-iommu.c', [(613, 621), (658, 754), (1015, 1087), (1588, 1603)]),
    'linux_iova': ('iommu-platform/linux/drivers/iommu/iova.c', [(178, 236), (439, 493), (697, 718), (839, 888)]),
    'linux_riscv_iommu': ('iommu-platform/linux/drivers/iommu/riscv/iommu.c', [(1198, 1210), (1777, 1817), (1841, 1846)]),
    'linux_idma': ('iommu-platform/linux/drivers/misc/idma.c', [(242, 268), (354, 399)]),
    'historical_idma': ('iommu-platform/out/dma-cache-sync-20260916/idma-before.c', [(157, 182), (264, 309)]),
    'linux_iova_mismatch': ('iommu-platform/out/dma-smoke-debug-20260916/mac-20260916-020422-run-_hjma4td/mismatch.txt', [(1, 1)]),
}

def digest(b):
    return hashlib.sha256(b).hexdigest()

def git(repo, *args):
    return subprocess.check_output(['git', '-C', str(ROOT / repo), *args], text=True).strip()

records = []
snippets = ['# 只读源码片段', '', '行号对应原始文件和同字节快照，不是本文件行号。', '']
for name, (rel, spans) in SOURCES.items():
    src = ROOT / rel
    data = src.read_bytes()
    dst = OUT / 'raw/local' / rel
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_bytes(data)
    lines = data.decode().splitlines()
    records.append({'id': name, 'original_path': rel, 'snapshot': str(dst.relative_to(OUT)),
                    'sha256': digest(data), 'line_count': len(lines), 'examined_spans': spans})
    repos = ['iommu-platform/cva6/corev_apu/riscv-iommu', 'iommu-platform/cva6', 'iommu-platform/linux']
    repo = next((p for p in repos if rel.startswith(p+'/')), None)
    if repo:
        tracked_path = rel[len(repo)+1:]
        committed = subprocess.check_output(['git', '-C', str(ROOT/repo), 'show', 'HEAD:'+tracked_path])
        records[-1]['working_tree_matches_HEAD_bytes'] = committed == data
        records[-1]['head_file_sha256'] = digest(committed)
    snippets += [f'## {name}', '', f'原件：`{rel}`；SHA-256 `{digest(data)}`。', '']
    for lo, hi in spans:
        snippets += [f'原件 L{lo}–L{hi}：', '', '```text']
        snippets += [f'{i}: {lines[i-1]}' for i in range(lo, min(hi, len(lines)) + 1)]
        snippets += ['```', '']
up = OUT / 'raw/upstream/rtl/translation_logic/rv_iommu_iotlb_sv39x4.sv'
lines = up.read_text().splitlines()
snippets += ['## upstream_iotlb', '', '官方固定版本见 upstream-source-receipt.json。', '']
for lo, hi in [(14, 68), (347, 377), (380, 456)]:
    snippets += [f'原件 L{lo}–L{hi}：', '', '```text']
    snippets += [f'{i}: {lines[i-1]}' for i in range(lo, hi+1)]
    snippets += ['```', '']
manifest = {'recorded_at_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
            'scope': 'source inspection and existing historical IOVA record only; no experiment',
            'repositories': [], 'sources': records}
for repo in ['iommu-platform/cva6', 'iommu-platform/cva6/corev_apu/riscv-iommu', 'iommu-platform/linux']:
    manifest['repositories'].append({'path': repo, 'head': git(repo, 'rev-parse', 'HEAD'),
                                    'identity_note': 'HEAD identifies history; file SHA-256 identifies inspected working-tree bytes'})
(OUT / 'source-manifest.json').write_text(json.dumps(manifest, ensure_ascii=False, indent=2)+'\n')
(OUT / 'source-excerpts.md').write_text('\n'.join(snippets)+'\n')
check = {r['id']: digest((OUT/r['snapshot']).read_bytes()) == r['sha256'] for r in records}
(OUT / 'local-hash-check.json').write_text(json.dumps(check, indent=2)+'\n')
print(json.dumps({'files': len(records), 'all_snapshot_hashes_match': all(check.values())}))
