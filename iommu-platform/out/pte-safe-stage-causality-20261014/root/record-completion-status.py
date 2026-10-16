"""Bind actual completion receipt to already-delivered status; no experiment replay."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re

W = Path('/home/xvhaocheng/work')
O = W / 'iommu-platform/out/pte-safe-stage-causality-20261014'
result_path = O / 'root/post-completion-verification.json'
assert not result_path.exists(), 'Completion status already recorded'
load = lambda path: json.loads(path.read_text())
complete = load(O / 'root/goal-completion.json')
state = load(O / 'root/goal-state.json')
verification = load(O / 'root/delivery-verification.json')
assert complete['goal']['status'] == 'complete'
assert verification['status'] == 'passed' and verification['check_count'] == 405
assert not verification['actual_goal_complete_called']
assert state['goal'] is None  # Completed goal slot cleared; receipt is authoritative.
tokens = complete['goal']['tokensUsed']
seconds = complete['goal']['timeUsedSeconds']
minutes, remainder = divmod(seconds, 60)
note = (f'实际goal已complete；五文档同步及405项新交付核验全部通过后调用。'
        f'工具最终用量{tokens:,} tokens、{minutes}分{remainder:02d}秒，无额外token预算；'
        'getter清空当前goal槽，完成状态以原样update_goal回执为准。论文未完成。')
changes = {}
manifest = load(O / 'before/manifest.json')
current_path = W / 'CURRENT_STATUS.md'
current = current_path.read_text()
old_current = '- actualgoal无额外预算；root+2新独占席实际6.1-sol/ultra/native有效475000、forknone/Codex-only。五文档同步与新交付核验后才actualcomplete，以工具回执为准。'
assert current.count(old_current) == 1
changes[current_path] = current.replace(old_current, '- ' + note + ' 全树实际6.1-sol/ultra/native有效475000。')
detail_phrase = '五文档同步及本轮新交付核验后才actualgoal complete；实际工具用量见原样回执。'
for record in manifest:
    if record['path'] == 'CURRENT_STATUS.md':
        continue
    path = W / record['path']
    text = path.read_text()
    assert text.count(detail_phrase) == 1, path
    assert path.read_bytes().endswith((O / record['archive']).read_bytes()), path
    changes[path] = text.replace(detail_phrase, note, 1)
report_path = W / 'iommu-platform/docs/pte-safe-stage-causality-20261014.md'
report = report_path.read_text()
report_phrase = '五文档同步、本轮新交付检查后才actualgoal complete，工具用量以'
assert report.count(report_phrase) == 1
changes[report_path] = report.replace(report_phrase, '五文档同步和405项新交付核验通过后，actualgoal已complete，工具用量以')
changes[report_path] += '\n' + note + '\n'
nav_path = O / 'README.md'
nav = nav_path.read_text()
nav_phrase = '五文档同步及本轮新交付检查通过后才actualcomplete，完成记录只在tool回执成功后保存。原论文窗口累计，论文未完成。'
assert nav.count(nav_phrase) == 1
changes[nav_path] = nav.replace(nav_phrase, note)
changes[nav_path] += '\n[完成后状态回读](root/post-completion-verification.json)仅核工具回执与最终文档；未重跑源码、build、模型或原始解析。\n'
for path, text in changes.items():
    path.write_text(text)
sync_path = O / 'root/document-sync.json'
sync = load(sync_path)
sync['actual_goal_complete_called'] = True
sync['completion_receipt'] = 'goal-completion.json'
sync['pre_completion_delivery_checks'] = 405
sync['final_status_edit'] = 'only actual tool completion receipt/usage in new current prefix; old bodies retained'
sync_path.write_text(json.dumps(sync, ensure_ascii=False, indent=2) + '\n')
ledger_path = O / 'root/execution-ledger.json'
ledger = load(ledger_path)
ledger['actual_goal_completion'] = {'receipt': 'goal-completion.json', 'status': 'complete',
    'tokensUsed': tokens, 'timeUsedSeconds': seconds, 'token_budget': None,
    'pre_completion_delivery_checks': 405, 'getter_current_slot_cleared': True}
ledger_path.write_text(json.dumps(ledger, ensure_ascii=False, indent=2) + '\n')

checks = []


def check(name, condition):
    checks.append({'name': name, 'passed': bool(condition)})


check('actual update_goal complete for correct registered goal',
      complete['goal']['status'] == 'complete' and complete['goal']['createdAt'] == 1791932744)
check('actual completion usage, no fabricated token budget', tokens == 554922 and seconds == 3120
      and complete['remainingTokens'] is None and 'tokenBudget' not in complete['goal'])
check('post-completion getter current slot cleared, not fabricated completed getter', state['goal'] is None)
check('delivery verification passed before actual complete', verification['status'] == 'passed'
      and verification['failed_count'] == 0 and not verification['actual_goal_complete_called'])
for record in manifest:
    old = (O / record['archive']).read_bytes()
    path = W / record['path']
    live = path.read_bytes()
    check('original archive unchanged:' + record['path'], hashlib.sha256(old).hexdigest() == record['sha256'])
    if record['path'] == 'CURRENT_STATUS.md':
        marker = '- 独立Linux贡献：'.encode()
        tail = old[old.index(marker):]
        check('CURRENT independent status exact and concise', live.endswith(tail)
              and len(live.decode().splitlines()) <= 20)
        prefix = live[:-len(tail)].decode()
    else:
        check('all historical document bytes retained:' + record['path'], live.endswith(old))
        prefix = live[:-len(old)].decode()
    check('current prefix actual completion and final usage:' + record['path'],
          note in prefix and '论文未完成' in prefix)
    for url in re.findall(r'\]\(([^\n)]+)\)', prefix):
        if '://' not in url:
            check('final current link:' + url, (path.parent / url.split('#', 1)[0]).resolve().exists())
for path in [report_path, nav_path]:
    text = path.read_text()
    check('report/navigation final actual completion:' + str(path.name), note in text)
    for url in re.findall(r'\]\(([^\n)]+)\)', text):
        target = (path.parent / url.split('#', 1)[0]).resolve()
        if '://' not in url:
            check('final navigation link:' + url, target.exists() or target == result_path)
check('synchronized receipt and ledger agree', load(sync_path)['actual_goal_complete_called']
      and load(ledger_path)['actual_goal_completion']['tokensUsed'] == tokens)
failed = [r for r in checks if not r['passed']]
result = {'schema': 'safe-post-completion-status-readback-v1',
    'created_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'actual goal receipt, final status/usage/local links and exact historical document preservation only',
    'no_DUT_build_raw_analyzer_or_old_validator_replay': True,
    'pre_completion_delivery_verification': 'delivery-verification.json',
    'actual_complete_receipt': 'goal-completion.json',
    'getter_receipt': 'goal-state.json',
    'check_count': len(checks), 'passed_count': len(checks) - len(failed),
    'failed_count': len(failed), 'failed': failed, 'checks': checks,
    'status': 'passed' if not failed else 'failed', 'paper_complete': False}
result_path.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({k: result[k] for k in ['status', 'check_count', 'passed_count', 'failed_count', 'failed']}))
raise SystemExit(0 if not failed else 1)
