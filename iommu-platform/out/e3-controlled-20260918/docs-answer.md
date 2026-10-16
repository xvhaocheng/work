# docs/e3-queue-results-review.md

> 说明：本文只整理 Codex 已确认的结论与给定事实，不做新分析。所有数字均直接取自 `out/e3-controlled-20260918/v5-reviewed-facts.json`，或为 Codex 明确给出的中位数；派生列已标注。v6 受控方案尚未实施，状态见 [E3_CONTROLLED](../E3_CONTROLLED.md)。

```markdown
# E3 v5 队列配对：上板结果复核

2026-09-18。**执行与覆盖目标达成；因果目标未达成。** 本组是 [E3_QUEUE_PAIR](../E3_QUEUE_PAIR.md) 的 v5 有限配对矩阵，3 短 + 10 正式 = 13 对、26 子轮。独立服务器脚本复核通过，但每对两轮落在**不同的前史/身份**上，因此不能把“第二轮减第一轮”当成延迟 D 的作用。其中只有第 5 对两轮身份一致，属探索性证据，一对样本不足以证明 HOL。

## 组会 3–5 句解释

1. 这轮 v5 上板跑了 3 短 + 10 正式共 13 对 26 子轮，独立复核 9472 项检查全部通过、0 失败，494 条唯一观测逐条对齐，原始哈希前后不变。
2. 执行与覆盖目标达到了：A8 十六个子轮**全部触发**，A1 四个子轮**全部不触发**；而且 16 次里有 9 次是“旧队头是 A 读”，正是旧触发条件看不到的情形，说明补的覆盖确实生效了。
3. 但因果目标没达到：每一对的两轮其实落在两种不同状态里——目标 A 的序号、PTE、是否截断、旧队头都不一样，D0/D0 两对（第 1、6 对）在 D 都为 0 时就已经出现同样的跳变。
4. 唯一两轮身份一致的第 5 对里，A 剩余服务正好 +128、V 剩余等待 +134、V 自身服务 −2，方向上像有一点传播；但 V 仍然首次 miss，预热 A 状态也不同（512→312），一对样本不能证明“纯 HOL”。
5. 下一步不扩旧矩阵，改做独立受控的有限实验 [E3_CONTROLLED](../E3_CONTROLLED.md)，先把“两轮身份一致”做出来，再谈因果。

## 数据与完整性

- 独立复核脚本：`server-review-v5.py`，**PASS，9472 项检查，0 失败**。
- 规模：**26 个子轮、13 对（3 短 + 10 正式）**；**494 条 GLOBAL 唯一 UART 观测行**，每条 sequence 19 行（3 条 batch + 16 条 path）。
- dmesg 是累计输出，跨阶段重复打印的观测副本必须完全一致；**重复行不计为新样本**。
- 原始输入文件前后 SHA256 不变（`short-records.json`、`main-records.json`、`short-summary.json`、`main-summary.json`、`short-assessment.json`、`main-assessment.json`、`host-validation.json`、`run.json`、`console.raw.log`）。

**路径**

- 复核报告（repo 相对）：[`out/mac-e3-queue-20260918-130545-run-d01lc9wp/server-review-v5.json`](../out/mac-e3-queue-20260918-130545-run-d01lc9wp/server-review-v5.json)
- 复核所用事实行（repo 相对）：[`out/e3-controlled-20260918/v5-reviewed-facts.json`](../out/e3-controlled-20260918/v5-reviewed-facts.json)
- 运行目录中的 `run.json` 状态 `completed_local_only` 是**上传前快照**；Dell 侧实际收到文件后**不修改**原始状态字段。

## 执行与覆盖结果

| 项目 | 结果 |
|---|---|
| 正式 A8 触发 | **16 / 16 子轮触发** |
| 正式 A1 不触发 | **4 / 4 子轮不触发** |
| 目标 V 首次查询 | **16 / 16 全部 miss** |
| 旧物理 AR 队头不是 V（`trigger_head_is_v = 0`） | **9 / 16** |
| 插入记录截断（实际 5 条、只存下 2 条） | **9 / 16** |
| 目标 A 自身方向 | **16 / 16 全部为写（`write = 1`）** |

两点必须写清楚，不能写错：

- **9/16 的旧队头是 A 读**（head `raw` 解码为 valid=1、DID=11、ID=4），这正是 v5 新触发条件要覆盖的场景；旧条件（只看 AR 队头是否为 V）在这些子轮会漏触发。
- 但**被选中的目标 A 本身在 16 个子轮里全部是写请求**（`A_record.write = 1`），不是读。不要把“旧队头是 A 读”误写成“A 打了一枪读”。

## 为什么还不能给出因果结论

### 两轮不是同一种状态

正式 A8 八对（16 子轮）里，**第 1 轮**几乎统一是这一组特征：旧队头不是 V、插入被截断（5 存 2）、目标 A `seq = 13`、PTE `= 2195337208`；**第 2 轮**（除第 5 对外）统一是另一组：旧队头是 V、插入不截断（1 存 1）、目标 A `seq` 在 53～71、PTE `= 2193147896`。第 5 对第 2 轮例外，它仍然落在“第一轮那种”特征里。

### D0/D0 两对本身就变了（D 都为 0）

| 对次 | 轮次减轮次 | V 剩余 | A 剩余 | V 自身服务 | 目标 A seq | 目标 A PTE |
|---|---|---:|---:|---:|---|---|
| 第 1 对（D0/D0） | 第 1 → 第 2 | 995 → 280 | 136 → 271 | 994 → 407 | 13 → 69 | 2195337208 → 2193147896 |
| 第 6 对（D0/D0） | 第 1 → 第 2 | 979 → 280 | 136 → 271 | 984 → 407 | 13 → 71 | 2195337208 → 2193147896 |

两轮 D 都是 0，却出现同一量级的跳变，且前缀（目标 A 序号、PTE、旧队头、截断）不同。**这说明顺序/内部状态在没有任何 D 干预时就已经不同**，因此把同对两轮相减不能归因于 D。

### 第 5 对：唯一两轮身份一致的探索性样本

第 5 对两轮的 A `seq = 13`、PTE `= 2195337208`、前缀（旧队头不是 V、插入 5 存 2 截断、中间请求、`v_pre_wait`、`a_pre_service`）都相同，是本组唯一可以做 D128−D0 探索性比较的对：

| 读数 | 第 1 轮（D0） | 第 2 轮（D128） | 差值 |
|---|---:|---:|---:|
| 触发后 A 剩余服务 | 136 | 264 | **+128** |
| 触发后 V 剩余等待 | 980 | 1114 | **+134** |
| V 自身翻译服务 | 989 | 987 | **−2** |

**保留的细微差别（不能省略）：**

- 这**方向上与“部分传播”一致**，但 V 仍然是首次 miss，不是命中。
- 中间请求为 5 条、只存下 2 条（截断），无法核对完整身份。
- 预热 A miss 两轮不同（512 → 312），**两轮并不真正可比**。
- **一对样本不足以证明纯 HOL**。

### 其他顺序混合的对：算不出干净的 D 因果

其余各对的两轮身份/前史不同（见上表特征分组），**不能算出干净的 D 因果**。本组只报告已观测字段，不做补采、不做配对凑数。

### 软件启动间隔：只是软件区间

各正式 A8 子轮的 `V.launch_begin_ns − A.launch_end_ns`（由交付字段直接相减）大致为：第 1 轮约 **92 µs**，第 2 轮 **237～319 µs**；**第 5 对第 2 轮例外，约 92 µs**，与它落在“第一轮那种”状态一致。必须说明：

- 这是**软件调用之间的区间，不是精确的硬件发射差**。
- **仅凭相关性不能在 CPU 层面给出因果证明**，也不能指认某个中断。

### 吞吐不是机制证据

Codex 给出的两组中位数：

| 场景 | 吞吐中位数 |
|---|---:|
| A8 | 109.29 |
| A1 | 149.51 |

单位与口径沿用 Codex 说明；**这不是机制证据**。

### 前史字段的读法

`e3_words[78..85]` 是 **active 窗口内**的首次 hit/miss，`e3_words[86..93]` 是 **ARM 之后**累计的 TR/COMP 历史。**不能由“窗口内进入”的计数反推窗口打开前已完成的请求**；也不能只靠 opening carry 还原窗口前已完成的请求。

## 表 1：正式 A8 八对（16 子轮）关键字段

`插入` 列为 `insert_total / inserted / truncated`；`旧队头是 V` 列为 `trigger_head_is_v`。

| 对次 | seq | 本轮 D | 触发 | 旧队头是 V | 插入 | 目标 A seq | 目标 A PTE | 触发后 A 剩余 | 触发后 V 剩余 | V 自身服务 | 中间请求 |
|---:|---:|---:|---:|---:|---|---:|---:|---:|---:|---:|---:|
| 1 | 7 | 0 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 136 | 995 | 994 | 5 |
| 1 | 8 | 0 | 1 | 1 | 1 / 1 / 否 | 69 | 2193147896 | 271 | 280 | 407 | 1 |
| 2 | 9 | 0 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 136 | 979 | 984 | 5 |
| 2 | 10 | 128 | 1 | 1 | 1 / 1 / 否 | 67 | 2193147896 | 401 | 410 | 406 | 1 |
| 3 | 11 | 128 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 264 | 1114 | 987 | 5 |
| 3 | 12 | 0 | 1 | 1 | 1 / 1 / 否 | 55 | 2193147896 | 271 | 280 | 407 | 1 |
| 5 | 15 | 0 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 136 | 980 | 989 | 5 |
| 5 | 16 | 128 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 264 | 1114 | 987 | 5 |
| 6 | 17 | 0 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 136 | 979 | 984 | 5 |
| 6 | 18 | 0 | 1 | 1 | 1 / 1 / 否 | 71 | 2193147896 | 271 | 280 | 407 | 1 |
| 7 | 19 | 128 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 264 | 1114 | 987 | 5 |
| 7 | 20 | 0 | 1 | 1 | 1 / 1 / 否 | 53 | 2193147896 | 276 | 285 | 406 | 1 |
| 8 | 21 | 0 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 136 | 982 | 981 | 5 |
| 8 | 22 | 128 | 1 | 1 | 1 / 1 / 否 | 53 | 2193147896 | 401 | 410 | 406 | 1 |
| 10 | 25 | 128 | 1 | 0 | 5 / 2 / 是 | 13 | 2195337208 | 264 | 1110 | 998 | 5 |
| 10 | 26 | 0 | 1 | 1 | 1 / 1 / 否 | 71 | 2193147896 | 271 | 280 | 407 | 1 |

A1 阴性对照：第 4 对（seq 13 / 14）与第 9 对（seq 23 / 24）**无触发**，`A_record`、`V_record`、`target` 均为 `null`。

## 表 2：硬件时间、预热 miss 与软件间隔

`硬件周期` 直接来自 JSON；`≈ms` 为派生列（按 50 MHz，周期 ÷ 50000）。`预热 A miss`、`预热 V miss` 取各轮 `warm_lane` HPM 的 `iotlb_miss`（lane 顺序 V、A）。`软件 A→V 间隔` 为派生列（`V.launch_begin_ns − A.launch_end_ns`）。

| 对次 | seq | 硬件周期 | ≈ms | 预热 V miss | 预热 A miss | 软件 A→V 间隔 |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 7 | 7326451 | 146.53 | 2 | 512 | 92.04 µs |
| 1 | 8 | 7262430 | 145.25 | 2 | 318 | 318.68 µs |
| 2 | 9 | 7327197 | 146.54 | 2 | 512 | 92.44 µs |
| 2 | 10 | 7266816 | 145.34 | 2 | 316 | 315.56 µs |
| 3 | 11 | 7331737 | 146.63 | 2 | 512 | 92.04 µs |
| 3 | 12 | 7264867 | 145.30 | 2 | 314 | 239.12 µs |
| 4 | 13 | 5365244 | 107.30 | 2 | 2 | （未取） |
| 4 | 14 | 5348232 | 106.96 | 0 | 0 | （未取） |
| 5 | 15 | 7327501 | 146.55 | 2 | 512 | 92.36 µs |
| 5 | 16 | 7324625 | 146.49 | 2 | 312 | 91.88 µs |
| 6 | 17 | 7319345 | 146.39 | 2 | 512 | 92.08 µs |
| 6 | 18 | 7267359 | 145.35 | 2 | 318 | 318.68 µs |
| 7 | 19 | 7327259 | 146.55 | 2 | 512 | 92.36 µs |
| 7 | 20 | 7271204 | 145.42 | 2 | 314 | 238.04 µs |
| 8 | 21 | 7321221 | 146.42 | 2 | 512 | 92.00 µs |
| 8 | 22 | 7269282 | 145.39 | 2 | 314 | 237.40 µs |
| 9 | 23 | 5353545 | 107.07 | 2 | 2 | （未取） |
| 9 | 24 | 5346760 | 106.94 | 0 | 0 | （未取） |
| 10 | 25 | 7325317 | 146.51 | 2 | 512 | 91.92 µs |
| 10 | 26 | 7266494 | 145.33 | 2 | 318 | 318.48 µs |

说明：A8 正式轮的 `warm_lane` 两轮均为 `submitted 256 / completed 256`，A lane 为 `submitted 256 / completed 255 / drained 1 / in_flight 1`、`warm_last_offset = 28672`；A1 两对的 A lane `warm_last_offset = 0`。预热提交数量与末页相同，**不代表 IOTLB、替换状态或请求到达顺序相同**。

## 表 3：短验收三对

| 短 | 场景 | seq | 触发 | 硬件周期 | 说明 |
|---:|---|---:|---:|---:|---|
| 1 | Solo（无 A），warmup 0 | 1 / 2 | 0 / 0 | 6842 / 5485 | 无 A、无触发 |
| 2 | A 1 页，warmup 32 | 3 / 4 | 0 / 0 | 83112 / 83484 | 无触发 |
| 3 | A8，warmup 0，first-delay 128 | 5 / 6 | 1 / 1 | 114478 / 113467 | seq5：D128、旧队头非 V、插入 5/2 截断、A seq13／PTE 2195337208、A 剩余 266／V 剩余 1115／V 自身 986；seq6：D0、旧队头 V、插入 1/1、A seq107／PTE 2193147896、A 剩余 271／V 剩余 280／V 自身 406 |

## 本组没有验证的事情（边界）

- 本轮**没有实测确认加载产物的哈希**，也**跳过 SD 回读**；记录中的期望哈希不能替代实测身份。
- 本轮**没有额外上板运行**；上面所有数字只来自这一次 26 子轮。
- 既有时钟约束、CDC、外部 IO 审查边界保留，**没有新增时序签核**。
- 不证明每个重复 DMA 描述符都完成了独立的数据正确性验证，**不证明 HOL**。

## 下一步

不扩旧矩阵。按独立受控的有限 E3 v6 方案重做，先保证“同一对两轮身份一致”，再做 D128−D0 比较；方案、寄存器、CLI、矩阵与判定顺序见 [E3_CONTROLLED](../E3_CONTROLLED.md)。在拿到新板结果之前，不决定仲裁、队列分区或缓解硬件，也不创建超出该有限计划的新实现范围。
```

---

# E3_CONTROLLED.md

```markdown
# E3_CONTROLLED：独立受控有限 E3 实验（v6）

**状态：实现评审中，尚未构建、尚未上板。** 本文只是把 Codex 已确定的接口、寄存器、CLI、矩阵与判定顺序写成文档，**不包含任何实测数据、构建结果或通过结论**。

## 组会 3–5 句解释

1. 上一轮 v5 是“整批轮流跑”，两轮之间的前史/身份对不上，做不出干净的因果比较。
2. v6 改成**独立受控**：每对只发一次 64 字节的 V 副本和一次 64 字节的 A 副本，各自只做一次读和一次写，用实际计数器核对“确实只发过一次”。
3. 由 launcher 在硬件里把“先放 A、A 读被选中后再放 V”做成受控时序，CPU 只负责准备、ARM 一次、GO 一次、轮询完成。
4. 每对固定同一份页表映射、只跑两轮，一轮 D0 一轮 D128（或两轮同 D 做基线），先看两轮身份是否一致，再看 D128−D0。
5. 这是**有限诊断**，不是吞吐基准；在拿到新板结果之前，不决定仲裁、队列分区或缓解硬件。

## 1. 用途与范围

- 目的：**机制识别**。判断“同一类 A 页表服务被延长后，已经排队的 V 是否增加相应等待，而 V 自身服务不变”。
- 不是 E2 性能实验，不是论文的完整贡献，也不是缓解方法评估。
- 是**有限实验诊断**，不是吞吐基准；**不与旧 A8 性能混算**。
- 在拿到下一轮板上结果之前，**不决定仲裁、队列分区或缓解硬件**；**不创建超出本有限计划的实现范围**。

## 2. 固定条件

| 项目 | 固定值 |
|---|---|
| 批供给 | DS 50/50 |
| 时钟 | 50 MHz |
| DMA 通道 | 4 |
| IOTLB | 16 |
| CPU | 整批任务轮询（**不是中断**） |
| V | DMA0 / DID10 / ID0 |
| A（可选） | DMA1 / DID11 / ID4 |
| V 副本 | 1 份 64 B 对齐的 DEFAULT 拷贝（一源页一目标页一设备） |
| A 副本 | 1 份 64 B 对齐的 DEFAULT 拷贝（一源页一目标页一设备） |
| 每设备每方向 | **各一次 AR、一次 AW**，由实际计数器核对 |
| 续写 | **无 endless A、无 CPU refill** |

## 3. 每对与每轮的执行流程

- **每对只用一个不变的映射生命周期，跑两轮。**
- 每轮检查顺序：

  1. GLOBAL host S1 IOTLB 处于 validation；
  2. hot A1 prime **或** cold A（不 prime）；
  3. V1 prime；
  4. V1 probe；
  5. formal。

- 每个 prep 任务必须**实际完成**；cache handoff 需核验；**下一个任务前要反向 reseed**；offset 恒为 0。
- probe 要求 V miss、S1＝0、S2＝0）；首个 V prime 的**实际 miss 保留为正向证据**。
- cache workaround 的 `FENCE.I` 是**平台相关的 CPU handoff** 手段，**不是 IOTLB flush**。
- 失效操作**不会重置** DDT / PDT / PLRU / 页表数据 cache / 全部内部状态。
- cold A / hot A 在 formal 里的**实际 hit/miss 仍要记录**；**不能宣称所有状态相等**。

## 4. 启动时序（launcher 行为）

- launcher 在 backend `VALID` 之前**已经持有完全准备好的 job**。
- CPU **只做一次**：保留 ID、ARM obs、GO。
- A 读被 IOMMU 实际选中（ID4 / DID11）后，**在下一个已寄存周期**发出 V go。
- A 事件到 V 释放之间**不做轮询**；CPU 仍然轮询完成，且**不重新提交**。
- **不允许撤销 AXI `VALID`。**
- 超时取消**只取消 HOLD**，不产生“实际已发出”的一次或计数；offered/issued 的事务走排空或隔离，**内存保留**。
- 任务保留 ID 用普通 `NEXT_ID`（任意），与 AXI ID 分开（V＝0、A＝4）。

## 5. 观测窗口定义

| 窗口 | 起 | 止 |
|---|---|---|
| observer / path / E3 打开 | V backend **实际发出** | V done |
| HPM | GO | 两侧均排空 |
| launcher | PREP | 两侧均排空 |

- 迟到的 A 返回**可能落在 V 窗口之外**；缺失标志**不等于**零耗时。
- A 的查询**可能发生在 V 窗口之前**，所以“窗口内 active A lookup = 0”**不等于没有请求**。

## 6. MMIO 映射（launcher，DMA0 配置窗口）

只有 DMA0 的 `0xb0..0xff` 被路由进 launcher 窗口；`obs_cfg_addr` 由 7 位加宽到 **8 位**；旧 observer 地址 `0x58..0x78` **不受影响**。其余被转发的地址**读返回 0**，**写置粘滞 bad-command**。**只接受完整 strobe**。

| 字节地址 | 名称 | 读 | 写 |
|---|---|---|---|
| `0xb0` | magic | `0x49444d414c4e4301` | — |
| `0xb8` | command / status | 状态字（见 6.2） | 命令字（见 6.1） |
| `0xc0` | index | 当前索引 | 设置索引 |
| `0xc8` | data | 读索引字 | 写索引字 |

### 6.1 命令字（写 `0xb8`）

| 命令 | 值 | 说明 |
|---|---|---|
| `PREP` | `1 \| (hasA ? 0x100 : 0)` | 只在 OFF 或“已完成冻结”且**所有端口**无 active/held 时接受 |
| `GO` | `2` | 只在 PREP 之后、两路都已 held、errors＝0 时接受 |
| `ABORT` | `4` | 任何 active 阶段都必须被接受 |
| `CLEAR` | `8` | 清除记录/错误 |

### 6.2 状态字（读 `0xb8`）

| 位 | 含义 |
|---|---|
| bit0 | `PREP` |
| bit1 | `RUNNING`（含 abort 排空阶段） |
| bit2 | `FROZEN`（done） |
| bit3 | `aborted` |
| bits 8..11 | 当前 held mask |
| bits 12..15 | 当前 active mask |
| bits 63..32 | error bits（含义见 RTL 注释） |

- errors **粘滞**，直到成功的 `PREP` 或 `CLEAR` 且不再 active 才清除。
- 硬件 **64 位 tick 计数器饱和即置故障**；ticks 从 PREP 持续拥有到 drain 结束，然后冻结。
- CPU 可以轮询 `0xb8`。

### 6.3 索引记录（`0xc0` / `0xc8`，32 字）

| 索引 | 名称 | 索引 | 名称 |
|---:|---|---:|---|
| 0 | magic | 16 | donemask |
| 1 | errors | 17 | cancelmask |
| 2 | status snapshot（冻结后读；live 状态在冻结时也可） | 18 | heldseenmask |
| 3 | ticks | 19 | expectedmask |
| 4 | goA_timestamp | 20 | selectedID |
| 5 | Aselect_timestamp | 21 | selectedDID |
| 6 | goV_timestamp | 22 | selectedIOVA |
| 7 | Vissue_timestamp | 23 | selectedwrite |
| 8 | Aissue_timestamp | 24 | Vissuecount |
| 9 | Vdone_timestamp | 25 | Aissuecount |
| 10 | Adone_timestamp | 26 | Vdonecount |
| 11 | VissueID | 27 | Adonecount |
| 12 | AissueID | 28 | hasA |
| 13 | VdoneID | 29 | currentheld |
| 14 | AdoneID | 30 | currentactive |
| 15 | issuemask | 31 | reserved（恒 0） |

时间戳表示“观测到边沿/发出命令”的精确时刻；`goV` 的寄存器输出是**下一个周期**的事件，文档与注释里要写清楚。

### 6.4 记录与错误规则

- 逐 lane 记录 issue/done 计数（每路唯一、期望为 1）、ID、时间戳、cancelled mask、seen-held mask。
- 拥有期间出现**任何非预期、非选中端口的 issue/done** → 置 error。
- **done 而没有 issue**，或 **ID 不匹配** → 置 error。
- ABORT 竞争下**可能捕获到真实 issue，必须排空它**；真实 issued 与 cancelled **互斥**；取消只对“已保留并处于 held”的 job 生效。
- 成功收尾条件：`issued = done = expectedmask`，且无 held/active 残留。
- aborted 收尾条件：无 held/active 残留，且每个真实 issued 都已完成，对 held 的已准备 job 记录 cancelled。
- 必须处理 **任何保留之前就 ABORT**（此时“aborted done”合法）与**部分保留**两种情况。
- **只在排空之后冻结**；**不要因为 ABORT 就提前冻结**。

## 7. RTL 改动清单（限定文件）

| 文件 | 改动 |
|---|---|
| `cva6/corev_apu/fpga/src/dma_controlled_launcher.sv` | **新增** |
| `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` | 加宽 `obs_cfg_addr` 到 8 位；按 `addr[7]` 拆分 legacy/launch 的 valid 与 rdata；接入新 launcher |
| `cva6/corev_apu/fpga/src/dma_observer.sv` | 新增可选 `ControlledEnable`（默认 0）与有限标记 |

细节：

- launcher：`NumPorts`（默认 4）；标量 select 输入 `selected_i`、`select_write_i`、`[7:0] select_id_i`、`[23:0] select_did_i`、`[63:0] select_iova_i`；MMIO cfg 的 valid/write/wdata/wstrb/rdata 语义与 observer 相同；输出 frontend 的 held/active/cancelled/error 向量、obs issue/done 及 ID 数组，以及 hold/go/cancel 向量。**不做 AXI gating。**
- 固定 V port0 = DID10 / ID0；可选 A port1 = DID11 / ID4；**其他端口永不被 held/go**。
- top 里：`LAUNCH_PORT` 仅对 `InclIOMMU && i < 2` 的 DMA0/DMA1 为真，其余为假；`ControlledEnable(InclIOMMU)`；关闭时 hold/go/cancel 接 0、launch rdata 接 0。
- observer：`ControlledEnable && E3Enable` 时外层 magic 为 `0x49444d414f425306`，否则**保持旧 magic 完全不变**；ARM bit29 新有限标志**仅在** `ControlledEnable && E3 && path && batch && target1` 且固定角色满足既有约束时置位；仅在该开关使能时扩展 valid mask bit29；latch `finite_q`，支持 reset/clear；**唯一**改变的不变量是 batch A active 检查在 `finite_q` 时跳过。既有 V 窗口、旧计数器语义、E3 bank v2 **完全不变**。

## 8. UAPI / ioctl / magic

| 项目 | 值 |
|---|---|
| 能力位 | `IDMA_EXPERIMENT_CAP_E3_CONTROLLED` = `(1U << 6)` |
| ioctl | `IDMA_IOC_RUN_E3_CONTROLLED` = `_IOWR('i', 0x0c, struct idma_e3_controlled)` |
| launcher 记录字数 | `IDMA_E3_CONTROLLED_WORDS` = `32` |
| observer 外层 magic（受控） | `0x49444d414f425306` |
| E3 bank v2（不变） | `0x49444d4145330002` |
| launcher magic | `0x49444d414c4e4301` |
| schema | `idma.e3_controlled.v1` |

驱动结构体以**已提供的 patch** 为准（`linux/include/uapi/linux/idma.h`：新增 `struct idma_e3_controlled_round`、`struct idma_e3_controlled`、宏 `IDMA_E3_CONTROLLED_WORDS 32U` 与 ioctl `0x0c`）。当前唯一实际新增的宏是 `IDMA_E3_CONTROLLED_WORDS = 32`；**不修改**旧 UAPI、驱动、旧 CLI。

## 9. CLI

```
idma_e3_controlled --aggressor none|1 --prep cold|hot --pair complementary|zero \
                   --first-delay 0|128 --timeout-ms 1..60000
```

| 选项 | 取值 | 默认 |
|---|---|---|
| `--aggressor` | `none` \| `1` | `1` |
| `--prep` | `cold` \| `hot` | `cold` |
| `--pair` | `complementary` \| `zero` | `complementary` |
| `--first-delay` | `0` \| `128` | `0` |
| `--timeout-ms` | `1..60000` | `5000` |

固定项：`/dev/idma0`、V/A 各 1 页、**64 B**、`transfers 1`、`warmup 0`，ABI/size 字段固定。

输入校验与失败行为：

- 拒绝未知/重复/缺失/负值/溢出、数字前后空白、尾随垃圾。
- `--pair zero` 只允许 `--first-delay 0`；`--aggressor none`（Solo）只允许 `--prep cold`。
- `--help` 单独使用有效。
- ioctl **在任何情况下都不重试**（`EINTR` 与普通失败都不重试）。
- 参数/open/能力/运行失败时：`errno` → exit 1，**仍然输出完整 JSON**；除 `/dev` 外不做文件 IO，输出走 stdout/stderr。

输出 JSON（`idma.e3_controlled.v1`）字段：输入固定字段（`prep_mode` / `pair_mode`，同时给出人类可读串和数值）、完整 raw pair 映射 8、2 轮 `rounds[]`（`sequence`、`delay`、`status`、测量标志、HPM、`stats`、`rawE3[256]`、`controlled{ flush_status, prep_complete, stage, prime[2], probe, launcher[32], observer_magic, v_hw_cycles }`）。**任何失败都要保留全部字段。**

成功判定：

- syscall 返回 0、pair status 0、两轮 completed、每轮 status 0；
- HPM / `prep_complete` 有效、flush＝0、stage＝8、observer magic 为 v6、bank v2 / error＝0、D 值符合预期、`v_hw_cycles` 为正；
- 每 lane 实际 `1 submitted / 1 completed`、64 B、无错误、无 quarantine、S2＝0；
- probe 为 1 份 64 B 拷贝、miss、S1＝0、S2＝0。

校验（与成功判定并行，独立读数）：launcher magic 正确 / error 0 / `FROZEN` bit2 / 非 aborted / 非 active / expected mask 为 1 或 3 / `issue == done` / cancel 0 / 计数与 ID 非零且相等；hot A prime＝1、cold A prime＝0、V prime＝1。

**明确不作要求**：不要求触发、不要求 V hit、不要求正好 128 周期。**无覆盖按“已执行数据”保留**，不自动丢弃、不自动重试。成功但元数据无效时置 `EPROTO`/`EIO`。**不给出 64 B 诊断的误导性吞吐。** HPM 口径必须写明：**GO 到两侧均排空**、observer 为 **V 窗口**。

### 输出位置

- 板端：**stdout → UART**（每 case 的 CLI stdout JSON 直接 `cat` 到串口）。
- 主机：**output dir**（用户指定路径，或 `/tmp` 下带唯一 PID 的目录；目录必须新建，**已存在即失败**，不覆盖）。目录内含每 case 的 CLI stdout JSON、stderr、dmesg/taint、阶段标记与（若简单）输入参数/矩阵元数据。

### 字段骨架（**不是实测数据，无任何真实数字**）

```json
{
  "schema": "idma.e3_controlled.v1",
  "prep_mode": "<cold|hot>",
  "pair_mode": "<complementary|zero>",
  "rounds": [
    {
      "sequence": "<整数>",
      "delay": "<0|128>",
      "status": "<整数>",
      "controlled": {
        "flush_status": "<整数>",
        "prep_complete": "<整数>",
        "stage": "<整数>",
        "prime": ["<整数>", "<整数>"],
        "probe": "<对象>",
        "launcher": ["<32 个整数>"],
        "observer_magic": "<整数>",
        "v_hw_cycles": "<整数>"
      },
      "rawE3": ["<256 个整数>"]
    }
  ]
}
```

> 上面只有字段名和占位符，**没有任何真实测量值**；不要当作示例结果引用。

## 10. 有限矩阵（固定，不自动扩）

**短验收（3 对，每对 2 轮）**

| 顺序 | aggressor | prep | pair | first-delay |
|---:|---|---|---|---:|
| 1 | none | cold | complementary | 0 |
| 2 | 1 | hot | complementary | 0 |
| 3 | 1 | cold | zero | 0 |

**正式（8 对，每对 2 轮）**

| 顺序 | aggressor | prep | pair | first-delay |
|---:|---|---|---|---:|
| 1 | 1 | cold | zero | 0 |
| 2 | 1 | cold | complementary | 0 |
| 3 | 1 | hot | complementary | 0 |
| 4 | 1 | cold | complementary | 128 |
| 5 | 1 | cold | zero | 0 |
| 6 | 1 | cold | complementary | 0 |
| 7 | 1 | hot | complementary | 128 |
| 8 | 1 | cold | complementary | 128 |

- 合计 **11 对、22 子轮**；每对**恰好 2 轮**。
- 若每轮仍为 19 条观测，则共 **418 条唯一观测**——**这是尚未测量的预期数，不得写成已得结果**。
- **实际执行出错立即停止**，保留 JSON 与 UART；**不自动重试、不扩矩阵**、不调页数/chunk/数量旋钮。
- `main` 只有在 `short` 执行通过后才跑；默认板卡 runner `platform-e3-controlled all OUTPUT_DIR` 会**自动先跑短验收门**。

板卡 runner（POSIX `sh` / BusyBox）：

- `platform-e3-controlled short|main OUTPUT_DIR`；目录必须新建，**已存在即失败**。
- 在 phase/case/exit/taint 前后打印标记；每 case 的 CLI stdout JSON 存文件并 `cat` 到 UART，stderr 存文件并 `cat` 到 stderr。
- CLI 返回非零**立即停止**，但通过 `trap` **始终保存并 `cat` dmesg 与 taint 终态**（保留原始退出码）；只有成功才打印 completion。
- **禁止**烧写 SD / 联网 / 系统升级；**禁止** ioctl 重试；**禁止**隐藏失败。
- 不清空 dmesg；日志为累计，跨阶段重复行按 `seq` 去重。

## 11. 因果判定（严格按顺序）

1. **执行有效**：本轮所有执行/字段/守恒检查通过。
2. **触发与身份**：cold 触发 A 读，`seq = 1`、DID11、ID4；V 目标 DID10、ID0、首次命中为 1；目标同一 IOVA、同一 PTE 层级；跨轮同前缀；中间请求身份完整（≤2 且未被截断）；队列/历史闭合。
3. **D0/D0 基线**：先看两轮同为 D0 时是否已经不同。
4. **D128−D0 比较**：只比较**条件合格**的配对与阴性对照。

规则：

- **不发明数值容差，不设任意显著性门槛。**
- 读数定义（`e3_words` 相对索引）：
  - A 剩余服务：`41 − 4`
  - V 剩余等待：`70 − 4`
  - V 完整排队：`70 − 69`
  - V 自身翻译服务：`73 − 71`
- 需要 A/V 的必需阶段位（bits）；**缺失 A 返回的轮次排除**。
- 必须报告**基线抖动**与**有限 N**；**不得仅因差值等于 128 就宣称证明**。
- 若 cold 无覆盖、V 首次 miss、身份不匹配：**保留并在固定矩阵跑完后停止，等 Codex 复核**。

## 12. 本轮不做的事

- 不做吞吐基准，不与旧 A8 性能混算。
- 不扩展启动方式、页数、chunk 或数量旋钮。
- 不在原因未明时实现仲裁/队列分区/缓解硬件。
- 不越过 [E3_QUEUE_PAIR](../E3_QUEUE_PAIR.md)、[E3 队列配对结果复核](e3-queue-results-review.md)、[E3 原协议](../E3_SERVICE.md)、[软件诊断字段](../E3_DIAGNOSTIC.md) 已定义的口径。
- **构建、上板与验证由 Codex 后续实际执行并填写；本文不含任何通过/失败/哈希/版本/commit 结论。**

## 13. 测试与验证范围（由实现任务产出，尚未运行验证）

- `tests/dma_controlled_launcher_test.cpp`
- `tests/test_dma_controlled_launcher.py`（Verilator 独立 launcher，必要时加 `tests/dma_controlled_launcher_top.sv` 平坦化标量端口/向量/ID）
- 焦点 observer finite v6 测试（复用真实 observer/path）
- CLI 主机测试 `tests/test_e3_controlled_cli.py` + 桩 `tests/e3_controlled_cli_stub.c`
- 板卡 runner `tests/board-e3-controlled.sh`

覆盖点（设计时列出，执行结果未验证）：Solo/nonSolo、部分准备后 abort、背压下释放后取消、恰好一次 go A/V、V 在 A 写/错 DID/错 ID 时永不 go、A 读被选中后 V go 恰在下一寄存周期、重复命令报错、不支持的写、计数/ID 一致性、CPU 读之前已完成、迟到的 A done 出现在 V 之后、abort/select 竞争。
```

---

# 建议插入：docs/next-mechanism-plan.md（不重写原文件）

建议插入到该文件**“紧接着执行什么”一节之前**（约第 76 行前）作为独立段落：

```markdown
**2026-09-18 更新（v5 队列配对复核对齐）：** [E3 队列配对结果复核](e3-queue-results-review.md) 确认 v5 上板**执行与覆盖目标达成**（A8 16/16 触发、A1 4/4 不触发；16 次里 9 次旧队头是 A 读，正是旧条件漏掉的场景；独立脚本 9472 项检查 0 失败，494 条唯一观测对齐，原始哈希前后不变），但**因果目标未达成**：每对两轮落入不同前史/身份，D0/D0 两对在 D 都为 0 时已出现同量级跳变，因此不能把轮次差当 D 的作用；唯一两轮身份一致的第 5 对是探索性样本（A 剩余 +128、V 剩余 +134、V 自身服务 −2，但 V 仍首次 miss、预热 A 状态不同），**一对不足以证明纯 HOL**。下一步不扩旧矩阵，改做独立受控有限实验 [E3_CONTROLLED](../E3_CONTROLLED.md)（新 BIT 与 payload 必须同时更新；3 短 + 8 正式；V/A 各一份 64 B 单副本、由实际计数器核对各一次 AR/AW；CPU 整批轮询而非中断）。在拿到新板结果之前**不决定仲裁、队列分区或缓解硬件**，也不创建超出该有限计划的新实现范围。
```

---

# 建议插入：docs/validation.md（不重写原文件）

> 未能读取 `docs/validation.md` 现有内容，以下为**待插入片段**；请按该文件既有排版风格（标题层级、状态表格式）落位，或作为新子节追加。

```markdown
## E3 受控有限实验（E3_CONTROLLED）——待实施

**状态：实现评审中；尚未构建、尚未上板；本节结论待 Codex 用实机证据填写。**

- 设计依据：[E3_CONTROLLED](../E3_CONTROLLED.md)。
- 上一轮 v5 队列配对结果：[E3 队列配对结果复核](e3-queue-results-review.md)（执行/覆盖目标达成，因果目标未达成）。
- 计划规模：3 短 + 8 正式 = 11 对、22 子轮；schema `idma.e3_controlled.v1`；UAPI ioctl `0x0c`、能力位 bit6；受控外层 magic `0x49444d414f425306`，E3 bank v2 `0x49444d4145330002`。
- 计划中的测试（launcher Verilator、CLI 桩/测试、observer finite 焦点测试、板卡 runner）由实现任务产出；**执行与结果尚未验证**。
- 上板前不得填写通过/失败、实测哈希、版本或 commit；本轮**下载产物哈希与 SD 回读均未验证**，记录中的期望哈希不能替代实测身份。
- 上板失败时保留原始 JSON 与 UART，不自动重试、不扩矩阵。
```

---

## 假设、未决项与验收建议（给 Codex）

**假设**

1. `docs/` 与 `out/` 均在仓库根下，故文中链接用 `../out/...`（与 `docs/e3-diag-results-review.md` 现有写法一致）。
2. facts JSON 中 `warm_lane[0]` 为 V lane、`warm_lane[1]` 为 A lane（依据 A8 轮 `warm_last_offset = 28672` = 第 7 页；A1 轮为 0）。若实际相反，表 2 的“预热 V/A miss”两列需互换。
3. `launch_intervals[0]` 对应 V、`[1]` 对应 A（与 `warm_lane` 顺序一致）；表 2 的“软件 A→V 间隔”由此推出。
4. `hardware_cycles ÷ 50000 = ms`（50 MHz），沿用 [E3_QUEUE_PAIR](../E3_QUEUE_PAIR.md) 第 19 行既有约定，故列为派生列。
5. `E3_CONTROLLED.md` 的 `0xb0/0xb8/0xc0/0xc8` 视为 DMA0 配置窗口内的字节偏移（与 `0x58..0x78` 旧 observer 偏移同段）。
6. CLI 选项 `--aggressor none` 即“Solo”。

**未决项（需 Codex 或实现方确认，本文未擅自裁决）**

- `b8` command/status 是 32 位还是 64 位读端口（状态位用到 63:32）；若为 32 位，需定义高 32 位错误的读法。
- `c8 data` 的写路径用于哪些命令（ABORT/CLEAR 是否需要写 data）。
- launcher 窗口确切范围（`0xb0..0xff` vs 其他）与是否存在 `0xd0` 以后的保留地址。
- 受控 schema 每轮的观测行数是否仍为 19（因此 22 子轮是否为 418 条唯一观测）。
- `docs/validation.md` 的实际标题层级与状态表格式。

**验收建议**

1. 结构检查：两个新文件可独立成篇；插入片段不改动原文件其余文字。
2. 数字抽查：表 1 全部 16 行与 facts JSON 的 `target.*`、`A_record.seq`、`pte`、`trigger_head_is_v`、`insert_*` 一一对应。
3. 覆盖计数：`trigger_head_is_v == 0` 的行数应为 9，`truncated == true` 的行数应为 9，触发子轮应为 16，A1 子轮应为 4。
4. 措辞检查：全文不得出现 v6 的“已构建/已通过/已上板/哈希/版本/commit”；不得把第 1 轮/第 2 轮之差直接写成 D 的因果。
5. 链接检查：`../E3_QUEUE_PAIR.md`、`../E3_CONTROLLED.md`、`../E3_SERVICE.md`、`../E3_DIAGNOSTIC.md`、`e3-queue-results-review.md`、`../out/mac-e3-queue-20260918-130545-run-d01lc9wp/server-review-v5.json`、`../out/e3-controlled-20260918/v5-reviewed-facts.json` 是否可解析。
6. 反例检查：CLI 示例 JSON 段落必须带“不是实测数据”标注，且不含任何真实数值。