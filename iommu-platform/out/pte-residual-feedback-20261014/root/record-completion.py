"""Record actual tool completion and perform focused final-document readback."""
from pathlib import Path
from datetime import datetime, timezone
import json
import hashlib
import re

W = Path('/home/xvhaocheng/work')
O = W / 'iommu-platform/out/pte-residual-feedback-20261014'
out = O / 'root/post-completion-verification.json'
assert not out.exists(), 'Final status already recorded'
read = lambda p: json.loads(p.read_text())
complete = read(O / 'root/goal-completion.json')
getter = read(O / 'root/goal-state.json')
verification = read(O / 'root/delivery-verification.json')
assert complete['goal']['status'] == 'complete' and complete['goal']['createdAt'] == 1791937547
assert getter['goal'] is None
assert verification['status'] == 'passed' and verification['failed_count'] == 0
assert not verification['actual_goal_complete_called']
g = complete['goal']
mins, secs = divmod(g['timeUsedSeconds'], 60)
note = (f'实际goal已complete；五文档同步及{verification["check_count"]}项新交付核验通过后调用。'
        f'原样工具回执最终用量{g["tokensUsed"]:,} tokens、{mins}分{secs:02d}秒，无额外token预算。'
        'getter当前goal槽已清空，完成状态以update_goal回执为准；论文未完成。')
manifest = read(O / 'before/manifest.json')
replacements = {
    W / 'CURRENT_STATUS.md': ('- 技术裁决稳定；五文档同步及必要新交付核验后才actualgoal complete，以原样工具回执为准。', '- ' + note),
    O / 'README.md': ('五文档同步与必要新交付检查通过后，才调用实际goal complete；最终工具记录另存，未提前伪造完成状态。', note),
    W / 'iommu-platform/docs/pte-residual-feedback-20261014.md':
        ('五文档同步及必要新交付核验通过后才actualgoal complete，以', '五文档同步及318项交付核验通过后actualgoal已complete，以')
}
old_detail = '五文档同步与必要新交付核验通过后才actualgoal complete，完成用量以原样工具回执为准。'
for r in manifest:
    if r['path'] != 'CURRENT_STATUS.md':
        replacements[W / r['path']] = (old_detail, note)
texts = {}
for p, (old, new) in replacements.items():
    text = p.read_text()
    assert text.count(old) == 1, str(p)
    texts[p] = text.replace(old, new, 1)
texts[W / 'iommu-platform/docs/pte-residual-feedback-20261014.md'] += '\n' + note + '\n'
texts[O / 'README.md'] += '\n[完成后回读](root/post-completion-verification.json)只核最终工具回执、入口文字与历史保留，未重跑源码或模型检查。\n'
for p, text in texts.items():
    p.write_text(text)
sync_path = O / 'root/document-sync.json'
sync = read(sync_path)
sync.update({'actual_goal_complete_called': True, 'completion_receipt': 'goal-completion.json',
             'pre_completion_checks': verification['check_count'], 'final_edit_only': 'actual completion and tool usage in current prefixes'})
sync_path.write_text(json.dumps(sync, ensure_ascii=False, indent=2) + '\n')
ledger_path = O / 'root/execution-ledger.json'
ledger = read(ledger_path)
ledger['actual_completion'] = {'receipt': 'goal-completion.json', 'status': g['status'],
    'tokensUsed': g['tokensUsed'], 'timeUsedSeconds': g['timeUsedSeconds'], 'no_added_token_budget': True,
    'getter_current_slot_cleared': True, 'pre_completion_checks': verification['check_count']}
ledger_path.write_text(json.dumps(ledger, ensure_ascii=False, indent=2) + '\n')

checks = []


def check(name, condition):
    checks.append({'name': name, 'passed': bool(condition)})


check('actual completion for correct goal', g['status'] == 'complete' and g['createdAt'] == 1791937547)
check('actual usage / no fabricated budget', g['tokensUsed'] == 297888 and g['timeUsedSeconds'] == 1765
      and 'tokenBudget' not in g and complete['remainingTokens'] is None)
check('getter current slot cleared after complete', getter['goal'] is None)
check('all new delivery checks precede actual complete', verification['status'] == 'passed'
      and not verification['actual_goal_complete_called']
      and datetime.fromisoformat(verification['created_utc']).timestamp() <= g['updatedAt'])
new_prefixes = []
for r in manifest:
    p = W / r['path']
    old = (O / r['archive']).read_bytes()
    live = p.read_bytes()
    check('original archive unchanged:' + r['path'], hashlib.sha256(old).hexdigest() == r['sha256'])
    if r['path'] == 'CURRENT_STATUS.md':
        marker = '- 独立Linux贡献：'.encode()
        tail = old[old.index(marker):]
        check('CURRENT independent tail exact / concise', live.endswith(tail) and len(live.decode().splitlines()) <= 20)
        prefix = live[:-len(tail)].decode()
    else:
        check('old history byte suffix exact:' + r['path'], live.endswith(old))
        prefix = live[:-len(old)].decode()
    check('current prefix actual goal usage and paper incomplete:' + r['path'], note in prefix)
    new_prefixes.append((p, prefix))
for p in [O / 'README.md', W / 'iommu-platform/docs/pte-residual-feedback-20261014.md']:
    text = p.read_text()
    check('final report/navigation completion usage:' + str(p.name), note in text)
    new_prefixes.append((p, text))
for p, text in new_prefixes:
    for href in re.findall(r'\]\(([^\n)]+)\)', text):
        if '://' not in href:
            dest = (p.parent / href.split('#', 1)[0]).resolve()
            check('final local link:' + href, dest.is_file() or dest == out)
check('sync and execution receipts agree', read(sync_path)['actual_goal_complete_called']
      and read(ledger_path)['actual_completion']['tokensUsed'] == g['tokensUsed'])
failed = [c for c in checks if not c['passed']]
result = {'schema': 'pte-residual-final-status-readback-v1',
    'created_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'actual goal receipt/usage, final current text/links and exact old history only; no unchanged critical-source/model verification replay',
    'check_count': len(checks), 'passed_count': len(checks) - len(failed),
    'failed_count': len(failed), 'failed': failed, 'checks': checks,
    'status': 'passed' if not failed else 'failed', 'paper_complete': False}
out.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({k: result[k] for k in ['status', 'check_count', 'passed_count', 'failed_count', 'failed']}))
raise SystemExit(0 if not failed else 1)
