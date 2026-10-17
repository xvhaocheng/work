#!/usr/bin/env python3
"""Render/check the historical ledger. Never execute experiment code.

The reviewed catalog is the input; discovery is deliberately separate. --check
also refuses new, unassigned scope units and changed quoted source bytes.
"""
import argparse
import csv
import hashlib
import io
import json
import os
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT / 'docs/experiment-ledger'


def sha(path):
    return hashlib.sha256((ROOT / path).read_bytes()).hexdigest()


def scope_units():
    units = {}
    for base in ['iommu-platform/out', 'paper/ispass2027/board/runs',
                 'paper/ispass2027/exp', 'platform-archives']:
        for p in sorted((ROOT / base).iterdir()):
            if p.is_dir():
                units[p.relative_to(ROOT).as_posix()] = 'directory'
    for base in ['iommu-platform', 'iommu-platform/docs', 'analysis', 'paper/carrv']:
        if (ROOT / base).exists():
            for p in sorted((ROOT / base).glob('*.md')):
                units[p.relative_to(ROOT).as_posix()] = 'report'
    for p in sorted((ROOT / 'iommu-platform/docs/history/plans').glob('*.md')):
        units[p.relative_to(ROOT).as_posix()] = 'historical_plan'
    for name in ['研究总览_2026-09-08.md', 'MAC_FPGA_HANDOFF.md', 'HANDOFF.md',
                 'iommu-platform/docs/status-history-through-stage84-20261009.md']:
        if (ROOT / name).exists():
            units[name] = 'historical_summary'
    return units


def csv_bytes(rows, fields):
    f = io.StringIO(newline='')
    w = csv.DictWriter(f, fields, lineterminator='\n')
    w.writeheader()
    w.writerows(rows)
    return f.getvalue().encode()


def link(path, label=None):
    # Repository-relative links, usable in GitHub and the desktop app.
    return f'[{label or path}](../../{path})'


def render(data):
    records = data['records']
    by_id = {r['id']: r for r in records}
    assert len(by_id) == len(records), 'duplicate record ID'
    units = scope_units()
    assigned = {x['path']: x for x in data['coverage']}
    assert set(units) == set(assigned), (
        f'scope changed: unassigned={sorted(set(units)-set(assigned))}; '
        f'removed={sorted(set(assigned)-set(units))}')
    for u in assigned.values():
        assert u['record_ids'] or u['reason'], u['path']
        assert set(u['record_ids']) <= set(by_id), u['path']
    for r in records:
        for k in ['question', 'result', 'conclusion', 'limits', 'evidence_level', 'status']:
            assert r[k], (r['id'], k)
        for p in r['artifacts']:
            assert (ROOT / p).exists(), p
        for s in r['sources']:
            assert sha(s['path']) == s['sha256'], f'source changed: {s["path"]}'
            lines = (ROOT / s['path']).read_text(errors='replace').splitlines()
            for q in s.get('quotes', []):
                assert '\n'.join(lines[q['start']-1:q['end']]) == q['text'], (r['id'], s['path'], q['start'])
        for edge in r['relations']:
            assert edge['target'] in by_id and edge['target'] != r['id'], edge

    out = {}
    table = ['# 历史实验与研究轮次总账', '',
             '查找顺序：关键词／别名 → 本表轮次 → 详细卡片 → 原报告、验收及原始数据。'
             '一行是具名轮次；板测、RTL、条件模型、来源核查、构建和未执行计划分别标注。'
             '目录、配置、标量行、模型事件和原始文件数均不是独立实验次数。', '',
             '本账为现存仓库范围的整理，不重新验证历史实验；完整范围和缺口见 '
             '[覆盖说明](README.md#覆盖与缺口)。后续修正具有单独关系，原始报告和数据不改写。', '',
             '| ID／关键词 | 日期 | 证据层／执行状态 | 结果与结论 | 后续关联 |',
             '| --- | --- | --- | --- | --- |']
    groups = {}
    flat = []
    for r in sorted(records, key=lambda r: (r['date_sort'], r['id'])):
        group = r['period']
        groups.setdefault(group, []).append(r)
        relation_text = '；'.join(f'{e["type"]}→{e["target"]}' for e in r['relations']) or '—'
        esc = lambda x: str(x).replace('|', '\\|').replace('\n', ' ')
        table.append(f'| [{r["id"]}](periods/{group}.md#{r["id"].lower()}) {esc(r["title"])} | '
                     f'{esc(r["date"])} | {esc(r["evidence_level"])}；{esc(r["status"])} | '
                     f'{esc(r["result"])} {esc(r["conclusion"])} | {esc(relation_text)} |')
        flat.append({k: r[k] for k in ['id', 'title', 'date', 'record_type', 'evidence_level', 'status', 'question',
                                     'result', 'conclusion', 'limits']} | {
            'aliases': ';'.join(r['aliases']), 'artifacts': ';'.join(r['artifacts']),
            'sources': ';'.join(s['path'] for s in r['sources']), 'relations': relation_text})
    out['index.md'] = ('\n'.join(table) + '\n').encode()
    out['experiments.csv'] = csv_bytes(flat, list(flat[0]))
    for group, rr in groups.items():
        md = [f'# {group} 实验与研究轮次', '',
              '[总账索引](../index.md) · [修正关系](../corrections.md)', '']
        for r in rr:
            md.extend([f'<a id="{r["id"].lower()}"></a>', f'## {r["id"]} {r["title"]}', '',
                       f'日期：{r["date"]}。证据层：{r["evidence_level"]}。状态：{r["status"]}。', '',
                       f'**问题：** {r["question"]}', '', f'**已报告结果：** {r["result"]}', '',
                       f'**结论：** {r["conclusion"]}', '', f'**适用范围与缺口：** {r["limits"]}', ''])
            if r['aliases']:
                md.extend(['检索别名：' + '、'.join(r['aliases']) + '。', ''])
            for s in r['sources']:
                md.extend([f'原文：[{s["path"]}](../../../{s["path"]})；本次取证 SHA256 `{s["sha256"]}`。', ''])
                for q in s.get('quotes', []):
                    md.extend([f'原文摘录 L{q["start"]}–L{q["end"]}（{s["review_scope"]}）：', '',
                               '> ' + q['text'].replace('[', '\\[').replace('\n', '\n> '), ''])
            if r['artifacts']:
                md.extend(['产物与原始记录入口：' + '、'.join(
                    f'[{p}](../../../{p})' for p in r['artifacts']) + '。', ''])
            if r.get('identity_refs'):
                md.extend(['历史身份：' + '；'.join(
                    f'[{i["record_id"]}](../../../{i["path"]})' for i in r['identity_refs']) +
                    '。保留原记录的 unknown；后来的归档提交不替代执行版本。', ''])
            for e in r['relations']:
                target = by_id[e['target']]
                md.extend([f'**{e["type"]}**：[{e["target"]}]({target["period"]}.md#{e["target"].lower()}) — {e["reason"]}', ''])
        out[f'periods/{group}.md'] = ('\n'.join(md).rstrip() + '\n').encode()

    coverage = [{'path': u['path'], 'unit_type': units[u['path']], 'classification': u['classification'],
                 'record_ids': ';'.join(u['record_ids']), 'reason': u['reason']}
                for u in sorted(data['coverage'], key=lambda u: u['path'])]
    out['coverage.csv'] = csv_bytes(coverage, list(coverage[0]))
    corrections = ['# 后续补充与结论修正', '',
                   '关系方向为“旧轮次 → 后续轮次”。“补充”不撤销原结果；“修正”限定旧解读；'
                   '“替代”只针对具名版本。旧计划不授权继续执行。', '',
                   '| 原轮次 | 关系 | 后续轮次 | 理由 |', '| --- | --- | --- | --- |']
    for r in records:
        for e in r['relations']:
            t = by_id[e['target']]
            corrections.append(f'| [{r["id"]}](periods/{r["period"]}.md#{r["id"].lower()}) | '
                               f'{e["type"]} | [{t["id"]}](periods/{t["period"]}.md#{t["id"].lower()}) | '
                               f'{e["reason"].replace("|", "/")} |')
    out['corrections.md'] = ('\n'.join(corrections) + '\n').encode()
    out['verification.json'] = (json.dumps({
        'schema': 1, 'catalog_sha256': sha('docs/experiment-ledger/catalog.json'),
        'records': len(records), 'scope_units': len(units),
        'unassigned_scope_units': 0, 'relations': sum(len(r['relations']) for r in records),
        'source_files': len({s['path'] for r in records for s in r['sources']}),
        'source_hashes_and_exact_quotes_checked': True,
        'new_experiments': 0, 'new_builds': 0, 'board_operations': 0,
        'scientific_reducers_executed': 0,
        'limits': 'Static inventory, referential integrity and exact source excerpts only; not scientific revalidation or proof of missing external history.'
    }, ensure_ascii=False, indent=2) + '\n').encode()
    return out


def evidence_files(data):
    """Index metadata paths, without counting them as trials or executing them."""
    records = data['records']
    ownership = {}
    for r in records:
        for path in r['artifacts']:
            ownership.setdefault(path, set()).add(r['id'])
    by_longest_path = sorted(ownership, key=len, reverse=True)
    result = []
    prune = {'obj', '.git', '.venv', '.venv-analysis', '__pycache__', 'node_modules',
             'frozen', 'sources', 'source-bundles', 'metadata', 'before'}
    exact = {'record.json', 'receipt.json', 'metadata.json', 'campaign.json', 'summary.json',
             'result.json', 'run-result.json', 'root-result-acceptance.json', 'acceptance.json',
             'execution-ledger.json', 'complete-execution-ledger.json', 'stage-evidence.json',
             'source.json', 'validation.json', 'delivery.json', 'software-manifest.json'}
    for base in ['iommu-platform/out', 'paper/ispass2027/board/runs', 'paper/ispass2027/board/packages']:
        for directory, dirs, files in os.walk(ROOT / base, followlinks=False):
            dirs[:] = sorted(d for d in dirs if d not in prune and not d.startswith(('obj-', 'obj_', '.')))
            for name in sorted(files):
                if name not in exact and not re.fullmatch(r'run\d+-result\.json|.+-(manifest|acceptance)\.json', name):
                    continue
                if name.startswith(('deepseek', 'worker')):
                    continue
                p = Path(directory) / name
                if p.is_symlink():
                    continue
                rel = p.relative_to(ROOT).as_posix()
                owner = next((prefix for prefix in by_longest_path if rel.startswith(prefix + '/')), None)
                result.append({'path': rel, 'record_ids': ';'.join(sorted(ownership.get(owner, []))),
                               'role': 'metadata_locator_not_independent_trial',
                               'review_scope': 'path_and_bytes_only; scientific_fields_not_revalidated',
                               'bytes': p.stat().st_size, 'sha256': sha(rel)})
    return csv_bytes(result, ['path', 'record_ids', 'role', 'review_scope', 'bytes', 'sha256'])


def check_evidence_files():
    path = DEST / 'evidence-files.csv'
    rows = list(csv.DictReader(io.StringIO(path.read_text())))
    assert len({r['path'] for r in rows}) == len(rows)
    for row in rows:
        assert sha(row['path']) == row['sha256'], f'historical metadata changed: {row["path"]}'
    return len(rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    parser.add_argument('--index-evidence', action='store_true',
                        help='Explicitly refresh metadata-path index; does not run validators or experiments')
    args = parser.parse_args()
    data = json.loads((DEST / 'catalog.json').read_text())
    if args.index_evidence:
        assert not args.check, '--check does not write an index'
        (DEST / 'evidence-files.csv').write_bytes(evidence_files(data))
    outputs = render(data)
    for name, body in outputs.items():
        p = DEST / name
        if args.check:
            assert p.read_bytes() == body, f'rendered output differs: {p}'
        else:
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_bytes(body)
    count = check_evidence_files() if args.check else None
    print(json.dumps({'ok': True, 'mode': 'check' if args.check else 'render',
                      'outputs': len(outputs), 'historical_metadata_unchanged': count}, ensure_ascii=False))


if __name__ == '__main__':
    main()
