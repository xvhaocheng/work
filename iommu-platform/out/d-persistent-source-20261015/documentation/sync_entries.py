from pathlib import Path
import json, hashlib, datetime, re
W=Path('/home/xvhaocheng/work'); R=W/'iommu-platform/out/d-persistent-source-20261015'; D=R/'documentation'
assert not (D/'receipt.json').exists(), 'Already applied; use verify_written_entries.py for read-only validation'
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
h=lambda b:hashlib.sha256(b).hexdigest()
# Immediate read and snapshot: use current bytes, never the task-start version.
before={p:(W/p).read_bytes() for p in paths}
manifest=[]
for p,b in before.items():
 q=D/'before-write'/p; q.parent.mkdir(parents=True,exist_ok=True); q.write_bytes(b)
 old=R/'before'/p
 manifest.append({'path':p,'before_write_sha256':h(b),'initial_before_sha256':h(old.read_bytes()) if old.exists() else None,'changed_since_run_start':old.exists() and old.read_bytes()!=b,'bytes':len(b)})
(D/'before-write-manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n')
base='iommu-platform/out/d-persistent-source-20261015'
summary='SSD正常覆写→FTL/GC与DRAM开行/刷新来源均成立；优先SSD，但尚不能接入P/PN选策模型。固定MQSim实际两次私有构建、8次冻结运行＋1次因缺完成触发的只读调试重放：512请求完整512/512且GC=0，2048/4096/8192覆写各缺1/1/2完成，原版/观察版XML 4/4逐字节一致。exit=0/eventlist=0不等于完整完成；三个覆写case为NOT_ESTABLISHED，不是否定GC持续性假设。root核1276项hash与事件守恒，3种负变异均拒绝。'
nextstep='下一最小动作固定2048输入，取得request→child transaction→barrier/PHY→CQE的唯一身份账本；先解决正确性，再接P/PN，不调参或扩大矩阵。XML Request_Count是生成数；AMU条件性删除用户事务路径尚未动态绑定当前缺失CID53，TSU receive_slots=1可能只是旧指针，不能当live证据。'
info='O_phys可重建不代表真实O_host已足够；新增host日志与Z均计采集、保存、传输、推断及切换费用，真实费用unknown。来源事实与内部模拟真值不构成原生接口、性能或novelty收益。'
link=f'[本轮入口与接受]({base}/README.md)／[下一最小合同]({base}/root/next-minimal-contract.json)'
after={p:b.decode() for p,b in before.items()}
s=after['CURRENT_STATUS.md']; s=s.replace('# 当前状态：D条件服务模型与信息价值裁决','# 当前状态：D持续服务来源与完整完成门')
start=s.index('- D的CS1'); end=s.index('\n- I有条件',start)
s=s[:start]+'- '+summary+' '+nextstep+' '+info+' CS1本版Z/费用/菜单局部停止保持，不关闭D。'+link+'。'+s[end:]
start=s.index('本轮执行新CS1'); end=s.index('\n\n## 独立工程事项',start)
s=s[:start]+'本轮为host-only MQSim私有构建与离线模拟；实际2次构建、8次冻结运行＋1次只读调试重放，root仅独立只读核账。0旧实验重跑/原生DMA/Linux或RTL或UAPI修改/collector/FPGA或板操作。原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究继续Codex-only；按任务分配模型/强度，实际配置与请求配置分别记录。'+s[end:]
after['CURRENT_STATUS.md']=s
s=after['研究计划.md']; s=s.replace('CS1条件模型已准入执行；当前Z/费用/菜单版本停止，转核有来源的持续服务机制与剩余信息缺口，固定M0不扩大','CS1局部停止保持；SSD/DRAM来源已核，先定位MQSim固定2048输入缺完成，正确性通过后才接P/PN，固定M0不扩大')
start=s.index('当前实际结果：');end=s.index('\n\n## D：',start)
s=s[:start]+'当前实际结果：'+summary+' '+nextstep+' '+info+' D主线、I轻量not_ready、A5/A2保留及10月15—28日窗口不变；固定M0与CS1当前版本局部停止保持。'+link+'。'+s[end:]
start=s.index('下一最小输入改为');end=s.index('\n\n## I：',start)
s=s[:start]+'持续服务来源关已完成，当前只优先SSD正常覆写→FTL/GC；DRAM行/刷新候选保留。'+nextstep+' '+info+' 若一次定向身份迹仍不能定位，报告事件缺口，停止将该配置用作性能provider，不能无界修工具。完成门通过后才冻结共享翻译/数据资源、quiet/config/clear/warm、完整成功终点及同信息等待/试选的条件研究投影。普通对照继续包含常驻池、合法大映射/准备、固定动作、完整O0、队深/poll与全费用试选，付费Z同样交给普通规则；不扩大固定M0、不调gap、不加collector或改guard。'+link+'。'+s[end:]
after['研究计划.md']=s
s=after['研究导航.md'];start=s.index('| D：');end=s.index('\n| I：',start)
s=s[:start]+f'| D：有限观测→完整控制选择 | 主线；SSD/DRAM持续服务来源已核，SSD优先；MQSim覆写缺完成，先核2048请求身份账本；CS1局部停止保持 | [本轮入口]({base}/README.md) · [稳定接受]({base}/root/root-result-acceptance.json) · [完整完成核账]({base}/root/boundary-verification.json) · [下一最小合同]({base}/root/next-minimal-contract.json) |'+s[end:]
start=s.index('下一关是');end=s.index('\n\n## 已有成果',start)
s=s[:start]+summary+' '+nextstep+' '+info+' [前轮CS1局部裁决](iommu-platform/out/d-service-model-20261015/README.md)、[SPDK来源链](iommu-platform/out/d-spdk-contract-20261015/README.md)和[旧机制门](iommu-platform/out/d-source-gate-20261015/README.md)保留；不关闭D、不自动转I/A，10月15—28日窗口不变。'+s[end:]
after['研究导航.md']=s
section='# 2026年10月15日 D持续服务来源与MQSim完整完成门\n\n'+summary+'\n\n'+nextstep+' '+info+'\n\n下一合同只做固定2048请求的唯一身份定位；当前未执行身份重放或语义修复。若补丁成为必要，须先证明根因，再标为私有语义变体，保持真实写入与完成因果，不补造成功CQE。完成门通过后才冻结来源驱动的共享资源模型与quiet/config/clear/warm及全费用普通对照。\n\nD主线/I轻量not_ready、A5/A2与独立工程事实、10月15—28日窗口不变；CS1本版Z/费用/菜单局部停止保持。MIG1792023314 paused，build6 WNS −0.391ns、598项setup失败、实际债务unknown。0原生DMA/Linux或RTL或UAPI修改/collector/FPGA或板操作/旧实验重跑；两次host模拟器构建与8＋1模拟执行已如实登记。共享研究Codex-only；证据请求Sol/high、普通实施/文档Sol/medium，实际模型与推理强度元数据unknown。旧个人研究方法不恢复。\n\n'
for p,prefix in [('HANDOFF.md',base),('iommu-platform/docs/validation.md','../out/d-persistent-source-20261015')]:
 links=f'[完整入口]({prefix}/README.md)／[稳定接受]({prefix}/root/root-result-acceptance.json)／[root裁决]({prefix}/root/decision-analysis.md)／[1276项hash及事件核账]({prefix}/root/boundary-verification.json)／[8次执行账本]({prefix}/offline/runs/execution-summary.json)／[调试重放]({prefix}/offline/diagnostic-2048/debugger.log)／[下一合同]({prefix}/root/next-minimal-contract.json)／[文档回执]({prefix}/documentation/receipt.json)。文档检查仅核hash、本地链接和保护段；以下旧全文逐字节保留。\n\n'
 after[p]=section+links+before[p].decode()
readme='''# D持续服务来源与完整完成门（2026-10-15）

'''+summary+'\n\n'+nextstep+' '+info+'''

本轮只接受机制来源和实际事件核账；完整覆写边界、同O_host状态对、动作排序差异、及时收费接口和强普通控制净收益均未成立。SSD优先是当前研究判断，DRAM候选保留。模型ns不等于实测或原生周期；不把模拟器规则或缺陷定位写成本项目新意。

## 阅读与原始证据入口

- 来源：[SSD来源卡](ssd/evidence-card.md)、[SSD裁决](ssd/assessment.md)、[DRAM来源卡](memory/card.md)、[DRAM核查](memory/findings.md)、[现有观测接口](observation/source-evidence.md)。
- root：[稳定接受](root/root-result-acceptance.json)、[决定与边界](root/decision-analysis.md)、[关键源码审查](root/direct-source-review.json)、[被动记录审查](root/passive-probe-review.json)、[运行器审查](root/runner-review.json)、[独立核账与负变异](root/boundary-verification.json)。
- 构建与完整hash：[原始source清单](offline/original-source-manifest.json)、[原始source SHA256](offline/original-source.sha256)、[archive SHA256](offline/archive.sha256)、[原版构建](offline/build-result.json)、[原版构建产物](offline/build-artifacts.json)、[观察版构建](offline/instrumented-build-result.json)、[观察版构建产物](offline/instrumented-build-artifacts.json)、[运行前manifest](offline/instrumented-prebuild-manifest.json)、[补丁](offline/instrumentation.patch)、[输入/二进制/日志/XML逐run完整hash账本](offline/runs/execution-summary.json)。
- 原始事件日志：[512](offline/runs/512/instrumented/stderr.log)、[2048](offline/runs/2048/instrumented/stderr.log)、[4096](offline/runs/4096/instrumented/stderr.log)、[8192](offline/runs/8192/instrumented/stderr.log)。每个目录均保留stdout、XML、run-result和pre-run-manifest，原版记录在对应original目录。
- 额外只读调试：[实际debugger日志](offline/diagnostic-2048/debugger.log)、[窄源码诊断](ssd/incomplete-flow-source.md)。AMU条件路径未动态绑定缺失命令，TSU旧指针不作为live证据。
- 后继：[MQ-IDENTITY-1最小合同](root/next-minimal-contract.json)尚未执行，限固定2048请求的唯一身份定位，不调参或扩矩阵；[冻结边界合同](root/boundary-contract.json)保留。
- 文档：[即时写前snapshot/hash](documentation/before-write-manifest.json)、[入口同步回执](documentation/receipt.json)、[本轮执行席账本](root/agent-ledger.json)。

D主线、I轻量not_ready、A5/A2保留及10月15—28日立题窗口不变；[CS1当前Z/费用/菜单局部停止](../d-service-model-20261015/README.md)保持。MIG1792023314继续paused；0原生DMA、Linux/RTL/UAPI修改、collector、FPGA/板操作或旧实验重跑。本轮实际两次私有host构建、8次冻结模拟＋1次失败触发的只读调试重放，不是0 build/0 sim。

共享研究Codex-only。证据任务请求Sol/high，普通实施及文档请求Sol/medium；actual运行元数据unknown，请求不等于运行证明。旧个人研究方法不恢复。
'''
# Protect current bytes until the final write, then apply only this batch.
for p,b in before.items():
 assert (W/p).read_bytes()==b, f'concurrent update: {p}; rerun merge required'
for p,s in after.items(): (W/p).write_text(s)
(R/'README.md').write_text(readme)
# Assertions use full byte segments, not paraphrases.
assert (W/'HANDOFF.md').read_bytes().endswith(before['HANDOFF.md'])
assert (W/'iommu-platform/docs/validation.md').read_bytes().endswith(before['iommu-platform/docs/validation.md'])
old=before['CURRENT_STATUS.md']; new=(W/'CURRENT_STATUS.md').read_bytes()
branch=old.split('## 各分支最新进展'.encode())[1].split('## 硬件与执行状态'.encode())[0]
engineering=old.split('## 独立工程事项'.encode())[1]
assert branch in new and new.endswith(engineering)
# Current canonical local links + newly prepended historical sections and new README only.
checks=[]
for p in paths+['iommu-platform/out/d-persistent-source-20261015/README.md']:
 text=(W/p).read_text()
 if p in ('HANDOFF.md','iommu-platform/docs/validation.md'):text=text[:-len(before[p].decode())]
 for m in re.finditer(r'\[[^\]]*\]\(([^)]+)\)',text):
  target=m.group(1).strip('<>')
  if target.startswith(('http:','https:','app:','codex:','#')):continue
  target=target.split('#')[0]
  q=Path(target) if target.startswith('/') else (W/p).parent/target
  checks.append({'document':p,'target':target,'exists':q.exists() or q.resolve()==(D/'receipt.json').resolve()})
assert all(c['exists'] for c in checks), [c for c in checks if not c['exists']]
receipt={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'canonical entry synchronization only; no new research/model/build/simulation by documentation worker','requested_model':'gpt-6.1-sol','requested_reasoning':'medium','actual_model_metadata':'unknown','files':[{**m,'after_sha256':h((W/m['path']).read_bytes())} for m in manifest], 'created_files':[{'path':str((R/'README.md').relative_to(W)),'before_sha256':None,'after_sha256':h((R/'README.md').read_bytes())}], 'protected_assertions':{'CURRENT_STATUS_A5_A2_branch_bytes':True,'CURRENT_STATUS_independent_engineering_suffix_bytes':True,'HANDOFF_entire_old_suffix_bytes':True,'validation_entire_old_suffix_bytes':True,'plan_retained_assets_and_hardware_suffix':after['研究计划.md'].split('## I：')[1]==before['研究计划.md'].decode().split('## I：')[1],'navigation_assets_code_and_history_suffix':after['研究导航.md'].split('## 已有成果')[1]==before['研究导航.md'].decode().split('## 已有成果')[1],'MIG_pause_preserved':all('paused' in after[p] or '暂停' in after[p] for p in paths),'no_old_method_restoration':not (W/'研究方法.md').exists()},'link_scope':'full three current canonical entries + new HANDOFF/validation prefixes + new run README; historical old suffix links not rescanned','local_link_count':len(checks),'local_links':checks,'actual_changes':{'CURRENT_STATUS.md':'replace D summary and execution paragraph; protected branch and engineering bytes retained','研究计划.md':'update current result, day18–19 step and next D action; other direction/asset/hardware text retained','研究导航.md':'update D row and next gate only','HANDOFF.md':'one new prefix, full old bytes as suffix','iommu-platform/docs/validation.md':'one new prefix, full old bytes as suffix','README.md':'new navigation to accepted source/decision/raw logs/full hash ledgers/next contract'}}
assert all(receipt['protected_assertions'].values())
(D/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'files':len(manifest),'local_links':len(checks),'protected':receipt['protected_assertions'],'concurrent_changes':[m['path'] for m in manifest if m['changed_since_run_start']]},ensure_ascii=False,indent=2))
