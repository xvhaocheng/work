# E3_CONTROLLED：独立受控有限 E3 实验（v6）

**状态：主机验证、完整软件与 FPGA 构建均已通过，新版尚未上板。** 当前软件内核为 6.5.0-rc2-00087-g2de370cd344e。详细验证见 [validation.json](validation.json)，构建状态见 [build-status.json](build-status.json)。本文的 11 对矩阵是下一轮计划，没有新版板上测量结果。

## 组会 3–5 句解释

1. 上一轮 v5 是“整批轮流跑”，两轮之间的前史/身份对不上，做不出干净的因果比较。
2. v6 改成**独立受控**：**每一轮的 formal 只发一次 64 字节的 V 副本，有 A 时再发一次 64 字节的 A 副本**，各自只做一次读和一次写，用实际计数器核对“确实只发过一次”；prime/probe 是额外执行的拷贝任务，使用同一组映射与 offset 0；每次完成后重新填入反向目标数据。
3. 由 launcher 在硬件里把“先放 A、A 读被选中后再放 V”做成受控时序，CPU 只负责准备、ARM 一次、GO 一次、**有限完成轮询**。
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
| 数据访问延迟 | **DS 50 / 50**——IOMMU **数据结构内存访问延迟**，输入 50、输出 50（**不是批供给/补充速率**） |
| 时钟 | 50 MHz |
| DMA 通道 | 4 |
| IOTLB | 16 |
| CPU | **有限完成轮询**（不是中断） |
| 硬件批处理 | **关闭**；observer 仍复用 batch 的 finite 模式 |
| V | DMA0 / DID10 / ID0 |
| A（可选） | DMA1 / DID11 / ID4 |
| V 副本 | 每轮 formal **1 份 64 B 对齐的 DEFAULT 拷贝**（一源页一目标页一设备） |
| A 副本 | 有 A 时，每轮 formal **1 份 64 B 对齐的 DEFAULT 拷贝**（一源页一目标页一设备） |
| prep 拷贝 | 额外执行的 64 B 任务；共用映射、offset 0，每次重新初始化目标数据 |
| 每设备每方向 | **各一次 AR、一次 AW**，由实际计数器核对 |
| 续写 | **无 endless A、无 CPU refill** |

## 3. 每对与每轮的执行流程

- **每对只用一个不变的映射生命周期，跑两轮**（每对恰好 2 轮）。
- 每轮检查顺序：

  1. 所有 DMA 已 quiesced，并做**已核验的全局 host S1 IOTLB invalidation**；
  2. hot A1 prime **或** cold A（不 prime）；
  3. V1 prime；
  4. V1 probe；
  5. formal。

- 每个 prep 任务必须**实际完成**；cache handoff 需核验；**下一个任务前要反向 reseed**；offset 恒为 0。
- probe 要求 **`iotlb_miss = 0`、`s1_ptw = 0`、`s2_ptw = 0`**，且数据为**真实拷贝**；首个 V prime 的**实际 miss 作为正向证据记录**（期望保留，但**不断言保证**）。
- cache workaround 的 `FENCE.I` 是**平台相关的 CPU handoff** 手段，**不是 IOTLB flush**。
- 失效操作**不会重置** DDT / PDT / PLRU / 页表数据 cache / 全部内部状态。
- cold A / hot A 在 formal 里的**实际 hit/miss 仍要记录**；**不能宣称所有状态相等**。

## 4. 启动时序（launcher 行为）

- launcher 在 backend `VALID` 之前**已经持有完全准备好的 job**。
- CPU **只做一次**：保留 ID、ARM obs、GO。
- A 读被 IOMMU 实际选中（ID4 / DID11）后，**在下一个已寄存周期**发出 V go。
- A 事件到 V 释放之间**不做轮询**；CPU 仍然只做**有限完成轮询**，且**不重新提交**。
- **不允许撤销 AXI `VALID`。**
- 超时/取消路径：**formal COUNT 走 issued→abort→自然排空**；**HOLD 走 cancel→不产生数据**；另有一个 **2 秒独立安全排空超时**，该超时**可能超出运行预算**。
- 任务保留 ID 用普通 `NEXT_ID`（任意），与 AXI ID 分开（V＝0、A＝4）。

## 5. 观测窗口定义

| 窗口 | 起 | 止 |
|---|---|---|
| observer / path / E3 打开 | V backend **实际发出** | V done |
| HPM | **GO 之前取一次样** | 两侧均排空 |
| launcher | PREP | 两侧均排空 |

- HPM 取样的口径是 **GO 前 → 两侧均排空**；它**不是原子快照**；失败或**被隔离（quarantined）**导致的残缺失效样本**视为无效**。
- 迟到的 A 返回**可能落在 V 窗口之外**；缺失标志**不等于**零耗时。
- A 的查询**可能发生在 V 窗口之前**，所以“窗口内 active A lookup = 0”**不等于没有请求**。

## 6. MMIO 映射（launcher，DMA0 配置窗口）

只有 DMA0 的 `0xb0..0xff` 被路由进 launcher 窗口；`obs_cfg_addr` 由 7 位加宽到 **8 位**；旧 observer 地址 `0x58..0x78` **不受影响**。其余被转发的地址**读返回 0**，**写置粘滞 bad-command**。**只接受完整 strobe**。

| 字节地址 | 名称 | 读 | 写 |
|---|---|---|---|
| `0xb0` | magic | `0x49444d414c4e4301` | — |
| `0xb8` | command / status | 状态字（见 6.2） | 命令字（见 6.1） |
| `0xc0` | index | 当前索引 | 设置索引 |
| `0xc8` | data | 读索引字 | **不支持**：置 `bad_c8_write` 错误（**不是写入索引字**） |

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

时间戳表示“观测到边沿/发出命令”的精确时刻；**`goA`/`goV` 时间戳存的是 `ticks + 1`，在 frontend 边沿取样**，寄存器输出是**下一个周期**的事件，文档与注释里要写清楚。

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
| `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` | 加宽 `obs_cfg_addr` 到 8 位；按地址 ≥ `0xb0` 拆分 legacy/launch 的 valid 与 rdata；接入新 launcher |
| `cva6/corev_apu/fpga/src/dma_observer.sv` | 新增可选 `ControlledEnable`（默认 0）与有限标记 |
| `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv` | **也改动**：提供 held/active/cancelled/error 状态，并接收 hold/go/cancel 控制；这些状态是 launcher 的输入 |
| `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv` | **也改动**：接线 held/active/cancelled/error、obs issue/done 与 ID、select tap，并把 launcher 的 hold/go/cancel 接回 frontend |

细节：

- launcher：`NumPorts`（默认 4）；标量 select 输入 `selected_i`、`select_write_i`、`[7:0] select_id_i`、`[23:0] select_did_i`、`[63:0] select_iova_i`；MMIO cfg 的 valid/write/wdata/wstrb/rdata 语义与 observer 相同。**方向必须写清**：frontend 的 `held/active/cancelled/error` 与 backend 的 `obs_issue/obs_done` 及 ID 数组是 launcher 的**输入**；只有 `hold/go/cancel` 是 launcher 的**输出**。**不做 AXI gating。**
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

驱动结构体以**已提供的 patch** 为准（`linux/include/uapi/linux/idma.h`：新增 `struct idma_e3_controlled_round`、`struct idma_e3_controlled`、宏 `IDMA_E3_CONTROLLED_WORDS 32U` 与 ioctl `0x0c`）。**这次 UAPI 新增的不只是那个宏**：**宏、ioctl、能力位 bit6、结构体都是新增项**。旧路径（旧 ioctl、旧 CLI、旧 UAPI）保持**兼容**；新驱动改动是**实际改动**，不是“只加一个宏”。**不修改**旧 UAPI 语义。

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

输出 JSON（`idma.e3_controlled.v1`）字段：输入固定字段（`prep_mode` / `pair_mode`，同时给出人类可读串和数值）、完整 raw pair 映射 8、2 轮 `rounds[]`（`sequence`、`delay`、`status`、测量标志、HPM、`stats`、`e3_words[256]`、`controlled{ flush_status, prep_complete, stage, prime[2], probe, launcher[32], observer_magic, v_hw_cycles }`）。**任何失败都要保留全部字段。**

成功判定：

- syscall 返回 0、pair status 0、两轮 completed、每轮 status 0；
- HPM / `prep_complete` 有效、flush＝0、stage＝8、observer magic 为 v6、bank v2 / error＝0、D 值符合预期、`v_hw_cycles` 为正；
- 每 lane 实际 `1 submitted / 1 completed`、64 B、无错误、无 quarantine、S2＝0；
- probe 为 1 份 64 B 拷贝、`iotlb_miss＝0`、S1＝0、S2＝0。

校验（与成功判定并行，独立读数）：launcher magic 正确 / error 0 / `FROZEN` bit2 / 非 aborted / 非 active / expected mask 为 1 或 3 / `issue == done` / cancel 0 / 计数与 ID 非零且相等；hot A prime＝1、cold A prime＝0、V prime＝1。

**明确不作要求**：不要求触发、不要求 V hit、不要求正好 128 周期。**无覆盖按“已执行数据”保留**，不自动丢弃、不自动重试。成功但元数据无效时置 `EPROTO`/`EIO`。**不给出 64 B 诊断的误导性吞吐。** HPM 口径必须写明：**GO 之前取一次样到两侧均排空**、observer 为 **V 窗口**。

### 输出位置

- 板端：**stdout → UART**（每 case 的 CLI stdout JSON 直接 `cat` 到串口）。
- 输出目录是**板端 Linux 的 `/tmp`**。
- 主机侧**没有自动 output dir**：JSON 由 **UART 捕获到 Mac**，再回传 **Dell**。**不宣称 Mac 上有自动输出目录。**

### 字段骨架（**不是实测数据，无任何真实数字**）

```json
{
  "schema": "idma.e3_controlled.v1",
  "input": {"prep_mode": "<0|1>", "prep": "<cold|hot>", "pair_mode": "<0|1>", "pair": "<complementary|zero>"},
  "rounds": [
    {
      "sequence": "<整数>",
      "delay": "<0|128>",
      "status": "<整数>",
      "controlled": {
        "flush_status": "<整数>",
        "prep_complete": "<整数>",
        "stage": "<整数>",
        "prime": ["<V 统计对象>", "<A 统计对象>"],
        "probe": "<对象>",
        "launcher": ["<32 个整数>"],
        "observer_magic": "<整数>",
        "v_hw_cycles": "<整数>"
      },
      "e3_words": ["<256 个整数>"]
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

- `platform-e3-controlled short|main|all OUTPUT_DIR`；板端输出目录为 `/tmp` 下的新建目录，**已存在即失败**。
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
- **必须先核验所需 bit 是否存在，再做减法**；**A 返回可能落在 V 窗口之外而不可用**，此类轮次**排除**。
- 需要 A/V 的必需阶段位（bits）；**缺失 A 返回的轮次排除**。
- 必须报告**基线抖动**与**有限 N**；**不得仅因差值等于 128 就宣称证明**。
- 若 cold 无覆盖、V 首次 miss、身份不匹配：**保留并在固定矩阵跑完后停止，等 Codex 复核**。

## 12. 本轮不做的事

- 不做吞吐基准，不与旧 A8 性能混算。
- 不扩展启动方式、页数、chunk 或数量旋钮。
- 不在原因未明时实现仲裁/队列分区/缓解硬件。
- 不越过 [E3_QUEUE_PAIR](E3_QUEUE_PAIR.md)、[E3 队列配对结果复核](docs/e3-queue-results-review.md)、[E3 原协议](E3_SERVICE.md)、[软件诊断字段](E3_DIAGNOSTIC.md) 已定义的口径。
- 本文顶部与第 13 节记录实际主机验证和构建状态；第 10 节仍是待执行的板上矩阵，不能当成实验结果。

## 13. 已完成的主机验证与边界

已执行并通过前端 HOLD/GO/cancel、独立 launcher、两路真实前端联测、finite observer 与旧模式回归；HPM session、checked S1 flush、19 场景驱动故障模型；新旧 CLI、板端 runner 及镜像打包检查。完整软件与 FPGA 构建均退出 0、manifest passed；BIT/MCS 与软件产物已生成，完整性由交付清单记录。成功日志只保存，未审查时序报告。

驱动主机模型核验取消、超时、实际任务编号/次数、未排空映射保留和完成轮询竞态；它简化了 backend/HPM，并模拟 observer/E3 收尾，不能证明真实缓存一致性、完整 SoC 时序或板卡功能。RTL 定向仿真也不能代替本轮上板验收。既有时钟、CDC、外部 IO 约束审查仍待完成。

DeepSeek 提供证据整理及实现、测试、文档草案；Codex 负责分析、设计、DMA 关键修正、最终审查、实际测试与构建。
