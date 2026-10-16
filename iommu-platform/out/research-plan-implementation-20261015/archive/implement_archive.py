#!/usr/bin/env python3
"""One bounded documentation migration; no builds, tests or old output writes."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import os
import re
import stat
from urllib.parse import unquote

ROOT = Path('/home/xvhaocheng/work')
PLATFORM = ROOT / 'iommu-platform'
DOCS = PLATFORM / 'docs'
DEST = DOCS / 'history/plans'
OUT = PLATFORM / 'out/research-plan-implementation-20261015/archive'
INPUT = PLATFORM / 'out/research-plan-implementation-20261015/root/input-manifest.json'
INPUT_DATA = json.loads(INPUT.read_text())
NAMES = INPUT_DATA['archive_names']
EXPECTED_NAMES = [
    'research-convergence-plan-20261010.md', 'next-mechanism-plan.md',
    'e3-next-method-plan.md', 'experiment-plan.md',
    'experiment-implementation-plan.md', 'e3-shared-service-plan.md',
    'e4-supply-timing-plan.md', 'nonblock-pilot-plan-20261004.md',
    'nonblock-v5-qualified-read-plan-20261004.md',
    'status-stage94-before-main-plan-20261010-root.md',
    'status-stage95-before-comprehensive-plan-20261010-root.md',
    'status-stage95-before-plan-review-20261010.md',
]
assert NAMES == EXPECTED_NAMES
SYMLINKS = {'experiment-plan.md', 'nonblock-pilot-plan-20261004.md'}
MAINTAINED = [
    'e1-results-review.md', 'e3-controlled-results-review.md',
    'experiment-design-v1.md', 'experiment-instrumentation.md',
    'experiment-v1-presentation.md', 'nonblock-dependency-diagnosis-20261004.md',
    'paper-logic.md', 'paper-scenarios-mitigation.md', 'path-results-review.md',
    'research-n-prototype-spec-20261004.md', 'research-overnight-progress-20261004.md',
    'research-review-2026-09-09.md', 'workflow-framing-20261010-root.md',
]
PURPOSES = {
    NAMES[0]: '2026-10-10研究收敛与前作/场景设计，已被当前总计划取代',
    NAMES[1]: '等待定位、共享读与改善方法的多代历史执行路线',
    NAMES[2]: 'P4M共同窗口、固定间隔和pilot阶段历史计划/测量合同',
    NAMES[3]: 'E0至E4科学判据及2026-10-02冻结84次实验规格，原文保留',
    NAMES[4]: '早期P0至P5平台实施工作包与验证分层',
    NAMES[5]: 'E3最小因果干预的事前实验卡及逐代修订',
    NAMES[6]: 'E4供给、完成观察及测量开销的事前实验卡',
    NAMES[7]: 'nonblock冻结15-case运行计划、v4追加合同，保留行号兼容',
    NAMES[8]: 'nonblock v5资格、安全范围、冻结对照及实际暂停结果',
    NAMES[9]: 'stage94原样状态快照，保留旧引用及原始错误',
    NAMES[10]: 'stage95全面计划前原样状态快照，保留旧引用及原始错误',
    NAMES[11]: 'stage95计划审查前原样状态快照，保留旧引用及原始错误',
}
LINK_RE = re.compile(r'\[[^\]]*\]\(([^)\n]+)\)')
TARGET_MAP = {str(DOCS / n): DEST / n for n in NAMES}
EXPECTED = {x['path']: x for x in INPUT_DATA['files']}
NOW = datetime.now(timezone.utc).isoformat()


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def rel(path, base=ROOT):
    return os.path.relpath(path, base)


def write_json(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n')


def parse_local(value, base):
    value = value.strip()
    if re.match(r'^[a-zA-Z][a-zA-Z0-9+.-]*:', value) and not value.startswith('/'):
        return None
    if value.startswith('#'):
        return None
    assert '"' not in value, ('unexpected Markdown title', value)
    if value.startswith('<') and value.endswith('>'):
        value = value[1:-1]
    address, sep, fragment = value.partition('#')
    suffix = ('#' + fragment) if sep else ''
    address = unquote(address)
    line = re.search(r'(:\d+(?::\d+)?)$', address)
    if line:
        suffix = line.group(1) + suffix
        address = address[:line.start()]
    if not address:
        return None
    path = Path(address) if address.startswith('/') else base / address
    return Path(os.path.abspath(path)), suffix


def migrated(path):
    return TARGET_MAP.get(str(path), path)


def strip_targets(text):
    return LINK_RE.sub(lambda m: m.group(0)[:m.start(1)-m.start()] + '<TARGET>' + m.group(0)[m.end(1)-m.start():], text)


# Read and validate all scoped inputs before any source mutation.
source = {}
drift = []
for name in NAMES:
    path = DOCS / name
    assert path.is_file() and not path.is_symlink(), path
    assert not (DEST / name).exists(), DEST / name
    raw = path.read_bytes()
    expected = EXPECTED[rel(path)]
    actual = sha(raw)
    snapshot = ROOT / expected['snapshot']
    assert snapshot.is_file() and sha(snapshot.read_bytes()) == expected['sha256']
    if actual != expected['sha256']:
        before_at_write = OUT / 'before-at-write' / rel(path)
        before_at_write.parent.mkdir(parents=True, exist_ok=True)
        before_at_write.write_bytes(raw)
        drift.append({'path': rel(path), 'expected_sha256': expected['sha256'],
                      'actual_sha256': actual, 'before_at_write': rel(before_at_write)})
    source[name] = {'raw': raw, 'mode': stat.S_IMODE(path.stat().st_mode),
                    'sha256': actual, 'bytes': len(raw), 'lines': len(raw.splitlines()),
                    'matches_root_input': actual == expected['sha256']}

maintenance = {}
changes = []
for name in MAINTAINED:
    path = DOCS / name
    raw = path.read_bytes()
    text = raw.decode()
    rows = []
    def replace_link(match):
        parsed = parse_local(match.group(1), path.parent)
        if parsed is None or str(parsed[0]) not in TARGET_MAP:
            return match.group(0)
        target, suffix = parsed
        new_target = rel(migrated(target), path.parent) + suffix
        line = text.count('\n', 0, match.start(1)) + 1
        rows.append({'line': line, 'before_target': match.group(1), 'after_target': new_target})
        start = match.start(1) - match.start()
        end = match.end(1) - match.start()
        return match.group(0)[:start] + new_target + match.group(0)[end:]
    new = LINK_RE.sub(replace_link, text).encode()
    assert strip_targets(text) == strip_targets(new.decode()), path
    assert len(raw.splitlines()) == len(new.splitlines()), path
    maintenance[name] = {'before': raw, 'after': new, 'rows': rows}

# Preserve all old Markdown link bases in a separate, clickable archive index.
link_rows = []
for name in NAMES:
    path = DOCS / name
    text = source[name]['raw'].decode()
    for m in LINK_RE.finditer(text):
        parsed = parse_local(m.group(1), DOCS)
        if parsed is None:
            continue
        target, suffix = parsed
        existed = target.exists()
        status = 'valid_before_move' if existed else 'preexisting_broken'
        candidate = target if existed else None
        if not existed and name.startswith('status-stage'):
            raw_path = m.group(1).split('#', 1)[0]
            if raw_path in ('研究计划.md', '研究方法.md'):
                possible = ROOT / raw_path
            elif raw_path.startswith('iommu-platform/'):
                possible = ROOT / raw_path
            else:
                possible = None
            if possible is not None and possible.exists():
                candidate = possible
        locator = migrated(candidate) if candidate is not None else None
        link_rows.append({
            'source': rel(DEST / name),
            'source_original_path': rel(path),
            'source_line': text.count('\n', 0, m.start(1)) + 1,
            'original_destination': m.group(1),
            'kind': 'absolute' if m.group(1).startswith('/') else 'relative',
            'original_relative_base': rel(DOCS),
            'original_resolved_path': rel(target),
            'status': status,
            'corrected_candidate_only': status == 'preexisting_broken',
            'locator_path': rel(locator) if locator is not None else None,
            'archive_index_href': (rel(locator, DEST) + suffix) if locator is not None else None,
            'preserved_fragment_or_line': suffix,
        })
assert sum(r['status'] == 'preexisting_broken' for r in link_rows) == 14
assert all(r['locator_path'] for r in link_rows), 'Unresolved local link'

OUT.mkdir(parents=True, exist_ok=True)
DEST.mkdir(parents=True, exist_ok=True)
write_json(OUT / 'source-check-before-move.json', {
    'created_utc': NOW, 'root_input_manifest': rel(INPUT), 'drift': drift,
    'files': [{k:v for k,v in source[n].items() if k not in ('raw','mode')} | {'name':n} for n in NAMES],
})

# Actual moves preserve the current worktree bytes. No git checkout or staging.
for name in NAMES:
    old = DOCS / name
    assert sha(old.read_bytes()) == source[name]['sha256'], ('concurrent source write', old)
    old.rename(DEST / name)
for name in sorted(SYMLINKS):
    (DOCS / name).symlink_to(Path('history/plans') / name)

for name, data in maintenance.items():
    if not data['rows']:
        continue
    path = DOCS / name
    assert path.read_bytes() == data['before'], ('concurrent report write', path)
    before = OUT / 'before-maintained' / rel(path)
    before.parent.mkdir(parents=True, exist_ok=True)
    before.write_bytes(data['before'])
    path.write_bytes(data['after'])
    changes.append({
        'path': rel(path), 'before_snapshot': rel(before),
        'before_sha256': sha(data['before']), 'after_sha256': sha(data['after']),
        'lines_before': len(data['before'].splitlines()),
        'lines_after': len(data['after'].splitlines()),
        'only_Markdown_targets_changed': strip_targets(data['before'].decode()) == strip_targets(data['after'].decode()),
        'changes': data['rows'],
    })

files = []
for name in NAMES:
    data = source[name]
    new = DEST / name
    old = DOCS / name
    successor = ROOT / ('CURRENT_STATUS.md' if name.startswith('status-stage') else '研究计划.md')
    files.append({
        'original_path': rel(old), 'archived_path': rel(new),
        'sha256': data['sha256'], 'bytes': data['bytes'], 'lines': data['lines'],
        'original_relative_link_base': rel(DOCS), 'purpose': PURPOSES[name],
        'successor': rel(successor), 'successor_index_href': rel(successor, DEST),
        'matches_root_input_snapshot': data['matches_root_input'],
        'compatibility': {'type': 'relative_symlink', 'target': os.readlink(old)} if name in SYMLINKS else {'type':'original_path_removed'},
        'archive_index_link_table': f'README.md#原链接定位表-{NAMES.index(name)+1:02d}',
    })
manifest = {
    'schema': 'research-plan-physical-archive-v1', 'created_utc': NOW,
    'archive_root': rel(DEST), 'root_input_manifest': rel(INPUT),
    'byte_policy': 'current-worktree original bytes, no added banner or link rewrite in archived source',
    'line_policy': 'all original physical line numbers unchanged',
    'relative_link_policy': 'original link base recorded; archive README provides each local link resolved from original base; candidate fixes do not modify originals',
    'historical_reference_policy': 'frozen HANDOFF, validation, other status snapshots and old output left unchanged; archive index records relocations',
    'model_scope': 'Codex-only; no external model, builds, tests, experiments or board work',
    'files': files, 'links': link_rows,
    'preexisting_broken_count': 14,
    'preexisting_broken_policy': 'only archived status snapshot links; corrected locator is explicitly a candidate, original text unchanged',
}
write_json(DEST / 'manifest.json', manifest)
write_json(OUT / 'maintenance-link-changes.json', {'schema':'plan-archive-maintained-links-v1','created_utc':NOW,'files':changes})

body = [
    '# 历史研究计划与实验规格\n\n',
    '2026年10月15日集中归档。这里保留12份历史计划/状态快照的工作树原始字节；它们不再是当前执行入口，也不因归档恢复实验或改变科学结论。\n\n',
    f'当前方向与阶段安排见[研究计划]({rel(ROOT / "研究计划.md", DEST)})；实际状态见[CURRENT_STATUS]({rel(ROOT / "CURRENT_STATUS.md", DEST)})。D主线与I轻量预筛是研究安排，不是创新已通过。A5局部合同保留在[原独立入口]({rel(DOCS / "research-execution-plan-20261011.md", DEST)})；MIG暂停、未取得新采集与债务unknown保持。\n\n',
    '## 归档清单\n\n',
    '| 原件 | 用途 | 原行数 | 兼容安排 |\n| --- | --- | ---: | --- |\n',
]
for i, item in enumerate(files, 1):
    name = Path(item['archived_path']).name
    compat = '原路径相对符号链接，保留行号引用' if name in SYMLINKS else '原路径移除；在本页查迁移定位'
    body.append(f'| [{name}]({name}) | {item["purpose"]} | {item["lines"]} | {compat} |\n')
body += [
    '\n原/新完整路径、SHA256、字节数、行数、后继及逐链接定位见[manifest.json](manifest.json)。所有原件SHA256与迁移前当前工作树一致。维护报告只调整Markdown链接目标，原件和逐项变化见',
    f'[维护链接清单]({rel(OUT / "maintenance-link-changes.json", DEST)})；[迁移验收]({rel(OUT / "archive-acceptance.json", DEST)})。\n\n',
    '## 阅读原文与历史引用\n\n',
    '原件没有添加说明行或重写相对链接，原行号和哈希保持。因此直接从新目录阅读原文时，其旧相对链接仍按原目录`iommu-platform/docs/`解释；请使用下方对应定位表。表中的链接已依据原基目录转换到新位置，不假称原件内的相对链接已经改好。\n\n',
    '仅保留两个旧路径相对符号链接：`docs/experiment-plan.md`（HANDOFF原文曾引`:57`）与`docs/nonblock-pilot-plan-20261004.md`（v5计划曾引`:115`及`:109–124`）。其余10个旧路径不设空壳。冻结HANDOFF、validation、其他status快照及旧out中的字面路径不改，按本清单定位。\n\n',
    '三份stage94/95原样状态快照中原有14个相对坏链，以下标成`preexisting_broken`；候选纠正依据是其原工作区根入口上下文，仅用于导航，不回写旧文、不改旧接受hash。路径存在性验收不等于全部标题锚点或论文/实验结论再次验收。\n',
]
for i, item in enumerate(files, 1):
    name = Path(item['archived_path']).name
    body += [f'\n## 原链接定位表 {i:02d}\n\n', f'原件：[{name}]({name})；原基目录：`iommu-platform/docs/`。\n\n',
             '| 原行 | 原链接目标 | 状态 | 正确定位/候选纠正 |\n| ---: | --- | --- | --- |\n']
    for row in link_rows:
        if row['source'] != item['archived_path']:
            continue
        label = '候选纠正' if row['corrected_candidate_only'] else '打开'
        body.append(f'| {row["source_line"]} | `{row["original_destination"].replace("|", "&#124;")}` | `{row["status"]}` | [{label}]({row["archive_index_href"]}) |\n')
(DEST / 'README.md').write_text(''.join(body))

# Direct bounded file/link validation; no repository test suite or build.
checks = []
for name in NAMES:
    new = DEST / name
    raw = new.read_bytes()
    checks.append({'name':name, 'hash_unchanged':sha(raw)==source[name]['sha256'],
                   'bytes_unchanged':len(raw)==source[name]['bytes'],
                   'lines_unchanged':len(raw.splitlines())==source[name]['lines']})
    assert all(checks[-1][k] for k in ('hash_unchanged','bytes_unchanged','lines_unchanged'))
symlink_checks = []
for name in sorted(SYMLINKS):
    old, new = DOCS / name, DEST / name
    row = {'path':rel(old),'is_symlink':old.is_symlink(),'target':os.readlink(old),
           'same_file':old.samefile(new),'same_bytes':old.read_bytes()==new.read_bytes(),
           'same_lines':len(old.read_bytes().splitlines())==source[name]['lines']}
    assert all(row[k] for k in ('is_symlink','same_file','same_bytes','same_lines'))
    symlink_checks.append(row)
removed = [{'path':rel(DOCS/n),'absent':not os.path.lexists(DOCS/n)} for n in NAMES if n not in SYMLINKS]
assert len(removed)==10 and all(x['absent'] for x in removed)
link_validation = []
for row in link_rows:
    path = ROOT / row['locator_path']
    ok = path.exists()
    assert ok, path
    link_validation.append({'source':row['source'],'line':row['source_line'],
                            'status':row['status'],'locator_exists':ok})
for item in changes:
    assert item['only_Markdown_targets_changed']
    for row in item['changes']:
        target, suffix = parse_local(row['after_target'], (ROOT/item['path']).parent)
        assert target.exists(), target
readme_local = []
for m in LINK_RE.finditer((DEST/'README.md').read_text()):
    parsed = parse_local(m.group(1), DEST)
    if parsed:
        path, suffix = parsed
        # The final acceptance is written immediately below.
        ok = path.exists() or path == OUT/'archive-acceptance.json'
        assert ok, path
        readme_local.append({'target':m.group(1),'exists_or_final_receipt':ok})
acceptance = {
    'schema':'research-plan-archive-acceptance-v1','created_utc':datetime.now(timezone.utc).isoformat(),
    'status':'pass','archive_root':rel(DEST),'manifest':rel(DEST/'manifest.json'),
    'root_input_manifest':rel(INPUT),'source_matches_root_input':sum(x['matches_root_input'] for x in source.values()),
    'source_drift':drift,'archived_files':len(NAMES),'archived_bytes':sum(x['bytes'] for x in source.values()),
    'archived_lines':sum(x['lines'] for x in source.values()),'preservation_checks':checks,
    'compatibility_symlinks':symlink_checks,'removed_original_paths':removed,
    'maintained_reports':len(changes),'Markdown_link_target_changes':sum(len(x['changes']) for x in changes),
    'maintenance_only_targets_and_line_counts_unchanged':all(x['only_Markdown_targets_changed'] and x['lines_before']==x['lines_after'] for x in changes),
    'maintenance_changes':rel(OUT/'maintenance-link-changes.json'),
    'indexed_relative_links':sum(x['kind']=='relative' for x in link_rows),
    'indexed_absolute_links':sum(x['kind']=='absolute' for x in link_rows),
    'preexisting_broken_links':14,'candidate_corrections_with_existing_targets':sum(x['status']=='preexisting_broken' for x in link_rows),
    'locator_checks':link_validation,'readme_checks':readme_local,
    'scope':{'old_out_writes':0,'root_entry_writes':0,'HANDOFF_writes':0,'validation_writes':0,
             'other_status_snapshot_writes':0,'A5_local_contract_writes':0,'builds':0,'tests':0,
             'experiments':0,'board_actions':0,'external_models':0},
    'limitations':[
        'Archived original inline relative links remain byte-for-byte; use README link maps based on original docs directory.',
        'Fourteen preexisting status snapshot broken links are labelled and only candidate navigational corrections are supplied.',
        'Frozen historical literal old paths are not globally rewritten; archive manifest records relocation and two line-cited paths retain symlinks.',
        'Existence checks do not validate every Markdown heading anchor or re-accept scientific results.',
        'No old output, dependency tree or full repository scan; former bounded reference search found no runner/test/bench/config/Makefile basename references.'
    ],
}
write_json(OUT/'archive-acceptance.json',acceptance)
print(json.dumps({k:acceptance[k] for k in ('status','archived_files','archived_bytes','archived_lines','source_matches_root_input','maintained_reports','Markdown_link_target_changes','indexed_relative_links','indexed_absolute_links','preexisting_broken_links')},ensure_ascii=False,indent=2))
print('receipt',rel(OUT/'archive-acceptance.json'))
