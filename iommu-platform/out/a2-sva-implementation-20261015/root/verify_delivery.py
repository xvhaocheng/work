#!/usr/bin/env python3
"""Delivery metadata and source/output identity only. Runs no experiments."""
from pathlib import Path
import hashlib
import json
import re
import subprocess

P = Path(__file__).resolve().parent.parent
R = P.parent.parent
W = R.parent
checks = []
# The report is itself a linked deliverable; ensure its path exists before
# checking those links, then replace it with the complete result below.
(P / 'root/delivery-verification.json').touch(exist_ok=True)


def check(name, ok, **evidence):
    checks.append(dict(check=name, passed=bool(ok), **evidence))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load(rel):
    return json.loads((P / rel).read_text())


for entry in load('patches/source-manifest.json'):
    check('current source: ' + entry['path'],
          sha(R / entry['path']) == entry['after_sha256'])
for entry in load('kernel/final-output-manifest.json'):
    check('current output: ' + entry['path'],
          sha(R / entry['path']) == entry['sha256'])
for entry in load('kernel/build-lock.json')['final_artifact_copies']:
    check('immutable copy: ' + entry['path'], sha(P / entry['path']) == entry['sha256'])
for entry in load('hero/evidence/final-artifact-manifest.json'):
    check('Hero final artifact: ' + entry['file'],
          sha(P / 'hero' / entry['file']) == entry['sha256'])
for entry in load('hero/evidence/changed-source-manifest.json'):
    repo = ('hero-tools' if entry['repository'] == 'hero-tools'
            else 'cva6-sdk-fb56489c97b41471b6b9d707f0a38b3bc789a76e')
    file = P / 'hero/source' / repo / entry['file']
    # The exact extracted SDK directory is versioned in the Hero lock.
    if not file.exists() and entry['repository'] != 'hero-tools':
        candidates = list((P / 'hero/source').glob('cva6-sdk*/' + entry['file']))
        check('unique exact SDK file: ' + entry['file'], len(candidates) == 1)
        file = candidates[0]
    check('Hero patched source: ' + entry['file'], sha(file) == entry['patched_sha256'])

app = load('task/locked-pte-batch/app/results.json')
driver = load('task/locked-pte-batch/lifecycle/results.json')
check('final app evidence/source identity', app['methods_passed'] == 16 and
      app['source_sha256'] == sha(R / 'bench/idma_sva_task.c'))
check('final driver evidence/source identity', driver['cases_passed'] == 20 and
      driver['driver_sha256'] == sha(R / 'linux/drivers/misc/idma_sva_task.h'))
check('borrowed disabled profile check', driver['profile_compile_check']['mode_mask'] == 3)
for entry in load('model/cq-build-inputs.json')['sources']:
    check('CQ compiled actual source: ' + Path(entry['path']).name,
          sha(Path(entry['path'])) == entry['sha256'])
check('CQ body passed four cases', load('model/cq-check-results.json')['cases_passed'] == 4)
check('Hero final static ELF accepted', load('hero/evidence/final-elf-acceptance.json')['status'] == 'passed')
check('Hero final implementation gap explicit',
      not load('hero/result.json')['unimplemented_hardware_backend']['deployment_or_native_timing_ready'])
check('runtime not silently claimed', not load('root-result-acceptance.json')['scope']['native_chain_accepted'])

for repo, name in [(R, 'platform'), (R / 'linux', 'linux'), (R / 'cva6', 'cva6')]:
    proc = subprocess.run(['git', '--no-optional-locks', 'status', '--short'],
                          cwd=repo, capture_output=True, text=True)
    (P / 'root' / (name + '-status-final.txt')).write_text(proc.stdout)
    if name == 'cva6':
        check('CVA6 status exactly unchanged', proc.stdout == (P / 'before/cva6-status.txt').read_text())
proc = subprocess.run(['git', 'diff', '--check'], cwd=R / 'linux', capture_output=True, text=True)
check('Linux whitespace', proc.returncode == 0, stderr=proc.stderr)
for name, cwd in [('linux-a2.patch', R / 'linux'), ('platform-a2.patch', R)]:
    argv = ['git', 'apply', '--check', '--reverse', str(P / 'patches' / name)]
    proc = subprocess.run(argv, cwd=cwd, capture_output=True, text=True)
    check('readonly reverse patch check: ' + name, proc.returncode == 0,
          argv=argv, stderr=proc.stderr)

pause = R / 'out/mig-pte-debt-observation-20261015/root/goal-pause.json'
check('MIG pause unchanged', pause.read_bytes() ==
      (P / 'before/iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json').read_bytes(),
      sha256=sha(pause))
texts = []
for final, before in [(W / 'HANDOFF.md', P / 'before/HANDOFF.md'),
                       (R / 'docs/validation.md', P / 'before/iommu-platform/docs/validation.md')]:
    raw = final.read_bytes()
    old = before.read_bytes()
    check('historical bytes preserved: ' + final.name, raw.endswith(old))
    texts.append((final, raw[:-len(old)].decode()))
old_status = (P / 'before/CURRENT_STATUS.md').read_text()
tail = old_status[old_status.index('\n- 独立Linux贡献'):]
check('independent status tail preserved', (W / 'CURRENT_STATUS.md').read_text().endswith(tail))
texts += [(f, f.read_text()) for f in [W / 'CURRENT_STATUS.md', W / '研究计划.md',
         R / 'docs/research-execution-plan-20261011.md', R / 'docs/a2-sva-implementation-20261015.md',
         P / 'README.md', P / 'hero/README.md']]
for file, text in texts:
    if file.name == 'CURRENT_STATUS.md':
        text = text[:-len(tail)]
    for link in re.findall(r'\[[^\]]*\]\(([^)]+)\)', text):
        if re.match(r'[a-z]+:', link) or link.startswith('#'):
            continue
        target = link.split('#', 1)[0]
        check('local new prose link: ' + file.name + ' -> ' + target,
              (file.parent / target).exists())

result = dict(schema='a2-delivery-verification-v1',
              scope='Metadata/source/output/history/link verification only, no functional or native replay',
              checks=checks, count=len(checks), all_passed=all(c['passed'] for c in checks))
(P / 'root/delivery-verification.json').write_text(json.dumps(result, indent=2, ensure_ascii=False) + '\n')
# Replace the earlier current-output snapshot with the final identities; preserve history.
before = P / 'root/source-output-verification.json'
if before.exists() and not (P / 'root/source-output-verification-before-mlock.json').exists():
    (P / 'root/source-output-verification-before-mlock.json').write_bytes(before.read_bytes())
before.write_text(json.dumps(dict(scope=result['scope'],
    authoritative='delivery-verification.json', all_pass=result['all_passed'],
    checks=[c for c in checks if c['check'].startswith(('current source:', 'current output:', 'MIG pause'))]),
    indent=2, ensure_ascii=False) + '\n')
print(json.dumps(dict(checks=len(checks), all_passed=result['all_passed'],
                     failures=[c for c in checks if not c['passed']])))
raise SystemExit(0 if result['all_passed'] else 1)
