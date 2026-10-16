"""Save actual completion usage to already synchronized docs; focused checks only."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json

W=Path('/home/xvhaocheng/work')
O=W/'iommu-platform/out/rpte-return-commitment-20261014'
receipt=json.loads((O/'root/goal-completion.json').read_text())
state=json.loads((O/'root/goal-state.json').read_text())
delivery=json.loads((O/'root/delivery-verification.json').read_text())
g=receipt['goal']
assert g['status']=='complete' and delivery['result']=='passed'
assert datetime.fromisoformat(delivery['created_utc']).timestamp()<=g['updatedAt']
tokens=g['tokensUsed'];seconds=g['timeUsedSeconds']
minutes,seconds_tail=divmod(seconds,60)
usage=f'{tokens:,}tokens／{minutes}分{seconds_tail:02d}秒'
current=W/'CURRENT_STATUS.md'
old=current.read_text()
needle='- actual goal无额外预算，root+2新独占席实际6.1-sol/ultra/native有效475000、forknone/Codex-only；五文档与交付核验完成后才actualcomplete，以原样工具回执为准。'
assert old.count(needle)==1
current.write_text(old.replace(needle,f'- actual goal已在五文档同步和302项交付核验通过后complete；工具记录{usage}，无额外预算。root+2新独占席实际6.1-sol/ultra/native有效475000、forknone/Codex-only；论文未完成。'))
manifest=json.loads((O/'before/manifest.json').read_text())
for r in manifest:
    if r['path']=='CURRENT_STATUS.md':continue
    p=W/r['path'];s=p.read_text();heading,rest=s.split('\n\n',1)
    p.write_text(heading+f'\n\nactual goal已在五文档同步、302项新交付核验通过后complete；工具记录{usage}，无额外token预算，论文未完成。\n\n'+rest)
report=W/'iommu-platform/docs/rpte-return-commitment-decision-20261014.md'
report.write_text(report.read_text()+f'\n实际完成回执：goal complete，工具记录{usage}，无额外token预算。完成后get_goal返回goal=null（当前槽已清空）；完成状态以原样update_goal回执为准，未伪造getter状态。\n')
readme=O/'README.md'
readme.write_text(readme.read_text()+f'\nactual goal回执：complete，{usage}，无额外预算。后续get_goal原样为goal=null，表示没有当前active目标；completed状态从goal-completion.json原回执读取。\n')
sync=O/'root/document-sync.json'
x=json.loads(sync.read_text());x['actual_goal_complete_called']=True;x['completion_usage']={'tokens':tokens,'seconds':g['timeUsedSeconds']};x['receipt']='goal-completion.json';sync.write_text(json.dumps(x,ensure_ascii=False,indent=2)+'\n')
ledger=O/'root/execution-ledger.json'
x=json.loads(ledger.read_text());x['completion_receipt_status']='complete, original update_goal receipt; getter cleared current slot (goal null)';x['receipt_formatter_diagnostic']='The first receipt-save exec wrote both raw receipts successfully, then its final display dereferenced nullable state.goal and failed. Corrected display/receipt interpretation only; no retry of goal completion, source/model/test/checker.';ledger.write_text(json.dumps(x,ensure_ascii=False,indent=2)+'\n')

checks=[]
def check(name,ok):checks.append({'name':name,'passed':bool(ok)})
check('actual-completion-authoritative',g['status']=='complete' and g['createdAt']==1791910499)
check('getter-no-active-goal',state['goal'] is None or state['goal']['status']=='complete')
check('no-extra-budget',receipt['remainingTokens'] is None and 'tokenBudget' not in g)
check('delivery-before-actual-completion',delivery['result']=='passed' and datetime.fromisoformat(delivery['created_utc']).timestamp()<=g['updatedAt'])
check('completion-links-exist',all((O/'root'/name).exists() for name in ['goal-completion.json','goal-state.json','delivery-verification.json']))
for r in manifest:
    archive=(O/r['archive']).read_bytes()
    check('original-archive:'+r['path'],hashlib.sha256(archive).hexdigest()==r['sha256'])
    p=W/r['path']
    if r['path']=='CURRENT_STATUS.md':
        a=archive.decode();s=p.read_text();mark='- 独立Linux贡献：'
        check('independent-status-tail-preserved',s[s.index(mark):]==a[a.index(mark):])
    else:check('historical-suffix:'+r['path'],p.read_bytes().endswith(archive))
    check('usage-and-paper-status:'+r['path'],usage in p.read_text()[:6000] and '论文未完成' in p.read_text()[:6000])
check('report-completion-usage',usage in report.read_text())
check('ledger-zero-reruns',all(json.loads(ledger.read_text())[k]==0 for k in ['simulation_runs','unit_or_RTL_tests','builds','old_experiment_replays','old_validator_replays','production_changes']))
result={'schema':'rpte-focused-postcompletion-check-v1','created_utc':datetime.now(timezone.utc).isoformat(),'scope':'new receipt and changed status lines only; 302 earlier checks not repeated','result':'passed' if all(r['passed'] for r in checks) else 'failed','checks':checks,'passed':sum(r['passed'] for r in checks),'total_checks':len(checks),'actual_completion_usage':{'tokens':tokens,'seconds':g['timeUsedSeconds']},'getter_raw_goal':state['goal'],'paper_complete':False}
(O/'root/post-completion-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'result':result['result'],'passed':result['passed'],'checks':len(checks),'usage':usage,'actual_goal':'complete','getter':'no active goal'},ensure_ascii=False))
assert result['result']=='passed'
