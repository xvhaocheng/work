from pathlib import Path
import json,hashlib,re,datetime
R=Path('/home/xvhaocheng/work');D=R/'iommu-platform/out/d-source-gate-20261015/documentation/ars-method-followup';A='/home/xvhaocheng/.codex/skills/academic-research-suite/'
link='[ARS-Codex方法入口]('+A+'SKILL.md)'
rule='以ARS-Codex对应工作流为方法入口；按当前任务阶段选择，不能把来源/条件预测当测量结论；遵守已确认研究边界与用户创新判断要求，不默认强制全pipeline。'
paths=['AGENTS.md','README.md','研究计划.md','研究导航.md'];before={n:(R/n).read_bytes() for n in paths}
for n,b in before.items():assert b==(D/'before'/n).read_bytes(),n
out={n:b.decode() for n,b in before.items()}
old='方法细节及模板见[研究方法.md](研究方法.md)。'
assert old in out['AGENTS.md'];out['AGENTS.md']=out['AGENTS.md'].replace(old,'方法细节及模板按'+link+'选择对应工作流。')
marker='## 贡献筛选与记录流程（2026-10-09 审查后执行）'
para='## 方法入口更新（2026-10-15 用户确认）\n\n用户确认旧个人研究方法已主动删除，认为不适合，后续以ARS-Codex为准；不恢复旧方法文件，不另建同名个人方法。'+rule+'入口为'+link+'；问题与文献阶段参见[deep-research工作流]('+A+'ars/deep-research/WORKFLOW.md)，实验规划与验证阶段参见[experiment-agent工作流]('+A+'ars/experiment-agent/WORKFLOW.md)。本更新不改变上述模型策略、idea判断、方向排序、暂停状态或实验授权。\n\n'
out['AGENTS.md']=out['AGENTS.md'].replace(marker,para+marker)
out['README.md']=out['README.md'].replace('[研究方法](研究方法.md)与[任务规则](AGENTS.md)：如何比较真实问题、前作、创新及关键证据。',link+'与[任务规则](AGENTS.md)：'+rule)
out['研究计划.md']=out['研究计划.md'].replace('方法沿用[研究方法](研究方法.md)及 academic-research-suite 的问题设计、实质前作比较和可推翻证据规划。',link+'负责工作流导航。'+rule)
out['研究导航.md']=out['研究导航.md'].replace('判定方法查[研究方法](研究方法.md)。','判定方法查'+link+'。'+rule)
for n,s in out.items():(R/n).write_bytes(s.encode())
sha=lambda b:hashlib.sha256(b).hexdigest()
changes=[{'path':n,'before_sha256':sha(before[n]),'after_sha256':sha((R/n).read_bytes()),'before_bytes':len(before[n]),'after_bytes':(R/n).stat().st_size} for n in paths]
# Exact invertibility proves no unrelated AGENTS rules changed.
a=out['AGENTS.md'].replace(para,'').replace('方法细节及模板按'+link+'选择对应工作流。',old)
checks={'old_method_file_still_absent':not (R/'研究方法.md').exists(),'old_method_links_per_current_doc':{n:len(re.findall(r'\]\(研究方法\.md\)',s)) for n,s in out.items()},'ARS_link_targets':{A+n:Path(A+n).is_file() for n in ['SKILL.md','ars/deep-research/WORKFLOW.md','ars/experiment-agent/WORKFLOW.md']},'AGENTS_all_other_bytes_unchanged':a.encode()==before['AGENTS.md'],'model_policy_bytes_unchanged':out['AGENTS.md'].split('## 研究方向不默认沿用A')[0].encode()==before['AGENTS.md'].decode().split('## 研究方向不默认沿用A')[0].encode(),'method_rule_present_in_all_four':all(rule in s for s in out.values())}
assert checks['AGENTS_all_other_bytes_unchanged'] and checks['model_policy_bytes_unchanged'] and checks['old_method_file_still_absent'] and not any(checks['old_method_links_per_current_doc'].values()) and all(checks['ARS_link_targets'].values())
(D/'changes.json').write_text(json.dumps({'files':changes},ensure_ascii=False,indent=2)+'\n');(D/'checks.json').write_text(json.dumps(checks,ensure_ascii=False,indent=2)+'\n')
(D/'receipt.json').write_text(json.dumps({'status':'pass','time_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':paths,'requested_model':'gpt-6.1-sol','requested_reasoning_effort':'medium','actual_model':'unknown','actual_reasoning_effort':'unknown','user_intent_provenance':'root relayed user confirmation; exact quote to be appended from root','method_deleted_by_user':True,'restored_or_created_method_file':False,'new_goal_actions':0,'science_or_authorization_changed':False,'checks':'checks.json','hashes':'changes.json'},ensure_ascii=False,indent=2)+'\n')
# Preserve the earlier receipt's historical unknown state; append dated follow-up only.
p=D.parent/'receipt.json';v=json.loads(p.read_text());v['followup']={'time_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'receipt':'ars-method-followup/receipt.json','resolution':'用户后续确认主动删除旧个人方法；四当前入口改ARS-Codex，未恢复旧文件；保留上述早时点unknown事实。'};p.write_text(json.dumps(v,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(checks,ensure_ascii=False,indent=2))
