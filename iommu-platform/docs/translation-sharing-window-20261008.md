# 共享译址窗口：普通方法的覆盖与下一机制（§48）

2026-10-08。结论：剩余用途保留在小模型中有效，但同信息普通方法能够实现。下一检验译址准备进度与请求步调的联合控制，目标仍是 V/A 整体性能。

## 0. 边界

本轮只执行一次有限逻辑请求/缓存模型，不是 36 次硬件实验。执行记录为 1 run、exit 0、`elapsed_seconds≈0.0156 s`；结果含 3 schedules × 3 capacities = 9 trace/capacity 组，4 普通策略共 36 policy records。DP 仅有小 N=2/E=2 与 E=3 两个比较，oracle 与 DP 均为 6，一致。本轮没有 DUT 构建/运行、生产 RTL/Linux/tests 改动或 BIT/JTAG/上板/写卡操作；最新实际完整 copy 仍为 §43。

## 1. 模型归属

模型脚本与证据草稿由 DeepSeek 起草；Codex 完成逐函数审查、应用 schema 修订并实际执行，同时负责机制设计判断。两席分别审查真实请求窗口/普通等价构造，以及最接近的三项前作。Codex 根席亲自核关键源码、原文与实际结果，未使用 Claude。

## 2. 实际未命中矩阵

模型中两份 32KiB 拷贝共享同一输入和翻译上下文，写入各自独立输出，均按 4KiB 对齐。每流每页有 D、S、D、S 四次逻辑译址请求；每 trace 共 64 次请求、24 个不同翻译 key。E 是总映射槽数；gap 是两流完成本页逻辑请求后的最大页数差，**不是数据退休差或周期差**。本模型也不代表旧 §43 的不同上下文、目的偏移 3072 几何。

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

`PTE_reads=3×misses` 仅限 Sv39 无 PWC、每 cold leaf 三读，不是实测硬件 PTE counts 或 cycle。所有 case 的 `sum(perflow)==misses`，且 `ordinary≥oracle≥24`。Oracle 知道未来 global 顺序，信息比 registered-use 更多，不作同信息可实现策略。Registered-use 只见两个登记 stream 各自固定规则/position，检测还有用途，不用 next global order。私有 dest 方案所有 physical mapping 总数 E，不免费加 cache；但这里只相同 entry 数，不是完整 state/ports/面积等价。

E8/gap4 LRU30（A16/V14）→普通 remaining-use24（A16/V8），说明已有额外 noncompulsory work 可被普通信息策略清除。E4 配对所有 24、E16 全 24：简单相位控制或足够容量在该 trace 已达首次工作下界，不能把 candidate 比 LRU 少 miss 称独有收益。该模型不提供 data latency/供给/heldAW/W 依赖；不能称配对更快，也不能称所有真实 copy 达到 24。

## 3. 事件、空洞与普通窗口

先停止“登记 job 剩余用途取交/OR 再 dead-first”作为独立算法：因为同信息普通 interval/refcount 逐项同 decision，模型中普通实现已达到部分场景最低工作；仅 cursor/interface 重编码也没有已证总成本增量。

保留的工程成果是可靠 materialization 事件与真 raw-live 的区别：原 full-parent fixture 可条件性前缀；src3072 短首父可被 helper 跳过，无 fault 下后页先成功。`done_q` 是 R 退休，不是 TL success。普通 Depth4 现有窗口组合 scan 能挡洞、无需另存 hole bitmap；如果要求短 parent 在 R 前推进，就需要额外成功证书或扩大合法结果资格，两方同计，不是免费接口。

root 指出一般 short-parent 洞：在 `CONF1、src=3072、dst=0x10000、len=4096` 下，legalizer 以 2KiB 边界形成 seq0 `[3072,4096)`/len127、seq1 `[4096,6144)`/len255、seq2 `[6144,7168)`/len127；seq1 可先被成功准备，而 seq0 无 source 译址。目的地址选 `0x10000`，保持页内 offset 为 0，并使源、目的范围不重叠。这个整体握手例是源码推演，未运行 DUT。若 watermark 取“最大成功 start page”，就会错误宣告 page0 用途结束；因此需要连续成功边界或保守窗口，而不是 max 地址。

## 4. 剩余用途的普通等价

对注册 job、每 direction 连续 frontier 与可靠结束状态，普通实现可构造 `Remaining(J,dir)`，再对共享 entry 计算 `need(e,J,dir)=compatible_context && overlap(entry_page_interval, Remaining)`，最后做 OR 或 refcount。因为该 boolean 与普通 interval/refcount 完全相同，所以不能把“注册 job 剩余用途取交/OR”本身作为独立机制创新。本文不把所有条件性 candidate 都说成已有实现，也不因为 Pichai2014/LocalityDescriptor2018/Orchestrated2023 未逐字写出该精确合同就断言无人做过。

## 5. 下一 root 候选与停止条件

继续共享 IOMMU V/A 整体性能主线；候选进一步变为：用译址 materialization 前沿约束共享源的超前，保留数据重叠，并让已承诺写义务有限继续。它不是已实现算法，也不是新的 TLB-aware pacing 原则，因为 Pichai2014 已有相近原则。

下一先建立两个可比控制：一是 ordinary page barrier/lag cap＋用途保留/有限 prefetch，保留 held VALID 与 committed W 义务；二是 materialization-frontier 控制，只限制尚未 first-offer 的新工作，迟到 DMA 保存 translation 后可释放缓存用途、不等数据整页结束，已 held/committed 义务继续所需 source/TL 并计允许 overshoot。允许普通 translate-first scheduler、hit-first/RR、bounded lookahead 和任务感知保留使用同样 job/context/window/完成信息。

如果普通组合已同决策、同完整成本/性能，就不立独立机制；如果候选有明确可实现状态/更新差异，达到较好 wholecopy 性能或较低完整成本，并证明有限继续与安全进度，就可以论证微架构贡献。不能以三篇前作未明确写此合同推出新颖性。

单一下一动作：在现有 prepared parent/window＋virtual/physical W obligation 上构造一条真实可达 trace，区分 TL-frontier 控制与普通 page/translate-first controls，列全同资源/同信息 cost 及 pause drain；如果无差异，就停止该枝。通过后只做最小 out complete-copy 对照，不先编译 BIT/扫阈值。当前 source 合同主要是静态 normal/4KiB/有限 job；error/epoch/失效/持续到达正确性后续另需覆盖，不能把现模型当这些验收。

## 6. 接近前作与证据导航

[ASPLOS 2014](https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf) 已用 TLB miss、victim tags、hit 的 LRU 深度控制 warp 调度，并按共享 PTE 历史重组线程。固定原稿对应 §7.2/8.2，不能说以翻译信息调 producer 是新原则。

[Locality Descriptor 2018](https://people.inf.ethz.ch/omutlu/pub/LocalityDescriptor-Cross-Layer-GPU-Data-Locality-Abstraction_isca18.pdf) 已按已知共享范围协调任务、缓存保护和预取距离；[Orchestrated 作者稿](https://libingyao.github.io/assets/files/DAC2023.pdf) 按翻译压力放置新 TB，并做分区/共享。三者覆盖的动作不同，不能混称已经证明多 DMA 精确前沿合同；未写该精确合同也不等于领域无人做过。本次原文版本、hash 和覆盖范围见[前作审核](../out/translation-sharing-window-20261008/prior-critical-agent.md)／[原作清单](../out/translation-sharing-window-20261008/primary/sources.json)。

- [根级实际结果审定](../out/translation-sharing-window-20261008/codex-result-acceptance.md)／[机制与源码判断](../out/translation-sharing-window-20261008/codex-window-proof-and-design.md)
- [原始模型 JSON](../out/translation-sharing-window-20261008/cache-model-result.json)／[实际执行账本](../out/translation-sharing-window-20261008/execution-ledger.json)／[脚本](../out/translation-sharing-window-20261008/sharing_cache_model.py)／[模型合同](../out/translation-sharing-window-20261008/codex-model-contract.md)
- [独立窗口审查](../out/translation-sharing-window-20261008/rules-critical-agent.md)／[下一单一判别](../out/translation-sharing-window-20261008/codex-next-decision.md)／[阶段账本](../out/translation-sharing-window-20261008/stage-evidence.json)

当前无真实 copy 性能增量、完整面积/频率数据或期刊分区结论。DeepSeek 起草模型、接口证据与报告；Codex 设计、审查应用、实际执行并审定，两席关键质疑，无 Claude。
