下面是本轮有界整理。**不实现 RTL、不跑仿真、不宣称任何测试已通过**；所有“通过”字样都只引用所给日志的原文。有限模型结论只针对 `out/n-prototype-20261004/coupled-overlap-banked-rr.log` 这次 fail，不等于“旧板 A8 已最终根因”。

---

## 0. 结论边界（先写在最前）

1. 给定证据能支持：**banked + 读接纳守卫**构建在 12 个共跑场景里 8 失败、4 solo 通过；同一场景集在 legacy 构建 12/12 通过。失败现场高度一致（同一组冻结寄存器）。
2. 给定证据能支持一个**有限模型内的循环等待描述**（第 2 节），并指出守卫本身的注释已经预告了这个循环（`rv_iommu_banked_ctl.sv:244-247`）。
3. 给定证据**不能**支持：旧板 A8 的最终根因、任何吞吐/性能结论、候选 A/B 的有效性、资源可放下。
4. 候选 A 明确要求“v2 先做仿真验证”；候选 B 也只到“可仅 BIT、无 SD”的待核对假设。二者都**不得**通过“放开 guard 重跑板”来验证。

---

## 1. 证据清单（file:line）

### 1.1 失败集合与首错现场（8/12）

| 场景 | 日志行 | 首错现场行 |
|---|---|---|
| mode1-a1-l1 | `coupled-overlap-banked-rr.log:1`, fail `:14` | `:2-13` |
| mode1-a1-l31 | `:15`, fail `:28` | `:16-27` |
| mode1-a8-l1 | `:29`, fail `:42` | `:30-41` |
| mode1-a8-l31 | `:43`, fail `:56` | `:44-55` |
| mode2-a1-l1 | `:57`, fail `:70` | `:58-69` |
| mode2-a1-l31 | `:71`, fail `:84` | `:72-83` |
| mode2-a8-l1 | `:85`, fail `:98` | `:86-97` |
| mode2-a8-l31 | `:99`, fail `:112` | `:100-111` |
| 合计 | `:131` “8/12 scenarios FAILED” | |

失败形态统一：`watchdog: no handshake progress`、`since_progress=4097`（`:14` 等）；同组寄存器逐场景一模一样，说明不是随机竞态，而是同一个确定性冻结态。

### 1.2 首错四要素（按任务给定口径逐条落 file:line）

**(a) 共享 R 头停在 V 第二段**
- `:2`（重复于 `:16,30,44,58,72,86,100`）：`sharedR.head port=0 id=0 addr=0x0000000080000800 len=255 beat=5 latency=0`
- 即：共享 R 通道的 head 是一笔 len=255 的突发，地址落在 `0x8000_0800`（`0x8000_0000` 页的第二段），已完成 5 beat 后不再推进。`port=0` 说明占着 R 的是 bank0 侧这笔读。

**(b) V 的第二段只有 3 beat**
- `:3`（重复 `:17,31,45,59,73,87,101`）：`DMA0 ... rx[0/1]=256/3 wx[0/1]=256/0 r_last[0/1]=1/0 b_done[0/1]=1/0 cfifo=3 half_cur R/B/W=1/1/1`
- 第二段读只收到 3 beat（`rx[1]=3`），与 `:2` 的 `beat=5` 同侧；同 DMA 第一段已 256 beat、写第一段 256 beat、`b_done[0]=1`。
- 对照 DMA1：`:4`（重复 `:18,32,...`）：`DMA1 ... rx[0/1]=0/0 wx[0/1]=0/0 ... cfifo=0` —— 该侧读数据一笔都没回来。

**(c) V 的第二 AW 未被接收**
- `:3`：`aw_want[0/1]=0/1 aw_done[0/1]=1/0` —— 第二 AW 仍在等（want=1），没有 done。
- 两个子情形：
  - a1 系列：`:5` `caw_q.head id=4 addr=0x0000000090100000 len=255 got=0`，`:6` `exp_aw_q.head (empty)` → 已翻译的 AW 停在 completion 侧等待、`got=0`。
  - a8 系列：`:33` `caw_q.head (empty)`，`:34` `exp_aw_q.head id=4 iova=0x0000000020008000 pa=0x0000000090100000 len=255` → 第二 AW 在“应有 AW”侧尚未被接收。
- 同拍上游还挂着 AR/AW/W/R：`:8-11` / `:36-39`（`AR id=4 addr=0x20000800`、`AW id=4 addr=0x20001000`（a1）/`0x20008000`（a8）、`W ... last=1`、`R ... last=0`）。

**(d) A 侧（当前 W owner）需要后方 R / PTW**
- `:13` / `:27` / `:41` / `:55` / `:69` / `:83` / `:97` / `:111`：`w_owner=1 in_flight=1 aw_order=1`，`mem_q=3 caw_q=1`（a1）或 `mem_q=2 exp_aw_q=1`（a8）。
- `:12`（a1 系列）：`TL req_v=0 req_r=0 req_bank=1 resp_v=0 ... ptw_active=0 ptw_r_fire=0 iotlb_look=0`
- `:40`（a8 系列）：`TL ... ptw_active=1 ptw_r_fire=0 iotlb_look=0 iotlb_hit=0` —— **PTW 已起但一条 R 都没拿到**，与 `:2` 的 R 头占用一致。
- 守卫的语义正是“owner 未排空 W 前不给别的 bank 放读”，并在注释里写明这与“本 bank 需要的源数据 / 页表读”构成循环：`rv_iommu_banked_ctl.sv:244-247`（“the shared memory R channel then cannot return the current owner's source data or page-table reads: a circular wait”），实现见 `:257`（`restrict_reads`）、`:258`（`read_owner`）、`:263-266`（`bank_read_admit`）、`:267-276`（`w_unfilled_q`/`w_owner_q`）。

### 1.3 对照组：4 solo 通过 + legacy 12/12 通过

- solo（无 aggressor，`a_pages=0`）：`:116`（mode1-solo-l1）、`:118`（mode1-solo-l31）、`:124`（mode2-solo-l1）、`:126`（mode2-solo-l31），四行均为 `COUPLED-SUMMARY`，无 STALL/FAIL。
- legacy 构建：`coupled-overlap-legacy-rr.log:1-2` 同样 `order=rr`、`overlap_mode=1`；12 条 summary `:4,6,8,10,12,14,16,18,20,22,24,26`；`:27` `PASS (12 scenarios)`。
- 推论边界：失败**不是场景集本身的问题**（同场景 legacy 全过），也不是“无共跑”的问题（solo 全过）；是 banked+读接纳在 `a_pages≥1` 且两侧共跑时出现。

### 1.4 相关 RTL 锚点

| 关注点 | 锚点 |
|---|---|
| 读接纳守卫（现存 v1） | `rv_iommu_banked_ctl.sv:244-276` |
| 每 bank FIFO 深度 = 原始容量一半 | `rv_iommu_banked_ctl.sv:160-162`；`rv_iommu_trans_ctl_wrap.sv:119-137,144-162,220-238` |
| 一银行一笔在途翻译、header 直到 COMPLETE | `rv_iommu_trans_ctl_wrap.sv:622-666,671-700,713-724` |
| W 数据受 `w_cnt_q` 门控（无 AW 不发 W） | `rv_iommu_trans_ctl_wrap.sv:172-178,218,358` |
| IDLE 里 AW 优先于 AR（注释明说要先开 W 排空） | `rv_iommu_trans_ctl_wrap.sv:620-623` |
| bank 内 err_slv（写故障在本 bank 内部消费，不转发） | `rv_iommu_trans_ctl_wrap.sv:577-596`；`rv_iommu_banked_ctl.sv:528-533` |
| in_flight / e3 quiet 语义（banked 只看 quiet） | `rv_iommu_banked_ctl.sv:556-582,618-621`；`rv_iommu_trans_ctl_wrap.sv:202` |
| 输入 demux / 输出 mux 容量 | `rv_iommu_banked_ctl.sv:289,430` |

### 1.5 iDMA 侧锚点（候选 B 相关）

| 关注点 | 锚点 |
|---|---|
| `MetaFifoDepth = BufferDepth + NumAxInFlight + MemSysDepth` | `idma_backend.sv:110-112` |
| 该值驱动的存储：last-flag FIFO | `idma_backend.sv:632-649`（`.Depth(MetaFifoDepth)`） |
| 该值也传给 error handler（本集成被旁路） | `idma_backend.sv:479-488`；`dma_core_wrap.sv:300` `ErrorCap=NO_ERROR_HANDLING`；旁路分支 `idma_backend.sv:519-534` |
| BufferDepth 驱动的数据缓冲（每字节一条 FIFO） | `idma_buffer.sv:33-52`；`idma_backend.sv:664-711`（`.BufferDepth` 传 transport layer） |
| NumAxInFlight 驱动的 FIFO（**不随 BufferDepth 变**） | `idma_backend.sv:554-570,591-608,613-625,841-854` |
| RAW coupler 路径（当前未用，旁路） | `idma_backend.sv:811-858`；`ariane_peripherals_xilinx.sv:1198` `RAW_COUPLING_AVAIL(0)` |
| 4 个 DMA 的实际参数（未覆盖 BufferDepth） | `ariane_peripherals_xilinx.sv:1189-1207`（`JOB_FIFO_DEPTH(8)` `:1195`、`NUM_AX_IN_FLIGHT(8)` `:1196`、`MEM_SYS_DEPTH(0)` `:1197`） |
| BufferDepth 默认值 | `dma_core_wrap.sv:27,349`（默认 3），`dma_core_wrap.sv:293` 透传 |
| job FIFO（与 BufferDepth 无关） | `dma_core_wrap.sv:170-187` |

### 1.6 资源现状（候选 B 唯一可用底数）

- 器件与状态：`ariane.utilization.rpt:8`（`xc7k325tffg900-2`）、`:10`（Routed）、`:6`（`report_utilization -hierarchical`）。
- 整片：`:25` LUT 170145（Logic 166042 / LUTRAM 3734）、SRL 369、FF 136956、RAMB36 49、RAMB18 2、DSP 27。
- DMA0：`:235` 6215 LUT / 4549 FF；其 backend `:440` 903 LUT / 923 FF；`i_idma_buffer` `:443` 169 LUT / 248 FF，8 条字节 FIFO `:444-467` 每条 18–29 LUT / 31 FF，**LUTRAM 列全 0**；`i_w_last` `:469-471` 28 LUT / 24 FF；transport layer `:441-442` 181 LUT / 259 FF；job FIFO `:253-255` 667 LUT / 1626 FF。
- 另外三个 DMA 同量级：`:484,689,938,1187`。
- banked IOMMU：`:1425-1428`（`rv_iommu_top` 30984 LUT / 21930 FF；`rv_iommu_banked_ctl` 5372 LUT / 5922 FF）。
- 未在给定文件里出现：目标器件总资源行、时序/Fmax 报告。**因此“余额是否足够”只能给上下界，不能断言。**

### 1.7 文献/规格锚点（候选 B 不得称创新）

- “整次传输全缓冲”已是既有做法：`docs/research-n-prototype-spec-20261004.md:89`（Benz 等 TC73(1) 2024 第 267 页：某些系统/端点需要完整缓冲，可用 SRAM 存下整次传输）。
- 重复同结论：`docs/research-literature-review-20261004.md:89`。
- 网络层“写命令/数据锁步避免下游死锁”前例：`docs/research-literature-review-20261004.md:90`。
- 守卫本身不是零开销、且不预设收益：`docs/research-n-prototype-spec-20261004.md:113`。
- 原型自认是已知有限 hit-under-miss 的基线，且银行划分已改变资源分配：`docs/research-n-prototype-spec-20261004.md:109`。

---

## 2. 有限模型根因（只到此为止）

**限定范围**：仅指 `out/n-prototype-20261004/coupled-overlap-banked-rr.log` 的确定性冻结态，在“共享单 R 返回通道 + 两 bank 均分容量 + owner 优先读接纳”的这套模型里成立；**不宣称**旧板 A8 的最终根因，也不宣称实际 DDR/互连行为。

观察到的冻结不变式（8 例一致）：

1. 共享 R 通道被一笔 bank0 的读占住，停在该读第二段的中途（`:2` `addr=0x80000800 beat=5` 不再增长）；该读第二段只进了 3 beat（`:3` `rx[1]=3`、`r_last[1]=0`）。
2. 同拍仍在途的写：第二 AW 已翻译或已在途，但 completion 侧 `got=0`（`:5` / `:34`），`aw_done` 第二项为 0（`:3`）。
3. 另一侧（DMA1）读数据 0 笔返回、`cfifo=0`（`:4`），即它完全被 R 通道/接纳规则挡住。
4. `w_owner=1`、`in_flight=1`、`mem_q∈{2,3}`、`aw_order=1`（`:13`/`:41`）：写 episode 未排空，读接纳由此对非 owner 关闭（`rv_iommu_banked_ctl.sv:257-266`）。
5. a8 系列里 `ptw_active=1 && ptw_r_fire=0 && iotlb_look=0`（`:40`）：PTW 需要一条 R 却拿不到，落在同一 R 通道上。

最简循环（**模型内**）：
R 返回通道被“尚需继续的读”占用 → 该读的下游缓冲（本侧只进 3 beat）不再被消费 → R 通道不前推 → owner 侧的写排空/或 PTW 的页表读需要 R 却拿不到 → 写 episode 无法排空 → 读接纳继续把非 owner 读（含后到的那笔 AR）挡在外面 → 回到第一步。
这与守卫注释的预言一致（`rv_iommu_banked_ctl.sv:244-247`），并且是“读接纳位置/时机”问题，而非“bank 划分容量”问题本身。

**待 Codex 裁定的开放点**（我不定论）：
- a1 与 a8 的停放点不同（`caw_q` vs `exp_aw_q`，`mem_q=3` vs `2`，`ptw_active=0` vs `1`），是同一根因的两个阶段还是两个机制，需要 Codex 结合不在此文件里的模型代码定。
- `sharedR.head port=0` 与 `w_owner=1` 分属两侧；哪一侧是任务口径中的 V、哪一侧是 A，请按任务给定口径确认（按给定口径：V 的第二段在 R 头、V 3 beat、V 第二 AW 未接收；A 为当前 W owner，需要后方 R/PTW）。

---

## 3. 候选 A / 候选 B 约束对比表

| 维度 | A：当前写 episode 的 source-read 预算 | B：DMA0/1 的 R→W BufferDepth 提到 512 beat |
|---|---|---|
| 改动位置 | 守卫层，`rv_iommu_banked_ctl.sv:254-266` 附近（不撤回已发出的 VALID） | 集成参数，`ariane_peripherals_xilinx.sv:1189-1207`（当前未覆盖 BufferDepth→默认 3，`dma_core_wrap.sv:27/349`） |
| 语义不变式 | `w_unfilled_q>0` 时，owner bank 的读还须 `budget>0`；AW 上游被接收 +1，该 bank 读翻译请求握手 −1；写 episode 完全排空清空余额 | 不引入新控制语义；靠容量让“先读数据后 AW”在 4KiB 内不再阻塞 |
| 明确适用域 | **仅**“当前写 episode 内 AR 数 ≤ AW 数”的已对齐拷贝（4KiB 里两 AR/两 AW 等量）。**不保证**任意 DMA 拆分、任意 AX 突发边界、读写不等量 | **仅**在“batch 前端每 job ≤4KiB 且下一 job 等两个 B”这一前提下才有 4KiB 全缓冲；该前提在给定文件里**找不到实现证据**（batch/midend 不在这批文件） |
| 对 solo / AR-先到 的影响 | 需显式说明：`w_unfilled_q<=0` 且无同拍 `aw_valid` 时 `restrict_reads=0`（`:257,263-266`），故“AR 先于任何 AW”的 standalone 读仍可通行；一旦进入 `w_unfilled_q>0`，owner 读被预算约束 | 对读路径无新语义；只增加缓冲 |
| 主要风险 | ① 预算耗尽的 owner 读可能被自己饿死：预算释放依赖“后续 AW 被接收”或“episode 排空”，若该 episode 的 AR 数 > AW 数则可能新死锁；② AR 早于 AW 到达的 phase 里读被压到与 AW 同节奏；③ W-before-AW 次序（AW 迟到、AW 永不达）下 `w_unfilled_q` 与预算的差值语义需重新定义；④ 写故障：bank 内 err_slv 内部消费不转发（`rv_iommu_trans_ctl_wrap.sv:577-596`；`rv_iommu_banked_ctl.sv:528-533`），ACW 被接受的预算是否会永久占住必须单独证明；⑤ 单拍写（`aw_accept && w_last_accept` 同拍，`:272-273` 的 netting）与“episode 完全排空清空余额”存在同拍竞争，需要明确优先级 |
| 隐藏开销 | 每个 bank 一份计数器 + 比较器，且要进 `bank_read_admit` 的组合路径（`:263-266`），两 bank 共用；属于控制成本，不得称零开销（同口径见 `docs/research-n-prototype-spec-20261004.md:113`） | 存储随动：`idma_buffer` 8 通道 ×512 深度（`idma_buffer.sv:33-52`）、`i_w_last` 深度从 `BufferDepth+8+0` 从 11 →520（`idma_backend.sv:110-112,632-649`）、transport layer 内同类 FIFO（`:667`）。`NumAxInFlight` 系列不变（`:555,592,614-625`） |
| 资源量级（**不可断言**） | 小、可控，但落在关键路径上 | 若按当前实现（FF，`rpt:444-467` LUTRAM=0）线性外推：每 DMA 约 8 通道×512×8 bit ≈ 32k FF 量级（现 top FF 用量 136956/`:25`）；若被综合器识别为分布式 RAM，可降到约 512 LUT 量级 + 控制。**必须综合后才能说** |
| 验证路径 | v2 先仿真（任务明示）；不放开 guard 上板 | 仿真 + 综合；S/N **同配置**（否则不公平） |
| 新颖性 | 不能称创新：hit-under-miss 已有（`docs/research-literature-review-20261004.md:19`），保护位置属于既有工作范畴（`:40-42`） | **明确不能称创新**：整次传输全缓冲已有（`docs/research-n-prototype-spec-20261004.md:89`） |
| 本轮禁止 | 不得建议“放开 guard 重跑板” | 不得宣称 BIT-only/SD 已确认、不得未综合就断言 fifo 是 FF 还是 LUTRAM |

---

## 4. 必要最小验收项（必要，不充分）

### 4.1 候选 A

1. 复现基线：12 场景（mode1/2 × a{1,8} × l{1,31}）全部通过，`8/12 FAILED`（`coupled-overlap-banked-rr.log:131`）不再出现；4 个 solo 场景（`:116,118,124,126`）行为不变。
2. 回归对照：legacy 12 场景仍 `PASS`（`coupled-overlap-legacy-rr.log:27`）。
3. 定向用例（必须新增，现有场景覆盖不到）：
   - a) `w_unfilled_q>0` 且本 episode 的 AR 数 **等于/等于** AW 数（对齐拷贝）通过；AR 数 **大于** AW 数（不匹配读流）**必须**有界推进，不能变成新的永久冻结；
   - b) AR 先于任何 AW 到达（`w_unfilled_q<=0` 路径）仍可读；
   - c) 同拍 `AW 入 FIFO` 的读抑制仍生效（`:263-266` 的 `!(aw_accept && ...)` 项不被新逻辑旁路）；
   - d) 写故障路径：AW 被 bank 内 err_slv 消费后，预算与本 episode 状态都能归零。
4. 计数不变式：新预算与 `w_unfilled_q` 的加减在同拍 netting 后无下溢/上溢（与 `:584-611` 同风格的 `$error`/断言）。
5. 排空：停止供给后预算余额归零，`in_flight_o`（`:578-582`）与 `e3_o.quiet`（`:620`）语义不被破坏。
6. 明示不证明：通用 AXI、非等量读写、跨 4KiB、多 master 共跑。

### 4.2 候选 B

1. 配置一致性：DMA0/1 使用 512；DMA2/3 的取值必须显式写明（当前 `ariane_peripherals_xilinx.sv:1189-1207` 连 BufferDepth 都没写，等价默认 3，`dma_core_wrap.sv:27/349`），S 与 N 版本必须同配置。
2. 仿真：12 场景在 S/N 两版都通过；legacy 对照仍通过（`coupled-overlap-legacy-rr.log:27`）。
3. **前提证据**（当前缺口）：给出“batch 前端每 job ≤4KiB、下一 job 等两个 B”的源码/寄存器证据；给定文件里没有 batch/midend 实现，无法核对，标为阻塞项。
4. 随动存储核对：`MetaFifoDepth=512+8+0=520`（`idma_backend.sv:112`）之后，`i_w_last`（`:632-649`）、`idma_buffer` 8 通道（`idma_buffer.sv:34-52`）、transport layer FIFO（`:667`）的综合映射与增量；若 `ErrorCap` 改为 `ERROR_HANDLING` 需重算（`dma_core_wrap.sv:300` → `idma_backend.sv:519-534`）。
5. 部署可落地性：参数在 FPGA 顶层参数列表（`:1189-1207`）内，未触及驱动；但“仅 BIT、无 SD”须由 Codex 确认，本轮不宣称。
6. 明示不证明：>4KiB、散射/聚集、非 batch 前端、其他 3 个 DMA、SoC 其他 master、真实 DDR 仲裁。

---

## 5. 不能靠测试证明的一般性限制（两种候选都适用）

1. **活性（liveness）**：有限场景通过 ≠ 无死锁。A 的预算语义只在“AR≤AW 的当前 episode”下可推理；B 只在“≤4KiB 且等两个 B”下可推理。
2. **通用 AXI 拆分/不匹配**：任意 `len/size/burst`、跨 4KiB 边界、读写不等量、AW 迟到或不到、W-before-AW，均未覆盖。
3. **故障路径**：写错误在 bank 内消费（`rv_iommu_trans_ctl_wrap.sv:577-596`）本就绕过 completion 转发，任何“以 completion 握手为预算依据”的计数都必须单独证明故障下能归零。
4. **性能/公平**：功能通过不蕴含收益；两候选都不得以“通过”暗示改善（`docs/research-n-prototype-spec-20261004.md:109,113`）。
5. **反向也不成立**：失败也不能单独证明设计错误，可能是模型/激励侧；因此第 2 节结论止于“有限模型”。

---

## 6. 资源 / 计数窗口 / 公平基线限制

- **资源**：底数只有 `rpt:25`（170145 LUT / 136956 FF / 49+2 BRAM / 27 DSP，器件见 `:8`）。候选 B 的增量以“每 DMA 8 通道 ×512 深度”为单位，FF 映射与 LUTRAM/SRL 映射差一个量级以上（现状 `:444-467` LUTRAM 全 0，即当前是 FF 实现）。**在综合前不得说“余额足够”**，也不得说“一定放不下”。
- **随动计数**：B 会连带 `MetaFifoDepth` 变化（`idma_backend.sv:112`），因此“只改一个参数、其它不变”这句话不成立；反之 `NumAxInFlight` 系列（`:555,592,614-625`）确实不变。A 不改变任何存储，但增加组合/时序压力。
- **计数窗口**：模型的 `rx/wx/aw_done/b_done/cfifo/half_cur`（`coupled-overlap-banked-rr.log:3`）与 legacy 的 E3/HPM 语义不同——banked 实例除 `quiet` 外全零（`rv_iommu_banked_ctl.sv:618-621`），legacy 的 `quiet` 定义也不同（`rv_iommu_trans_ctl_wrap.sv:202`），且规格已声明旧 E3 观察器不适用于双 owner（`docs/research-n-prototype-spec-20261004.md:105,107`）。任何比较必须同一仪器、同一窗口、同一容量口径（`docs/research-literature-review-20261004.md:71-74`）。
- **公平基线**：静态银行划分已经改变资源分配，后续不能再把同一划分改名为额外保护方法（`docs/research-n-prototype-spec-20261004.md:109`）；若只对 DMA0/1 提缓冲，另外两个 DMA 的差异必须在报告里显式列出。
- **禁止**：不要建议放开 guard 重跑板；不要用旧 60/88 结论做新方法独立证明（`docs/research-literature-review-20261004.md:56`）。

---

## 7. 短诊断文档草稿（可直接落到 docs/ 后由 Codex 审）

```markdown
# banked+读接纳：宽 IOV 共跑冻结的有限模型诊断（草案）

状态：仅基于 out/n-prototype-20261004/coupled-overlap-banked-rr.log 与
coupled-overlap-legacy-rr.log 的离线整理；未改 RTL、未编译、未上板。
本稿不主张旧板 A8 的最终根因。

## 1. 现象
- 同场景集：banked+读接纳构建 8/12 失败（coupled-overlap-banked-rr.log:1,15,29,43,57,71,85,99；
  汇总 :131），4 个 solo 通过（:116,118,124,126）；legacy 构建 12/12 通过
  （coupled-overlap-legacy-rr.log:27）。
- 8 例首错现场完全一致（:2-13 等），`watchdog: no handshake progress`、`since_progress=4097`。

## 2. 首错现场（冻结态）
- 共享 R 头：`:2` sharedR.head port=0 addr=0x80000800 len=255 beat=5 latency=0（不再推进）。
- V 第二段只 3 beat：`:3` DMA0 rx[0/1]=256/3，r_last[1]=0。
- 另一侧 0 笔返回：`:4` DMA1 rx[0/1]=0/0，cfifo=0。
- V 第二 AW 未被接收：`:3` aw_want[0/1]=0/1 aw_done[0/1]=1/0；
  a1 停在 caw_q（`:5` got=0，`:6` exp_aw_q 空），a8 停在 exp_aw_q（`:33,:34`）。
- 写 episode 未排空、owner 生效：`:13`/`:41` w_owner=1 in_flight=1 aw_order=1。
- 后方 R/PTW：a1 侧 TL 全 0（`:12`）；a8 侧 ptw_active=1 但 ptw_r_fire=0（`:40`）。

## 3. 有限模型根因（不越界）
共享单 R 返回通道被一笔 bank0 读占住且下游缓冲不再消费 → R 不前推 →
owner 侧排空所需 R/PTW 读拿不到 → 写 episode 不排空 → 读接纳持续
（rv_iommu_banked_ctl.sv:257-266）挡住非 owner 读 → 回到第一步。
守卫注释已预告该循环：rv_iommu_banked_ctl.sv:244-247。
不主张：旧板 A8 根因、真实 DDR/互连行为、任何性能结论。

## 4. 两个候选（仅列举约束，不定论）
- A：当前写 episode 的 source-read 预算。适用域=「本 episode 内 AR≤AW」的
  对齐拷贝；不保证任意拆分/不等量读写。风险：预算耗尽可能饿死 owner 读、
  AR 早于 AW 的相位被压、W-before-AW/写故障下计数语义、单拍写与
  episode 清空的同拍竞争。开销=每 bank 计数+比较进组合读接纳路径。
- B：DMA0/1 的 R→W BufferDepth=512 beat（4KiB）。适用域=「batch 前端每 job
  ≤4KiB 且下一 job 等两个 B」，该前提在本批文件中无实现证据（缺口）。
  随动：MetaFifoDepth=BufferDepth+NumAxInFlight+MemSysDepth
  （idma_backend.sv:112）→ i_w_last 与 idma_buffer 8 通道同步增长。
  资源：不得在综合前断言 FF/LUTRAM 归属；现状 LUTRAM 列为 0
  （ariane.utilization.rpt:444-467）。
- 二者均不构成创新：整次传输全缓冲已有
  （docs/research-n-prototype-spec-20261004.md:89）。

## 5. 不做
不放宽读接纳守卫重跑板；不替换 Codex 的架构/RTL 决策；
不据 8 例失败外推旧板结论。
```

---

## 8. 未决项 / 需 Codex 决策

1. a1 与 a8 是两个阶段还是一个机制的两个分支（`caw_q` vs `exp_aw_q`、`ptw_active` 0/1）——需要不在本批文件里的模型代码。
2. `rz` 侧“V/A 指派”以任务口径为准；日志字段可同时支持另一种读法，我未做强指派。
3. 候选 B 的“batch 前端每 job ≤4KiB / 等两个 B”在给定文件内**无法核对**（batch/midend 未提供）——这是 B 的阻塞性缺口。
4. 目标器件总资源与 Fmax 不在给定报告内；任何“余额足够”结论必须等综合。
5. 候选 A 的 episode 边界与同拍优先级（`aw_accept` 与 `w_last_accept` 同拍、故障消费 AW）需 Codex 给最终语义，否则不变式无法闭合。

**我未执行任何工具，未修改任何文件；以上均为可核对的离线整理与提案。**