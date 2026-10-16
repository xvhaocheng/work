# §48 根级实际结果审定与下一机制

root Codex完成脚本逐函数审查、应用DeepSeek schema修订并执行唯一模型run。当前结果是有限逻辑译址请求/缓存模型，不是DUT/cycle/wholecopy/板卡数据。Root亲读原始JSON；执行记录见execution-ledger.json。

## 1. 实际执行

run exit0，elapsed_seconds=0.01556497160345316；3 schedules×3 capacities=9 trace/capacity组，4普通策略共36records。每trace64requests/24uniquekeys（8 shared source＋各8 dest），两DMA perflow顺序D,S,D,S；logical source page gap=8/4/1。小N2、E2/3两个memo DP比较各oracle6==DP6，只小case验证，不声称穷尽所有trace。无DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD动作，最新actualcopy仍§43。

脚本原proposal有重复trace字典key，会覆盖场景名；执行前已回传DeepSeek并应用trace_name/request_trace修订。此前未运行/未生成结果；worker修订说明中“上一轮已有JSON”的假设不适用，本轮不重跑。已接受脚本SHA4a0aac692caebaa9e179bfd4ac8623e2427e28dfc8eef3fa1151d7efaf99ae0c。

## 2. 未命中实际矩阵

| trace gap | E | global LRU | ordinary remaining-use | 2 private dest+shared src | offline oracle |
|---|---:|---:|---:|---:|---:|
| 8（A全后V） | 4 | 32 | 30 | 32 | 29 |
| 8 | 8 | 32 | 26 | 32 | 25 |
| 8 | 16 | 24 | 24 | 24 | 24 |
| 4（A先4页再V/A） | 4 | 32 | 30 | 32 | 26 |
| 4 | 8 | 30 | 24 | 26 | 24 |
| 4 | 16 | 24 | 24 | 24 | 24 |
| 1（按页A/V配对） | 4 | 24 | 24 | 24 | 24 |
| 1 | 8 | 24 | 24 | 24 | 24 |
| 1 | 16 | 24 | 24 | 24 | 24 |

PTE_reads=3×misses仅限Sv39无PWC、每cold leaf三读，不是实测硬件PTEcounts或cycle。所有case sumperflow==misses，ordinary≥oracle≥24。Oracle知道未来global顺序，信息比registered-use更多，不作同信息可实现策略。Registered-use只见两个登记stream各自固定规则/position，检测还有用途，不用nextglobal order。私有dest方案所有physical mapping总数E，不免费加cache；但这里只相同entry数，不是完整state/ports/面积等价。

E8/gap4 LRU30(A16/V14)→普通remaining-use24(A16/V8)说明已有额外noncompulsory work可被普通信息策略清除。E4配对所有24、E16全24：简单相位控制或足够容量在该trace已达首次工作下界，不能把candidate比LRU少miss称独有收益。该模型不提供data latency/供给/heldAW/W依赖；不能称配对更快/所有真实copy达到24。

## 3. 创新判断

停止“登记job剩余用途取交/OR再dead-first”作为独立算法：同信息普通interval/refcount逐项同decision（rules报告§6），模型中普通已达到部分场景最低工作。仅cursor/interface重编码也没有已证总成本增量。

保留工程成果是可靠materialization事件与真raw-live区别：原full-parent fixture可条件性前缀；src3072短首父可被helper跳过，无fault下后页先成功。done_q是R退休，不是TL success。普通Depth4现窗口组合scan能挡洞、无需另存hole bitmap；若要求短parent在R前推进则需额外成功证书/扩大合法结果资格，两方同计，不是免费接口。

## 4. root下一可判定机制合同

继续共享IOMMU V/A整体性能主线；候选进一步变为**用译址materialization前沿约束共享源的超前，保留数据重叠，并让已承诺写义务有限继续**。它不是已实现算法，也不是新的TLB-aware pacing原则（Pichai2014已有）。

下一先建立两个可比控制：
- ordinary page barrier/lag cap＋用途保留/有限prefetch，保留held VALID与committed W义务；
- materialization-frontier控制：只限制尚未first-offer新工作；迟到DMA保存translation后可释放缓存用途，不等数据整页结束；已held/committed义务必须继续所需source/TL并计允许的overshoot。

允许普通translate-first scheduler、hit-first/RR、bounded lookahead和任务感知保留用同样job/context/window/完成信息。若普通组合已同决策同完整成本/性能，不立独立机制；如果候选有明确可实现状态/更新差异，达到较好wholecopy性能或较低完整成本、并证明有限继续与安全进度，就可以论证微架构贡献。不能以3论文没明确写此合同推出新颖性。

单一下一动作：在现有prepared parent/window＋virtual/physical W obligation上构造一条真实可达trace，区分TL-frontier控制与普通page/translate-first controls，列全同资源/同信息cost及pause drain；若无差异，停止该枝。通过后只做最小out complete-copy对照，不先编译BIT/扫阈值。当前source合同主要是静态normal/4KiB/有限job；error/epoch/失效/持续到达正确性后续另需覆盖，不能把现模型当这些验收。

