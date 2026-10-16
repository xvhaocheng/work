#!/usr/bin/env python3
"""Source/hash/history/link checks; no rerun of the rule probes or old tests."""
from pathlib import Path
import hashlib
import json
import re

P = Path(__file__).resolve().parent.parent
R = P.parent.parent
W = R.parent
OUT = P / 'root/delivery-verification.json'
OUT.touch(exist_ok=True)
checks = []


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check(name, ok):
    checks.append(dict(check=name, passed=bool(ok)))


source = json.loads((P / 'source/source-contract.json').read_text())
for label, entry in source['sources'].items():
    file = Path(entry['path'])
    check('source identity ' + label, sha(file) == entry['sha256'])
    check('source line coverage ' + label, len(file.read_text().splitlines()) == entry['lines'])


def visit(item):
    if isinstance(item, dict):
        if {'path', 'sha256', 'lines'} <= item.keys() and isinstance(item['lines'], list):
            file = Path(item['path'])
            lo, hi = item['lines']
            check('evidence range ' + file.name + ':' + str(lo),
                  sha(file) == item['sha256'] and
                  1 <= lo <= hi <= len(file.read_text().splitlines()))
        for value in item.values():
            visit(value)
    elif isinstance(item, list):
        for value in item:
            visit(value)


visit(source)
ledger = json.loads((P / 'validation/execution-ledger.json').read_text())
check('one actual symbolic probe run', ledger['runs'] == 1 and ledger['passed'] == 8)
for rel, key in [('validation/check_rules.py', 'script_sha256'),
                 ('validation/rule-probes.json', 'result_sha256')]:
    check('probe identity ' + rel, sha(P / rel) == ledger[key])
check('no fabricated time result', json.loads((P / 'validation/rule-probes.json').read_text())['full_task_time'] is None)
for entry in json.loads((R / 'out/a2-sva-implementation-20261015/patches/source-manifest.json').read_text()):
    check('A2 production unchanged ' + entry['path'], sha(R / entry['path']) == entry['after_sha256'])
for rel in ['out/a2-sva-implementation-20261015/root-result-acceptance.json',
            'out/a2-sva-implementation-20261015/next-decision-contract.json',
            'out/mig-pte-debt-observation-20261015/root/goal-pause.json',
            'docs/research-novelty-assessment-20261015.md']:
    check('accepted or author material unchanged ' + rel,
          (R / rel).read_bytes() == (P / 'before/iommu-platform' / rel).read_bytes())

texts = []
for final, before in [(W / 'HANDOFF.md', P / 'before/HANDOFF.md'),
                       (R / 'docs/validation.md', P / 'before/iommu-platform/docs/validation.md')]:
    raw, old = final.read_bytes(), before.read_bytes()
    check('historical bytes ' + final.name, raw.endswith(old))
    texts.append((final, raw[:-len(old)].decode()))
old = (P / 'before/CURRENT_STATUS.md').read_text()
tail = old[old.index('\n- 独立Linux贡献'):]
current = (W / 'CURRENT_STATUS.md').read_text()
check('independent status preserved', current.endswith(tail))
texts.append((W / 'CURRENT_STATUS.md', current[:-len(tail)]))
texts += [(file, file.read_text()) for file in [W / '研究计划.md',
    R / 'docs/research-execution-plan-20261011.md',
    R / 'docs/a3-consumer-position-rules-20261015.md', P / 'README.md',
    P / 'method/design.md', P / 'source/findings.md']]
for file, content in texts:
    for link in re.findall(r'\[[^\]]*\]\(([^)]+)\)', content):
        if re.match(r'[a-z]+:', link) or link.startswith('#'):
            continue
        path = link.split('#', 1)[0]
        check('local link ' + file.name + ' -> ' + path, (file.parent / path).exists())
result = dict(schema='a3-delivery-verification-v1', checks=checks, count=len(checks),
              all_passed=all(c['passed'] for c in checks),
              scope='metadata only; no source episode, timing, old validation, compiler or hardware replay')
OUT.write_text(json.dumps(result, indent=2, ensure_ascii=False) + '\n')
print(json.dumps(dict(count=len(checks), all_passed=result['all_passed'],
                     failures=[c for c in checks if not c['passed']])))
raise SystemExit(0 if result['all_passed'] else 1)
