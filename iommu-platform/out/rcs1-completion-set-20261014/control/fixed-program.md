# RCS1：同信息普通控制的固定程序

版本 `RCS1-controls-v3`，2026-10-14。普通控制作者为 Codex。本文件冻结五个指定对照、更强 B5-RefMin及root在任何模型执行前明确新增的B6-fractionInsert；后续不能因为某个场景结果而改选择规则。v1 的五个规则保持，v2 接受root的18key范围/B5，v3增加独立B6而不修改B4。完整组合是本席新构造的普通控制，不声称是某篇已发表论文的完整复现。

## 共同输入、边界与事件顺序

原共享 cache 为 16 ways、单 PTW，原队列、原 native in-flight 和数据 buffer 不变。可信登记最多四个软件 task。17key 单缺 key 草案被 B5 完整覆盖；root 冻结的最终纸面范围为 `K<=18`，第17/18项均是付费元数据，不是新增 cache entry。一个软件 task 可以由多个原生 64B copy 组成；它们顺序供给，不变成四个新增并发 DMA，也不把当前 64B HOLD 当成通用批量硬件接口。

正式 key、上下文、映射代、R/W 权限、页大小、task/source/phase/slot generation、任务/数据/buffer 所有权、原生驻留/fill/invalidation、精确剩余 lookup 用途和真实 private handoff 由同一个付费登记与反馈合同提供。所有控制包括 RCS1 都能看到相同的 task-key incidence、登记年龄、完整 cache 及原 PLRU 状态。当前 cache 状态读取不是免费 oracle。未被接受的请求、未来 PTE、未知 DDR 时间、未来 trace 和期望的成功回调都不进入信息集。

本版先限定同一正式 S1/4KiB/non-global key 类；resident 还要求相关 task 的权限/epoch 匹配，不能把 tag 命中当成任意权限授权。其他类别走相同普通 fallback。正式失效、错误、取消和 drain 优先；保护永远不阻正式失效。

对当前完整成功 walk 结果 q，固定顺序如下：

1. 验证正式结果、请求/owner/context/epoch 的关联。
2. 把完整结果交给已经接受的原 TC 请求 owner。bypass 必须有这个新增、付费的普通完整结果直达 TC 路径；不能靠“TC 回头重查已填 cache”假装已经旁路。路径只能交付当前合法 owner，不广播授权给其他 task。
3. 只有可证明该 owner 的这个 key 此后无需共享 lookup，才退休其相应 R/W 用途。数据、任务、B、consumer/cache 和 buffer 所有权均不由这一事件退休。
4. 在这份一致 snapshot 上选择 cache 插入 victim 或 bypass，并原子提交。无法维持 snapshot/epoch 一致，采用同一个保守普通 fallback，不能使用一半新一半旧资料。

上面的 handoff 可能需要响应暂存和反压；没有额外免费完成槽。实际端口、路由、排空、软件发布及周期费用未测，见 `cost.json`。RCS1 和普通控制取得同一 direct-result 路径、同一服务时延和容量合同。

上述2→3→4是已经真实稳定handoff时的逻辑snapshot含义。实现可以在当前accepted owner及完整结果/epoch关联确定后，对本次证明为最后用途的q做**prospective**扣除来预先计分；实际private许可、用途退休和cache选择提交必须在稳定TC handoff后原子生效。未完成handshake只是一份待提交选择，不是private receipt，不允许copy数据、task/B/consumer或buffer提前行动。root后续条件模型固定共同choice服务1tick、正常完整结果交付1tick，双方同付；这不是物理RTL时序验收。

`U[t]` 是 task t 当前仍需共享 lookup 的不同正式 key 集合；已经合法私有接收且证明不再共享 lookup 的用途不在 U 中。同一个 key 的多个 R/W phase 用途只有全部退休才从 U 删除。计数按不同 task/key，不能把一个 task 重复 lookup 算成多个 task。U 为空的 task 不参加保护目标挑选，但 RCS1 可将其视为已有完整翻译集合；这项常数对动作排序无影响。

```text
live[t]       := 登记有效、epoch/关联正确且任务生命周期尚活
R             := 目前真实 resident 且正式 key/权限有效的集合
missing[t]    := U[t] - R
missing_n[t]  := |missing[t]|
remaining[t]  := |U[t]|
fraction[t]   := |U[t] intersect R| / |U[t]|       # U非空
refs[k]       := { t : live[t] AND k in U[t] }
legal_ways    := 原 cache 此次允许替换的 ways     # 本见证是全部16
```

所有控制保持原任务/请求供给顺序，不新增 task dispatch 排序、全 key ready gate、全任务 barrier 或等保护解除的停顿。候选保护只是此刻 victim 选择的 soft hint。不能把尚未 ready 的被登记 task 当成已授权的数据请求。

## 唯一原生 PLRU 与掩码函数

`P(M)` 是原 tree-PLRU 在合法非空 mask M 内的唯一 victim；不是任意 LRU 列表。每个节点优先走原 tree bit 指向且含合法叶子的子树，否则走唯一有合法叶子的另一侧。所有 way 的 mask 重现原 victim。若保护 mask 覆盖全部合法 ways，固定立即回 `P(legal_ways)`；不等待、不加 TTL。

本工作树的具体定义是 `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:364-373,405-422`：逐层 `choose_right = right_has_eligible && (!left_has_eligible || tree_bit)`。原 shared history 的 hit 更新在 `:384-400`；本轮不启用 PartitionLocalHistory，不清 tree、不复制 PLRU history。该源码检查是只读，未修改或运行。

```text
INSERT_WITH_PROTECTION(S):
    Pmask := { w in legal_ways : key(w) in S }
    M := legal_ways - Pmask
    if M is empty: M := legal_ways
    v := P(M)
    insert q into v
```

已有 invalid/free way 的处理和 native 合法性相同；当前登记 key 重合时不创建第二份同 key cache 内容。bypass 的结果交接与失效处理也不能省略。

## 初始五个冻结对照与独立新增B6

### B0：原 PLRU

每次可缓存的正常 full result 都插入 `P(legal_ways)`。登记资料可见但不用于 victim 选择。bypass 路径共同存在，本程序的正常 cacheable 分支不主动 bypass。

### B1：OC1-min 的 per-key refs + PLRU

从与 OC1-min 相同的两个 R/W task-user 位图派生 `S={k: refs[k]非空}`，执行 `INSERT_WITH_PROTECTION(S)`。保留原 phase 最后用途退休、正式失效及 all-protected 原 PLRU escape；不添 per-key TTL、独立 rank 或第二份计数。这是精确 OC1-min 的 cache 替换部分，任务供给使用本轮共同固定顺序。

### B2：整任务保护 oldest

在 `live[t] AND U[t]非空` 中仅选择登记年龄最小的一个 t，保护其**目前 resident**的 U[t]，执行 `INSERT_WITH_PROTECTION(U[t] intersect R)`。无 task 则原 PLRU。年龄相同按可信 task-slot 数字小优先。它不为了完善被保护 task 而等其缺 key，不把多个 task 后验拼成保护组合。

### B3：整任务保护最少当前 missing

仅选择一个 t，按 `(missing_n[t], remaining[t], reg_age[t], task_slot[t])` 升序排序；保护其 resident U[t]，执行同一函数。无 task 则原 PLRU。这里“最少 missing”仅指已经知道的共享 lookup 缺 key，不声称是真实最短 DMA 执行时间。

### B4：当前 fraction 驱动的普通 MeDiC-like 组合

这是本席新写的普通 fraction 控制，**不是 MeDiC 原论文的逐程序复现**，也不使用原论文没有提供的免费 task 登记接口。使用当前 U/R 的 fraction，不重放历史 trace、不免费取得历史命中率；没有历史计数状态。

```text
F := {t: live[t] AND U[t]非空}
Q := {t in F: q in U[t]}
if Q is empty:
    bypass q                       # 当前登记无后续共享用途，owner已获完整结果
else if max(fraction[t] for t in Q) < 1/5:
    bypass q                       # 固定mostly-miss阈值；等号允许插入
else:
    t := F中按 fraction降序、missing_n升序、remaining升序、age升序、slot升序的第一项
    INSERT_WITH_PROTECTION(U[t] intersect R)
```

比例用整数交叉乘法比较，阈值 `1/5` 是预先固定的本席普通选择，不由 witness 调参。保护只取一个 task，all-protected 一律原 PLRU。若 scope 的完整登记或 resident 资料缺失，整分支禁用优化，执行原 PLRU 与合法 direct-result/drain。

该冻结B4有明确cold局限：如果其未来users的current fraction一直为0，它会持续bypass而不能自行形成本见证的resident cold前缀。不能把其他控制生成的prefix免费赋给B4，不能声称它是原MeDiC复现，也不事后修改阈值。整个episode比较必须运行其自身真实选择并记录这个局限。

### B6：fractionInsert整任务保护（root在执行前独立新增）

这是在任何模型结果前为覆盖B4的cold admission缺口新增的明确强普通分支，B4原程序保留。B6与B4的task排序/单task保护完全相同；每个正常cacheable full result总是插入，没有1/5阈值，也不因Q为空而旁路：

```text
F := {t: live[t] AND U[t]非空}
if F is empty: insert q into P(legal_ways)
else:
    t := F中按 fraction降序、missing_n升序、remaining升序、age升序、slot升序的第一项
    INSERT_WITH_PROTECTION(U[t] intersect R)
```

相同fraction交叉乘法、soft/all-protected fallback、正式permission/epoch和paid direct-result合同适用。无历史hit计数/trace。B6同样是本轮新普通组合，不是MeDiC原程序；其cold前缀按自身victim过程演化，不能复制另一policy的cache状态。

## RCS1 的比较函数（候选接口）

对每个合法 v 的插入以及 bypass 用同一 snapshot 算：

```text
R_insert(v) := (R - {key(v)}) union {q}
R_bypass    := R
C(a)        := |{t: live[t] AND U[t] subset_of R_a}|
D(a)        := |union_{live t}(U[t] - R_a)|
choose a by C descending, D ascending, insertion before bypass,
then P(mask of score-tied legal insertion ways)
```

此处已按共同 handoff 删了确实不用再共享 lookup 的用途。C 只表示翻译用途集合完整，不表示 task 已发出、数据/B/consumer 完成或 buffer 可回收。score 不是新的 barrier；正常 DMA 请求继续原流程。

## 更强普通覆盖检查：固定 B5-RefMin（单缺 key 子族）

除五个指定对照，必须公开一个简单、固定且没有 completion-set 枚举的普通程序，避免把 witness 中的 per-key incidence 换名误判成独立能力：

```text
B5-RefMin（作为v2的正式固定第六对照）:
    insertion(v)的费用 := |refs[key(v)]|
    bypass的费用       := |refs[q]|
    选择费用最小的动作；tie插入优先；插入tie按同一P(mask)
```

它只数当前每 key 的不同 task 用户，用户位图已有，不强迫独立 refcount/TTL/rank。B5 与 RCS1 在一般多缺 key 输入上不声称等价。

**但在当前登记的不同 key 并集 H 满足 `H - (R union {q}) = 空集`，且 q 是当前唯一可能缺 key 的子族，B5 对全部任务 incidence 和所有合法 victims 精确覆盖 RCS1。** 插入 v 后唯一可能缺 key 是 v，所以 `C(insert(v))=Nlive-|refs[v]|`；bypass 后唯一可能缺 key 是 q，所以 `C(bypass)=Nlive-|refs[q]|`。第一目标完全等价于费用最小。第二目标对 ref=0 的动作是0，否则1；因此没有额外区分。相同插入优先与原 PLRU tie 产生相同动作。此证明不依赖 witness 的 task 名字、年龄或某一 PLRU 序列。

特别地，满 cache16 的所有16个驻留 key 都有当前 refs、登记全并集最多17key且 incoming q 是唯一缺 key 时，整个这种有限子族被 B5 覆盖。只区别 B0–B4 不能支持“完整集合规则在这一族新增独有选择能力”。若后续要声称超出 B5，必须给至少另一个当前缺 key 或准确例外的合法 snapshot，完整计费并与这个固定 B5 比较。B5 在多缺 key 族是否覆盖不由“可能能加模块”推断；本轮没有运行枚举。

另一个程序如果直接逐 victim 计算 C/D，确实能在全域表达 RCS1，但它已经实现了候选的集合评分规则；不能仅因能改名为 ordinary 来当成准确已发表覆盖。公开前作覆盖、B5 的具体子族覆盖和一般可编程实现能力分别裁决。

RCS1本身还有如下精确代数正常形，成本应允许它采用这份更小实现，不能强迫存17份虚拟cache来制造复杂度：

```text
H := union_live_t U[t]
E_q := {t: live[t] AND (U[t]-R) subset_of {q}}
C(insert(v)) = |E_q| - |refs[key(v)] intersect E_q|
C(bypass)    = |{t: live[t] AND U[t] subset_of R}|
D(insert(v)) = |(H-R)-{q}| + 1[refs[key(v)]非空]
D(bypass)    = |H-R|
```

这对不同正式 key、q 当前确实 missing 的合同成立；q 已存在、权限/epoch例外或非正式 alias 走共同一致处理。候选比 blind B5 多做的是“除 q 外无其他当前 missing”的 task 过滤 E_q。task-key incidence本身是共同输入，不是RCS1独有的新信息；是否已有准确 E_q 过滤前作须由root核原文，不能由本席新组合冒充已发表。

## 冻结与证据限制

五个指定程序的规则是在收到 root 的详细冷前缀消息前选定；由于消息自动送达，文件落盘时间晚于那条消息。不能伪称存在更早文件哈希。没有依据该前缀改选 task 排序、阈值、保护范围、bypass 或 PLRU tie。B5 是随后的必要覆盖审查，单列且不改 B0–B4。

本文件给出固定可执行伪程序和单缺 key 的代数构造；没有运行实验、枚举模型、test/build、旧 validator/trace 或 FPGA。完整生产可达性、时序与性能仍需 root 独立验收；本席不把纸面动作差异当成实际收益。
