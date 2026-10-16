from pathlib import Path
import datetime
import json

WORK = Path('/home/xvhaocheng/work')
OUT = WORK / 'iommu-platform/out/tl-memory-feedback-20261014'
ROOT = OUT / 'root'

def save(path, value):
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')

acceptance = {
    'schema': 'tl-memory-feedback-root-acceptance-v1',
    'created_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
    'status': 'initial_symmetric_turn_instance_not_admitted; bounded_entry_round_delivered',
    'scope': 'F1 root two-order architecture; initial symmetric R/W-turn mathematical instance only; no full-family closure',
    'root_original_design': 'root/mechanism-card.md',
    'gates': {
        'legal_pre_firstVALID_choice': 'conditional source-derived AXI interface prefix accepted; no actual iDMA trace',
        'source_bound_feedback_for_initial_instance': 'reject: current drive_ddr shares R/PTE queue but W is independently serviced; no exclusive R/W server or turnaround tau',
        'independent_choice_witness_for_initial_instance': 'reject: one frozen O-turn rule exactly reproduces the symmetric fixed-work family including max tie, sum and held/fair/missing-history guards',
        'complete_cost_boundary': '37 categories, owned-obligation/history/ports/selection costs enumerated; exact implementation widths and total not frozen, not a numeric cost acceptance',
        'four_condition_microbenchmark': 'not admitted; none executed'
    },
    'accepted_new_facts': [
        'Native bank merge eagerly commits at first VALID, before PTW availability; accepted request spills cannot be freely reordered.',
        'Legal dual fresh heads are conditionally source reachable with simultaneous AR0+AW1+accepted single-beat WLAST1 and subsequent zero source-W balance; AR0+AW1 alone is insufficient.',
        'Native NormalTL header lacks full AXI ID, LEN/SIZE and final R/B debt; existing two bank translation tags cannot replace complete data-retirement tracking.',
        'Current behavioural drive_ddr shares completion-R and PTE returns in mem_q, whereas AW/W/B use separate caw_q/cb_q; a common physical AXI path does not establish an exclusive turn-cost server.',
        'Explicit private banked/NativeN/IoTLB16 modules exist; nominal FPGA default flags do not invalidate that source domain, and do not prove current board bit configuration.'
    ],
    'conditional_proof': {
        'source': 'ordinary-cost/hand-witness.md',
        'type': 'hand-derived conditional mathematics, not runtime performance',
        'assumptions': ['single exclusive R/W memory server', 'DS priority at service choices', 'symmetric turn tau>=0', 'A one PTE and one WRITE beat', 'B two PTE with one-unit dependency gap and two READ beats', 'accurate completed-history estimates', 'no unrecorded carry-in; sufficient receiver; immediate abstract B completion'],
        'AB_completion': ['3+tau', '6+2*tau'],
        'BA_completion': ['7+tau', '6'],
        'AB_max_minus_BA_max': 'tau-1',
        'AB_sum_minus_BA_sum': '2*tau-4',
        'tau_2': {'AB': [5, 10], 'BA': [9, 6], 'candidate_and_O_turn': 'B then A'},
        'tau_0': {'AB': [3, 6], 'BA': [7, 6], 'candidate_and_O_turn': 'A then B'},
        'tau_1_max_tie': {'AB_rank': [8, 12], 'BA_rank': [8, 14], 'candidate_and_O_turn': 'A then B'},
        'mathematical_expected_work': {'PTE_reads': 3, 'PTE_bytes': 24, 'payload_read_bytes': 16, 'payload_write_bytes': 8, 'B_responses': 1},
        'physical_measured_work': None,
        'limits': 'Asymmetric turns, additional carried obligations, different native timings, actual R/B/quiet and histogram accuracy are not proved equivalent. Legal source-prefix proof and mathematical pair are separate evidence, not one executed native fixture.'
    },
    'prior_art': {
        'MASK': 'translation-aware memory priority, fairness and feedback components covered; no exact whole-F1 coverage evidence in read text',
        'ISCA2018': 'walk scheduling/work estimation/instruction batching/aging components covered; no exact whole-F1 rollout coverage evidence in read text',
        'Johnson': 'only publisher abstract retrieved; independent fixed two-stage/two-job formula applies only in its proven domain',
        'published_whole_mechanism_coverage': None,
        'novelty_accepted': False,
        'two_negative_prior_hits_are_not_novelty_proof': True
    },
    'performance_and_physical': {'new_measured_gain': None, 'area': None, 'ff': None, 'LUT': None, 'BRAM': None, 'Fmax': None, 'energy': None, 'selection_cycles': None, 'software_application_cost': None, 'workflow_gain': None},
    'counts': {'new_models': 0, 'microbenchmark_conditions_executed': 0, 'new_performance_runs': 0, 'RTL_builds': 0, 'synthesis': 0, 'board_actions': 0, 'old_experiment_or_validator_replays': 0, 'production_changes': 0, 'new_exclusive_child_agents': 2, 'old_agents_restored': 0, 'frozen_online_controls': 9, 'cost_categories': 37, 'source_files': 26, 'source_excerpt_ranges': 65, 'original_author_PDFs': 2},
    'preserved': ['R2 exact closure and all earlier closed branches', 'RemotePTW/E1/E2/E3/E4/batch/M0/method88/Mode7/stage117 assets', 'F1 positive architecture and new source facts', 'shared-memory feedback research question', 'independent Linux/upstream/RTL contribution status'],
    'forbidden_inferences': ['no global joint-kernel impossibility', 'no paper completion', 'no actual FPGA turnaround or measured completion claim', 'no zero-area/zero-predictor-cost claim', 'no posthoc best-of-controls claimed as one online policy'],
    'goal_completion_order': 'root acceptance, all five document syncs, new delivery checks, then actual update_goal complete',
    'paper_complete': False
}
save(OUT / 'root-result-acceptance.json', acceptance)
save(ROOT / 'action-ledger.json', {
    'schema': 'tl-feedback-action-ledger-v1',
    'actual_goal_created_at': 1791907381,
    'goal_registration': 'goal-registration.json',
    'native_tool_records': 'runtime-tree.json points to original root/child execution logs; logs are read-only',
    'actions': ['read CURRENT_STATUS/applicable AGENTS/method/current plans and exact R2 next contract', 'register actual goal without token budget', 'archive five entry documents byte-exactly', 'spawn two new exclusive Sol/ultra/forknone seats', 'root original F1 design/critical source/prior text/cost/conditional proof review', 'source-prior targeted source snapshots and original-author PDF retrieval', 'ordinary-cost nine online algorithm contracts and conditional hand proof; publisher abstract only for Johnson', 'record actual runtime metadata, no configuration writes', 'stable bounded-entry verdict and five document syncs', 'new delivery-only checks; no old validator'],
    'execution_counts': acceptance['counts'],
    'read_lookup_errors': 'Some readonly path lookups failed and were corrected by targeted rg listings; original tool records retained. These were not failed builds, models or experiment retries.',
    'model_runtime': 'root and both new agents observed gpt-6.1-sol/ultra, effective context475000; nominal500000 not independently inferred from effective metadata',
    'external_models': [],
    'configuration_or_cache_history_mutation': False
})
save(OUT / 'next-goal-contract.json', {
    'schema': 'after-tl-feedback-entry-v1',
    'status': 'bounded_initial_turn_instance_closed_as_admission_witness; no_next_goal_registered',
    'next_decision_question': 'Under source-supported R/PTE shared returns, independent W service, native read admission/held VALID/same-ID ordering, is there a specific observable order-feedback witness unreplicated by frozen DS-priority/owner/short-TL/cheap-debt controls?',
    'entry_requires': ['a new source-supported service action/causal prefix rather than invented unified turnaround', 'all heads legal before first VALID and actual observable history/owned debt', 'one concrete uncaptured choice or complete cost point; complete ordinary algorithm identified', 'preserved onePTW/IoTLB16/native capacities/NativeN and full bytes/R/W/B/fault/quiet'],
    'no_automatic_actions': ['no tau/asymmetric-turn matrix', 'no rerun of old M0/validators or R2 paperwork', 'no old branch/seat restoration', 'no artificial pressure or new capacity', 'no workflow/FPGA before surviving mechanism', 'no automatic characterization-paper switch'],
    'preserve': acceptance['preserved'],
    'unknowns': ['source-faithful uncovered feedback witness', 'history accuracy and generation cost', 'exact complete forecast implementation costs', 'actual MIG service policy', 'application and board benefits'],
    'execution_policy': 'Codex-only gpt-6.1-sol/ultra/native defaults; new bounded goal only when explicitly authorized; original paper window cumulative',
    'paper_complete': False
})

report = '''# TL/PTW共享服务反馈：本轮实例止步于进入门

2026-10-14。执行了用户批准的机制探索入口。root交付两顺序预测的正向设计，独立来源席与普通控制席分别核证。结论是：**最小对称读写切换实例不准入四条件microbenchmark**。没有关闭共享服务反馈的整个方向，也没有确认论文创新。

## 新事实和设计

F1在NormalTL第一次VALID之前比较两个bank各自合法队头，按当前owned义务的预测最晚完成、完成时间之和、年龄及RR选择；缺有效历史回退RR，持续等待头被越过一次后必须服务。保留一个PTW、16项IoTLB、全walk执行、现有容量、NativeN命中旁路及原有W/ID/退休义务。[root机制卡](../out/tl-memory-feedback-20261014/root/mechanism-card.md)给出完整设计与成本边界。

源码允许一个条件双头前缀：t0同沿接受AR0、AW1和bank1单beat WLAST；写源余额净0，t1分别选择READ/WRITE，t2在两头首次合流VALID前选择。仅AR0/AW1同沿而W未供齐不够；两个新AR不能在一个AR通道同沿接受。这是接口源码推导，不是实际iDMA轨迹或板测。Native合流是eager VALID，PTW后来空闲不允许重选；已接受spill也不能重新排序。

NormalTL的118bit头没有完整AXI ID、LEN/SIZE和最终R/B欠账。两个TL tag只覆盖翻译生命周期；bank在completion地址内部接受后可空闲，数据/B仍有所有权。新增预测需要有费用的头元数据、完整owned义务、历史、读写端口、选择逻辑与延迟，不能拿旧observer/诊断记录当免费在线wire。来源原文及配置区分见[来源证据](../out/tl-memory-feedback-20261014/source-prior/source-evidence.md)。

## 为什么当前实例不进入性能验证

现有私有DDR驱动把READ/PTE放在同一个mem_q中，可在保持同ID与held R的条件下优先DS；AW/W/B由独立caw_q/cb_q服务。它没有统一R/W排他服务器、方向状态或切换惩罚tau。共用DDR端口不能证明这个额外服务规则；加入tau后运行只能测人为增加的规则，不能验证当前路径。

独立条件手算仍产生有用边界：A一PTE加一WRITE beat，B两PTE（依赖空隙1）加两READ beat，假设统一服务器、准确历史和对称切换tau。两顺序完成向量是A→B `(3+tau,6+2tau)`，B→A `(7+tau,6)`。tau=2时最晚完成10对9，tau=0时6对7；单位是数学服务单位，不是RTL周期或实测。tau=1最晚都8，但sum12对14，仍选A。

一个提前冻结的完整在线O-turn规则——历史tau大于PTE依赖空隙时读优先，否则短翻译——覆盖这个固定对称族及相同held/公平/缺历史guard。它不计算两份完整轨迹。这个具体选择见证因此被普通控制解释；没有证明真实面积优劣、全部joint算法等价或非对称服务族等价。合法源码前缀和这个数学pair也是两份不同证据，不能拼成一次原生运行。[手算及严格覆盖域](../out/tl-memory-feedback-20261014/ordinary-cost/hand-witness.md)保留完整时间线。

## 强对照、前作和完整成本

冻结九项完整在线普通规则：RR、FCFS/年龄、owner/阻塞路径、短翻译、少欠账、多欠账、释放密度、O-turn及有严格适用域的两阶段排序。各规则同信息/原生容量；可删除未用观测逻辑，不强加预测器来人为等成本。不同场景事后最优的包络不是一个在线规则。

亲读MASK与ISCA2018原作者正文：前者覆盖PTE优先及公平反馈，后者覆盖翻译工作量估计、同指令聚批和年龄保护。没有获得它们整体覆盖F1的证据，也不能据两篇没有该组合就确认创新。Johnson只获取出版商原始摘要；固定独立两阶段公式是独立推导，不能外推到共享服务反馈。[前作边界](../out/tl-memory-feedback-20261014/source-prior/prior-art.md)保留正文页码、URL和原PDF。

37项成本分类已列完整公共继承、新观察、owned元数据、历史/有效性、两个预测、运算/端口/扫描/选择延迟及软件费用。参数宽度和实现未冻结，所以精确总FF、面积、Fmax、能耗及应用成本保持null；不是完整数字成本已闭合，也不借R2的1329bit当新预测器总成本。[完整接口成本卡](../out/tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md)给出公式和寿命。

## 实际执行与后继

26份定向源码、65段原文与SHA，两份原作者PDF、九项算法卡、root机制卡和稳定裁决已交付。0新性能运行、0microbenchmark、0RTL/build/综合/板操作、0旧实验或validator重跑，无生产改动。四条件执行依赖门未通过，全部未运行；没有为得到正结果添加压力或服务惩罚。

root和两个新独占席实际均gpt-6.1-sol/ultra，运行记录有效上下文475000；forknone，无外部模型、旧席恢复或上下文覆盖。未由有效窗口推断未直接观察的原生nominal500000。首轮未超过两工作日，原立题窗口累计。

唯一后继入口是现有R/PTE共队、W独立服务和真实准入/held VALID之下的**新、合法、可观察反馈见证**，并说明哪个完整普通规则未覆盖；不自动扩大tau矩阵、恢复关闭版本或安排FPGA。存活机制后才选一个GPU/AI优先工作流核实际共享翻译路径。R2及8/9月以来全部资产保留；论文未完成。

根裁决：[root-result-acceptance.json](../out/tl-memory-feedback-20261014/root-result-acceptance.json)。后继范围：[next-goal-contract.json](../out/tl-memory-feedback-20261014/next-goal-contract.json)。实际goal先登记、无额外预算，在五文档同步与交付核验后才结束；最终状态/用量见[root/goal-state.json](../out/tl-memory-feedback-20261014/root/goal-state.json)，完整回执见[root/goal-completion.json](../out/tl-memory-feedback-20261014/root/goal-completion.json)。
'''
(WORK / 'iommu-platform/docs/tl-memory-feedback-entry-20261014.md').write_text(report)
(OUT / 'README.md').write_text('''# TL/PTW共享服务反馈入口交付（2026-10-14）

本轮停止最小对称R/W-turn实例进入microbenchmark：真实私有DDR源不支持统一turn服务，且条件选择被具体O-turn覆盖。整体joint-memory问题、root正向设计与旧资产保留，不恢复关闭分支。

- [稳定根裁决](root-result-acceptance.json)；[执行合同](execution-contract.json)；[后继合同](next-goal-contract.json)。
- [root机制卡](root/mechanism-card.md)；[报告](../../docs/tl-memory-feedback-entry-20261014.md)。
- [来源裁决](source-prior/source-bound-verdict.json)；[原文与配置](source-prior/source-evidence.md)；[26源/65段SHA快照](source-prior/source-snapshot.json)。
- [精确前作边界](source-prior/prior-art.md)；[原作者PDF来源](source-prior/pdf-provenance.json)。
- [九项普通合同](ordinary-cost/baseline-contract.json)；[条件数学与覆盖域](ordinary-cost/hand-witness.md)；[37项完整成本接口](ordinary-cost/cost-interface-card.md)。
- [实际模型/上下文记录](root/runtime-tree.json)；[实际动作账本](root/action-ledger.json)；[入口五文档原件](before/manifest.json)。
- [交付核验](root/delivery-verification.json)；[实际goal登记](root/goal-registration.json)；[实际完成回执](root/goal-completion.json)；[实际goal状态](root/goal-state.json)。

model目录保持空：未过差异门，没有运行四条件或生成假性能结果。数学10/9等值不是实测或RTL周期。未测成本保持null；论文未完成。
''')

summary = '''## 2026-10-14 TL/PTW共享服务反馈入口裁决

按批准计划完成F1正向两顺序预测设计、关键源/前作、九项完整在线普通规则和37项成本接口。条件双头在AR0+AW1+已接受单beatWLAST1之后可由源码达到；firstVALID已锁不能等PTW空闲重选。NormalTL不带LEN/SIZE/fullID和全部R/B债务，新增反馈/所有权/历史/预测端口及延迟须付费。

最小对称turn实例不准入：真实私有DDR共享R/PTE队列但W独立服务，没有统一R/W排他server或tau切换；手算tau2最晚10对9、tau0为6对7均被一个冻结O-turn规则解释，tau1的sum12对14也一致。这些是条件数学单位，不是RTL/实测；不证明joint全族或整体发表前作已覆盖。两篇原作者正文仅覆盖组件，成本总FF/面积/Fmax/应用null。候选完整成本边界已列，数字实现成本未闭合。

0新模型/性能/microbenchmark/RTL/build/综合/板、0旧实验或validator重跑，无生产改动；四条件均因门未过而未运行，不加压力。26源/65段、2原作者PDF、root设计和全部普通合同保留。root+2新独占席实测Sol/ultra/有效475000，forknone/Codex-only，不恢复旧席，无额外预算。

后继只接受现有R/PTE共队、W独立服务及真实准入/heldVALID之下的新可观察反馈见证与具体普通差异；不自动追加tau矩阵/新goal/FPGA或切换论文定位。R2/R107/C112及所有旧关闭裁决保留，8/9月资产不重跑，原立题窗口累计，论文未完成。

[本轮报告](PREFIXdocs/tl-memory-feedback-entry-20261014.md)／[根裁决](PREFIXout/tl-memory-feedback-20261014/root-result-acceptance.json)／[后继范围](PREFIXout/tl-memory-feedback-20261014/next-goal-contract.json)／[交付核验](PREFIXout/tl-memory-feedback-20261014/root/delivery-verification.json)／[实际goal回执](PREFIXout/tl-memory-feedback-20261014/root/goal-state.json)。actual goal在五文档同步及交付核验后才完成，以最终工具回执为准。以下完整保留入口历史；其中旧“下一步”仅指当时状态。

'''

archives = json.loads((OUT / 'before/manifest.json').read_text())
for row in archives:
    dest = WORK / row['path']
    before = (OUT / row['archive']).read_bytes()
    if row['path'] == 'CURRENT_STATUS.md':
        tail_start = before.index('- 独立Linux贡献：'.encode())
        current = '''# 当前状态：TL/PTW共享服务反馈初始实例未准入（2026-10-14）

- F1两顺序预测正向设计及来源/普通控制门已交付。合法双头仅作条件源码推导；firstVALID已锁不能重选，原生NativeN和完整R/W/B所有权必须保留。
- 初始对称turn实例不进入四条件：当前DDR源仅R/PTE共队、W独立，没有统一切换tau；条件数学选择被单个完整O-turn覆盖。不关闭joint反馈全族，不证明整体发表覆盖或实测收益。
- 九项在线普通合同、37项完整费用边界、26源/65段、两原作者PDF保留；数值预测成本/面积/Fmax/CPU应用null。0新性能/模型/RTL/build/板、0旧实验或validator重跑，无生产改动。
- 下一步仅凭现有R/PTE共队、真实准入/heldVALID的新可观察反馈见证和普通差异重新立具名合同；不自动扩大tau矩阵、恢复R2/R107/C112或安排FPGA。8/9月及后续资产保留，原窗口累计，论文未完成。
- 实际goal已登记、无额外预算；root+2新席实际gpt-6.1-sol/ultra、有效475000、forknone/Codex-only，未恢复旧席/覆盖上下文。最终goal状态及用量以交付核验后的工具回执为准。

[本轮报告](iommu-platform/docs/tl-memory-feedback-entry-20261014.md)／[稳定裁决](iommu-platform/out/tl-memory-feedback-20261014/root-result-acceptance.json)／[下一范围](iommu-platform/out/tl-memory-feedback-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/tl-memory-feedback-20261014/root/delivery-verification.json)／[实际goal回执](iommu-platform/out/tl-memory-feedback-20261014/root/goal-state.json)。前轮R2当前页原件在本轮before目录。

'''
        dest.write_bytes(current.encode() + before[tail_start:])
    else:
        is_local = row['path'].startswith('iommu-platform/docs/')
        prefix = '../' if is_local else 'iommu-platform/'
        # docs/ paths are siblings of the report, out/ paths are in ../out/.
        content = summary.replace('PREFIXdocs/', '' if is_local else 'iommu-platform/docs/').replace('PREFIXout/', '../out/' if is_local else 'iommu-platform/out/')
        title = {'HANDOFF.md':'# 阶段交接：TL/PTW共享服务反馈入口\n\n','研究计划.md':'# 多DMA共享IOMMU研究总计划：TL/PTW共享服务反馈入口\n\n','iommu-platform/docs/research-execution-plan-20261011.md':'# 当前执行：TL/PTW共享服务反馈入口\n\n','iommu-platform/docs/validation.md':'# 最新验证记录：TL/PTW共享服务反馈入口\n\n'}[row['path']]
        dest.write_bytes((title + content).encode() + before)
print('stable root verdict, report, navigation, next contract, ledger and five document syncs written')
