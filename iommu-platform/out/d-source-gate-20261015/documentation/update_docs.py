from pathlib import Path
import json,hashlib,re
R=Path('/home/xvhaocheng/work'); run=R/'iommu-platform/out/d-source-gate-20261015'; D=run/'documentation'
# Re-read every destination immediately before writing; refuse unseen concurrent edits.
names=['研究计划.md','研究导航.md','CURRENT_STATUS.md','HANDOFF.md','iommu-platform/docs/validation.md']
B={n:(R/n).read_bytes() for n in names}
for n,b in B.items():
 assert b==(D/'before'/n).read_bytes(),f'Concurrent change requires merge: {n}'
T={n:b.decode() for n,b in B.items()}
p=T['研究计划.md']
p=p.replace('D寻找同合同、已有信息近似相同而P/PN完整效果反号的状态对；必要时用现有闭环COPY模型作有界原理验证','D先固定同实例来源/合法动作/状态持续性及相邻相反条件预测；满足接口后用小模型判别，固定M0本轮不扩大')
p=p.replace('状态有真实来源；已知配置或普通结构指标不能直接解释区别；模型结果明确限于模型','预测有真实来源与因果链；不要求建模前已测反号；模型结果明确限于新合同')
a=p.index('首批实际结果：');z=p.index('\n\n## D：',a)
p=p[:a]+'''当前实际结果：首批6份M0完整COPY日志只有P/PN持平或PN更慢；本轮继续核机制、两张自然来源卡和I后端，**固定M0的D来源准入不通过，停止该版扩大参数、调gap或加collector**。原因是来源、接口与状态匹配缺项，不是要求研究前先保证收益。D保持主线，保留SPDK完成驱动供给的条件重设计；I仍not_ready，A5保留，不自动转向。[本轮完整交付与稳定接受](iommu-platform/out/d-source-gate-20261015/README.md)。'''+p[z:]
p=p.replace('首批交付：','首批交付（历史证据与假设；准入规则以本轮澄清为准）：')
a=p.index('若已有信息已经足够、状态没有真实来源');z=p.index('\n\n## I：',a)
p=p[:a]+'''若已有信息已经足够、同成本普通试选达到相同效果，或新信息太迟/太贵，结束对应版本的贡献主张，保留诊断成果。**来源绑定的条件预测可以进入小模型，不要求建模前已测出完整反号。** 准入材料须固定真实事件/调用链、状态来源与身份、共同目标/O0/合法动作及相邻相反预测，并保证模型可忠实表达因果边。本轮缺的是这些来源/接口/状态绑定，故未启动模型；不得用先验保证收益挡住研究。

本轮已核边界：N的walk窗口仅约束probe启动，实际fast接受可能在walk结束后，故fast计数不等于节省剩余walk或完整COPY获益。板端成功quiet配置清IOTLB valid/PLRU，不清整个DDR；M0每case另reset内存/队列/RTL再固定预热排空。上一窗口观测要预测下一任务，须核相关状态在quiet/config/clear/warm之后的持续性，不能拼接独立reset case。[机制](iommu-platform/out/d-source-gate-20261015/mechanism/mechanism.md)／[root决策](iommu-platform/out/d-source-gate-20261015/root/decision-analysis.md)。

两张来源卡分别是SPDK＋iperf3、SDI/YAVTA＋待识别独立后台，均未取得联合部署或自然反号。SPDK真实completion回调驱动同task/buffer重提交；LBA不同于host IOVA，不改IOVA仍有供给时序用途，但服务态、共享动作及跨边界预测尚缺。已核YAVTA正常三槽/十帧/skip7示例保存尾三帧时已不再QBUF，故该例不支持“保存拖慢后续重供给”；不外推其他采集配置。

下一最小材料是一份**同实例源—动作—服务联合合同及已有实际事件段**：成功完成/重提交、buffer/映射与已知命令、实际共享结构和可用动作、O0/付费信号可见时刻、跨边界状态联系及完整目标/费用。若另建闭环模型，须在每个动作下重算完成→重提交反馈，不能把一个动作的固定到达当另一动作的反事实程序。未取得前不扩大固定M0、不调gap、不加collector或改guard。不得运行覆盖历史结果的包装脚本；新增分析用独立目录。'''+p[z:]
p=p.replace('D未通过时，只有I已有明确剩余缺口','本轮后端门补充：驱动有条件strict DMA-IOMMU通路；虽接受DMA_FQ类型，未声明DEFERRED_FLUSH导致通用setup回退strict，不能当成当前有效FQ合批。保存的构建config禁用IXGBE，当前DTS无对应PCI/ixgbe链，lowrisc Ethernet也无IOMMU关联，因此外部ixgbe源码不是当前固定profile的原生任务。保存配置不证明当前启动身份；仍无native运行/正确性测试。[I后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)。\n\nD未通过时，只有I已有明确剩余缺口')
T['研究计划.md']=p
n=T['研究导航.md']
n=n.replace('主线来源门；现M0无反号状态对，新增观测/扩大模型未准入，未知费用不按零计算','主线条件重设计；固定M0本轮来源门不通过，停止扩大/调gap/collector；来源绑定预测可进小模型，不要求先测反号')
n=n.replace('[来源/动作费用](iommu-platform/out/research-plan-implementation-20261015/d/source-and-action-costs.md) · [观测]', '[本轮机制/两卡/接受](iommu-platform/out/d-source-gate-20261015/README.md) · [来源/动作费用](iommu-platform/out/research-plan-implementation-20261015/d/source-and-action-costs.md) · [观测]')
n=n.replace('轻量预筛not_ready；正常TX源码来源已核，共享部署、live预算及联合轨迹待取得','轻量not_ready；strict后端存在/FQ回退，当前固定profile无ixgbe原生链；实际部署与live预算待取得')
n=n.replace('[预筛](iommu-platform/out/research-plan-implementation-20261015/i/prescreen.md) · [源证据]', '[后端/配置门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md) · [预筛](iommu-platform/out/research-plan-implementation-20261015/i/prescreen.md) · [源证据]')
n=n.replace('## 已有成果怎样复用','本轮机制校准、SPDK完成驱动供给的最强条件版本、YAVTA尾三帧边界及下一最小联合材料见[本轮入口](iommu-platform/out/d-source-gate-20261015/README.md)。两张卡不是已取得的联合部署/自然反号；不声称性能或新意已证。\n\n## 已有成果怎样复用')
T['研究导航.md']=n
s=T['CURRENT_STATUS.md'].replace('# 当前状态：共享IOMMU的D/I首批证据与入口整理','# 当前状态：D来源门裁决与条件重设计')
a=s.index('- D首批');z=s.index('- 本批已完成入口',a)
s=s[:a]+'''- D本轮固定M0来源准入不通过，停止该版扩大/调gap/collector；保留SPDK完成驱动供给的条件重设计。N窗口仅限probe启动，fast接受可能在walk结束后；板端quiet清IOTLB/PLRU不清整个DDR，与M0逐case reset不同，上一窗口预测须跨clear/warm持续。两张来源卡未取得联合部署或自然反号。来源绑定的条件预测可进小模型，不要求先测反号；本轮缺来源/接口/状态匹配，不是缺先验收益保证。[本轮接受与下一最小材料](iommu-platform/out/d-source-gate-20261015/README.md)。
- I有条件strict DMA后端，FQ因能力缺项回退strict；保存config禁IXGBE/DTS无对应PCI链，ixgbe是外部正常源码来源，非当前原生任务。实际部署/live预算/联合trace仍缺，not_ready；保留正确精确PSCID/range、合法合批、持久映射与分domain对照及full-walk余量反解释。D缺证不自动提升I或退回A。[I后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)。
'''+s[z:]
s=s.replace('本批0新模型/实验回放/编译/Linux或RTL或ABI修改/FPGA或板操作。','本轮仅源码/旧记录核查与文档落盘，0新模型/实验回放/编译/Linux或RTL或ABI修改/FPGA或板操作。')
T['CURRENT_STATUS.md']=s
handoff='''# 2026年10月15日 D固定M0来源门与I后端门裁决

D主线/I轻量/A5保留及10月15—28日窗口不变。本轮稳定接受：固定M0来源准入不通过，停止该版扩大参数、调gap、collector和改guard；不据此宣称PN普遍无效或自动转向A/I。来源绑定的条件预测可进入小模型，不要求建模前先测完整反号；本轮缺来源/接口/状态绑定，不能把先验保证收益当研究前提。

机制新校准：N的walk窗口只约束probe启动，fast接受可能在walk结束后；不等于剩余walk节省或完整COPY收益。板端成功quiet配置清IOTLB valid/PLRU，不清整个DDR；M0逐case reset内存/队列/RTL并固定预热排空，两者不同。上一窗口观测用于下一任务，须补跨quiet/config/clear/warm的状态持续与预测依据。

两卡为SPDK＋iperf3及SDI/YAVTA＋未识别独立后台，均未取得联合部署或自然反号。SPDK同task/buffer完成重提交是实际源链；LBA不是host IOVA，不改IOVA仍保留时序用途。YAVTA所核正常三槽/十帧/skip7尾三帧保存时已不再QBUF，只排除该例“保存拖慢后续重供给”链，不否定其他配置。保留最强合理后继：同映射池完成驱动供给；先补一份同实例源—动作—服务联合合同及已有真实事件段，含O0/信号时机、共享与动作、跨边界持续性、成功完整目标和全费用。后继模型须每个动作重算完成→重提交反馈，不固定重放另一动作的到达。

I有条件标准strict DMA-IOMMU后端；未声明DEFERRED_FLUSH令DMA_FQ回退strict。保存config禁IXGBE，当前DTS无对应PCI/ixgbe链，lowrisc Ethernet无IOMMU关联；故ixgbe正常map/DD/unmap链仍是外部来源，不是当前固定profile原生任务。保存配置非当前启动身份；I继续not_ready，普通精确scope/合法合批/持久映射/分domain及full-walk余量反解释保留。

本轮0新模型/旧实验回放/编译/native DMA/FPGA或板操作/生产Linux或RTL或ABI修改/goal变更，未取得性能或新意证明；MIG1792023314 paused、build6时序失败/债务unknown保持。A5/A2及独立Linux/上游PR/RTL事项按写前状态全文保留。文档席请求Sol/medium，实际模型/强度独立记录未取得，记unknown，不以请求冒充实际。Codex-only，无DeepSeek/Claude；以下旧交接逐字节保留。

[本轮完整入口](iommu-platform/out/d-source-gate-20261015/README.md)／[稳定接受](iommu-platform/out/d-source-gate-20261015/root/root-result-acceptance.json)／[root决策](iommu-platform/out/d-source-gate-20261015/root/decision-analysis.md)／[文档回执](iommu-platform/out/d-source-gate-20261015/documentation/receipt.json)／[总核验](iommu-platform/out/d-source-gate-20261015/root/delivery-verification.json)。

'''
validation='''# 2026年10月15日 D来源/机制及I后端门核验

本轮稳定接受固定M0的D来源准入不通过；停止该版扩大/调gap/collector，保留SPDK完成驱动供给的条件重设计，不接受PN普遍无效、自动A/I转向、新性能或新意已证。来源绑定条件预测可以小模型检验，不要求先实测反号；本轮缺来源/接口/状态对应。

新增核查边界：N窗口仅限probe启动，fast接受可在walk结束后；板端quiet清IOTLB/PLRU不清整个DDR，M0每case另reset，下一任务预测须跨clear/warm持续。两来源卡不是联合部署/自然反号；SPDK同buffer重提交真实而LBA不同IOVA，所核YAVTA三槽十帧skip7保存尾三帧已不再QBUF。I strict后端源码可达、FQ回退strict；保存config禁IXGBE/源DTS无对应PCI链，非当前原生实例，未验启动/运行身份。

仅源码/旧记录核查及文档完整性验证，0新模型/回放/编译/native DMA/FPGA或板操作/生产改动/goal变更。文档核验检查本地引用、前后hash、状态独立事项与A5/A2引用、旧交接/验证后缀原字节；不作系统性能PASS。请求Sol/medium，实际运行模型/强度无独立证据，unknown。MIG暂停/旧时序失败/债务unknown保持。

[完整入口](../out/d-source-gate-20261015/README.md)／[稳定接受](../out/d-source-gate-20261015/root/root-result-acceptance.json)／[关键源码核对](../out/d-source-gate-20261015/root/critical-source-check.json)／[文档回执](../out/d-source-gate-20261015/documentation/receipt.json)／[执行账本](../out/d-source-gate-20261015/root/execution-ledger.json)／[总核验](../out/d-source-gate-20261015/root/delivery-verification.json)。以下旧验证正文逐字节保留。

'''
T['HANDOFF.md']=handoff+T['HANDOFF.md'];T['iommu-platform/docs/validation.md']=validation+T['iommu-platform/docs/validation.md']
for n,t in T.items():(R/n).write_bytes(t.encode())
readme='''# D下一来源关口：本轮交付

2026-10-15。**固定M0的D来源准入不通过；停止该版扩大参数、调gap、collector或改guard，保留SPDK完成驱动供给的条件重设计。** 全局D主线/I轻量/A5保留，10月15—28日窗口不重置。没有接受新性能、PN普遍无效或独立新意主张，也不自动转向A/I。

N的walk窗口只限制probe启动，fast接受可能在walk结束后。板端quiet成功配置清IOTLB valid/PLRU，不清整个DDR；M0逐case reset及排空不同。上一窗口观测预测下一任务，须核quiet/config/clear/warm后的状态持续性与可用信号费用。已有M0六日志不增加新反号见证。

SPDK＋iperf3和SDI/YAVTA两张卡都不是已取得的联合部署/自然反号。SPDK确有同task/buffer完成重提交；LBA不同IOVA不取消时序用途。所核YAVTA正常三槽/十帧/skip7保存尾三帧时已不再QBUF，只限制该例因果链。I有条件strict DMA后端，FQ回退strict；保存config禁IXGBE/当前源DTS无对应PCI链，外部ixgbe生命周期不能当当前固定profile原生任务，I仍not_ready。

**来源绑定的条件预测可进入小模型，不要求先实测完整反号。** 当前停止原因是来源、接口、状态对应缺项，而非未先保证收益。下一最小材料是一份同实例源—动作—服务联合合同及已有真实事件段，明确成功完成/重提交、buffer/映射/已知命令、真实共享结构/可用动作、O0与付费信号时机、跨边界持续性、完整目标和全部费用。若后继另建模型，每个动作须重算完成→重提交反馈；不以调gap、固定到达重放或大collector代替材料。本轮未启动后继模型/原生取证。

| 交付 | 入口与边界 |
| --- | --- |
| root稳定接受与决策 | [接受JSON](root/root-result-acceptance.json) · [决策分析](root/decision-analysis.md) |
| N/M0/quiet机制 | [机制](mechanism/mechanism.md) · [源码位置/hash](mechanism/sources.json) · [评估](mechanism/assessment.json) |
| 两张自然来源卡 | [来源卡](sources/source-cards.md) · [原件索引](sources/source-index.json) · [关键原文](sources/source-excerpts.json) · [评估](sources/assessment.json)；来源卡准入措辞以root第6节澄清为准 |
| I后端/配置门 | [发现](i-backend/findings.md) · [原文位置/hash](i-backend/sources.json) · [评估](i-backend/assessment.json) |
| root原件亲核 | [关键源码检查](root/critical-source-check.json) |
| 实际执行与配置 | [执行账本](root/execution-ledger.json)；请求配置与实际配置分开，未取证字段unknown |
| 文档前后与保留验证 | [写前清单](documentation/before-manifest.json) · [变更清单](documentation/changes.json) · [检查](documentation/checks.json) · [回执](documentation/receipt.json) |
| 总交付验证 | [总核验](root/delivery-verification.json)；若root尚未生成，暂列待生成，由root最终核验 |

本轮仅源码/旧记录读取与文档处理；0新模型/旧实验回放/编译/native DMA/生产Linux或RTL或ABI修改/FPGA或板操作/goal变更。MIG1792023314继续paused，旧时序失败/债务unknown保留。A5/A2及独立Linux/上游PR/RTL状态保留；历史正文、原日志和旧out不改。Codex-only，无DeepSeek/Claude。
'''
(run/'README.md').write_text(readme)
print('Updated six owned documents; retained history bytes for verification.')
