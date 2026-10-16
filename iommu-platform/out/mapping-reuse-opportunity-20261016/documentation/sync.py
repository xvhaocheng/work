from pathlib import Path
from datetime import datetime, timezone
import json, hashlib, re
W=Path('/home/xvhaocheng/work'); R=W/'iommu-platform/out/mapping-reuse-opportunity-20261016'; D=R/'documentation'
def h(b): return hashlib.sha256(b).hexdigest()
def meta(b): return {'bytes':len(b),'sha256':h(b)}
def js(p,o): p.parent.mkdir(parents=True,exist_ok=True); p.write_text(json.dumps(o,ensure_ascii=False,indent=2)+'\n')
targets=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md',str((R/'README.md').relative_to(W))]
agents=['AGENTS.md','iommu-platform/AGENTS.md']
# Immediate complete reads, including explicit absence of the new README.
before={p:(W/p).read_bytes() if (W/p).exists() else None for p in targets+agents}
assert before[targets[-1]] is None, 'README already exists: merge required'
for p,b in before.items():
    if b is not None:
        dest=D/'before-write'/p; dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(b)
frozen={str(p.relative_to(R)):h(p.read_bytes()) for part in ['root','reuse','observation'] for p in (R/part).rglob('*') if p.is_file()}
accepted=json.loads((R/'root/root-result-acceptance.json').read_text())
assert all(h((R/x['path']).read_bytes())==x['sha256'] for x in accepted['frozen_files'])
previous=W/'iommu-platform/out/mapping-integration-delta-20261016'
oldrun={str(p.relative_to(W)):h(p.read_bytes()) for p in previous.rglob('*') if p.is_file()}
js(D/'before-write-manifest.json',{'utc':datetime.now(timezone.utc).isoformat(),'files':{p:meta(b) if b is not None else {'exists':False} for p,b in before.items()},'frozen_files':frozen,'previous_run_hashes':oldrun,'method':'complete immediate bytes plus absence record; current sources re-read and concurrent facts retained'})

P='iommu-platform/out/mapping-reuse-opportunity-20261016/'
def links(p):return f'[本轮入口]({p}README.md)／[裁决]({p}root/decision-analysis.md)／[稳定接受]({p}root/root-result-acceptance.json)／[证据合同]({p}root/evidence-contract.json)／[下一合同]({p}root/next-minimal-contract.json)'
done='MAP-REUSE-OPPORTUNITY-1已完成源码来源、观测、配对与CPU合同。选定cyclic、linear、native-XDP、order0子切面每次RX allocation一页一个packet位置，重复B映射须跨真实recycle/再次alloc。源码支持正常再用机会及受限/退出条件；频率、完整CPU和收益未取得，native统计为null，不能写成0。'
nexttext='下一MAP-EVIDENCE-INPUT-1已指定、未执行：只定向定位并验收已有匹配原始输入及构建/设备配置，先核身份再算合格L0。无匹配输入保留条件界与精确缺项，不重复同合同/原语表/旧阈值，不自动开发collector、驱动、模型或板测，也不等一般授权或用户保证痛点。'
l0='若一allocation一packet、单次B redirect、无clone/retry、调用来源/记录唯一性及完整source/config epoch均已证明，两次成功B map可推出跨recycle再用，合格L0可先粗筛，不必先建完整逐alloc/CQ collector。L0不等accept、CQ、last-ref或CPU；redirect入队、batch sent不能逐frame配对。'
observation='hold/release不记录每次alloc/recycle，source release不等物理free。page map/unmap事件均在backend后，两post时间戳间距不是完整CPU；SG unmap在前不可混用。零丢失仍可能有IOVA复用歧义，丢失、ABA与删失须隔离；未测值为unknown/null。'
host='本机一次只读身份检查为6.8.0-60-generic x86_64，可见物理netdev驱动e1000e/cdc_ncm、mlx5计数0，tracefs元数据读取EACCES。仅限当前可见sysfs，非全PCI/其他命名空间/远端清查；EACCES不等内核无功能，IOMMU group不证明有效translated/shared服务，当前身份不匹配固定v6.12链。'
hypothesis='V1-N/K继续作为条件低成本原生整合假设：固定B/config、K预留lazy sparse关联、满额O0、无独立idle eviction，frame/全部片段保护在途。source最后frame/片段归还后撤旧B图；target关闭另需禁新、CPU reader drain与真实DMA drain，竞争须唯一撤图。普通O3同接口等价只判策略增量，R是条件普通构造而非已发表网络实现；未证独立创新、净收益或共享IOMMU干扰，未选论文主线。'
resource='相同工作/服务下CPU或不可回收内存改善可作主要价值；实际每策略S/P/C/Q物理并集与分配反馈、metadata/PTE、IOVA、带宽和有效完成分别报告。stock也无idle B pin，V1无天然payload RAM节省；K仅界关联库存/Q，不保证总RAM差或时限。O0轨迹不能直接作改变回收策略后的候选轨迹。'
execution='本轮新增3份固定commit trace header HTTP，19项worker来源hash/58项行界核验通过，含root共16个唯一路径；只核来源定位。1次只读host元数据检查，无sudo/mount、trace enable、probe attach、流量或设备写；0原生工作负载、0新模型/旧账重跑、0构建、0生产Linux/RTL/ABI编辑及板测。'
runtime='共享研究Codex-only，未用DeepSeek/Claude；请求source再用Sol/xhigh、trace源核Sol/high、文档Sol/medium，实际独立模型/强度元数据unknown，未声称切换root或改模型规则。'
boundaries='B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2及独立工程保持；MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集、实际债务unknown。10月15—28日窗口不重启，不自动升位其他分支。'
historylinks='[整合差项](iommu-platform/out/mapping-integration-delta-20261016/README.md)／[寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)／[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)'
oldcur=before['CURRENT_STATUS.md'].decode(); branch=oldcur[oldcur.index('## 各分支最新进展'):oldcur.index('## 硬件与执行状态')]; independent=oldcur[oldcur.index('## 独立工程事项'):]
current=f'''# 当前状态：再用与观测合同完成，下一验收已有匹配输入

2026年10月16日（JST）。**{done}** 保留V1-N/K条件整合假设，未证独立创新/净收益/共享IOMMU干扰，未选论文主线。[研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- {nexttext} {links(P)}。
- {l0} [来源机会]({P}reuse/source-opportunity.md)／[观测合同]({P}observation/observation-contract.md)。
- {observation} [CPU与机会界]({P}root/cpu-and-opportunity-contract.md)。pool_size是回流ring容量，不是全部source页预算；实际是否采用cyclic仍缺配置。
- {resource} 前轮历史：{historylinks}。

{branch}## 硬件与执行状态

{host} [只读身份]({P}root/host-readonly-identity.json)。{execution} [执行账]({P}root/execution-ledger.json)。{runtime} {boundaries}

{independent}'''
plan=f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{done}** {nexttext} 10月15—28日窗口、既有授权与暂停状态保持，未选论文主线。

本文件是唯一总计划；[当前状态](CURRENT_STATUS.md)记录执行，[研究导航](研究导航.md)定位资产；分支合同不改写全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按阶段使用，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 整合差项、正常source再用/退出、观测/CPU证据合同已完成 | 下一转已有匹配输入定位，不用更多相同合同代替数据 |
| 10月18—19日：首个决定性判别 | 已有原始记录的身份与完整性验收，合格L0先粗筛 | 证明几何/调用来源/epoch；L0不升级accept/CQ/CPU |
| 10月20—24日：通过后扩大 | 依匹配输入评估正常重复和可归属完整费用 | 仅有post标记不算CPU；无合格输入保留条件界，不自动开发 |
| 10月25—28日：研究裁决 | 具体普通对照差异、完整资源与服务结果、后继合同 | CPU或不可回收内存可作主要价值，未证不写论文完成 |

{links(P)}。当前后继只指定、尚未执行，原窗口不重算。

## 来源与最小证据入口

固定官方v6.12 mlx5e cyclic RQ、native-XDP linear、普通page-backed order0非XSK子切面；T为完整granule及后续用途持续授权的条件投影，S另列。不是所有部署均采用cyclic；本轮不以generic fragment支持推同allocation多包。正常refill先free再alloc与pool LIFO支持再用偏序，consumer未归还、ring满或实际NUMA变化可限制/退出，尚不知频率或主导原因。pool_size只界回流ring容量。

{l0} {observation} [source分析]({P}reuse/source-opportunity.md)／[事件矩阵]({P}observation/event-matrix.json)／[CPU合同]({P}root/cpu-and-opportunity-contract.md)。条件配对机会界不是CPU或在线未来输入，直接删除map/unmap的CPU预算界也不是总系统/吞吐界。

{host} [身份回执]({P}root/host-readonly-identity.json)。只查明确资产范围未提供匹配native记录，不声称全仓或远端无记录。[已查范围]({P}root/scoped-asset-audit.json)。

## V1-N/K与既有研究边界

{hypothesis} 两逻辑FIFO项未编译，普通R/slot/O3同享。{resource}

{nexttext} 前轮历史：{historylinks}／[整页历史](iommu-platform/out/mapping-organization-source-20261016/README.md)／[B1](iommu-platform/out/b1-stripe-cost-20261016/README.md)。旧累计阈值归普通O3，不重跑数字恢复新意；旧整页协议不执行、候选2不展开。{boundaries}

'''
plan+='## I：'+before['研究计划.md'].decode().split('## I：',1)[1]
nav=f'''# 共享IOMMU研究导航

2026年10月16日（JST）。MAP-REUSE-OPPORTUNITY-1已完成；下一MAP-EVIDENCE-INPUT-1指定未执行，转已有匹配原始输入验收。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；阶段原文见[HANDOFF](HANDOFF.md)。

## 当前研究与下一关口

| 内容 | 当前结论与边界 | 入口 |
| --- | --- | --- |
| 正常source再用 | cyclic/linear native XDP order0一allocation一packet；重复B须跨recycle，频率未知 | [本轮入口]({P}README.md)／[source分析]({P}reuse/source-opportunity.md)／[机会卡]({P}reuse/opportunity-card.json) |
| L0/L1/L2与完整CPU | 合格L0可先粗筛；不等accept/CQ/last-ref/CPU，native统计null | [证据合同]({P}root/evidence-contract.json)／[观测]({P}observation/observation-contract.md)／[事件矩阵]({P}observation/event-matrix.json)／[CPU合同]({P}root/cpu-and-opportunity-contract.md) |
| 当前可达身份 | 可见本机6.8，mlx5计数0；tracefs EACCES非功能缺失，非全PCI/远端清查 | [只读身份]({P}root/host-readonly-identity.json)／[已查资产范围]({P}root/scoped-asset-audit.json) |
| MAP-EVIDENCE-INPUT-1 | 已指定未执行，只验收已有匹配输入，不自动collector/驱动/板测 | [下一合同]({P}root/next-minimal-contract.json) |
| V1-N/K条件整合假设 | 保留，未证净收益/独立创新/共享干扰；O3同接口只判策略，普通R是条件构造 | [裁决]({P}root/decision-analysis.md)／[前轮整合](iommu-platform/out/mapping-integration-delta-20261016/README.md) |
| 历史与其他分支 | 前轮原文及B1/I/D/A5/A2边界保持，不自动升位 | [寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)／[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)／[整页历史](iommu-platform/out/mapping-organization-source-20261016/README.md)／[B1](iommu-platform/out/b1-stripe-cost-20261016/README.md) |

{execution} [稳定接受]({P}root/root-result-acceptance.json)／[执行账]({P}root/execution-ledger.json)。原窗口与MIG暂停保持，未选论文主线。方法按[ARS-Codex入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)分阶段使用。

'''
nav+='## 已有成果怎样复用'+before['研究导航.md'].decode().split('## 已有成果怎样复用',1)[1]
def history(p):return f'''# 2026年10月16日 MAP-REUSE-OPPORTUNITY-1完成：转已有匹配输入验收

本次仅同步root稳定接受。{done}

{l0} {observation} 正常refill与pool LIFO给条件再用机会；consumer未归还、ring满和实际NUMA变化给限制/退出条件，未证频率或主导原因。pool_size是回流ring容量，不是全部source页上限；实际cyclic配置尚缺。

{host} 已查范围仅明确列出的源码/设计/说明账资产，不声称全仓或远端无匹配记录。

{hypothesis} {resource}

{nexttext}

{execution} {runtime} {boundaries}

{links(p)}／[来源核验]({p}root/source-verification.json)／[执行账]({p}root/execution-ledger.json)／[文档验收]({p}documentation/verification.json)。文档即时完整原件/hash、四保护块、两旧全文suffix、双AGENTS及冻结研究输入均保护；以下旧全文字节保留，14个既存历史坏链照录不修。

'''
new={'CURRENT_STATUS.md':current.encode(),'研究计划.md':plan.encode(),'研究导航.md':nav.encode(),'HANDOFF.md':history(P).encode()+before['HANDOFF.md'],'iommu-platform/docs/validation.md':history('../out/mapping-reuse-opportunity-20261016/').encode()+before['iommu-platform/docs/validation.md']}
readme=f'''# 正常再用机会与现成观测：有界来源合同交付

2026-10-16 JST。{done}

{l0} {observation}

{host}

{hypothesis} {resource}

{nexttext}

| 内容 | 入口 |
| --- | --- |
| root稳定接受 | [裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[证据合同](root/evidence-contract.json)／[CPU与机会界](root/cpu-and-opportunity-contract.md) |
| 来源再用 | [source分析](reuse/source-opportunity.md)／[机会卡](reuse/opportunity-card.json)／[来源清单](reuse/source-manifest.json) |
| 现成事件 | [观测合同](observation/observation-contract.md)／[事件矩阵](observation/event-matrix.json)／[来源](observation/source-manifest.json)／[3份HTTP回执](observation/download-receipts.json) |
| 输入与执行 | [下一合同](root/next-minimal-contract.json)／[只读host身份](root/host-readonly-identity.json)／[已查范围](root/scoped-asset-audit.json)／[来源核验](root/source-verification.json)／[执行账](root/execution-ledger.json)／[同步合同](root/canonical-sync-contract.json) |
| 文档回执 | [即时完整快照清单](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[验收](documentation/verification.json)／[变更清单](documentation/changed-files.json) |
| 前轮历史 | [整合差项](../mapping-integration-delta-20261016/README.md)／[寿命设计](../mapping-lifetime-design-20261016/README.md)／[接纳账](../mapping-admission-20261016/README.md)／[整页历史](../mapping-organization-source-20261016/README.md)／[B1](../b1-stripe-cost-20261016/README.md) |

{execution} {runtime} {boundaries}

文档席未重做来源研究或执行原生测量。历史全文、两AGENTS与冻结研究产物保护；14个既存历史document-target坏链照录，未修历史，也不声称原来有效。
'''
new[targets[-1]]=readme.encode()
assert all(((W/p).read_bytes() if (W/p).exists() else None)==b for p,b in before.items()),'concurrent changes: re-read and merge'
for p,b in new.items(): (W/p).write_bytes(b)
checks={}
for p,b in before.items():
    checks['snapshot_'+p]= (D/'before-write'/p).read_bytes()==b if b is not None else not (D/'before-write'/p).exists()
for p in agents:checks['unchanged_'+p]=(W/p).read_bytes()==before[p]
checks['CURRENT_branches_exact']=branch.encode() in (W/'CURRENT_STATUS.md').read_bytes()
checks['CURRENT_independent_exact']=(W/'CURRENT_STATUS.md').read_bytes().endswith(independent.encode())
for p,mark in [('研究计划.md','## I：'),('研究导航.md','## 已有成果怎样复用')]: checks['protected_suffix_'+p]=(W/p).read_bytes().endswith(before[p][before[p].index(mark.encode()):])
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']: checks['full_old_suffix_'+p]=(W/p).read_bytes().endswith(before[p])
for p,sha in frozen.items():checks['frozen_'+p]=h((R/p).read_bytes())==sha
checks['previous_run_all_bytes_unchanged']=all(h((W/p).read_bytes())==sha for p,sha in oldrun.items())
for p,b in new.items():checks['written_'+p]=(W/p).read_bytes()==b
prior=json.loads((previous/'documentation/verification.json').read_text());broken=prior['preexisting_broken_local_links']
checks['preexisting_14_still_broken']=len(broken)==14 and all(not ((W/x['file']).parent/x['target']).exists() for x in broken)
changes={p:{'before':meta(before[p]) if before[p] is not None else None,'after':meta(b),'mode':'prepend_full_original_suffix' if p in ['HANDOFF.md','iommu-platform/docs/validation.md'] else 'allowed_current_sections' if before[p] is not None else 'create'} for p,b in new.items()}
js(D/'changed-files.json',{'files':changes,'receipts_and_snapshots':str(D.relative_to(W)),'AGENTS_changed':False,'production_changed':False})
js(D/'update-ledger.json',{'date':'2026-10-16','task':'MAP-REUSE-OPPORTUNITY-1 final canonical sync','source_of_truth':'../root/root-result-acceptance.json','configuration':{'requested_model':'gpt-6.1-sol','requested_reasoning':'medium','actual_model':'unknown','actual_reasoning':'unknown'},'authorship_execution':'Codex-only documentation seat, no DeepSeek/Claude','files':list(changes),'concurrency':'complete current reads and immediate originals; rechecked unchanged just before application; protected concurrent facts retained','completed':'MAP-REUSE-OPPORTUNITY-1','next':'MAP-EVIDENCE-INPUT-1 specified_not_executed','execution':'administrative document/hash/link checks only; no new research, downloads, host probe, source check rerun, model, old ledger, build or device action'})
js(D/'verification.json',{})
newtexts={p:b.decode() for p,b in new.items()};newtexts['HANDOFF.md']=history(P);newtexts['iommu-platform/docs/validation.md']=history('../out/mapping-reuse-opportunity-20261016/')
linklist=[]
for p,t in newtexts.items():
    for dest in sorted(set(re.findall(r'\[[^\]]*\]\(([^)]+)\)',t))):
        if '://' in dest or dest.startswith('#'):continue
        path=re.sub(r':\d+(?::\d+)?$','',dest.strip('<>').split('#')[0]);linklist.append({'file':p,'target':dest,'exists':((W/p).parent/path).exists()})
checks['all_current_and_new_links_exist']=all(x['exists'] for x in linklist)
assert all(checks.values()),[k for k,v in checks.items() if not v]
js(D/'verification.json',{'passed':True,'checks':checks,'check_count':len(checks),'new_local_links':linklist,'new_local_link_count':len(linklist),'preexisting_broken_local_links':broken,'preexisting_broken_count':14,'preexisting_handling':'copied prior 14 unique(document,target); untouched historical bytes preserve records; not repaired','final_hashes':{p:meta(b) for p,b in new.items()},'frozen_file_count':len(frozen),'previous_run_file_count':len(oldrun),'configuration':{'requested_model':'gpt-6.1-sol','requested_reasoning':'medium','actual_model':'unknown','actual_reasoning':'unknown'},'execution':'document bytes/hash/link checks only; research/source-range checks not rerun','before_write_manifest':'before-write-manifest.json'})
print(json.dumps({'passed':True,'checks':len(checks),'new_links':len(linklist),'frozen_files':len(frozen),'previous_run_files':len(oldrun),'historical_bad_links':14,'files':list(changes)},ensure_ascii=False))
