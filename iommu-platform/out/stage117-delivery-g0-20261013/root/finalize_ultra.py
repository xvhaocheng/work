"""Final root ultra acceptance and custody. Does not run old tests or simulations."""
import pathlib,json,hashlib,re,subprocess
W=pathlib.Path('/home/xvhaocheng/work');P=W/'iommu-platform/out/stage117-delivery-g0-20261013';R=P/'root'
def load(p):return json.loads(p.read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def save(p,o):p.write_text(json.dumps(o,ensure_ascii=False,indent=2)+'\n')
session=pathlib.Path('/home/xvhaocheng/.codex/sessions/2026/10/13/rollout-2026-10-13T20-40-38-01a13113-3bc0-7142-8e2d-9b7bcea65b76_01a13510-8885-7720-b6ef-d2f1500773af.jsonl')
ctx=None;window=None
for line in session.open():
 o=json.loads(line)
 if o.get('type')=='turn_context':ctx=o
 if o.get('type')=='event_msg' and o.get('payload',{}).get('type')=='token_count':
  value=o['payload'].get('info',{}).get('model_context_window')
  if value is not None:window=value
q=ctx['payload'];effort=q['collaboration_mode']['settings']['reasoning_effort']
assert q['model']=='gpt-6.1-sol' and effort=='ultra'
runtime=dict(file=str(session),timestamp=ctx['timestamp'],turn_id=q['turn_id'],model=q['model'],effort=effort,effective_context_window=window,historical_medium_deviation_preserved=True,manual_context_override=False)
save(R/'final-ultra-runtime.json',runtime)
paths=['CURRENT_STATUS.md','HANDOFF.md','研究计划.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md','iommu-platform/docs/stage117-delivery-g0-20261013.md']
archive=R/'before-ultra-finalization';archive.mkdir(exist_ok=True);snapshot=[]
for name in paths+['iommu-platform/out/stage117-delivery-g0-20261013/root-result-acceptance.json','iommu-platform/out/stage117-delivery-g0-20261013/README.md','iommu-platform/out/stage117-delivery-g0-20261013/next-goal-contract.json']:
 dest=archive/name;dest.parent.mkdir(parents=True,exist_ok=True)
 assert not dest.exists(),'finalization already performed; inspect instead of rerun'
 dest.write_bytes((W/name).read_bytes());snapshot.append(dict(path=name,sha256=sha(dest)))
save(archive/'manifest.json',snapshot)
source=load(R/'source-contract-evidence.json')
for row in source['facts']:assert sha(W/row['path'])==row['sha256']
review=dict(schema='stage117-ultra-root-final-review-v1',runtime='final-ultra-runtime.json',reviewer='root gpt-6.1-sol ultra',accepted=True,critical_source_reviews=[{'path':'implementation/delivery_g0.cpp','lines':'50-98','finding':'Allactive publicDONE→prepared successor before completeprefix; shared metadata/drain release; actual consumer once; source and B release separate; no halt polling/prepublish full-record read/overwrite.'},{'path':'implementation/cpu_cache.hpp','lines':'20-98','finding':'Actual native source read returns feed destination byte assembly. Independent read/store queues and oldtag/newindex pipeline honored; real WT/controller drains and fence ack/halt release.'},{'path':'implementation/delivery_protocol.hpp','lines':'9-23','finding':'All spans join; completeprefix acquisition/publication/consumer/return and mapping teardown requirements bound.'},{'path':'checker/verify_delivery.py','lines':'595-642,859-917','finding':'Actual returned-data byte dependency, exact coverage and each cacheAR uniquely pending native load + exact own packet/span byte closure, no peer global-union authorization.'},{'path':'root/source-contract-evidence.json','finding':'Six pinned primary DPDK excerpts reviewed and live full-source SHA verified; timely completion/prefix publication/source hostfree versus B return distinction; no rate/deadline claim.'}],raw_coverage=dict(cases=6,cache_AR_bound=16356,negative_controls_unique=7,positive_controls_unique=1,same_simulated_binary=True),research_decision='Close finite ordinary delivery performance extension; no G1 or independent IOMMU performance innovation claim, no proven reachable avoidable translation loss.',tradeoffs=['CPUeager earliestpacket1/2; singlebatch earliestpacket3','parallel_eager first71earlier but packet2/3 each109later and oneadditional engine','firstprefix source-return metadata differs; translation/context traffic and consumer/fence/supply costs differ'],limits=['Physical cacheport deterministic optimistic CPU frontend; instructions/MMU/ALU/software/application/FPGA unknown null','No production error recovery or physical A reuse promise','Mixed CPU/DMA dispatch and all ordinary baselines not exhausted','Initial build1 inputs/binary snapshot missing and unused; build2 exact executed dependency custody retained','Root historical medium deviation remains disclosed; current ultra final acceptance supersedes provisional decision without rewriting execution history'],no_new_simulation_build_control_or_old_suite=True,paper_complete=False)
save(R/'ultra-final-review.json',review)
a=load(P/'root-result-acceptance.json');a['full_goal_status']='ready_for_actual_goal_complete_after_ultra_final_acceptance';a['model_policy'].update(root_actual='gpt-6.1-sol/ultra/effective'+str(window),satisfied=True,authoritative_runtime='root/final-ultra-runtime.json',historical_root_medium_deviation=True,historical_ultra_compliance=False,current_final_acceptance_ultra=True)
a['ultra_final_review']='root/ultra-final-review.json';a['next']='Retain ordinary adapter/finiteG0 asset; stop this autonomous matrix extension. Select one workflow with new original facts, GPU/AI priority; no new goal registered by this finalization.'
save(P/'root-result-acceptance.json',a)
n=load(P/'next-goal-contract.json');n['prerequisite']='Final root ultra review accepted; current goal complete receipt and synchronized delivery required before starting a separate bounded new-source goal.';save(P/'next-goal-contract.json',n)
old='实际goal已blocked（同一模型阻塞连续三轮核验）：两个新席Sol/ultra，root最新运行记录Sol/medium，与ultra约定不符；已如实通知，工具不能原位切换，不伪造切换。技术交付先完成，恢复root ultra后只作最终审查/交付签收，不重复实验；未宣称全树ultra或goal complete。有效上下文475000，不设额外预算。论文未完成，原两周窗口累计。'
new='root当前实际gpt-6.1-sol/ultra，最终原文/复制/cache/协议/独立checker与研究裁决亲核已接受，模型配置阻塞已解除。此前medium运行与blocked记录原样归档，不声称历史全树一直ultra；两个新席实际Sol/ultra，原生有效上下文475000。技术及五文档交付已完成，待实际goal complete回执。没有重跑实验/控制/构建，没有额外预算；论文未完成，原两周窗口累计。'
for name in ['HANDOFF.md','研究计划.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:
 f=W/name;text=f.read_text();assert text.count(old)==1,name;f.write_text(text.replace(old,new,1))
f=W/'CURRENT_STATUS.md';text=f.read_text();oldline=next(l for l in text.splitlines() if l.startswith('- 实际goal已blocked'));text=text.replace(oldline,'- root当前实际Sol/ultra、475000原生有效上下文；最终亲核已接受，模型配置阻塞解除。此前medium/blocked历史保留，不声称全程ultra。完整交付已验，待实际goal complete回执，无额外预算。',1)
text=text.replace('- 下一步：恢复root ultra后仅最终审查/签收，不重跑六案；随后按新增原始事实选择一个有界工作流，GPU/AI优先，核任务/生命周期/共享翻译路径/普通强对照和全部成本。停止本例自主矩阵扩展，R107/C112等关闭分支不恢复。','- 下一步：按新增原始事实选择一个有界工作流，GPU/AI优先，核任务/生命周期/共享翻译路径/普通强对照和全部成本。停止本例自主矩阵扩展，R107/C112等关闭分支不恢复；已验六案不重跑。',1);f.write_text(text)
f=W/'iommu-platform/docs/stage117-delivery-g0-20261013.md';text=f.read_text();start=text.index('实际 goal 已 blocked');end=text.index('\n\n',start);text=text[:start]+new+' [最终ultra运行](../out/stage117-delivery-g0-20261013/root/final-ultra-runtime.json)／[亲核接受](../out/stage117-delivery-g0-20261013/root/ultra-final-review.json)。'+text[end:]
text=text.replace('先完成 root ultra 最终签收；随后依新事实选择一个有界工作流','root ultra 最终签收已接受；随后依新事实选择一个有界工作流',1);f.write_text(text)
f=P/'README.md';text=f.read_text();text=text.replace('实际 goal blocked（同一模型阻塞连续三轮核验），root medium 与约定 ultra 偏差待最终运行签收。','root当前实际Sol/ultra，最终亲核接受，配置阻塞解除；此前medium/blocked历史原样保留，待实际goal完成回执。',1)
text+='\n[最终ultra运行事实](root/final-ultra-runtime.json)／[最终亲核裁决](root/ultra-final-review.json)／[最终交付核验](root/ultra-delivery-check.json)。前述active/blocked审查链接均为具名历史快照，最终状态以本次回执为准。\n';f.write_text(text)
# Existing suites are not replayed; hashes bind current state to accepted runs.
checks=[]
def ck(name,condition):assert condition,name;checks.append(name)
for row in load(P/'implementation/artifact-sha256.json'):ck('implementation '+row['path'],sha(P/'implementation'/row['path'])==row['sha256'])
for name,digest in load(P/'checker/artifact-sha256.json').items():ck('checker '+name,sha(P/'checker'/name)==digest)
for row in load(P/'implementation/actual-compiled-dependencies.json')['files']:ck('dependency '+row['path'],sha(pathlib.Path(row['path']))==row['sha256'])
for row in load(P/'before/manifest.json'):
 ck('initial snapshot '+row['path'],sha(P/'before'/row['path'])==row['sha256'])
 if row['path']!='CURRENT_STATUS.md':ck('full historical suffix '+row['path'],(W/row['path']).read_bytes().endswith((P/'before'/row['path']).read_bytes()))
for row in load(R/'dirty-files-before.json'):
 if row['repo']=='iommu-platform' and row['path']=='docs/validation.md':continue
 ck('existing dirty '+row['repo']+'/'+row['path'],sha(W/row['repo']/row['path'])==row['sha256'])
for row in load(R/'prior-assets.json'):ck('prior asset '+row['path'],sha(W/row['path'])==row['sha256'])
for name in paths+[str((P/'README.md').relative_to(W))]:
 f=W/name
 if name in ('HANDOFF.md','研究计划.md'):continue # unchanged old history has external legacy navigation
 for target in re.findall(r'\]\(([^)]+)\)',f.read_text()):
  if target.startswith(('http:','https:','#')):continue
  resolved=(f.parent/target.split('#')[0]).resolve()
  if resolved==R/'ultra-delivery-check.json':continue
  # Only inspect new current section, never legacy historical references.
  if not resolved.exists():
   assert target not in f.read_text().split('\n## 2026-10-13 stage117真实缓存交接接受')[0],target
docsha={name:sha(W/name) for name in paths}
save(R/'ultra-delivery-check.json',dict(schema='stage117-ultra-final-custody-v1',passed=True,checks=checks,checks_count=len(checks),technical_data_unchanged=True,model_gate_current_passed=True,historical_model_deviation_retained=True,root_ultra_review='ultra-final-review.json',document_sha256=docsha,acceptance_sha256=sha(P/'root-result-acceptance.json'),next_scope_sha256=sha(P/'next-goal-contract.json'),no_simulation_build_or_checker_control_run=True,ready_for_actual_goal_complete=True,paper_complete=False))
print(json.dumps(dict(current_runtime=runtime,critical_review_accepted=True,custody_checks=len(checks),ready_for_actual_goal_complete=True),ensure_ascii=False))
