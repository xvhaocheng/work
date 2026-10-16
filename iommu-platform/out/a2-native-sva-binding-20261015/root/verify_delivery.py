#!/usr/bin/env python3
"""Check this bounded delivery; never runs DMA, RTL, or prior validators."""
from pathlib import Path
import hashlib
import json
import re
import struct

OUT = Path(__file__).resolve().parents[1]
PLATFORM = OUT.parents[1]
WORK = PLATFORM.parent
checks = []

def check(name, condition):
    checks.append({'name': name, 'pass': bool(condition)})

def read_json(path):
    return json.loads(path.read_text())

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

source_sets = [
    ('public originals', read_json(OUT / 'sources/date-raw/file-manifest.json')),
    ('DATE key sources', read_json(OUT / 'sources/date-software.json')['key_sources']),
    ('current interface', list(read_json(OUT / 'interface/publisher-owner.json')['sources'].values())),
    ('current software', read_json(OUT / 'platform/software-binding.json')['source_snapshot']['files']),
    ('root critical review', read_json(OUT / 'root/source-evidence.json')['sources']),
]
source_counts = {}
for label, rows in source_sets:
    source_counts[label] = len(rows)
    for row in rows:
        path = Path(row.get('local_path') or row.get('absolute_path') or row['path'])
        if not path.is_absolute():
            path = PLATFORM / path
        check(f'{label}: {path}', path.is_file() and digest(path) == row['sha256'])

before = read_json(OUT / 'before/source-manifest.json')
for row in before:
    if row['path'] in ('AGENTS.md', 'iommu-platform/AGENTS.md',
                       'iommu-platform/sources.lock.json',
                       'iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json'):
        check('preserved ' + row['path'], digest(WORK / row['path']) == row['sha256'])
for file in ('HANDOFF.md', 'iommu-platform/docs/validation.md'):
    check('old bytes retained ' + file,
          (WORK / file).read_bytes().endswith((OUT / 'before' / file).read_bytes()))
old_status = (OUT / 'before/CURRENT_STATUS.md').read_text()
old_tail = old_status[old_status.index('- 独立Linux贡献：'):]
check('unrelated current contributions retained', (WORK / 'CURRENT_STATUS.md').read_text().endswith(old_tail))

task = read_json(OUT / 'design/native-task-input.json')
blob = (OUT / 'design/three-frames.bin').read_bytes()
pos = 0
for p, length in enumerate(task['packet_lengths']):
    cookie, actual_length = struct.unpack_from('<QI', blob, pos)
    pos += 12
    check(f'functional input packet {p}', cookie == p + 1 and actual_length == length and
          blob[pos:pos + length] == bytes((17 * p + q) & 255 for q in range(length)))
    pos += length
check('functional input complete bytes/hash', pos == len(blob) == 1675 and
      sum(task['packet_lengths']) == 1639 and digest(OUT / 'design/three-frames.bin') == task['input_sha256'])
descriptors = task['descriptors']
check('functional descriptor payload', sum(d['bytes'] for d in descriptors) == 1639)
check('functional page and permission scope', task['arena_bytes'] == 32768 and all(
    0 <= d['src_offset'] < d['src_offset'] + d['bytes'] <= 4 * 4096 and
    4 * 4096 <= d['dst_offset'] < d['dst_offset'] + d['bytes'] <= 7 * 4096 and
    d['did'] == 10 + d['lane'] for d in descriptors))
check('no extra permits or translation feedback', task['publication']['extra_payload_permits'] == 0
      and task['publication']['software_pending'] == 0 and not task['publication']['translation_feedback'])

result = read_json(OUT / 'root-result-acceptance.json')
contract = read_json(OUT / 'validation/full-task-contract.json')
check('no false native/timing acceptance', not result['delivered']['native_adapter_implemented']
      and not result['delivered']['full_time_backend_implemented']
      and all(not c['frozen'] and c['prediction'] is None and c['executions'] == 0
              for c in contract['time_cases'].values()))
check('no new old experiment or hardware execution', all(result['execution'][k] == 0 for k in (
    'new_semantic_episodes', 'timing_runs', 'RTL_builds', 'FPGA_builds', 'board_operations',
    'production_modifications', 'DeepSeek_calls', 'Claude_calls')))
check('MIG paused/debt unknown', result['inherited']['MIG_status'] == 'paused' and result['inherited']['MIG_debt'] is None)
check('paper and independent novelty pending', not result['inherited']['paper_complete'] and
      not result['novelty']['independent_contribution_accepted'])

authored = [WORK / f for f in ('CURRENT_STATUS.md', '研究计划.md', 'HANDOFF.md')]
authored += [PLATFORM / 'docs/a2-native-sva-binding-20261015.md',
             PLATFORM / 'docs/research-execution-plan-20261011.md', PLATFORM / 'docs/validation.md']
authored += [OUT / f for f in ('README.md', 'design/method-increment.md', 'design/minimal-sva-adaptation.md',
                              'sources/date-software.md', 'platform/software-binding.md', 'interface/publisher-owner.md')]
link_count = 0
for path in authored:
    body = path.read_text()
    if path.name in ('HANDOFF.md', 'validation.md'):
        body = body.split('以下旧交接字节保留。')[0] if path.name == 'HANDOFF.md' else body.split('以下旧验证原文字节保留。')[0]
    for link in re.findall(r'\[[^\]]*\]\(([^)]+)\)', body):
        if re.match(r'^(?:https?://|#|codex:)', link):
            continue
        target = link.split('#')[0]
        if not target:
            continue
        link_count += 1
        f = path.parent / target
        # The delivery receipt is the output of this exact checker.
        check(f'local link {path.name}: {link}', f.exists() or f.resolve() == OUT / 'root/delivery-verification.json')

receipt = {'schema': 'a2-delivery-verification-v1', 'scope': 'source hashes, task input arithmetic, provenance/status/link preservation only; no execution of new or old research tasks',
           'source_record_counts': source_counts, 'local_links_checked': link_count,
           'checks': checks, 'passed': sum(c['pass'] for c in checks),
           'failed': sum(not c['pass'] for c in checks)}
(OUT / 'root/delivery-verification.json').write_text(json.dumps(receipt, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({k: v for k, v in receipt.items() if k != 'checks'}, ensure_ascii=False))
for c in checks:
    if not c['pass']:
        print('FAILED:', c['name'])
raise SystemExit(1 if receipt['failed'] else 0)
