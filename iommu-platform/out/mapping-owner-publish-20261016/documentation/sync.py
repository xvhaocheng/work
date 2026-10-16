from pathlib import Path
import hashlib,json,re,datetime
W=Path('/home/xvhaocheng/work'); R=W/'iommu-platform/out/mapping-owner-publish-20261016'; D=R/'documentation'
def info(b): return {'bytes':len(b),'sha256':hashlib.sha256(b).hexdigest()}
def dump(p,x): p.write_text(json.dumps(x,ensure_ascii=False,indent=2)+'\n')
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
protected=['AGENTS.md','iommu-platform/AGENTS.md']
# Immediate reread, rather than the task-start snapshot, supplies every preserved byte.
before={p:(W/p).read_bytes() for p in paths+protected}
assert not (R/'README.md').exists(), 'README already exists; reconcile rather than overwrite'
for p,b in before.items():
    dest=D/'before-write'/p; dest.parent.mkdir(parents=True,exist_ok=True); dest.write_bytes(b)
initial={x['path']:x for x in json.loads((R/'root/input-manifest.json').read_text())['files']}
drift={p:{'initial':{k:initial[p][k] for k in ['bytes','sha256']},'immediate_before_write':info(b),'changed_before_write':initial[p]['sha256']!=info(b)['sha256'],'handling':'latest bytes used; protected blocks and full history suffixes retained'} for p,b in before.items()}
dump(D/'before-write-manifest.json',{'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'files':[{ 'path':p,**info(b)} for p,b in before.items()],'initial_to_before_write_drift':drift})
frozen={str(p.relative_to(R)):info(p.read_bytes()) for area in ['root','napi-source','close-protocol'] for p in (R/area).rglob('*') if p.is_file()}
dump(D/'frozen-before.json',frozen)
base='iommu-platform/out/mapping-owner-publish-20261016'
lead='MAP-OWNER-RECALL-PUBLISH-1已完成源码绑定的条件控制协议，保留owner-assisted private alloc.cache-only冷B关联回收及低成本整合假设；未实现，未证独占创新或净收益，未选论文主线。'
next_text='下一MAP-OWNER-INTEGRATION-CONTRIBUTION-GATE-1为evidence_gated_not_auto_scheduled：仅有新具体实现／部署／费用差项或新代表性可运行输入才作有界贡献判断；不继续拆相同协议，不重复已结束的数据入口搜索或旧阈值账，不由缺成本自动开发。无需等待一般授权、CPU／内存价值确认或用户保证痛点。'
protocol='每source池／固定B配置用有界mailbox，miss先pending再合法kick，短irq-safe锁同序关门与直接napi_schedule；positive-budget owner在complete前只取旧snapshot，每次最多一页，ACK不擦新pending，当前帧仍O0。NO_ELIGIBLE终结本代，terminal ACK后才显式REARM，不自动cache-entry通知或空转。REQUEST_ACCEPTED、OWNER_ACK、UNMAP_DONE、RETIRED分开，必要CPU metadata退休后才返K；无未来owner服务或返槽墙钟保证，ring-only／全在途／无owner／退休不前进可不返槽。'
lifetime='napi_schedule的bool、MISSED、complete和disable均不是应用ACK或DMA排空；enabled普通完成三竞态仅给条件唤醒偏序，实际特殊模式unknown。disable只等所有权释放、不等poll返回；enable不替应用重放pending。poll处于RCU读侧，不等自身grace或反等自己的closer。gate＋kick线性化，解绑后禁止旧NAPI；拟议ctx source-anchor须组合保留现有XDP disconnect，ctx不pin pool，最后source release后不回读pool。实际anchor/ref、锁序和reader集合未实现验证。'
bclose='持久组织关闭可以保留合法B O0及共享A；真正B/config销毁须禁全部新B DMA（含O0）、CPU静默、实际DMA完成或stop及配置寿命。正常隔离idle页不另要求全B drain；真实core/FW stop语义本轮未取得。'
value='O0本有B-only／A留池，DMA_PMD本有idle＋allocator排他原理。R-OB4K-T-N／O3／slot同享新付费接口可用同协议，本轮无独占策略／控制算法或必须更低费用的依据；这不等已发表网络实现或实测同总成本，保留整合价值。无独立逐TX／peralloc use token不等零新增热费用。CPU、真实S/P/C/Q并集及分配反馈、ctx/mailbox/K-retire metadata、PTE、IOVA、带宽与有效完成分报；回收关联不等payload RAM下降，不强套O0 PFN，native数值null。'
execution='本轮定向新增2个官方固定commit文件及HTTP200 receipts；9项worker primary hash/size、17处root直接读段行界通过，仅provenance。0新状态或性能模型、native／旧实验／旧账／构建、host设备probe、driver／collector／RTL／ABI编辑或板测。Codex-only；NAPI核查请求Sol/high、关闭协议Sol/xhigh、文档Sol/medium；独立actual model/effort metadata unknown，不声称root切换。'
keep='B1/I/D/A5/A2及并发GitHub/Linux/RTL独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。'
def refs(prefix):
 return '／'.join(f'[{label}]({prefix}/{target})' for label,target in [('本轮入口','README.md'),('裁决','root/decision-analysis.md'),('假设卡','root/hypothesis-card.json'),('接受','root/root-result-acceptance.json'),('下一证据gate','root/next-minimal-contract.json')])
old={p:b.decode() for p,b in before.items()}
cur='# 当前状态：owner冷关联回收条件协议完成，下一为贡献证据门槛\n\n2026年10月16日（JST）。**'+lead+'** [研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。\n\n## 全局安排与下一证据\n\n- '+protocol+'\n- '+next_text+'\n- 同接口R/O3可采用本协议，未证独占或完整成本更低；回收B关联不等payload RAM下降，CPU、物理并集与分配反馈、metadata/PTE/IOVA、带宽及完成分报，native仍null。'+refs(base)+'。\n\n'
cur+=old['CURRENT_STATUS.md'].split('## 各分支最新进展',1)[1].split('## 硬件与执行状态',1)[0].join(['## 各分支最新进展',''])
cur+='## 硬件与执行状态\n\n'+execution+' '+keep+' 详见[执行账]('+base+'/root/execution-ledger.json)。\n\n## 独立工程事项'+old['CURRENT_STATUS.md'].split('## 独立工程事项',1)[1]
plan=old['研究计划.md']
old_intro=plan.split('\n\n')[1]
plan=plan.replace(old_intro,'2026年10月16日（JST）。**'+lead+'** '+next_text+' 原10月15—28日窗口和既有暂停保持。',1)
plan=plan.replace('owner显式回收设计接受；下一有界发布/关闭协议 | 请求/ack/lifetime及失唤醒、自等待手工正反序列','owner显式回收及条件发布/关闭协议已接受 | 后继贡献gate仅由新具体差项或代表性输入触发')
plan=plan.replace('仅协议结果及具体成本问题触发后继扩大','仅新具体实现／部署／费用差项或代表性输入触发后继判断')
start=plan.index('[本轮入口]'); end=plan.index('## 前轮来源与最小证据入口')
plan=plan[:start]+refs(base)+'。前轮[cache-only选择](iommu-platform/out/mapping-recycle-reclaim-20261016/README.md)及[公开输入验收](iommu-platform/out/mapping-evidence-input-20261016/README.md)保留。\n\n## 当前owner-assisted cache-only条件协议\n\n'+protocol+'\n\n'+lifetime+' '+bclose+'\n\n'+value+' '+next_text+'\n\n'+plan[end:]
stale='下一MAP-OWNER-RECALL-PUBLISH-1已指定、未执行：仅核请求/pending→owner→ack/退休的发布、失唤醒及pool/B关闭寿命，给源码绑定的手工正反序列。不自动driver/collector/RTL、状态或性能模型、数据扫描、旧账或板测，不等一般授权/价值或保证痛点。'
assert stale in plan
plan=plan.replace(stale,next_text)
nav=old['研究导航.md']
nav=nav.replace(nav.split('\n\n')[1],'2026年10月16日（JST）。'+lead+' native统计仍null。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；阶段原文见[HANDOFF](HANDOFF.md)。',1)
nav=nav.replace('| MAP-OWNER-RECALL-PUBLISH-1 | 已指定未执行；只核请求/ack/寿命、失唤醒和RCU自等待，不自动实现 | [下一合同](iommu-platform/out/mapping-recycle-reclaim-20261016/root/next-minimal-contract.json) |','| MAP-OWNER-RECALL-PUBLISH-1 | 条件协议完成；有界mailbox、snapshot ACK、显式rearm及四层完成；并非实现证明 | '+refs(base)+' |\n| 下一贡献证据门槛 | MAP-OWNER-INTEGRATION-CONTRIBUTION-GATE-1；evidence_gated_not_auto_scheduled，仅新具体差项／代表性输入触发 | [贡献gate]('+base+'/root/next-minimal-contract.json) |')
a=nav.index('12项worker'); b=nav.index('## 已有成果怎样复用')
nav=nav[:a]+execution+' '+next_text+' 原窗口及MIG暂停保持，未选论文主线。\n\n'+nav[b:]
heading='# 2026年10月16日 MAP-OWNER-RECALL-PUBLISH-1：冷B关联条件控制协议完成\n\n'
body=lead+'\n\n'+protocol+'\n\n'+lifetime+' '+bclose+'\n\n'+value+'\n\n'+next_text+'\n\n'+execution+' '+keep+'\n\n'
new={'CURRENT_STATUS.md':cur,'研究计划.md':plan,'研究导航.md':nav}
for p,prefix in [('HANDOFF.md',base),('iommu-platform/docs/validation.md','../out/mapping-owner-publish-20261016')]:
 new[p]=heading+body+refs(prefix)+'／[文档验收]('+prefix+'/documentation/verification.json)。以下完整即时旧字节后缀及14条既存历史document,target坏链保留，双AGENTS不改不暂存。\n\n'+old[p]
readme='# MAP-OWNER-RECALL-PUBLISH-1 交付入口\n\n2026-10-16 JST。'+body+'''## 裁决与精确入口

- [root裁决](root/decision-analysis.md)：源码事实、首版协议、手工正反序列、具名普通对照、资源与决定。
- [假设卡](root/hypothesis-card.json)／[冻结接受](root/root-result-acceptance.json)／[下一贡献gate](root/next-minimal-contract.json)。
- [NAPI核查](napi-source/findings.md)／[API卡](napi-source/api-card.json)／[HTTP回执](napi-source/http-receipts.json)：固定Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`；本轮新增 `include/linux/netdevice.h`、`Documentation/networking/napi.rst`。
- [关闭协议核查](close-protocol/findings.md)／[协议卡](close-protocol/protocol-card.json)：source pool、NAPI/channel、ctx、B/config寿命分开。
- [来源清单](root/source-manifest.json)／[来源核对](root/source-verification.json)／[实际执行账](root/execution-ledger.json)。hash与行界仅为provenance；手工偏序不等并发实现或运行证明。
- [前轮cache-only选择](../mapping-recycle-reclaim-20261016/README.md)／[普通R](../mapping-integration-delta-20261016/owner/integration-analysis.md)／[slot适配](../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。

## 文档与保留验收

[同步合同](root/canonical-sync-contract.json)／[即时快照清单与初始drift](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[变更文件](documentation/changed-files.json)／[文档验收](documentation/verification.json)。

五份canonical以写前即时原文为基准合并；CURRENT分支和独立工程块、计划I起后缀、导航已有成果起后缀逐字保护，HANDOFF和validation完整旧字节作为后缀。14条既存历史document,target坏链照录，不修冻结历史。AGENTS已有用户编辑保留，本席未改／暂存；提交推送由root独立验收后处理。
'''
new[str((R/'README.md').relative_to(W))]=readme
# Fail closed on any concurrent change after the reread.
for p,b in before.items(): assert (W/p).read_bytes()==b, 'concurrent drift '+p
for p,s in new.items(): (W/p).write_text(s)
changes=[{'path':p,'before':info(before[p]) if p in before else None,'after':info((W/p).read_bytes()),'action':'updated' if p in before else 'created'} for p in new]
dump(D/'changed-files.json',{'scope':'5 canonical + run README; documentation artifacts listed separately','files':changes})
dump(D/'update-ledger.json',{'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'task':'MAP-OWNER-RECALL-PUBLISH-1 canonical sync','authorship':'Codex-only documentation synchronization from root accepted frozen evidence; no new research','runtime':{'requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown','root_switch_claimed':False},'initial_to_before_write_drift':drift,'protected':['CURRENT branch block and independent tail','plan I through EOF','navigation existing-assets through EOF','full HANDOFF and validation suffix','both AGENTS','root/napi-source/close-protocol frozen'],'no_commit_push_by_documentation_seat':True,'next':'MAP-OWNER-INTEGRATION-CONTRIBUTION-GATE-1 evidence_gated_not_auto_scheduled','production_edits_or_experiments':0})
print(json.dumps({'changed':list(new),'drift':{p:v['changed_before_write'] for p,v in drift.items()}},ensure_ascii=False))
