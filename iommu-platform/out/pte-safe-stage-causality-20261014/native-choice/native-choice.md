# Native 双安全头的源门：有严格条件偏序，真实共同 capture 仍 unknown

作者为 Codex gpt-6.1-sol / ultra，本席只读关键源码并写本目录；没有调用外部模型，没有模型、编译、仿真、测试、旧 validator 或旧实验。root 拥有最终源门和机制裁决。

## 本席结论

**当前真实 private 双 iDMA、每源 sole DEFAULT 对齐 4KiB、512 拍容量的源码，允许一个局部完整且不造 FIFO 的双 READ fresh 条件偏序，但本席没有证明该相位条件在固定正常供给计划和正常 DDR/DS 服务下实际达成。实际 capture 的判定是 unknown，不是不可达。** 不可把后到的第二安全头加到已经 firstVALID 的第一头上再称 fresh。

决定性新增条件是：**bank0 最后一个 WRITE 的 bank-local completion-AW 接受，与 source1 第一个真实 AR 入 bank1 FIFO，发生在同一 edge；其后两 bank 同拍从 IDLE 选 READ，再同拍进入 REQ_TRANS。** 这里 source1 不是 READ-only 合成 wrapper，而是一个正常 4096B 作业，同时生成自己的两组 AW/AR；它的 AW 受现有 bank0 W 所有权挡在原始上游路径，private full-buffer 证书让它的 READ 能合法选择。bank-local AW接受表示原生completion mux/spill接走该头，**并不表示DDR已经接受AW**；后面的各级义务仍要带齐。

本报告为该条件提供逐拍源偏序、身份和所有债务；没有给条件等式伪造一个绝对时钟值，也没有以它准入 ABBA probe。

## 固定实例及关键原文入口

- `tests/nonblock_actual_dma_top.sv:352-468`：真实两个 `dma_core_wrap`，每源 `BufferDepth=512`、`RAWCouplingAvail=0`、1D；DMA0 DID10/PID20、DMA1 DID11/PID21、各 AXI ID0。
- `tests/nonblock_actual_dma_top.sv:544-558,680-704,772`：只有 mux source0/source1 活跃；pool mode0 是原样通路，4-input AXI mux 给出完整上游 ID0/4；capacity permission 来自两个真实证书，不能静态写 11。
- `tests/nonblock_actual_dma_top.sv:389-391,448-450,970-1066`：当前 top 的 hold/go/cancel 全为0，配置一次只驱动一个 slave；不能把可选短作业 launch HOLD 当当前长作业共同 arm。预先写好两源正常寄存器后可分别做正常 next-ID read，但两个接受 edge 不会被本 top 的 cfg FSM 一次并发驱动。
- `tests/nonblock_actual_dma_system_top.sv:79-96,1102-1119`：沿用上述真实实例和 native xbar 配置；本席不改变 DS 固定延迟、DDR 返回策略或任何回压来对齐。
- `dma_core_wrap.sv:173-207` 与 `idma_fullbuffer_certificate.sv:16-42`：证书在 frontend job FIFO 接受时出生，条件为容量至少4096、DEFAULT、长度4096、src/dst低12位为0；pending sole job 到最后 backend completion 才退休；unexpected overlap/retire 会撤销资格。normal1D completion consumer 恒 ready。
- `idma_pkg.sv:107-111` 与 `idma_legalizer.sv:75-85,139-178,203-267,285-316,449-477`：AXI最大256拍、64bit数据，正常非deburst对齐4KiB被分成**两个2048B/256拍的成对AW/AR**。DEFAULT rw coupled 只要求原生本地两侧 storage 都可用，不等待 AW 被 IOMMU/DDR 接受，也不等待首 R。
- `idma_backend.sv:471-473,610-625,837-857` 与 `idma_axi_transport_layer.sv:195-202,288-291`：AR/AW 各有1-entry fall-through；RAWCoupler关闭，原始 transport 独立转发两个 AX 通道。
- `axi_mux.sv:47-54,188-271`：原始 AW/AR 各有 native spill（AW与AR分开）；AW first offer 已登记 W-source 顺序，W FIFO pop 只在该 source 真正 WLAST 握手。`axi_demux_simple.sv:175-189,232-253`：当 bank0 W尚未填完，bank1 AW不能转入输入 demux，但独立 AR 可以走其原有 id credit 路径（:320-335）。
- `rv_iommu_banked_ctl.sv:259-268,315-343`：现有 read_admit 与 AW-owner限制保留；private bank0/DID10、bank1/DID11 full-buffer 例外在 TC 里使用。
- `rv_iommu_trans_ctl_wrap.sv:121-163,625-681,716-745`：TC FIFOs 不 fall-through；IDLE 先选 AW，仅当无可选 AW才选合法 AR；选完下一拍进入 REQ_TRANS 才产生有效翻译请求；completion AW/AR 接受后才 pop 该头并回 IDLE。
- `rv_iommu_banked_ctl.sv:372-391`、`stream_arbiter_flushable.sv:34-52`、`rr_arb_tree.sv:151-196`：merged RR没有等“候选齐备”的额外等待；任一源 VALID 即为首次 offer，stall即锁定已见集合和选择。

上述路径为平台根目录下相对路径，完整原文、范围和完整文件 SHA 见本目录 `source-excerpts.md` 与 `source-snapshot.json`。旧验收 native-ready 的容量和完整 ID/退休事实直接复用；没有重复旧仿真。

## 一个可以被源码逐步推导的最小条件前缀

以下不是一次实测 capture。`E(n)` 表示第 n 个上升沿，`C(n)` 是 `E(n)` 之后直到下一沿的组合周期。时钟符号只表示相对偏序。每源只启动一次 DEFAULT 长作业；batch disabled、conf.decouple=0、deburst=0、pv=0，两个 src/dst 都4KiB对齐且正常映射，无前一作业 payload 残留。source0先正常启动，source1配置已准备；不加任务、不改容量、不手填 FIFO。

source0 的原生两个 AW/AR 已入自己 TC FIFOs。AW-first 使它先合法完成 AW0[0]、AW0[1] 两次 WRITE 翻译；两次 completion AW 的成功许可/物理地址已由 TL 返回。选定 `E(e)` 为第二个 bank-local completion AW 真正接受 edge，即 `bank_comp_req[0].aw_valid && bank_comp_resp[0].aw_ready`。它不同于外层 `comp_aw_valid && comp_aw_ready`，更不同于 `ddr_aw_valid && ddr_aw_ready`。在它之前 source0 首 READ 尚未选择。此处两次 W payload 仍全欠，无法自然 WLAST：还没有 source0 source-read 被翻译/下发，也没有任何 source0 R payload。原始输入 demux 因 bank0 未填 W 所有权阻止 source1 AW。

条件是正常 source1 job FIFO 接受发生在 `E(a)`，且 `e = a+3`。空本地 metadata/fallthrough/spill、无其它 source 的 AR、native id credit 可用时，源码给出：

| Edge/周期 | 原生动作和状态 | 不能省略的所有权事实 |
|---|---|---|
| `E(a)` | frontend COUNT 的 sole4096B request 进原始 non-fall-through job FIFO；真实证书 pending/eligible 变1 | job1已分配原生 functional job ID；不是翻译 permit，不是 IOMMU AR已接受 |
| `C(a)` / `E(a+1)` | backend从 job FIFO把唯一4096B request装入legalizer | read/write 两 mutable state 都加载，DEFAULT类型、地址和 DID均原样保留 |
| `C(a+1)` / `E(a+2)` | legalizer第一组256拍 AR1[0]/AW1[0]同时进入本地 metadata及各原始 fall-through，mux可接受首对到 native spill | AW firstoffer登记完整 source1 W义务；AW不得撤回；此时bank1 AW还未获输入demux接受 |
| `C(a+2)` / `E(a+3)=E(e)` | first AR1[0]从原始 mux AR spill、独立输入 demux进入bank1非fallthrough AR FIFO；同时bank0 second bank-local completion AW真正握手，pop最后AW并回IDLE。第二组256拍AR1[1]/AW1[1]也由sole4096B legalizer产生，经原生 local/spill容量排队 | bank0仍是当前W-owner，所有512拍W还欠；第二source0 AW现在是原生输出mux/spill拥有，仍需外层completion及DDR接受；source1 AW0可保持在mux spill，AW1可用原生fallthrough/spill暂存，无额外slot；bank1真实证书已经有效 |
| `C(e)` / `E(e+1)` | 两bank都处于IDLE。bank0 AW FIFO已空而AR0[0]已有；bank1 AW没有进bank，AR1[0]刚可见。两bank各自选择READ，在 `E(e+1)` 同时写入 request owner 并进入REQ_TRANS；AR1[1]可正常入队 | bank0按owner read_admit合法；bank1按 DID11+真实 fullbuffer 例外合法，不强行改read_admit；选择动作不是AR翻译完成或物理AR许可 |
| `C(e+1)` | 两 `bank_trans_req_valid` 同时首次为1；此前两bank分别为COMPLETE/IDLE、都没有TL VALID，所以merge无旧锁。merged NormalTL firstVALID现在才出生 | 此周期起只能改变此 fresh 二选一；firstVALID后须保持完整选择/头，不能用稍后 arrival重新选 |

本实例 scalar pack/unpack不添加寄存器，bank wrapper明确将输入demux `SpillAw=0, SpillAr=0`（`:280-308`）；所以本地空路径对应 `e=a+3`。如果真实capture里的原生spill已带其它owned header，必须按capture真实状态重算，不能套这个空路径。这里的条件不替代正常源/内存运行证明。

**局部充分条件**：上述真实一作业 prefix 已执行、最后AW completion与first AR1入FIFO edge吻合、两个 BC合法、前一merged offer无锁。满足就有两个同步出生的合法READ heads。**未完成的全局问题**：哪一个预先固定、无未来PTE oracle的正常 config/next-ID供给时序与原始 TL/DS/DDR完整运行，会真的给出该 edge等式；没有该证据不能称为自然 capture，也不能给出数字结果。

同时把两源配置为“已经启动”本身不证明本条件。若 bank1 READ较早一拍 firstVALID，merged RR已经锁它；若较晚一拍，bank0 READ已经锁它。之后双头都处于 REQ_TRANS 也不能补造 fresh 初态。用强制等候一个安全头、修改 DDR/WREADY 或额外压力来补这个相位，均不在本轮合同内。

## Capture 必须携带的完整身份与债务

局部条件前缀可选择尚无任何 payload R返回的时点，这一时点源0/1没有消费任何数据；这不是断言所有自然运行都无载荷债务。每一次真正capture仍须记录实况，不能从本表推导空状态。

| 项目 | source0 | source1 |
|---|---|---|
| 原生源 / bank / 完整IOMMU AXI ID | source0 / bank0 / 0 | source1 / bank1 / 4 |
| 完成DDR完整ID（复用旧验收） | 32 | 36 |
| DID / PID域 | DID10，pv=0/pid=0（若pv=1则PID20必须实际记录） | DID11，pv=0/pid=0（若pv=1则PID21必须实际记录） |
| 作业 | 唯一正常frontend job ordinal J0，4096B、源S0/目的D0对齐 | 唯一正常frontend job ordinal J1，4096B、源S1/目的D1对齐 |
| fresh READ头 | AR0[0]：iova=S0，len255,size3,INCR,UNTRANSLATED_R,priv0 | AR1[0]：iova=S1，同属性 |
| 后续READ头 | AR0[1]：S0+2048，仍欠256R拍 | AR1[1]：S1+2048，若刚在原生spill/FIFO，仍须带其位置与VALID锁 |
| certificate | pending=1,eligible=1,fault=0,normal=1，来源为唯一job_fire | 同左；这才是跨read_admit的凭据 |
| WRITE许可 | 两AW翻译成功，bank-local completion AW0[0]/[1]都已真正接受，物理目的地址已拥有；原生输出mux/spill、外层completion、xbar/DDR各级已接受或仍held的位置须实际记录，不能从TC已IDLE推出DDR已接受 | 自己的正常AW0/[1]真实生成且在原始fallthrough/spill/axi_mux路径受阻；未获IOMMU输入AW接受、未获WRITE翻译许可 |
| R/W/B债务（本局部前缀） | 两AR翻译/下发、512R拍、512W拍、2B都欠；两AW已由TC移交原生completion输出路径，但尚未被DDR接受的AW亦须保留 | 两AR翻译/下发、512R拍、512W拍、2B都欠；两AW真实产生但尚未进入bank |
| W所有权 | 当前global W FIFO源为source0；输入demux bank0 W未填2burst；W FIFO字段/计数必须实录 | source1后续W ownership不能冒充已获；它可以在自有512拍内吸收整个sole job |
| 既有PTE/DS债务（本局部前缀） | 两WRITE翻译已经成功返回；目的页walk已消费完，第二WRITE可为同页hit。需capture实际TL/PTW/DS队列，而不能仅按类型认定为空 | 还没有选中本job的READ，不能存在其预先伪造的PTE请求 |

安全的是这两个实际 READ在各自sole4096B且512容量的真实证书域内；不论谁先服务，它都不靠未拥有的W许可才能吸收本job全部512拍。**WRITE类型本身不构成安全证书**：若采用READ/WRITE邻例，WRITE必须还带真实lease、fullsource W-owner、数据已经可供/独立推进和剩余W/B债务；本席未准入一个具体WRITE安全邻例。

## 给 root 的 PTE 身份连接接口

1. 在真正frontend `job_fire` 记录 source index、functional job ordinal/issue ID、DEFAULT、S/D/4096和pv/PID；one sole job意味着该源两AR/AW都从这个job而来，但不能把AXI ID0当job ID。
2. 在真实AR/AW header入TC FIFO和fresh选择时，用完整源 ID0/4、DID10/11、pv/PID、IOVA、方向和burst ordinal把TC retained head连接到 J0/J1。TC填入did/pid/iova/type，banked wrapper仅改 `.bank`、其余位原样复制（`trans_ctl_wrap:631-651`, `banked_ctl:372-376`）。硬件 NormalTL request不带functional job ID，所以该连接必须由已完成的合法header历史和当前retained owner给出。
3. root 按自己亲核的 TL/PTW owner链，将 accepted NormalTL owner连接到 PTW walk epoch、stage/level、实际context key/root（GSCID/PSCID、S1/S2及其有效域），再连接到 DS接受、DDR接受和各R消费。PTW/DDR AR ID不能单独标识“同一PTE”，因为同一walker可重复该ID；walk epoch+owner+stage/level+实际PTE地址/该次accept ordinal才不会混淆。不得从未来PTE内容或未来PA给头分配优先级。
4. ABBA对每个固定原作业/方向/page/context/walk-stage/level匹配同一个逻辑PTE事件。两次试验的公共capture前缀完全一致，所有已有债务及VALID锁也一致；改变的只有 `C(e+1)` 首次二选一。若某个顺序使对应读变IoTLB hit而没有PTE，应明确记录事件不存在，不能借另一个PTE补齐或当作同PTE的等待周期。
5. 相对PTE因果量应从root指定的同一前驱PTE消费事件到后继PTE firstVALID/DS接受/DDR接受/DDR firstRVALID/R握手/PTW消费计算。仅first selected head的开始整体平移，不等于后继PTE出生或服务窗口反馈改变。

## 源门未解与停点

- 已证：真实2×256拍成对AW/AR、原始证书出生、fullbuffer越read_admit而不越AW-first、一个精确的同步fresh双READ局部充分偏序、不可补选的firstVALID锁边界。
- 未证：该同步edge在既定正常job供给和完整native memory服务中实际达成；capture绝对周期；任何PTE相对等待差额；普通完整控制覆盖/成本；收益、面积、Fmax或应用效果。相关数值均为null。
- root未接受新的源门并单独派发前，不建原型、不改RTL、不开新模型/编译/仿真。若root认为条件对齐不够，应结束此源门为unknown，保留这个最小决定问题，不能扩大成对DEFAULT全部正常历史的不可达定理。
