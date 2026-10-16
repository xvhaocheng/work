"""Check the new source-gate delivery; never invoke models, builds or old experiments."""
from pathlib import Path
import datetime
import hashlib
import json
import re
from urllib.parse import unquote

ROOT = Path('/home/xvhaocheng/work')
RUN = ROOT / 'iommu-platform/out/d-source-gate-20261015'
OUTPUT = RUN / 'root/delivery-verification.json'


def js(path):
    return json.loads(path.read_text())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


checks = []


def check(name, ok, detail=None):
    checks.append({'name': name, 'pass': bool(ok), 'detail': detail})


OUTPUT.write_text('{"status":"verification_running"}\n')
acceptance = js(RUN / 'root/root-result-acceptance.json')
check('root evidence hashes match delivered artifacts',
      all(sha(RUN / p) == h for p, h in acceptance['evidence'].items()))
critical = js(RUN / 'root/critical-source-check.json')
drift = [x['path'] for x in critical['checks'] if sha(Path(x['path'])) != x['sha256']]
check('all 71 registered source versions preserved',
      critical['source_registrations'] == 71 and not drift, drift)

inputs = js(RUN / 'root/input-manifest.json')['files']
mutable = {'CURRENT_STATUS.md', '研究计划.md', '研究导航.md', 'HANDOFF.md',
           'iommu-platform/docs/validation.md'}
protected = [x for x in inputs if x['path'] not in mutable]
method_followup = RUN / 'documentation/ars-method-followup'
if (method_followup / 'receipt.json').is_file():
    # User explicitly superseded the old personal-method entry after the
    # bounded goal completed. Preserve the original snapshot; verify the
    # authorized method-only change and unchanged model policy separately.
    protected = [x for x in protected if x['path'] != 'AGENTS.md']
    before_agents = (method_followup / 'before/AGENTS.md').read_text()
    current_agents = (ROOT / 'AGENTS.md').read_text()

    def model_section(text):
        start = text.index('## 按任务选择模型与推理强度')
        end = text.find('\n## ', start + 3)
        return text[start:end if end != -1 else len(text)]

    method_targets_ok = all('](研究方法.md)' not in (ROOT / p).read_text()
                            for p in ['AGENTS.md', 'README.md', '研究计划.md', '研究导航.md'])
    check('user ARS-method clarification applied without changing model policy',
          js(method_followup / 'receipt.json')['status'] == 'pass'
          and model_section(before_agents) == model_section(current_agents)
          and not (ROOT / '研究方法.md').exists() and method_targets_ok)
check('AGENTS policies, fixed contract, code and prior acceptance unchanged',
      all(sha(ROOT / x['path']) == x['sha256'] for x in protected),
      [x['path'] for x in protected])

source_assessment = js(RUN / 'sources/assessment.json')
check('two candidate cards retain unknown states and outcomes',
      len(source_assessment['candidates']) == 2 and
      all(all(x[k] is None for k in ['state_plus', 'state_minus',
                                   'complete_delta_PN_minus_P', 'O0_equivalence'])
          for x in source_assessment['candidates']))
check('bounded model not admitted; strongest conditional redesign retained',
      not acceptance['D_model_admission']['admitted']
      and acceptance['positive_development']['no_IOVA_change_required_for_timing_value']
      and acceptance['I']['status'] == 'not_ready'
      and not acceptance['global_arrangement']['automatic_ranking_or_A_fallback'])
ledger = js(RUN / 'root/execution-ledger.json')
check('zero scientific executions; per-task requested and actual configurations distinct',
      all(v == 0 for v in ledger['scientific_execution'].values())
      and {x['requested']['effort'] for x in ledger['subtasks']} == {'high', 'xhigh', 'medium'}
      and all(x['actual_model'] is None and x['actual_effort'] is None
              for x in ledger['subtasks']))

before = RUN / 'documentation/before'
prefixes = {}
for name in ['HANDOFF.md', 'iommu-platform/docs/validation.md']:
    previous = (before / name).read_bytes()
    current = (ROOT / name).read_bytes()
    ok = current.endswith(previous)
    check(name + ' old body preserved as exact suffix', ok)
    prefixes[name] = current[:-len(previous)].decode() if ok else current.decode()
old_status = (before / 'CURRENT_STATUS.md').read_text()
status = (ROOT / 'CURRENT_STATUS.md').read_text()
tail = '- 独立Linux贡献：' + old_status.split('- 独立Linux贡献：', 1)[1]
check('independent engineering status tail retained', status.endswith(tail))
check('A5, paused hardware and original window retained in current state/plan',
      all(t in status for t in ['A5', '1792023314', 'paused'])
      and '10月15—28日' in (ROOT / '研究计划.md').read_text())

active = ['AGENTS.md', 'README.md', '研究计划.md', '研究导航.md', 'CURRENT_STATUS.md']
texts = [(p, (ROOT / p).read_text()) for p in active] + list(prefixes.items())
for p in [RUN / 'README.md', RUN / 'root/decision-analysis.md',
          *sorted((RUN / 'mechanism').glob('*.md')),
          *sorted((RUN / 'sources').glob('*.md')),
          *sorted((RUN / 'i-backend').glob('*.md')),
          *sorted((RUN / 'documentation').glob('*.md'))]:
    texts.append((str(p.relative_to(ROOT)), p.read_text()))
links = []
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
            links.append({'source': name, 'line': line_number,
                          'target': m.group(1), 'pass': ok})
broken = [x for x in links if not x['pass']]
known_missing = []
unexpected = []
for row in broken:
    # Preserve a missing file whose deletion intent is unknown. Never recreate
    # it or silently count the link as valid to make the delivery check pass.
    old = before / row['source']
    inherited = (row['source'] in ['研究计划.md', '研究导航.md']
                 and row['target'] == '研究方法.md' and old.is_file()
                 and '](研究方法.md)' in old.read_text())
    if inherited:
        known_missing.append({**row, 'target_text_present_before_our_edit': True,
                              'origin_of_absence': 'unknown; existed at prior delivery',
                              'handling': 'preserve current absence; async user clarification pending'})
    else:
        unexpected.append(row)
check('new delivery/current entry links have no unexpected missing targets', not unexpected,
      {'checked': len(links), 'known_missing': known_missing, 'unexpected_broken': unexpected})

report = {'schema': 'd-source-gate-delivery-verification-v1',
          'time_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
          'status': ('pass_with_known_missing_targets' if known_missing else 'pass')
          if all(x['pass'] for x in checks) else 'fail',
          'checks': checks, 'local_link_checks': links,
          'known_missing_targets': known_missing,
          'scope': 'source/file/hash/current-navigation integrity; historical bodies not revalidated; no performance or native-execution certification',
          'goal_completion_scope': 'this bounded source-gate round, not research/paper completion'}
OUTPUT.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'status': report['status'], 'checks': len(checks),
                  'local_links': len(links), 'failed': [x for x in checks if not x['pass']]},
                 ensure_ascii=False, indent=2))
raise SystemExit(0 if report['status'] in ['pass', 'pass_with_known_missing_targets'] else 1)
