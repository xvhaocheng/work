"""Verify this documentation/evidence delivery; does not build or replay experiments."""
from pathlib import Path
import datetime
import hashlib
import json
import os
import re
from urllib.parse import unquote

ROOT = Path('/home/xvhaocheng/work')
RUN = ROOT / 'iommu-platform/out/research-plan-implementation-20261015'
OUTPUT = RUN / 'root/delivery-verification.json'


def read_json(path):
    return json.loads(path.read_text())


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


checks = []


def check(name, passed, detail=None):
    checks.append({'name': name, 'pass': bool(passed), 'detail': detail})


# The only prospective link is this report's own path, created before link checks.
OUTPUT.write_text('{"status":"verification_running"}\n')
inputs = read_json(RUN / 'root/input-manifest.json')
indexed = {x['path']: x for x in inputs['files']}
archive = ROOT / 'iommu-platform/docs/history/plans'
compat = {'experiment-plan.md', 'nonblock-pilot-plan-20261004.md'}
archive_checks = []
for name in inputs['archive_names']:
    old = ROOT / 'iommu-platform/docs' / name
    new = archive / name
    baseline = indexed[str(old.relative_to(ROOT))]
    preserved = (new.is_file() and digest(new) == baseline['sha256']
                 and new.stat().st_size == baseline['bytes']
                 and len(new.read_bytes().splitlines()) == baseline['lines'])
    if name in compat:
        placement = (old.is_symlink() and os.readlink(old) == 'history/plans/' + name
                     and old.samefile(new) and old.read_bytes() == new.read_bytes())
    else:
        placement = not os.path.lexists(old)
    archive_checks.append({'name': name, 'preserved': preserved, 'placement': placement})
check('12 raw archives byte/hash/line preserved and old paths correct',
      len(archive_checks) == 12 and all(x['preserved'] and x['placement'] for x in archive_checks),
      archive_checks)
manifest = read_json(archive / 'manifest.json')
check('14 broken links recorded as preexisting', manifest['preexisting_broken_count'] == 14)

maintenance = read_json(RUN / 'archive/maintenance-link-changes.json')['files']
maintenance_checks = []
target_pattern = re.compile(r'(?<=\]\()[^)]+(?=\))')
for row in maintenance:
    before = ROOT / row['before_snapshot']
    after = ROOT / row['path']
    old, new = before.read_text(), after.read_text()
    ok = (digest(before) == row['before_sha256'] and digest(after) == row['after_sha256']
          and len(old.splitlines()) == len(new.splitlines())
          and target_pattern.sub('TARGET', old) == target_pattern.sub('TARGET', new))
    maintenance_checks.append({'path': row['path'], 'only_targets_and_hashes_match': ok})
check('13 reports / 16 target-only repairs',
      len(maintenance) == 13 and sum(len(x['changes']) for x in maintenance) == 16
      and all(x['only_targets_and_hashes_match'] for x in maintenance_checks), maintenance_checks)

protected = read_json(RUN / 'root/protected-inputs.json')
check('protected methods, A5, stage117, old acceptances and critical code unchanged',
      all(digest(ROOT / x['path']) == x['sha256'] for x in protected), protected)
check('critical source / old record checks pass',
      read_json(RUN / 'root/critical-evidence-check.json')['status'] == 'pass')

prefixes = {}
for name in ['HANDOFF.md', 'iommu-platform/docs/validation.md']:
    previous = (RUN / 'before-final-write' / name).read_bytes()
    current = (ROOT / name).read_bytes()
    unchanged = current.endswith(previous)
    check(name + ' historical body is exact preserved suffix', unchanged)
    prefixes[name] = current[:-len(previous)].decode() if unchanged else current.decode()
previous_status = (RUN / 'before-final-write/CURRENT_STATUS.md').read_text()
current_status = (ROOT / 'CURRENT_STATUS.md').read_text()
tail = '- 独立Linux贡献：' + previous_status.split('- 独立Linux贡献：', 1)[1]
check('independent engineering status tail preserved verbatim', current_status.endswith(tail))
check('current status separates global / branch / hardware',
      all(x in current_status for x in ['全局安排与下一证据', '各分支最新进展', '硬件与执行状态',
                                      '1792023314', 'paused', 'not_ready']))

entry_receipt = read_json(RUN / 'entry-sync/verification.json')
check('entry synchronization receipt and exact file versions',
      all(entry_receipt['checks'].values()) and
      all(digest(ROOT / n) == h for n, h in entry_receipt['after_sha256'].items()))
agent_before = (RUN / 'entry-sync/before/AGENTS.md').read_text()
agent_after = (ROOT / 'AGENTS.md').read_text()
policy_start = '## 按任务选择模型与推理强度'
policy_end = '## 研究方向不默认沿用A'
policy = lambda text: text.split(policy_start, 1)[1].split(policy_end, 1)[0]
check('latest model policy preserved with Sol low/medium for extraction',
      policy(agent_before) == policy(agent_after)
      and '| 单纯字段提取、格式转换、按明确规则分类 | `gpt-6.1-sol` / `low`或`medium` |' in agent_after
      and 'luna' not in agent_after.lower())

acceptance = read_json(RUN / 'root/root-result-acceptance.json')
check('D/I bounded decisions and unchanged selected directions',
      acceptance['first_batch_complete'] and not acceptance['D']['opposite_sign_pair_established']
      and acceptance['I']['status'] == 'not_ready' and not acceptance['novelty']['accepted']
      and acceptance['global_arrangement']['main'] == 'D')
check('acceptance evidence SHA matches actual files',
      all(digest(RUN / name) == sha for name, sha in acceptance['evidence'].items()))

active = ['README.md', 'AGENTS.md', '研究计划.md', '研究导航.md', 'CURRENT_STATUS.md',
          'iommu-platform/README.md', 'iommu-platform/AGENTS.md',
          'docs/research-history/README.md',
          'iommu-platform/docs/history/plans/README.md']
active += [str(p.relative_to(ROOT)) for p in [RUN / 'README.md',
           *sorted((RUN / 'd').glob('*.md')), *sorted((RUN / 'i').glob('*.md')),
           RUN / 'entry-sync/receipt.md']]
links, broken = [], []
texts = [(name, (ROOT / name).read_text()) for name in active] + list(prefixes.items())
for name, text in texts:
    for line_number, line in enumerate(text.splitlines(), 1):
        for m in re.finditer(r'\]\(([^)\n]+)\)', line):
            target = m.group(1).strip().strip('<>')
            if re.match(r'^[A-Za-z][A-Za-z0-9+.-]*://', target) or target.startswith('#'):
                continue
            target = unquote(target.split('#', 1)[0])
            if not target:
                continue
            line_ref = re.search(r':(\d+)(?:[-–]\d+)?$', target)
            ref = int(line_ref.group(1)) if line_ref else None
            if line_ref:
                target = target[:line_ref.start()]
            path = Path(target)
            if not path.is_absolute():
                path = ROOT / Path(name).parent / path
            ok = path.exists()
            if ok and ref is not None:
                ok = path.is_file() and ref <= len(path.read_bytes().splitlines())
            row = {'source': name, 'line': line_number, 'target': m.group(1), 'pass': ok}
            links.append(row)
            if not ok:
                broken.append(row)
check('current entry / first-batch / archive-index local links and cited line bounds', not broken,
      {'checked': len(links), 'broken': broken,
       'scope': 'whole current entries and new HANDOFF/validation prefixes only; frozen historical bodies excluded'})

report = {'schema': 'research-plan-delivery-verification-v1',
          'time_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
          'status': 'pass' if all(x['pass'] for x in checks) else 'fail',
          'checks': checks, 'local_link_checks': links,
          'scope_limit': 'document/source provenance integrity only; not a performance, native DMA or hardware test',
          'historical_raw_link_limit': 'use archive README locators; frozen raw link text and 14 preexisting broken links are preserved'}
OUTPUT.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'status': report['status'], 'checks': len(checks),
                  'failed': [x for x in checks if not x['pass']],
                  'local_links_checked': len(links)}, ensure_ascii=False, indent=2))
raise SystemExit(0 if report['status'] == 'pass' else 1)
