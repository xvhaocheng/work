"""Root-authored custody/document synchronization; never executes simulation."""
import json, hashlib, pathlib, ast, subprocess
W = pathlib.Path('/home/xvhaocheng/work')
P = W/'iommu-platform/out/stage117-delivery-g0-20261013'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def read(p): return json.loads(p.read_text())
def write(p,o): p.write_text(json.dumps(o,ensure_ascii=False,indent=2)+'\n')
I=P/'implementation'; C=P/'checker'; R=P/'root'
checker=read(C/'delivery.json'); summary=read(C/'summary.json')
assert checker['passed'] and summary['sixcases_passed']
# Seat has handed off and stopped. Root changes only the additive custody pointer.
delivery=read(I/'delivery.json'); old=delivery['independent_checker_acceptance'].copy()
delivery['independent_checker_acceptance'].update(delivery_sha256=sha(C/'delivery.json'),scope='new interface probe, six raw verifications, additive exact refill-owner proof on same six traces; seven unique negative controls and one PASS independence control; no RTL reexecution')
# Implementation manifest intentionally excludes delivery.json to avoid circular hash.
manifest=read(I/'artifact-sha256.json')
delivery_entries=[x for x in manifest if x['path']=='delivery.json']
assert not delivery_entries, 'inspect cycle convention before changing manifest'
write(I/'delivery.json',delivery)
write(R/'checker-pointer-update.json',dict(author='root',reason='current checker owner-binding correction after implementation seat handoff',previous=old,current=delivery['independent_checker_acceptance'],implementation_source_or_binary_changed=False,implementation_manifest_changed=False))

draft=read(R/'research-judgment-draft.json')
accept=dict(schema='stage117-delivery-g0-root-acceptance-v1',technical_status='accepted_six_bounded_ordinary_controls',full_goal_status='active_pending_root_ultra_runtime_and_final_signoff',paper_complete=False,primary_vectors=draft['primary_vectors'],used_only_componentwise_relations=draft['used_only_componentwise_relations'],used_vector_frontier=draft['used_vector_frontier'],facts=draft['facts'],G1_enter=False,innovation_claim=False,translation_causality_proved=False,reason='No specific reachable avoidable translation loss established after ordinary controls; used tradeoffs, context traffic, polling, fencing, consumer and metadata composition differ.',not_exhausted=['native full CPU/software costs','mixed CPU/DMA dispatch','all ordinary baselines'],CPU_instruction_ALU_software_application_FPGA_costs=None,root_review=['pinned DPDK source/interface and gate before cases','raw source/cache handshakes and partial strobes','independent exact refill native-load ownership correction','lifecycle and costs binding','first-prefix source metadata confound'],checker_delivery_sha256=sha(C/'delivery.json'),implementation_delivery_sha256=sha(I/'delivery.json'),same_binary_sha256=delivery['same_binary_sha256'],model_policy=dict(required='gpt-6.1-sol/ultra/native defaults',new_workers='gpt-6.1-sol/ultra/effective475000',root_actual='gpt-6.1-sol/medium/effective475000',satisfied=False,authoritative_runtime='root/root-latest-runtime.json',in_place_switch_available=False),next='Close this finite performance extension; retain ordinary adapter asset. Final ultra root signoff first; then bounded new-original-source selection with GPU/AI priority.',production_change=False,old_experiments_replayed=False)
write(P/'root-result-acceptance.json',accept)
write(P/'next-goal-contract.json',dict(schema='stage117-delivery-g0-next-scope-v1',status='scope_only_no_new_goal_registered',prerequisite='Current active goal receives truthful ultra root final review and final custody signoff, without repeating accepted runs.',question='Does one workflow with genuinely new original facts expose a specific ordinary-method gap on the actual shared translation path?',priority='GPU/AI',bounds=['one selected workflow','original task/ownership/lifetime/dependencies/bytes','actual shared IOMMU route and platform reachability','executable ordinary strong control including residency, large pages and legal batching','all resources and lifecycle costs; unknown null','no old R107/C112 or closed branch revival','no artificial pressure and no autonomous six-case expansion'],enter_model='Specific falsifiable G0 gap and reachable interfaces; no prerequisite repetitive residual test.',enter_G1='Ordinary controls leave specific reachable avoidable translation loss with causal evidence.',end='Advance, redesign or archive selected workflow with raw sources, reasons and synchronized status; no claim paper complete.'))

rows=[]
labels={'cpu_eager':'CPU 即时','cpu_batch':'CPU 整批','single_eager':'单 DMA 即时','single_batch':'单 DMA 整批','parallel_eager':'双 DMA 即时','parallel_batch':'双 DMA 整批'}
for case,c in summary['cases'].items():
 rows.append('| '+labels[case]+' | '+str(c['used_publish_vector'])+' | '+str(c['consumer_vector'])+' | '+str(c['B_return_vector'])+' | '+str(c['A_return_vector'])+' | '+str(c['global_quiet_relative'])+' |')
table='| 普通控制 | used 发布 | 消费完成 | B 归还 | A 逻辑归还 | 全局 quiet |\n|---|---|---|---|---|---|\n'+'\n'.join(rows)
report='''# stage117：来源支持的交付 G0 六案

六个有界普通控制的功能、原始证据和成本绑定已接受。CPU 即时控制最早发布前两包，单 DMA 整批控制最早发布第三包；双 DMA 即时存在首包/后两包取舍，双 DMA 整批被普通单 DMA 整批覆盖。没有证明具体可达、可避免的共享翻译损失，因此不进入 G1，不把自然消费重叠或普通适配作为独立创新。论文未完成。

实际 goal 仍 active：本轮两个新席为 gpt-6.1-sol/ultra，root 最新运行记录为 gpt-6.1-sol/medium，与约定 ultra 不符。现有工具不能原位切换。技术交付与模型要求分开记录；恢复 root ultra 后只需最终审查与交付签收，不重跑已验仿真。实际有效上下文记录 475000，未使用手工上下文覆盖。当前已通知用户恢复界面强度，不伪造运行记录或宣称全树 ultra。

## 问题、来源与进入/结束条件

问题是同一接收队列的三包复制完成后，何时合法交付原 consumer，以及源 A、目的 B 和驻留映射何时归还。DPDK v25.11（ed957165eadbe60a47d5ec223578cdd1c13d0bd9）的 async vhost 文档要求及时 poll 完成并释放 ring；源码按完整连续前缀发布 used。host mbuf 释放是 A 归还，不能冒充 guest B 消费/归还。来源没有本轮到达率、期限、丢包或压力参数。普通 sync 与 async 路径存在，不等于源码已证明一个完整混合 CPU/DMA 前缀调度器。原文及精确行号见 [root 来源摘录](../out/stage117-delivery-g0-20261013/root/source-contract-evidence.json)和 [checker 原文审查](../out/stage117-delivery-g0-20261013/checker/source-review.json)。

进入条件是此前真实 cache 功能已验、新独立 CPU 读/写接口亲核、同一个新二进制探针通过、root 实际授权六案后才运行。结束条件是同原任务六案各一次、实际 consumer/字节/guard/metadata/退休独立核验、完整成本与研究裁决及五份状态文档同步；模型 ultra 条件仍需最终满足。该交付不恢复关闭分支，不代表论文完成。

## 冻结任务与实际执行路径

原三帧 60/65/1514B、payload 1639B、记录 1675B、四 span 60/65/503/1011B、cookie 7/11/19 不变；同一 owner/队列，两 dispatch group，不宣称两条原生流。span 源/目的偏移分别 131→8343、397→8617、791→9043、3803→12055。CPU PA 0x80000000+偏移和 DMA IOVA 0x10000000+偏移指向同一 16KiB 物理池、同一驻留 2MiB leaf。单 DMA DID10 顺序四 span；双 DMA DID10/DID11 分配 span0/1 与 span2/3，共用真实 IOMMU 和内存路径。DDT4KiB、页表8KiB、逐包物理 metadata 64B，各生命周期单独绑定。

受控无故障成功路径移除额外发布前全记录校验和源覆写；真实 cache 初始化和 B consumer 各执行一次，离线原始审计保留。A 在真实源读退休的安全公开边界逻辑归还；源物理内容保持不变，映射到最终释放一直驻留，未实现物理 free/复用新包。B 在原 cookie 真实 consumer 完成后归还。

CPU copy 使用实际缓存端口读响应，通过 64bit 对齐源读和未对齐 masked 目的写组合；host 原始数组仅作初始化/oracle，不能作为复制返回数据。端口1读与端口2写允许实际独立 grant；旧返回和下一请求及跨4KiB tag 在新探针中核验。CPU copy 仅做必要 WT/metadata drain。DMA 发布前必须真实 controller fence.i 完成 I/D cache flush、ack 与 halt 释放。

即时控制在合法完成 CPU 操作边界 poll 全部 active DMA，完成即供给已准备后继，再取完整前缀；controller halt 内不 poll。完整前缀共享 metadata drain 和一次 release；DMA 每个新前缀一次新 fence。整批控制全复制完成后共享一次 fence/发布，再按原 cookie 消费；四 span 不均匀，不能虚构硬件 COUNT 批量编码。

## 原始结果

下表是共同就绪 11419 之后的模型周期。used、消费、归还、quiet 分开；不是实际 FPGA、CPU 指令或应用时延。

'''+table+'''

CPU 即时 [114,392,2051] 和单 DMA 整批 [1455,1455,1455] 体现早期交付与第三包完成的取舍。只按 used 向量，单 DMA 整批覆盖 CPU 整批和双 DMA 整批；CPU 整批、单 DMA 即时也各覆盖双 DMA 整批。used 前沿保留 CPU 即时、单 DMA 即时、单 DMA 整批、双 DMA 即时；不能把这个指标的覆盖泛化成全部资源或原生成本优势。

双 DMA 即时首包比单 DMA 即时早71周期，后两包各晚109周期，同时多占一个 active DMA。root 原始 metadata 审查发现单 DMA 首前缀已额外退休包2的 source-return 字段，双 DMA 该前缀只有包1；这是完成工作和控制成本的差异，不能把71周期全部归因于翻译，也不据此新调参数重跑。见 [前缀成本原始见证](../out/stage117-delivery-g0-20261013/root/prefix-cost-observation.json)。新即时 DMA 普通控制自然出现 consumer/peer backend 重叠；这属于普通控制能力。前轮 cache 功能关口的不同合同仍保留原“未观察到实际消费重叠”，没有改写或重跑。

## 完整成本和不能推出的结论

四个 DMA 案各真实 AR/AW/B 6次，源 R 总线1672B、有效目的写1639B、写总线1688B；翻译接受/返回12次、lookup12/hit11、PTW AR2。单 DMA DDTW AR1、双 DMA AR2，表流量80/144B，双 DMA 多64B context读取。次数与实际表流量支持共享路径可达，缺少完整按请求地址/TID 的因果配对，不能证明某包时延差全部源于共享翻译。

CPU copy 各实际源 load208/目的store210，有效源字节1639B、完整源word1664B、缓存填充1680B。主路径 CPU cache actor R 为 CPU案3584B、DMA案1904B；selected W 为1759B/120B，另列 DMA 与 walk 流量。CPU 前端 host vector 容量256×80=20480B，实际 descriptor peak 即时191/整批208；pending read peak1、queued store peak2 word/11 usefulB，并有 partial8B 与 returned8B。它是乐观确定性端口前端，不是执行原生 CVA6 memcpy，也不是最优 CPU 性能下界的证明。queue 分配、指令/ALU拼装/软件/应用/FPGA时延保持 null。

每 DMA 后端512×64bit buffer=4KiB，FIFO8、AXI在途8；各案使用原配置，不人为限制合法 DMA 额度制造优势。DMA 即时自然形成两个前缀/两次主路径 fence，整批一个前缀/一次 fence；CPU 无额外 DMA payload fence。主路径周期与各 phase 服务存在重叠，不能直接相加作虚假的全局时延分解。

初始化共同成本11419周期；最终离线 snapshot/审计及 postscan release 每案36804周期单列。六案总生命周期模型周期为51910/51848/51853/51678/51962/52118（按表顺序）。A 逻辑 hold bytecycles、实际源 R、source metadata 物理B退休、B归还、全局quiet与最终映射释放在 [完整成本](../out/stage117-delivery-g0-20261013/implementation/costs-and-overlap.json)和 [独立成本绑定](../out/stage117-delivery-g0-20261013/checker/cost-summary-binding.json)中分别记录。宿主构建/仿真耗时只作执行成本。

## 实际验证与修正账本

复用此前隔离 Verilator5.040，无重装；两次成功构建（完整约7.934秒、追加 resource counter 增量约1.625秒）均先于任何模拟执行。构建1未留独立历史输入/二进制快照且未运行；实际所有运行仅使用已保存320项真实编译依赖和117项声明输入的最终 build2，限制见 build-history-custody。一个新接口探针和六案各一次，同二进制 SHA256 0993b5384b1bbfa81c913103616f555e53c118da179d9422e9677ac7d96c554c；无失败仿真、旧协议/cache/有限矩阵重跑。

独立 checker 核原始 CPU 请求/响应、memory R/W/B、源/目的字节、guard、物理 metadata 退休、前缀发布、供给/poll/cache fence 和成本，不依赖 executor PASS。累计7项不同负控制与1项 PASS 独立性正控制通过；实际控制应用10次，包含两次有据修正，不隐去失败历史。初始 CPU batch 事件同周期边界按事件顺序修正只重验该原始记录，没有重跑仿真。

root 最终发现全局允许集合错误扩展目的字节闭包且未绑定本次 CPU pending load；checker 已收紧为精确字节集合，每个 cache AR 唯一绑定真实未返回端口1 load及本包/span，源在A归还前退休，consumer在发布/acquire后只读本包，uncached表只在quiet之后审计。六份原记录16356次AR（主路径924，postquiet15432）通过新增谓词；活跃 peer 目的地址篡改被拒绝。只补验新谓词与一个新控制，不重跑原六控制、探针、完整 verifier 或仿真；旧完整报告及源码 hash 原样归档。见 [所有权补证](../out/stage117-delivery-g0-20261013/checker/refill-ownership-correction.json)。root 仅在实现席交回后更新 checker 指针，不修改实际执行源码/二进制。

## 裁决和下一步

该六案关闭本轮有限交付性能扩展，保留 C117R3 作为普通适配/有限G0资产，不继续调序、加压力、补矩阵或开展RTL/板测试。因为普通控制后没有确证具体可达可避免翻译损失，本轮不能将普通交付适配、消费重叠或71周期取舍称为共享 IOMMU 的独立性能创新。若新来源证明具体普通方法缺口，并在实际共享路径上通过强对照与因果核验，才可进入 G1 的机制筛选。

先完成 root ultra 最终签收；随后依新事实选择一个有界工作流，GPU/AI优先，核任务、所有权、生命周期、依赖、字节、真实共享翻译路径、普通强对照和全部成本。没有新事实不自动扩大本网络例子；未测试的混合 CPU/DMA 路径不冒充已实现/已穷尽。R107、C112及其他关闭分支保持关闭。原两周窗口累计，论文未完成。

[稳定技术裁决](../out/stage117-delivery-g0-20261013/root-result-acceptance.json)／[下一范围合同](../out/stage117-delivery-g0-20261013/next-goal-contract.json)／[证据导航](../out/stage117-delivery-g0-20261013/README.md)／[实际goal登记](../out/stage117-delivery-g0-20261013/root/goal-registration.json)。
'''
(W/'iommu-platform/docs/stage117-delivery-g0-20261013.md').write_text(report)
(P/'README.md').write_text('''# stage117 delivery G0 证据导航

六案技术接受；没有共享翻译因果收益或 G1/论文完成声明。实际 goal active，root medium 与约定 ultra 偏差待最终运行签收。

- [完整中文报告](../../docs/stage117-delivery-g0-20261013.md)、[稳定裁决](root-result-acceptance.json)、[下一范围](next-goal-contract.json)
- [冻结执行合同](comparison-contract.json)、[实际goal登记](root/goal-registration.json)、[root运行事实](root/root-latest-runtime.json)
- [来源原文/精确行号](root/source-contract-evidence.json)、[接口授权gate](root/interface-gate.json)、[root探针亲核](root/probe-direct-review.json)
- [实际实现交付](implementation/delivery.json)、[完整成本](implementation/costs-and-overlap.json)、[实际执行账本](implementation/execution-ledger.json)、[构建账本](implementation/build-ledger.json)、[历史输入限制](implementation/build-history-custody.json)
- [320项实际编译依赖](implementation/actual-compiled-dependencies.json)、[声明输入](implementation/build-inputs.json)、[实现资产哈希](implementation/artifact-sha256.json)
- [独立交付](checker/delivery.json)、[六案汇总](checker/summary.json)、[原始成本绑定](checker/cost-summary-binding.json)、[有界控制](checker/bounded-falsification.json)、[精确refill owner补证](checker/refill-ownership-correction.json)、[checker实际账本](checker/execution-ledger.json)
- 六案原始记录位于 implementation/results/{cpu,single,parallel}_{eager,batch}/，每案run.json、CPU/cache/memory总线csv、事件、前端和原始binary snapshot均保留；探针在implementation/results/interface_probe/。
- [前缀metadata差异](root/prefix-cost-observation.json)、[root机械指针修正](root/checker-pointer-update.json)、[交付核验](root/delivery-check.json)
- before/ 为五份共享文档原字节快照；checker/before-refill-ownership-correction/为补正前报告和源码，旧阶段资产保持原样。
''')

intro='''## 2026-10-13 stage117 来源支持交付 G0 六案技术接受

CPU/单DMA/双DMA×即时/整批发布六案同新二进制各一次通过；原1639B/1675B、四span、真实cache/consumer、所有权和物理metadata退休闭合。三包used向量：CPU即时[114,392,2051]，CPU整批[1625,1625,1625]，单DMA即时[1209,1804,1804]，单DMA整批[1455,1455,1455]，双DMA即时[1138,1913,1913]，双DMA整批[1895,1895,1895]（共同就绪后模型周期）。CPU即时最早前两包，单DMA整批最早第三包；不存在按全部资源的统一赢家。

普通即时控制自然出现实际consumer/peer后端重叠。双DMA即时首包早71、后两包晚109且资源/前缀metadata不同；全部DMA翻译12/命中11，单/双DDT流量80/144B。没有具体可达可避免翻译损失的因果证据，不进G1；全CPU/ALU/软件/应用/FPGA成本null。停止自主扩矩阵，保留普通适配资产；下一范围是有新增原始事实的单个工作流，GPU/AI优先，先核实际共享路径和普通强对照，不恢复关闭分支、不加压、不重跑。

新探针1次＋六案6次；两成功构建均在运行前，最终320项实际编译依赖；独立7负＋1正控制（实际10次应用）通过。root发现refill允许范围/owner绑定缺口后，只对原六trace补验新增谓词与1负控制：16356次AR全部绑定实际pending native load、本包/span及精确字节闭包；没有重复仿真、旧控制/探针或全verifier。最终审计/释放36804周期单列，生产driver/ABI/RTL和板未变。

实际goal仍active：两个新席Sol/ultra，root最新运行记录Sol/medium，与ultra约定不符；已如实通知，工具不能原位切换，不伪造切换。技术交付先完成，恢复root ultra后只作最终审查/交付签收，不重复实验；未宣称全树ultra或goal complete。有效上下文475000，不设额外预算。论文未完成，原两周窗口累计。

'''
for rel in ['HANDOFF.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:
 prefix='' if rel=='HANDOFF.md' else '../../' if rel.endswith('research-execution-plan-20261011.md') else '../../'
 link='iommu-platform/docs/stage117-delivery-g0-20261013.md' if rel=='HANDOFF.md' else 'stage117-delivery-g0-20261013.md'
 oldbytes=(P/'before'/rel).read_bytes()
 (W/rel).write_bytes((intro+'[本轮报告]('+link+')给出原始证据、成本、稳定技术裁决与下一范围。以下原样保留此前完整历史；其中“本轮complete”各指其历史关口。\n\n').encode()+oldbytes)
oldbytes=(P/'before/研究计划.md').read_bytes()
(W/'研究计划.md').write_bytes(('# 多 DMA 共享 IOMMU 研究总计划：当前交付 G0 结论\n\n'+intro+'[本轮报告](iommu-platform/docs/stage117-delivery-g0-20261013.md)／[下一范围合同](iommu-platform/out/stage117-delivery-g0-20261013/next-goal-contract.json)。旧“下一步一次G0”已由本轮六案完成技术裁决；后续不重复该题，先完成当前模型约定签收，再凭新来源选题。\n\n## 前轮计划完整历史\n\n').encode()+oldbytes)
current=(W/'CURRENT_STATUS.md').read_text(); tail=current[current.index('- 独立Linux贡献：'):]
(W/'CURRENT_STATUS.md').write_text('''# 当前状态：stage117交付G0六案技术接受（2026-10-13）

- 六案同新二进制各一次通过；原三包/1639B/四span与实际缓存、consumer及完整所有权成本保持。CPU即时最早前两包；单DMA整批最早第三包；双DMA无整体优势。
- 独立7负＋1正控制通过；精确refill owner补证覆盖六原trace的16356次AR，仅补新谓词/控制，没有重跑仿真、旧验证或关闭分支。没有具体共享翻译可避免损失的因果证据，不进入G1。
- 技术裁决、共享总/局部计划、交接和验证已同步，完整CPU/ALU/软件/应用/FPGA成本null；生产driver/ABI/RTL和板未改。论文未完成。
- 实际goal仍active，等待root ultra最终运行签收：新两席Sol/ultra，root实际Sol/medium偏差已报告，现有工具不能原位调整；不能声称全树ultra或goal complete。有效上下文475000，无额外预算。
- 下一步：恢复root ultra后仅最终审查/签收，不重跑六案；随后按新增原始事实选择一个有界工作流，GPU/AI优先，核任务/生命周期/共享翻译路径/普通强对照和全部成本。停止本例自主矩阵扩展，R107/C112等关闭分支不恢复。

[报告](iommu-platform/docs/stage117-delivery-g0-20261013.md)／[技术裁决](iommu-platform/out/stage117-delivery-g0-20261013/root-result-acceptance.json)／[下一范围](iommu-platform/out/stage117-delivery-g0-20261013/next-goal-contract.json)／[完整交付核验](iommu-platform/out/stage117-delivery-g0-20261013/root/delivery-check.json)。

'''+tail)
print('technical acceptance and five shared documents written; actual goal remains active')
