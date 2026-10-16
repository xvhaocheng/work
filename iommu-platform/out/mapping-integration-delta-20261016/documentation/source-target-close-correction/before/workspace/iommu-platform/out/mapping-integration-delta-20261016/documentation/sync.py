from pathlib import Path
import hashlib, json, re
from datetime import datetime, timezone

W = Path('/home/xvhaocheng/work')
R = W / 'iommu-platform/out/mapping-integration-delta-20261016'
D = R / 'documentation'
def digest(b): return hashlib.sha256(b).hexdigest()
def info(b): return {'bytes': len(b), 'sha256': digest(b)}
def js(p, obj): p.write_text(json.dumps(obj, ensure_ascii=False, indent=2) + '\n')
targets = ['CURRENT_STATUS.md', '研究计划.md', '研究导航.md', 'HANDOFF.md', 'iommu-platform/docs/validation.md']
protected = ['AGENTS.md', 'iommu-platform/AGENTS.md']
# Re-read every complete current target immediately before snapshot and write.
before = {p: (W / p).read_bytes() for p in targets + protected}
for p, b in before.items():
    dest = D / 'before-write' / p
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_bytes(b)
frozen = {str(p.relative_to(R)): digest(p.read_bytes()) for part in ['root', 'owner', 'dmabuf', 'before'] for p in (R / part).rglob('*') if p.is_file()}
acceptance = json.loads((R / 'root/root-result-acceptance.json').read_text())
assert all(digest((R / f['path']).read_bytes()) == f['sha256'] for f in acceptance['frozen_files'])
js(D / 'before-write-manifest.json', {'utc': datetime.now(timezone.utc).isoformat(), 'files': {p: info(b) for p,b in before.items()}, 'frozen_files': frozen, 'method': 'complete immediate bytes; no pre-task snapshot used as current state'})

base = 'iommu-platform/out/mapping-integration-delta-20261016/'
def links(prefix):
    return f'[本轮入口]({prefix}README.md)／[裁决]({prefix}root/decision-analysis.md)／[稳定接受]({prefix}root/root-result-acceptance.json)／[下一合同]({prefix}root/next-minimal-contract.json)'
summary = 'OWNER-BINDING-INTEGRATION-DELTA-1已完成。保留V1-N/K：固定B与配置、K个预留lazy sparse关联、满额走O0、无独立idle eviction；真实frame及全部片段寿命可取消逐TX token/CQ binding handle。拟议mode＋frame两逻辑FIFO项未编译，普通R/slot/O3同享，不作独占、字节或速度收益主张。未确认独立论文贡献或净赢，未选论文主线。'
nexttext = '下一MAP-REUSE-OPPORTUNITY-1已指定、未执行：只定正常map/unmap CPU费用、source再用与真正退出机会的来源、观测及配对证据合同。无匹配原生记录可作来源绑定的条件分析，不等保证胜例或一般授权，不重列同字段表/旧阈值，不自动采集、开发或建模。'
resources = '源本来持有、B实际延后、复制目标C与退出Q按物理并集去重，各策略分别导出回收/分配反馈，metadata/PTE与IOVA另计。stock O0也无idle B pin，V1相对stock无天然payload RAM节省；K只界关联库存/Q数量，不保证总RAM差或完成时限。相同工作与服务下CPU或不可回收内存改善可作主要贡献。'
boundaries = 'B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2及独立工程事项保持；MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集、实际债务unknown。10月15—28日窗口不重启，不自动升位其他分支。'
runtime = '共享研究Codex-only，未用DeepSeek/Claude；请求dma-buf源核Sol/high、owner设计Sol/xhigh、文档Sol/medium，实际独立模型/强度元数据unknown，未声称切换root或改模型规则。'
execution = '14项worker来源hash、38项行界检查通过，含root共16个唯一路径；只核来源定位，不证明并发实现正确。0新下载、0新模型/旧阈值账、0原生运行/构建/生产Linux/RTL/ABI编辑/设备动作；设计推导不是运行结果。'
ordinary = 'R-OB4K-T-N是本轮具名普通构造，不是已发表或已实现同名系统；其覆盖当前原语不推出架构已发表、所有完整成本相同或思想族无价值。O3共享付费接口等价只判策略增量0。dma-buf允许K个可重用对象、聚合与控制面准备，不强收每包export；标准睡眠map/unmap、自有热cache与私有range协议分别列。'
close = 'source真正退出或target关闭须禁新、CPU reader drain、真实DMA drain与唯一撤图责任；相同DMA API前端支持稳定target下条件同步退出。关闭竞争仍可能需要Q，不能一概全收或全免；CPU grace不等于DMA完成。'

cur = before['CURRENT_STATUS.md'].decode()
branch = cur[cur.index('## 各分支最新进展'):cur.index('## 硬件与执行状态')]
independent = cur[cur.index('## 独立工程事项'):]
current = f'''# 当前状态：局部整合差项完成，下一核来源绑定的再用机会

2026年10月16日（JST）。**{summary}** [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- {nexttext} {links(base)}。
- {close} [关闭与K合同]({base}root/close-and-capacity-contract.md)。
- {ordinary} [owner比较]({base}owner/integration-analysis.md)／[dma-buf比较]({base}dmabuf/adapter-analysis.md)。
- {resources} 前轮[寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)与[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)留作历史入口。

{branch}## 硬件与执行状态

{execution} [来源验证]({base}root/source-verification.json)／[执行账]({base}root/execution-ledger.json)。{boundaries} {runtime}

{independent}'''

plan = f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{summary}** {nexttext} 10月15—28日窗口和既有授权/暂停状态保持。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支合同不替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按阶段使用，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 源寿命设计及字段/调用/锁/退出责任比较已完成；下一来源/可得性证据合同 | 固定原生链与条件授权，区分代码可得信息和已有记录 |
| 10月18—19日：首个决定性判别 | 正常B重复使用、source真正退出与完整CPU费用的可配对来源 | 无原生记录给条件上下界与缺项，不用M0/PMD数字替代 |
| 10月20—24日：通过后扩大 | 只依具体证据需求及可达身份决定最小采集/实现 | 不因成本未知自动开发；普通对照获得同接口与优化 |
| 10月25—28日：研究裁决 | 具体贡献、完整资源/服务结果与后继合同 | CPU或不可回收内存可作主要价值，未证不写论文完成 |

{links(base)}。当前只指定证据合同，未启动后继执行；不重算窗口。

## V1-N/K：共享最小设计与尚待核的价值

固定官方v6.12普通page-backed order0 mlx5e native XDP→B或本地UDP链；单buffer不等于每页一包，T持续授权是条件投影而非实际部署事实，S逐对象撤权另列。源owner、B关联及在途frame/全部片段寿命分清；固定B且无独立eviction时不再强制每TX独立token或CQ handle。两逻辑FIFO项只是未编译设计，同样给予普通适配。

{close} [关闭与容量合同]({base}root/close-and-capacity-contract.md)。

{ordinary} [普通owner整合]({base}owner/integration-analysis.md)／[dma-buf调用与上下文]({base}dmabuf/adapter-analysis.md)。低成本source退出接口/原生整合仍为候选；不能凭接口缺项或原语数量认领创新、CPU周期或完整成本优势。

{resources} {nexttext} [完整后继合同]({base}root/next-minimal-contract.json)。

## 既有问题与分支边界

累计接纳阈值归普通O3，补同例数字不恢复策略新意。保留[前轮寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)、[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)、[整页历史](iommu-platform/out/mapping-organization-source-20261016/README.md)及[B1入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)。旧整页协议不执行、候选2不展开；{boundaries}

'''
plan += before['研究计划.md'].decode().split('## I：', 1)[1].join(['## I：', ''])

nav = f'''# 共享IOMMU研究导航

2026年10月16日（JST）。{summary} 先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；阶段证据见[HANDOFF](HANDOFF.md)。

## 当前研究与下一关口

| 版本/方向 | 当前安排 | 入口 |
| --- | --- | --- |
| V1-N/K与普通R/slot/O3 | 局部整合比较已完成；共享无逐TX token的固定B设计，未编译、未证独占或净收益 | [本轮入口]({base}README.md)／[裁决]({base}root/decision-analysis.md) |
| MAP-REUSE-OPPORTUNITY-1 | 已指定未执行；正常再用/退出及完整CPU费用的来源、观测和配对合同 | [下一合同]({base}root/next-minimal-contract.json) |
| 关闭与资源 | 稳定target条件同步分支、具体竞争/Q责任；K只界库存，stock也无idle pin | [关闭/K合同]({base}root/close-and-capacity-contract.md) |
| 具名普通对照 | R为本轮构造；dma-buf允许slot/span/prepare，自有cache与标准睡眠入口分开 | [owner分析]({base}owner/integration-analysis.md)／[原语表]({base}owner/primitive-table.json)／[dma-buf分析]({base}dmabuf/adapter-analysis.md)／[调用表]({base}dmabuf/call-cost-table.json) |
| 前轮历史入口 | 寿命设计、普通O3费用账和整页合同原样保留，不重复执行 | [寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)／[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)／[整页历史](iommu-platform/out/mapping-organization-source-20261016/README.md) |
| B1 / I / D / A5A2 | 既有触发/未就绪/版本边界及工程资产保持，不自动升位 | [B1](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md) |

{nexttext} {execution} [稳定接受]({base}root/root-result-acceptance.json)／[执行账]({base}root/execution-ledger.json)。MIG暂停与原窗口保持，未选论文主线。

'''
nav += '## 已有成果怎样复用' + before['研究导航.md'].decode().split('## 已有成果怎样复用', 1)[1]

def history(prefix):
    return f'''# 2026年10月16日 OWNER-BINDING-INTEGRATION-DELTA-1完成与再用机会下一合同

仅同步root稳定接受，不重调研或重跑模型。{summary}

{ordinary}

{close} {resources}

{nexttext}

{execution} {runtime} {boundaries}

{links(prefix)}／[关闭/K合同]({prefix}root/close-and-capacity-contract.md)／[来源验证]({prefix}root/source-verification.json)／[执行账]({prefix}root/execution-ledger.json)／[文档验证]({prefix}documentation/verification.json)。写前完整即时原件/hash、四保护块、两旧全文suffix和双AGENTS保护见文档回执；以下旧记录全文字节保留，14个既存历史坏链接照录，不修历史。

'''

new = {'CURRENT_STATUS.md': current.encode(), '研究计划.md': plan.encode(), '研究导航.md': nav.encode(), 'HANDOFF.md': history(base).encode()+before['HANDOFF.md'], 'iommu-platform/docs/validation.md': history('../out/mapping-integration-delta-20261016/').encode()+before['iommu-platform/docs/validation.md']}
readme = f'''# 固定B的owner关联整合差项：本轮交付

2026-10-16 JST。{summary}

{close} {ordinary}

{resources}

{nexttext}

| 内容 | 入口 |
| --- | --- |
| root接受与分析 | [稳定接受](root/root-result-acceptance.json)／[裁决](root/decision-analysis.md)／[关闭与K合同](root/close-and-capacity-contract.md) |
| worker整合证据 | [owner分析](owner/integration-analysis.md)／[原语表](owner/primitive-table.json)／[owner来源](owner/source-manifest.json)／[dma-buf分析](dmabuf/adapter-analysis.md)／[调用表](dmabuf/call-cost-table.json)／[dma-buf来源](dmabuf/source-manifest.json) |
| 后继与执行边界 | [下一合同](root/next-minimal-contract.json)／[来源验证](root/source-verification.json)／[执行账](root/execution-ledger.json)／[同步合同](root/canonical-sync-contract.json) |
| 即时文档回执 | [写前完整快照清单](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[验证](documentation/verification.json)／[变更清单](documentation/changed-files.json) |
| 前轮历史 | [寿命设计](../mapping-lifetime-design-20261016/README.md)／[接纳账](../mapping-admission-20261016/README.md)／[整页历史](../mapping-organization-source-20261016/README.md)／[B1](../b1-stripe-cost-20261016/README.md) |

{execution} {runtime} {boundaries}

文档席只同步冻结研究结论；现行分支、独立工程及两份AGENTS保持，旧研究输入不改。14个既存历史document-target坏链接照录，不声称原来有效。
'''
# Detect concurrent writes between immediate read and application.
assert all((W/p).read_bytes() == b for p,b in before.items()), 'concurrent write; re-read and merge required'
for p,b in new.items(): (W/p).write_bytes(b)
(R/'README.md').write_text(readme)

checks = {}
now = {p:(W/p).read_bytes() for p in targets+protected}
checks['CURRENT_branches_exact'] = branch.encode() in now['CURRENT_STATUS.md']
checks['CURRENT_independent_exact_suffix'] = now['CURRENT_STATUS.md'].endswith(independent.encode())
for p, marker in [('研究计划.md','## I：'), ('研究导航.md','## 已有成果怎样复用')]:
    suffix = before[p][before[p].index(marker.encode()):]
    checks[p+'_protected_suffix_exact'] = now[p].endswith(suffix)
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']: checks[p+'_old_full_suffix_exact'] = now[p].endswith(before[p])
for p in protected: checks[p+'_full_exact'] = now[p] == before[p]
for p,b in before.items(): checks['snapshot_'+p] = (D/'before-write'/p).read_bytes() == b
for p,h in frozen.items(): checks['frozen_'+p] = digest((R/p).read_bytes()) == h
for p,b in new.items(): checks['written_'+p] = now[p] == b

prior = json.loads((W/'iommu-platform/out/mapping-lifetime-design-20261016/documentation/verification.json').read_text())
preexisting = prior['preexisting_broken_local_links']
checks['14_preexisting_broken_targets_unchanged'] = len(preexisting) == 14 and all(not ((W/e['file']).parent / e['target']).exists() for e in preexisting)
changes = {p:{'before':info(before[p]), 'after':info(b), 'mode': 'prepend_preserving_full_suffix' if p in ['HANDOFF.md','iommu-platform/docs/validation.md'] else 'replace_allowed_prefix_or_sections'} for p,b in new.items()}
changes[str((R/'README.md').relative_to(W))] = {'before': None, 'after':info((R/'README.md').read_bytes()), 'mode':'create'}
js(D/'changed-files.json', {'files':changes, 'receipt_and_snapshot_directory':str(D.relative_to(W)), 'AGENTS_changed':False, 'production_changed':False})
js(D/'update-ledger.json', {'date':'2026-10-16', 'task':'OWNER-BINDING-INTEGRATION-DELTA-1 canonical sync', 'requested_configuration':{'model':'gpt-6.1-sol','reasoning':'medium'}, 'actual_model':'unknown', 'actual_reasoning':'unknown', 'authorship_and_execution':'Codex documentation seat; no DeepSeek/Claude', 'source_of_truth':'../root/root-result-acceptance.json', 'files':list(changes), 'concurrency':'complete current originals re-read immediately; assert unchanged at write; protected independent facts merged', 'next':'MAP-REUSE-OPPORTUNITY-1 specified_not_executed', 'checks_only':'document bytes/hash/links/JSON; no research/source rerun, model, download, build, production or device action'})
# Create the receipt target before checking all newly written links.
js(D/'verification.json', {})
newtexts = {p:b.decode() for p,b in new.items()}
newtexts['HANDOFF.md'] = history(base)
newtexts['iommu-platform/docs/validation.md'] = history('../out/mapping-integration-delta-20261016/')
newtexts[str((R/'README.md').relative_to(W))] = readme
newlinks = []
for p, txt in newtexts.items():
    for target in sorted(set(re.findall(r'\[[^\]]*\]\(([^)]+)\)',txt))):
        if '://' in target or target.startswith('#'): continue
        target = target.strip('<>').split('#')[0]
        path = re.sub(r':\d+(?::\d+)?$', '', target)
        newlinks.append({'file':p,'target':target,'exists':((W/p).parent/path).exists()})
checks['all_current_and_new_links_exist'] = all(e['exists'] for e in newlinks)
assert all(checks.values()), [k for k,v in checks.items() if not v]
js(D/'verification.json', {'date':'2026-10-16','passed':all(checks.values()),'checks':checks,'check_count':len(checks),'new_local_links':newlinks,'new_local_link_count':len(newlinks),'preexisting_broken_local_links':preexisting,'preexisting_broken_count':len(preexisting),'preexisting_handling':'copied prior 14 document-target records; original full histories preserved, not repaired','before_write_manifest':'before-write-manifest.json','frozen_input_count':len(frozen),'actual_model':'unknown','actual_reasoning':'unknown','execution':'document administrative checks only; frozen source/range checks not rerun'})
print(json.dumps({'passed':True,'check_count':len(checks),'new_link_count':len(newlinks),'historical_broken':len(preexisting),'frozen_files':len(frozen),'changed_files':list(changes)},ensure_ascii=False))
