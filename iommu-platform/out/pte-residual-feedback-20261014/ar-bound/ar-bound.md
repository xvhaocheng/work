# 有限真实实例的 AR 边界：空复位、信用、spill 与冻结 O-DS-AR

本席结论：在本任务给定的两真实 iDMA、每源 sole DEFAULT 对齐 4KiB、私有 buffer512、CPU 从复位起关闭、atomics0、NativeN、正常 S1／无 ATS／无 implicit／无错误注入的有限域内，**DDR 读队列和 source-local 读信用不可能满**。从真实空复位归纳，共享 AR 输出 spill 的输入 READY 恒为 1，共享 AR chooser 的 held 锁不可达。DS 输入 50 的准备窗口允许 payload 提前承诺，但那些承诺不会增加 PTE 的 AR 阶段等待。冻结 O-DS-AR 的当前 VALID 优先覆盖原生 RR 唯一可能的可选相对等待；其 fairness4 逃逸在此有限实例不可达。

这个结论只处理 AR。它不删除 DDR 中已接受 payload 的 R 债务，也不排除旧 DDTW 返回、共同 R spill、DS 返回 delayer 或 payload 的 held R。若 payload 经这些返回边改变下一 PTE 出生、工作内容或所有权，须由 root 的生成／返回证明裁决。本席不据此关闭 F1 全族、认可全完成收益、论文覆盖或成本等价。

## 1. 精确域及证据入口

- 起点是真实复位：AR spills、demux counters、native locks 均依源码清零，DDR `mem_q` 初始空；CPU `cpu_enable_i` 从这个起点一直为 0。不能从已有 CPU 工作未退休的“后来关闭”状态开始。
- 两个 DMA 各只一个给定 job，每 job 恰为两个 payload read AR，总计最多 4 笔；这是本任务由 root 给定并已验的源工作量接口，**不是用旧 trace 的 queue 峰值代替证明**。不加入 warmup job、旧 batch、pool、CPU、额外应用请求、latency_overlap、tailpause、压力或新 fault 注入。
- 每笔请求走当前 DDR 地址路由；源 1 硬连 0，额外 DS 读入口硬连 0。无 ATOP 读注入。`comp_pack` 从 `'0` 构造 AW，未设置 ATOP；PTW/DDTW pack 只设置 AR/RREADY（`tests/nonblock_actual_dma_system_top.sv:400–430,486–509`）。`DdrAtomics=0` 直接连接 xbar DDR 端（同文件 `:1037–1039`）。
- 正常 S1、pdtv0、S2 Bare；ATS/debug/ATS-DDTC 请求固定 0（`tests/nonblock_actual_dma_test.cpp:535–548`；`tests/nonblock_banked_top.sv:625–632`）。NativeN 只接受另一 bank 的缓存 hit，不另启动 DS walk。
- shared AR 控制只采用上一轮冻结的 `out/pte-safe-stage-causality-20261014/ar-cause/ordinary-ar-contract.json`。既有 firstVALID、输出 spill、全部 AR 字段、fullID、源额度、R/W/B/fault 债务均继承；不看 future PTE，不撤销承诺，不任意初始化 FIFO，不按目标时刻反推启动。这里推导该合同“无新增注册选择周期”的抽象行为；合同尚未实现或时序验收。

完整当前字节 SHA256、实际文件路径、关键原文及行号见同目录 `source-snapshot.json`；下文的 `cva6/…/riscv-iommu` 均指工作树实际 IOMMU 依赖。runner 明确使用 CVA6 xbar/cut/delayer 与 IOMMU vendor mux/demux/common_cells 的混合链：`tests/test_nonblock_actual_dma.py:173–192,409–415`、`tests/test_nonblock_banked.py:21–45`。本席没有编译，不能将 source list 核对写成重新编译验证。

## 2. 生产者守恒给出的容量不等式

定义 `E_P(t)` 为 PTW 已提出该次 AR、其对应 PTE 还未在 walker 消费的义务数；定义 `E_D(t)` 为 DDTW 同类义务数。未消费的义务包括尚在 AR spill／准备阶段、DDR 队列中以及已从 DDR 弹出但还在返回路上的请求。**DDR pop 不使 walker 退休。**

PTW 的唯一 localID 为 0，ARLEN 为 0（`rv_iommu_ptw.sv:276–278`）；`MEM_ACCESS` 一次 AR handshake 后进入 `PROC_PTE`（`:384–389`），在该 len0 R 有效并被消费前没有下一 AR，non-leaf 的下一 `MEM_ACCESS` 位于该消费分支内（`:393–397,507–510`）。因此 `E_P≤1`。

DDTW 的唯一 localID 为 1；非叶 ARLEN0，DC 叶为 ARLEN3 或 7（`rv_iommu_ddtw.sv:178–185`）。一笔 AR 接受后进入 NON_LEAF/LEAF（`:247–253`）；非叶仅在 R 消费分支回 MEM_ACCESS（`:257–261,299–300`）；正常叶在最后 slot3／slot7 消费后到 UPDATE，再更新 DDTC并回 IDLE（`:370–408,424–428,452–471`）。错误 drain 的 `wait_rlast` 也不能当作提前免费退休（`:475–481`）。本域正常、无 implicit，因此 `E_D≤1`。两 context 不是两个独立并发 AR engine，本证明不用“context数 × 随意读次数”。

先不用 TL 的更紧串行性质，也有：

```text
Q_payload_DDR(t) ≤ 4
Q_PTW_DDR(t) ≤ E_P(t) ≤ 1
Q_DDTW_DDR(t) ≤ E_D(t) ≤ 1
Q_DDR(t) ≤ 4 + 1 + 1 = 6 < 16
```

DDR 模型只在真实 DDR AR fire 时 enqueue（`tests/nonblock_actual_dma_test.cpp:1291–1305`），只在 DDR RLAST handshake 时 pop（`:1371–1399`），ARREADY 条件恰为 `mem_q.size()<16`（`:668`）。所以本域中 **DDR ARREADY 从空起点每拍为 1**。同拍先 enqueue、后 pop 的临时大小也不超过所有仍活着的 6 笔义务。这个结论甚至不要求 R 最终前进：若 R 永久 held，walker 不消费而不再生成下一 AR，队列仍不会增长到 16。

demux 的 `MaxTrans=8` 被实现为 3 位 per-ID counter；4 位 localID 对应 16 个 counter。正常 full 条件为 7，`full_o` 是所有 ID 的 OR，另含 overflow（`axi_demux_simple.sv:69,355–372,537–557,599–615`；`cf_math_pkg.sv:57–58`）。这里：

```text
source2 任一 localID 的未 pop 数 ≤ 全部 payload 数 ≤ 4 < 7
source3 localID0 的未 pop 数 ≤ E_P ≤ 1 < 7
source3 localID1 的未 pop 数 ≤ E_D ≤ 1 < 7
其他 source3 localID 的读数 = 0
```

counter pop 是 demux cut 侧的 RLAST handshake，发生不晚于原始 walker 消费，故以上上界保守有效；旧返回缓冲不能让 walker 同 ID 再生成未消费的第二个 AR。没有 ATOP inject，因此也不触发 counter overflow。全部本域读请求走同 DDR 目的地，不出现同 localID 目的地占用冲突（`axi_demux_simple.sv:324–325`）。所以 DS 未锁新 AR 的 `full` 与 lookup 两个准入条件均不因 payload 改变。`MaxSlvTrans=8` 只接 mux 的 `MaxWTrans`（`axi_xbar.sv:148`），不能作为额外“总读8”额度。

## 3. 从空复位归纳，不能遗漏旧 spill/held

原生非 bypass spill 是 A/B 两项，`ready_o=!a_full_q || !b_full_q`；`b_fill=a_drain && !ready_i`；输出先 B 后 A（`spill_register_flushable.sv:38–96`）。所有 full 位在真实复位时为 0，普通 wrapper 固定 flush0（`spill_register.sv:31–43`）。

**共享 AR output spill。** 其下游就是已证恒 1 的 DDR ARREADY。由 `b_fill=0` 和复位 `b_full=0`，归纳得 shared AR 的 B 永远空，故 shared chooser 的 `ar_ready` 恒 1。A 可以每拍持有一个上一拍已接受的 AR，但可同拍 drain、fill；它不会将本拍 chooser 输入锁住。mux native `LockIn=1` 的条件为 `req_o && !gnt_i`（`axi_mux.sv:409–439`；`rr_arb_tree.sv:149–159`）；这里 `gnt_i=1`，所以 shared AR chooser lock 始终 0。冻结 O-DS-AR 的 held 也只存在于 VALID 未获 input handshake，故该 shared held 不可达。

这不是“任意给一个不满 queue 的局部状态就没有 held”：任意初始化一个 B满/已有 held 的局部初态仍可能需 drain。这样的初态没有正常 empty boot 来源，不能用它制造本域反例。

**DS per-source frontend 与 DDR route cut。** frontend AR spill 和 selection spill 保存同两笔请求，不可相加为四笔（`axi_demux.sv:132–160`）；source3 DDR route 的一个 multicut 是另一个两项 spill（`axi_xbar_unmuxed.sv:231–250`；`axi_multicut.sv:47–74`；`axi_cut.sv:86–98`）。它们与 source2 的对应储存独立。

DS stream_delay 的每个 output AR handshake 后进入 Idle；若下一输入持续 VALID，必须再经历一次 50 加载／倒数到 0／转 Ready。精确采样下相邻两个 delayed AR fire 最短间隔 **53 拍**（`stream_delay.sv:64–93,109–131`；counter load 优先 `delta_counter.sv:63–72`）。它不拥有另一个可提前重叠计数的请求槽。

在 DDR READY 恒1、credit已合法的情况下，route head 的 native RR 最多先接受两笔 comp（仅 reset0 的通用局部界；见下一节），所以某 delayed DS 头从 frontend input accept 到 DDR accept 最多 **5 拍**。这足够建立归纳：第一个头到达时 source3 frontend/route 空；该头在 ≤5 拍内 drain，而下一个 delayed 头 ≥53 拍后才出现。因而所有 delayed DS heads 到来时 source3 frontend/route 均空，其 AR 输入 READY 恒1、每个 DS 下游 stage 的存储等待只剩自身固定一拍，没有 payload 造成的 full 或 demux `lock_ar_valid`。证明不预先假设它们空，而是由空复位、53 间距和有限 service bound 得出。

**不要扩成“所有 native AR cuts 都恒 READY”。** source2 的 front/route 仍可在 DS 获选一拍时积累 payload，甚至形成 source2 demux 的本地 locked VALID；这些锁和条目必须保留。它们不占 source3 的 frontend/route，且不能迫使 shared chooser 在 current DS VALID 时选 comp。这里排除的是 payload 对 DS AR 的反压，不是清空 source2 的义务。

## 4. DS 内部、原生 RR 与 fairness4 分开裁决

`rv_iommu_ds_if.sv:81–108` 是七输入 internal arbiter 加两项 DS AR spill。comp 不进入该 arbiter。只用保守 `E_P+E_D≤2` 时，新的 PTW 原始 head 出生前，其自己的旧义务已消费，另一 DDTW 最多占一项，因此该新 head 不会碰到满两项的 spill。若 PTW 与 DDTW 同拍原始 VALID，internal arbiter仍可能先接受 DDTW再接受PTW；**producer并发界本身不能否定这一个机会的内部仲裁差。**

本任务正常 S1 域另外有 root 亲核、由本席核关键原文的更紧互斥：`normal_gate` 只在 IDLE 且无旧 req 才放新的 normal owner（`rv_iommu_tl_wrap.sv:284–286`）；NativeN 的旁路窗口及模块接口只有 DDTC/IoTLB cache lookup、touch、response，无 walker start（`:303–329`）。DDTW init 只在 DDT／ATS-DDTC，PTW init 只在 PT（`:360,375`），ATS-DDTC固定0。正常 DDT 必须等 DC 最后 beat 消费、DDTC update/hit后才进入 PT（DDTW `:424–428,469–471`；TL `:1103–1107,1153–1166`）；没有 pdtv/S2 implicit 分支。PT 结束到 COMPLETE 后再通过原 normal response回 IDLE（TL `:1329–1331,1545–1565`）。因此 PTW 与 DDTW 的原始 AR以及未消费完整 DS 义务不重叠，internal co-VALID／spill满不能成为本域 payload 相对 PTE AR等待的遗漏边。

**原生 shared RR。** 实际 FairArb 用旧 `rr_q` 与 active vector求 next_idx，而非直接 winner+1（`rr_arb_tree.sv:195–219,251–290`）。CPUoff时 active只有2/3；从reset0局部状态，DS连续VALID可先comp、再comp、再DS。但只要此前成功接受过任意 shared AR，native `rr_q∈{2,3}`，之后 DS连续可见最多前置 **1笔 comp input accept**。这是 handshakes 的位移界；同ID/长R burst不把这一笔 AR grant 自动扩成整个 256拍 payload。

一般协议中，READY能永久为0，所以该位移界不是无条件 clock bound。本有限域已经独立证明 shared chooser READY每拍1及上游DS READY，于是才可换算成 native shared可选等待最多1拍（有接受前史）或2拍（仅reset0局部界）。PTE的先前DDTW／PTE响应本身保证已有shared AR接受前史；无需人为重置RR以制造第二拍。固定自然前缀的 co-VALID=0仍只是一条已验事实，不由此宣称 fresh双安全选择全域不可达。

**冻结 fairness4。** 计数单位是 existing output spill 的 input accepts；只有接受DS且同时有non-DS currentVALID才递增，接受comp或没有non-DS pending时清0。DS route heads的53拍间距且≤5拍drain意味着：一笔DS接受的下一拍，不会有下一DS头；若comp仍VALID，shared READY1使其当拍接受并清0；若comp不VALID，no-non-DS规则当拍清0。因此每个后续DS接受前 streak=0，最大可到1，**streak4及fairness escape不可达**。这也适用于从 native历史按合同导入计数：前史不能无凭据初始化为4。仍保留冻结阈值4和成本合同，不因该实例没触发而事后删计数器或调阈值。

ODS的通用“DS可见后至多一笔已held或fairness comp”的界，在本域收紧为 **0笔新comp先于当前DS chooser accept**：shared held没有自然来源，fairness escape不可达。已有 output A若是comp，仍按旧顺序在DDR端退休；它与本拍DS写入A并行，不算被撤销。

## 5. DS 尚未 shared-VALID 时的 payload 承诺：完整局部偏序

以下以“边沿前稳定样本n的VALID&&READY在边沿n接受”为时间标签。以某PTE原始出生／DS内部ARaccept为n=0；在本域互斥和前驱消费门下，其DS AR前端及delayer已空／Idle。只列固定结构过程，不利用future PTE。

| 样本拍 | PTE AR事件 | 依据／必须满足条件 |
|---|---|---|
| 0 | PTW原始VALID，internal arbiter接受进DS ARspill A | 无旧PTW义务；DDTW完整义务已消费；internal spill空，唯一active PTW |
| 1 | DSspill输出VALID；AR stream_delay Idle加载50 | DSspill原生一拍；payload不能参与internal选择 |
| 2…51 | Valid状态计数50…1 | 固定输入delay，计数不以shared READY决定 |
| 52 | 看到count0，决定转Ready | 还不输出delayed VALID |
| 53 | delayed DS VALID并接受进source3 frontend ARspill | frontend READY由上一节归纳恒1 |
| 54 | frontend输出，demux接受进source3 DDR route cut | DS localID credit合法；同DDR目的地；route空/READY1 |
| 55 | route输出首次shared current DS VALID；ODS chooser接受DS进existing output spill A | sharedREADY1；streak<4；无sharedheld；当前VALID判断，不看future |
| 56 | existing outputspill输出DS，DDR ARaccept | DDR READY1；native一拍cut |

即该合同零新增选择周期的抽象路径：`birth→internal=0`，`internal→DDR=1+52+1+1+1=56`。它是源码时序推导，非本席新运行／测量，且不代表整个walk或job完成固定。

令某comp在n=54（DS尚未shared可见）依法经chooser输入握手进output A。这笔comp在n=55由DDR接受；同一拍A接受DS，n=56接受DS到DDR。与没有该comp相比，DS ARaccept仍n=56。若comp更早已被DDR接受，它留在`mem_q`及返回pipeline的R义务上，不能靠AR优先删掉；若comp在n=55才与DS共同currentVALID，ODS当拍选择DS，覆盖native RR可能的一拍差。不存在通过“预先承诺comp”在本域 shared AR held/B满中额外压DS的自然反例。

如果把初态手填为DDRqueue16／shared B满／shared held-comp，即使当前DS被优先也可能需等旧承诺。这是协议层合法的**条件局部反例**，其状态违反本域从empty boot的容量／spill归纳，不能当作自然来源证明。若改变CPU、job数、DS源、ATOP、输入delay、路由、队列或复位来源，本证明不能直接外推；须重建对应容量和head间距界。

## 6. 完整债务、覆盖与未解

本席覆盖：有限域 DDR queue16/read-credit7 饱和；由该饱和留下的 shared AR held；shared output AR spill full；payload对source3 frontend/route的反压；DS50准备期payload提前承诺造成的额外AR等待；冻结fairness4。native当前shared co-VALID时纯RR的一拍选择差由同一份冻结ODS规则覆盖。

仍继承且本席不裁决：所有source2 AR队列/本地锁、所有已接受DDR请求及beat/FullID/LAST、共同和分路R spills、DS返回delayer、walker未消费的旧DDTW/PTE、已held的R拍、W/R/B/fault退休与完整字节守恒。没有用DDR pop或DS原始VALID冒充消费／成功许可。

未解由root统一验收：payload是否经R/DS返回反馈改变同PTE相对等待、下一PTE birth或NormalTL结果所有权；其他合法fresh双安全头／ABBA自然前缀；九规则外独立机制动作；生产范围和不同负载；ODS真实实现的新增观察/选择注册周期、增量面积及Fmax。若收益完全来自上述当前VALID-RR边，它被普通ODS覆盖；若收益来自其他边，必须拿那条边的完整原始因果证明。本报告没有新收益、没有任意初态正例、没有全成本等价或发表覆盖声明。

成本沿用冻结合同：独立选择器状态绝对编码8位，复用native state后的准确增量未知，额外payload槽0。未实现／综合／板测，新增选择周期实测null，面积/Fmax/CPU应用null；“这里fairness不触发”不能据以宣称零增量成本。

执行：只读决定性source与冻结合同，写本席三份独占产物。**build=0，simulation/model run=0，test=0，旧validator=0，生产修改=0，公共文档修改=0，spawn=0，DeepSeek/Claude/Astra=0。**完整原始旧trace没有重parse；旧queuepeak3、12PTE56/88及held-DDTW49仅作为已验背景，不参与上述全域不等式。
