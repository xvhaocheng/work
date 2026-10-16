"""Synchronize five authorized entry documents once, preserving exact archive bytes."""
from pathlib import Path
import hashlib,json
W=Path('/home/xvhaocheng/work');O=W/'iommu-platform/out/pte-safe-stage-causality-20261014'
manifest=json.loads((O/'before/manifest.json').read_text())
for r in manifest:
 b=(O/r['archive']).read_bytes();assert hashlib.sha256(b).hexdigest()==r['sha256'] and (W/r['path']).read_bytes()==b,r['path']
current='''# 当前状态：两安全队头PTE因果门——固定前缀未准入（2026-10-14）

- 本轮源门及一次新被动RTL前缀完成：两真实DMA各一个DEFAULT对齐4KiB/512拍，原生NormalTL/AR仲裁不改；仅DS返回优先覆盖全部job及R/W/B/faultquiet。
- 本固定前缀双NormalTL VALID/fresh安全双READ/共享AR co-VALID均0；没有合法ABBA分叉。12笔准确绑定PTE内部→DDR56、DDR→PTW88、下一birth1拍，未成立相对阶段因果差异。其他合法前缀可达性unknown，F1全族open。
- 每源2AR/2AW/512R/512W/2B，目的各4096B实际逐字节核对、0error、真实jobquiet3190/4145及allquiet4145模型拍；DONEpoll是上界，非F1收益。DDR queue峰值3，208 held全DDTW49，不是payload/PTE阻塞。
- 0 ABBA/四性能条件，0旧实验/build/validator重跑、0生产/板/加压/容量/关闭分支恢复。新build3失败+1成功原件保留，1新运行，独立原始PTE/FullID/LAST/B/quiet核验通过。新增预测/选择/面积/Fmax/CPU应用null，raw strobe/functional ID/credit缺字段明确。
- 下一门仅有新明确合法输入/同步capture及单个完整普通DS-AR/既有九规则外完整差异才准入；不重跑本prefix、扫相位或自动新goal，存活后再≤4新条件、GPU/AI工作流、FPGA。8/9月成果保留、原窗口累计、论文未完成。
- actualgoal无额外预算；root+2新独占席实际6.1-sol/ultra/native有效475000、forknone/Codex-only。五文档同步与新交付核验后才actualcomplete，以工具回执为准。

[本轮报告](iommu-platform/docs/pte-safe-stage-causality-20261014.md)／[稳定裁决](iommu-platform/out/pte-safe-stage-causality-20261014/root-result-acceptance.json)／[证据导航](iommu-platform/out/pte-safe-stage-causality-20261014/README.md)／[下一门](iommu-platform/out/pte-safe-stage-causality-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/pte-safe-stage-causality-20261014/root/delivery-verification.json)／[actual完成回执](iommu-platform/out/pte-safe-stage-causality-20261014/root/goal-completion.json)。入口原件在本轮before目录。

'''
old=(O/'before/CURRENT_STATUS.md').read_text();mark='- 独立Linux贡献：';assert old.count(mark)==1
(W/'CURRENT_STATUS.md').write_text(current+old[old.index(mark):])
body='''## 2026-10-14 两安全队头PTE阶段等待因果门

按用户计划登记actualgoal，无额外预算，root亲核fresh/PTW/R/源门、设计并执行一次新被动nativeprefix；两个新独占Sol/ultra/forknone席核自然capture与AR路径/完整普通对照，AR席在同goal独立解析新原始trace，未恢复旧席。全部实际gpt-6.1-sol/ultra、原生有效475000、Codex-only，无DeepSeek/Claude/Astra/context override。

保留原生1PTW/IoTLB16/NativeN、DEFAULT、两live DMA各512拍/一个对齐4KiB、每bank4 outstanding/16W FIFO、held/fullID/sameID/完整W/R/B/fault。CPU/atomics/warm/batch/旧pool/header/反序/gate关闭，正常串行NEXT先source0再source1，没有debug驱动kick、人工hold、pressure/容量/latency策略扩张。新spy37项637声明bit只读无RTL新state，NormalTL/AR选择不改；唯批准DS Rpriority扩至全部job/owned obligations/quiet，保留head准备31及DS50/50。

4151连续preedge rows中双NormalTL VALID/fresh safeREAD/共享ARcoVALID均0，故本固定前缀没有合法ABBA分叉；不等第二头，不撤VALID，不扫相位。12 PTE按owner source/DID/IOVA/type/stage/level/PTE_PA与准确消费数据绑定：birth→内部0、内部→DDR56、DDR→firstRVALID32、DDR→PTWconsume88，后继birth1拍；PTE8/9有真实payload在途仍同值。单trace不证明全域相对阶段不变。208held全属两笔DDTW49各104，payload32/36与PTW48 held0；DDR queue峰值3、无自然16槽饱和。

每源2AR/2AW/512R/512W/2B，两个4096B目的实际逐字节核对、0response error，原fullID/LAST及无WID的AW/WLAST/B归属独立重建清空。实际jobquiet共同t0后3190/4145、完整quiet4145模型拍，DONEpoll3196/4150与quiet调用4151仅上界，不是F1收益。起点是两descriptor配置完成后的共同ready；配置/初始化CPU和物理/应用总成本未测。raw WDATA/WSTRB/numeric功能ID/直接credits/DDTW消费字段缺失，未造数或称逐beat strobe独立验证；正常byte结论由实际目的比较、aligned源约束和ordered beat联合支持。

AR新source事实：实际mixedvendor，per-source16ID/3bit counter满7、mux8管W、CPPqueue16。CPUoff且有共享AR前史的未锁RR最多一个comp grant，含旧comp lock最多两个，非clock全域界；冻结O-DS-AR当前VALID DS优先、4次公平逃逸、全部承诺/退休/接口费用完整，未实现运行，不声称全F1覆盖或数值成本优势。原九控制/37费用边界复用。F1预测器/select成本、面积/Fmax/CPU/应用null。

结论仅关闭本固定prefix作为两安全选择/阶段因果正例；其他合法固定供给初态unknown、F1全族open。0ABBA/四性能条件、0旧实验/build/validator重跑、0生产driver/ABI/RTL/综合/板、0pressure/extra容量或关闭分支恢复。新build3失败(依赖/只读hier scope/Tb周期引用)+1成功均保留，每次后续输入改变，成功binary1运行，root与独立raw检查通过。新输入/同步capture以及单个完整普通控制之外的完整差异存活，才另准入一prototype≤4新条件，随后GPU/AI共享路径与FPGA；不自动新goal/phase矩阵/论文定位变更。原窗口累计，论文未完成。

LINKS

五文档同步及本轮新交付核验后才actualgoal complete；实际工具用量见原样回执。以下原样保留入口全部历史，旧“下一步”仅指当时状态。

'''
heads={'HANDOFF.md':'# 阶段交接：两安全队头PTE因果门','研究计划.md':'# 研究总计划：两安全队头PTE因果门','iommu-platform/docs/research-execution-plan-20261011.md':'# 当前执行：两安全队头PTE因果门','iommu-platform/docs/validation.md':'# 最新验证记录：两安全队头PTE因果门'}
for r in manifest:
 p=r['path']
 if p=='CURRENT_STATUS.md':continue
 pre='' if p.startswith('iommu-platform/docs/') else 'iommu-platform/docs/'
 out='../out/' if p.startswith('iommu-platform/docs/') else 'iommu-platform/out/'
 links=f'[本轮报告]({pre}pte-safe-stage-causality-20261014.md)／[稳定裁决]({out}pte-safe-stage-causality-20261014/root-result-acceptance.json)／[导航]({out}pte-safe-stage-causality-20261014/README.md)／[下一门]({out}pte-safe-stage-causality-20261014/next-goal-contract.json)／[交付核验]({out}pte-safe-stage-causality-20261014/root/delivery-verification.json)／[actual完成回执]({out}pte-safe-stage-causality-20261014/root/goal-completion.json)。'
 (W/p).write_bytes((heads[p]+'\n\n'+body.replace('LINKS',links)).encode()+(O/r['archive']).read_bytes())
(O/'root/document-sync.json').write_text(json.dumps({'schema':'safe-five-doc-sync-v1','documents':[r['path'] for r in manifest],'archives_exact':True,'four_old_bodies_as_byte_suffix':True,'CURRENT_independent_tail_exact':True,'actual_goal_complete_called':False},ensure_ascii=False,indent=2)+'\n')
print('Five documents synchronized, history and independent tail retained.')
