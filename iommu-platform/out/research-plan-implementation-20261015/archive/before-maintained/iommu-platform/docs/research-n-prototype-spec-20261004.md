# 研究规格：跨上下文 N 原型（v2 原型约束与待验收）

本文件记录 v2 原型约束与待验收状态。v2 架构方向已定，FPGA 集成参数已由 Codex 应用（见 §v2更新），但 v2 模型/编译/板测尚未执行。v1 两个 BIT 已完整编译 exit 0、WNS 均 +0.177 ns，但新增 overlap rr banked 8/12 失败（8 共跑 fail，4 Solo pass），legacy 12/12 通过，v1 功能拒绝、未上板；不能写成 v1 仍在编译或只有构建中。文末接受条件均为"将来须证明"，不是"已通过"。本稿只做对上一版 N 提案的有限必要纠正，依据 Codex 已直接核对的 critical 源码（out/method88-literature-20261004/codex-n-critical-review.md），不重复完整 source 索引。

## 1. 目标与第一阶段边界

- 研究对象固定为跨上下文：A 为 DID 11 的 aggressor，V 为 DID 10 的 victim。same-DID 原案否决——在 A 占用 dc_q/pc_q 时仅复用其上下文，无法评估跨设备机制，最多只能验证一个接口，不构成 AV baseline（review.md:3）。
- 目标是观察"合法且真实的 V 命中，在 A 未完成时被真实服务并向前推进"，而不是 tap 命中。
- 本阶段只做 S/N 的最小闭环。P（与 N 同每类总容量的静态预留对照）只有在真实容量或出向压力瓶颈被观察到之后才决定，不预先实现所有组合（review.md:11）。
- 尚不主张新规则或增量 novelty；本小基线原型不以"先证明 novelty"为前置条件，但任何完整性能工程都必须写明研究用途。

## 2. 请求与结果生命周期接口契约

### 2.1 确定项

1) 完整请求 header 必须逐 slot 保存。现状：AR/AW 的 stream_fifo 只暴露 head（rv_iommu_trans_ctl_wrap.sv:140-158、:130-133）；控制状态机在启动翻译时把字段拷入单寄存器对（:622-662），且只有一份 trans_req_data_q/n 与 trans_resp_data_q/n（:101-102）。新契约要求每个在途请求持有完整 header（id、stream_id、ss_id_valid、substream_id、addr、len/size/burst、prot 等），不依赖 head。

2) 新增内部 tag。trans_req_data_t/trans_resp_data_t 目前无 tag（rv_iommu_top.sv:117-144），响应只能靠"只允许一笔在途"隐式匹配。tag 必须在请求接纳到结果交付之间稳定，并由内部请求槽产生。互连会扩展 AXI ID 以区分来源，但同一源可重复使用 ID；来源 ID 不能代替每笔翻译的身份和同 ID 顺序跟踪。

3) V 必须自持上下文。仅凭 IOTLB content 不足以授权；需要 V 自身的 DID/PID-valid、DC/PC stage modes、PSCID/GSCID、privilege/request type。应由 V 自己的 DDTC/PDT 真实查找，或严格校验的 context generation snapshot，并带失效/fault barrier。不得复用 A 上下文（review.md:7）。

4) 权限与 ordering 不得绕过。V 的命中必须走与主请求相同的检查：DC/TTYP 检查（rv_iommu_tl_wrap.sv:1001-1007）、PDT privilege（:1128-1132）、S1/S2 权限（:1251-1288）。错误权限不得返回 translated access。

5) 写路径保持原纪律。必须保留 W 排空计数、AW 先于 AR 的前向理由以及容量满时的保护语义；具体结构允许改变，但不能放弃这些安全条件（rv_iommu_trans_ctl_wrap.sv:168-174、:200-201、:214、:616-618）。AW/W 关联与原排空安全保留。

### 2.2 尚缺设计（实现前须先补）

- AR FIFO 的随机访问/多 slot 提取与出队策略，以及输出 header 的关联方式（见第 3 节）。
- context generation 的具体实现：真 lookup vs 版本化 snapshot，及其失效/fault barrier 的粒度。
- 单一 trans_resp_data_o 在两请求同周期完成时的仲裁策略（rv_iommu_tl_wrap.sv:71-75、:1446-1449）。
- lookup 采用 timemux 还是额外端口，及其吞吐/面积权衡（见第 4 节）。
- 真实被服务 hit 与 read-only probe 下 PLRU 的一致性契约。

## 3. AR FIFO head 保持与年轻响应提取的身份矛盾（必须解决）

现状（rv_iommu_trans_ctl_wrap.sv）：AR stream_fifo 只暴露 head（:140-158）；completion ID 取自 ar_data.id（:340）；head 只在 COMPLETE 且 fire 时出队（:714-718）。

若让 A 一直停在 head，同时把先完成的年轻 V 按 tag "路由回正确的 head"，则与上述结构自相矛盾：FIFO 无法随机访问（review.md:4）。仅增加 tag 不能等价于可随机出队的队列，不能假装已经能出队。

本批至少须定义并给出：
- 每个 slot 的完整 header 保留；
- V 的提取（extraction）与出队（dequeue）语义；
- 输出 header 的实际关联（完成的 V 与它自己的 AR header 对应）；
- AXI 同 ID 排序与 W-data 顺序约束；
- A 仍在 head 未完成时，排空与前进不被破坏。

在策略未定之前，不接受"加 tag 即可出队"的实现路径。

## 4. 查找端口与替换策略的契约

- 单 lookup 端口并不必然禁止 timemux。当前并发之所以不可能，是 FSM 不允许（rv_iommu_tl_wrap.sv:886-1456 为单状态机；DC/PC/1S/2S/spaddr 均单份，:193-210），而不是"必须第二物理端口"这一已被证明的最小条件（review.md:5）。timemux 的 state-save/独立 context 路径须与额外读端口一起，按吞吐/面积对比后再定，不以"min 两 ports"作为需求证明。
- read-only probe 与真实被服务的 hit 是两件事。probe 可作无副作用诊断，但默认不等于公平被服务 hit 的实现（review.md:6）。
- 公平 baseline 要求 N 与 P 使用相同的 replacement 更新规则。真实被服务 hit 必须参与既定替换策略，不能用只读观察替代；否则不满足替换契约。
- V 的 hit 只有在通过上述真实权限检查，并被输出通道承接之后，才计入"hit 前进"。

## 5. 容量、输出、失效与 fault 的一致约束

- 容量：保持 16 项 IOTLB 和原 DDT/PDT 缓存、一个 PTW，不复制 IOMMU。逐类列出请求槽、查表记录、结果槽和端口数量；仅保持参数名不变不等于总容量相同，移出原 FIFO 的请求仍须计入容量。N/P 比较必须保持每类总容量与仲裁/替换规则相同。v2 已由 Codex 应用每路数据缓冲改变：DMA0/1 在 IOMMUNonblockMode != 0 且 i < 2 时由 3 拍（24 B）/路 变为 512 拍（4096 B）/路，DMA2/3 仍为 3 拍（24 B）/路；两侧净增 8144 B（见文末 §v2更新）。逐类核算必须按 4096 B/路（DMA0/1）重做，不能再引用"数据深度不增加"。
- 输出：仅一路 trans_resp_data_o（rv_iommu_tl_wrap.sv:71-75、:1446-1449）；两完成同周期不可表示，需仲裁，不能假装多端口。
- 失效：probe/V 必须尊重 IOTLB 失效与更新（rv_iommu_iotlb.sv:205-303、:306-320）；必须定义与 inval 同周期、已取得结果尚未交付及在途查表的顺序。未证明能合法交付时，暂停命中通道并按原顺序处理。软件 quiet 不能替代内部顺序和失效代约束。
- fault：错误路径与 axi_err_slv 对主/次请求都必须完整（rv_iommu_tl_wrap.sv:1398-1425；rv_iommu_trans_ctl_wrap.sv:577-592）。权限失配返回 fault，不返回 hit。

## 6. Linux 与特性位（不得假 role / 混淆旧计量）

- 旧特性 mask 的额外 bit 不必然导致拒绝：idma_research2_preflight（linux/drivers/misc/idma_research.h:488-518）只对已知 MASK 子集、magic/version/role 做校验；额外的 bit 本身不被拒（review.md:8）。
- 因此新实验必须显式检测"代语义（generation）"元数据，旧测试保持不变；Linux 是否需要新 bit/新 ioctl，应由真实控制/数据接口需求决定，而不是靠"发明一个拒绝"来制造必要性。
- 当前 00093 已确认运行，无须为本次文献/规格工作重启或改 SD。未来部署是否重启、是否更新 Linux，取决于实际接口方案；本稿尚不能断言 BIT-only 或必须更新 Linux。
- 新实验的 identity 不得伪装成旧 role，也不得与旧的 raw 计量混用。若现有字段语义发生改变，增加明确版本/能力标记，不能让旧分析器默默按旧含义解释；仅追加兼容只读字段则不必默认另建整套 bank。idma_research.h:341-445 的 *_validate 为 E2/Cost 专用，不得放宽以把 N 塞进 E2。

## 7. 三功能接受条件（将来须证明，非已通过）

1) 合法跨上下文 hit 在 A 未完成时真实前向：在第一笔未翻译读 IOVA 未命中（iotlb_lu_hit==0 且 state_q==PT，rv_iommu_tl_wrap.sv:243、:271）期间，第二笔合法 V（跨上下文、通过权限）的真实 hit 必须在 A 的 PTW 响应返回前被服务并向前推进。
2) 读写、权限、fault、失效安全：真实接纳的请求与自己保存的 header/地址/结果一一对应，按协议每笔恰好完成或报告错误；验证同 ID 顺序、AW/W 关联及完整 4 KiB 数据搬运。权限失败不得送出受保护物理访问；失效与在途请求满足规定的顺序。
3) 结果反压与排空：在出口持续拒收后恢复受理、命中/未命中同时完成及槽满等用例中，保存的地址和身份保持稳定，没有丢失/重复完成。A 与 V 的合法请求都能在必要条件恢复后取得进度；停止供给后所有输入槽、查表、结果、写数据与回应均排空，新的 quiet 覆盖全部新增状态。

以上为"目标契约"，尚未运行、未板测，不构成通过声明。

## 8. 后续最小工作 batch

界定为一份"完整可审阅的跨上下文请求与结果生命周期/接口"：
- 每个请求槽的完整 header、内部 tag 及容量核算；
- AR 多 slot 提取/出队与输出 header 关联策略；
- V 的 own-context 获取（真 lookup 或带 barrier 的 snapshot）与权限路径；
- 输出仲裁与 ordering/backpressure 规则。

本次明确不做：
- 无科学意义的 same-DID probe；
- 提前的全 FPGA 改造。

新规则/增量 novelty 尚未成立；小基线原型不以先证 novelty 为前置，但完整性能工程须写明研究用途。

## 9. 既有结果保留

- 本轮 method88 88/88 已完成；四格对最强匹配简单对照均负，停止固定 A8 上的 p5 主创新路线，将 p5 保留为已测候选/对照。
- 先行论文已有命中绕过未命中，基本方法不能称为创新；更具体的新规则或设计取舍是否有新增贡献，仍须证据。
- E2 的 64 项局部替换结论与 E3 的受限机制结论保留。


## 实现更新：两银行生命周期（2026-10-04，尚在验证）

先前“仍缺身份/出队方案”是草案状态。当前选定两银行方案：按原始 AXI ID 的 DMA 来源位固定分银行，每个银行保留原控制器的FIFO与生命周期，并增加可选读接纳输入；旧版实例默认关闭该输入的作用；一银行至多一笔翻译在途，响应携带银行号回到自己的头。总 AW/AR 槽与 W FIFO 深度均分，缓存与查表器仍各一份。输出 AXI mux 临时增加银行位，外部仍使用原 ID，响应按原来源位还原；同 ID 始终在同银行，AW/W 由现有 demux/mux 保持关联。

命中绕过只支持用户态、无 PID、一级 Sv39/4 KiB、二级 Bare、无 MSI 翻译的合法读写。V 必须查到自己的真实 DDTC 与 IOTLB；权限拒绝、缓存未命中、上下文失效或不支持的请求不提前接收，随后走原正常/错误路径。与原查表器共用查找端口；探测不触碰 PLRU，实际接收才更新。已接收结果保存到反压解除，单响应口仲裁，不能把缓存命中误称整次 DMA 已完成。

两个构建使用相同银行布局、容量和正常请求接收规则，均实例化支持有限绕过的翻译模块，仅运行时使能常量为 0/1。银行布局本身的影响另与原串行版比较。扩展 64 字节设备上下文格式保留；原型关闭 MRIF、保留 flat MSI，启用 MSI 上下文的请求不走快速路径。旧 E3/COST 观察器语义不适用，P4M 的搬运窗口/数据校验用于首轮工程筛选，快速路径计数仍需独立接口。

`in_flight` 覆盖已接受 AW 到 B、AR 到最后 R、已接受 W 到 B及未完成 W burst，加上两银行状态；不是仅看查表器空闲。I/O fence 使用该状态等待在途搬运。首轮真实 TL 仿真及旧 IOTLB 行为对照已通过；追加测试与联合数据通路验证仍未完成，无新板测/性能结论。

本原型是前人已知有限 hit-under-miss 的基线，不宣称新方法成立。静态银行已经改变资源分配，后续不能再把同一划分改名为额外保护方法；必须通过公平对照找剩余瓶颈。

## v1更新：共享写数据所有权下的读接纳（2026-10-04）

v0“AW/W关联已保持”不足以排除读写依赖循环。S/N两版都增加相同读接纳规则：另一银行的W尚未收到LAST时，暂缓选择新读；新AW进入FIFO的同一拍也暂缓选择读，下一拍由已有AW优先规则处理。已经发出的AXI/TL VALID不撤回；缓存与查表器仍各一份，AW/AR 槽总数不变；"数据缓冲深度不增加"只对 v1 成立，v2 已把 DMA0/1 每路耦合缓冲从 24 B 提高到 4096 B（见 §v2更新）。新增有符号AW减WLAST余额和银行所有者属于控制成本，不能称零开销。

v1 两个 BIT 已完整编译，exit 0，WNS 均 +0.177 ns；但新增 overlap rr banked 8/12 失败（8 共跑 fail，4 Solo pass；日志 out/n-prototype-20261004/coupled-overlap-banked-rr.log），legacy 12/12 通过（日志 out/n-prototype-20261004/coupled-overlap-legacy-rr.log），v1 功能拒绝、未上板。不能写成 v1 仍在编译或只有构建中。有限共享返回模型36场景通过，原控制器对照12场景通过，旧联合命中/权限/背压测试通过。模型不是实际DMA后端或DDR；同一4KiB内两段请求更大范围的重叠、完整平台活性仍需板测。此规则可能减少命中绕过机会，不预设N有吞吐收益或创新。详见docs/nonblock-dependency-diagnosis-20261004.md。

## v2更新：4 KiB 整缓冲（方案B，2026-10-04；硬件参数已应用，模型/编译/板测未执行）

背景：v1 的读接纳守卫只延迟"读的选择"，不增加耦合缓冲容量；板级 A8 停滞的精确根因仍未确认（docs/nonblock-dependency-diagnosis-20261004.md:139-141）。v2 因此选择"整缓冲"方案 B：把 DMA0/1 的每路读→写耦合缓冲从 3 拍提高到 512 拍，使一个 4 KiB job 的两半（2 × 256 个 8 B 拍）都能被缓冲，而不是引入更一般的读写 credit 规则。S（mode1，双银行）与 N（mode2，双银行+有限命中绕过）同为 512；DMA2/3 与原版 mode0 仍为 3 拍。

集成点（由 Codex 修改，不在本稿内给 diff）：`ariane_peripherals_*.sv` 的 genDMA 生成块把 `BUFFER_DEPTH` 绑为 `(IOMMUNonblockMode != 0 && i < 2) ? 512 : 3`（本仓库 Xilinx 侧见 cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1202）：S/N 两个 nonblock 实例的 DMA0/1 为 512，DMA2/3 与原版 mode0 仍为 3。DMA 侧该参数透传路径已核对，真实方向为 `ariane_peripherals_*.sv` 的 `BUFFER_DEPTH` → `dma_core_wrap_intf` 的 `BUFFER_DEPTH`（cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:349）→ `dma_core_wrap` 的 `BufferDepth`（同文件:411，参数定义在同文件:27）→ `idma_backend` 的 `BufferDepth`（同文件:293）。不能写成相反方向。注意路径是 `systems/cva6_reg/`，不是 `systems/cva6/`。

### 容量核算（数据字节，非面积）

- DMA0/1：每路耦合缓冲 3 拍 × 8 B = 24 B → 512 拍 × 8 B = 4096 B。
- 两路合计：48 B → 8192 B，数据容量净增 8144 B。
- DMA2/3：仍 3 拍 × 8 B = 24 B/路，不增加。
- 随动元数据存储：`MetaFifoDepth` 11 → 520（随 BufferDepth 缩放）。该值已由源码核对：`cva6/corev_apu/iDMA/src/idma_backend.sv:110-112` 明确定义 `MetaFifoDepth = BufferDepth + NumAxInFlight + MemSysDepth`；本设计为 `BufferDepth=512`、`NumAxInFlight=8`、`MemSysDepth=0`，故为 520（原 3+8+0=11）。不再写"未核对"。
- 本稿不断言综合面积、FF 或 LUT/RAM 结果；这些必须在真实综合后测量，上面只给数据字节数。
- v1 的读接纳守卫不删除；IOTLB/DDTC/PTW/AXI 槽/W FIFO 预算不变。

### 该方案能/不能支撑的主张

- 预期（尚未执行）：在有限 C++ 模型里，把耦合缓冲从 3 拍到 512 拍后，"共享 R 返回队头已满、另一路 W 无法推进"那类循环（docs/nonblock-dependency-diagnosis-20261004.md:83-106）不再由 3 拍容量触发；这是待验收预期，不是已测结果。
- 不能外推：batch 路径只启动正好 4096 B 的 job——`idma_reg64_frontend.sv:146-150` 要求 `num_bytes == 4096`、`src_addr[11:0] == 0`、`dst_addr[11:0] == 0`，`:151` 的 `batch_task_issue` 在 COUNT 且 ready 时发出，`:152` 的 `batch_task_done` 必须等 WAIT 且 `trans_complete_i`，完成后才进入下一个 job。因此本方案只覆盖"整 4 KiB、对齐、一次一个 job"的形状，不能据此断言任意 job、descriptor、多 job 或一般 AXI 情况无死锁。
- 不能外推（区分度）：原版板级数据是在 3 拍缓冲下取得的（scripts/mac_nonblock_pilot.py:103 `DEFAULT_CHUNK = 4096`；:141-144 的 short/long 计划），只能描述"银行布局 + 必要缓冲合并开销"，不能用来分离 N 的绕过增量。只有 S 与 N 同为 512 的公平对照，才可能把绕过增量从缓冲容量本身的影响中分离出来。
- 仍不主张 novelty：N 仍是前人已知 hit-under-miss 的基线（见上文"实现更新：两银行生命周期"段末），full-transfer-buffer 也已有工作；静态银行划分已改变资源分配，不能改名为额外保护方法。

### 测试侧状态

- C++ 有限模型与 runner 支持 `+couple-depth=3|512`（tests/nonblock_coupled_test.cpp）与 `--couple-depth`（tests/test_nonblock_coupled.py，choices 3/512，默认 3）。同一个可执行文件覆盖两个深度：RTL 源列表与 obj 目录不变，不新增 SV adapter。
- 反压（tr_r_ready）与耦合 FIFO 溢出检查共用同一深度值，避免"按一个深度反压、按另一个深度检查"。
- 尚未执行：本稿不声称 v2 的任何仿真、编译、综合或板级结果。验收命令（尚未运行）：
  - `python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512`
  - 对照：`python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr`（默认深度 3）
- 基线失败/通过日志（已存在，保留不重跑）：out/n-prototype-20261004/coupled-overlap-banked-rr.log（8/12 失败）与 out/n-prototype-20261004/coupled-overlap-legacy-rr.log（12/12 通过）。更早的不同 guard 失败日志不作为当前对照。

历史保留：上文第 1–10 节及 v0/v1 原始条目按原始状态保留为历史记录；其中"未就绪实现/本轮不改 RTL"等表述仅描述当时状态，v2 硬件参数已由 Codex 应用但未验证。


### N v3实际FIFO存储验证通过，两版BIT编译中

DeepSeek起草与修订实际production模块验证；Codex审查、应用并执行。功能9/9通过，454278检查/13150周期，覆盖512满队列、拒绝写入、排空、多轮回绕、并行读写、输出背压、占用时flush/复位后重用、旧3深度路径；有独立软件队列数据判定。首轮strict检查因Verilator4.038删除gen_asic_queue名称误报，未执行功能；后单独执行功能通过，Codex用实际生成mem_q/RAM数组更正名称匹配并确认结构，未重复跑功能。证据out/n-prototype-20261004/fifo-ram-v3-acceptance.json及fifo-ram-v3-functional.log。无效数据存储可不同（记录1次差异），仅有效数据须一致。本验证不是完整DMA/DDR集成证明。两版完整v3编译并行运行，不巡查正常日志，尚未上板。


<!-- APPEND to docs/research-n-prototype-spec-20261004.md -->

## v4更新：v4 代码已实现、两版 BIT 构建中、未上板（追加，不改写历史）

以下为追加记录，不重写上文历史条目。证据仅取自已提供的源码、日志和 JSON；未执行新命令，也不声称新构建/板测通过。

- 状态：v4 代码已实现；两版 BIT 构建进行中；**未上板**。不重跑既有原版 15 项，不更新 Linux/SD。
- 原版15项加v3 S/N各15项，共45项技术性通过；N 上 fast 计数为 0。该 0 **不是 hit-rate**，也**不能解释为没有机会（not no-opportunities）**；不得据此推导绕过收益或无机会结论。
- 实际运行时 DDR/DC：DID 10/11 的 live DC 为 flat MSI mode1（`out/n-prototype-20261004/v4-live-context-evidence.json:23-27`）。v3 的 MSI guard 是 mode0-only 静态排除，因此该运行内容被排除；DDTP mode2 受支持（`cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:91-93`）。没有理由把 U 位默认写成未设置；leaf 必须按实际 IOTLB 的 `u` 位判断，v4 `leaf_ok` 仍要求 `content_1S.u`（同文件 `:94-95`）。
- v4 MSI 匹配：输入侧使用 `VLEN39`/`VPNW39` 宽度；cached S1 PPN 侧使用 `GPPNW39`；leaf 侧使用已捕获的 `dc_q`（`rv_iommu_nonblock_hit.sv:62-79`）。仅允许 mode0 或 flat1 ordinary 输入/leaf 两侧均非 MSI；MSI范围内的读写及保留模式均回退到正常路径。原有 request/canonical/Sv39/Bare/4 KiB 权限/稳定性 guard 保留：候选与请求身份、canonical、Sv39、Bare、无 1G/2M、读写权限、`candidate_i == req_q`、`ddtp_i == ddtp_q`、`ddtc_dc_i == dc_q` 等（同文件 `:84-103`）。
- 被动观察器新增 16-word bank：observer局部索引16..31（研究索引D10..D1F，对应前端raw F10..F1F）映射新 bank；旧 0..15（0x00..0x0F，F00..F0F）的 ABI 与 word15 error 语义不变（`dma_nonblock_observer.sv:196-232`）。新 bank 的 seen 位映射：bit0 = base S1PTW（含 implicit/debug）；bit1 = full window；bit2 = candidate；bit3 = start；bit4 = DC；bit5 = DC good；bit6 = LEAF；bit7 = leaf good；bit8 = DC MSI rej；bit9 = leaf MSI rej；bit10 = leaf miss；bit11 = actual accept；bit12 = DC miss；bit13 = DC other；bit14 = leaf other；bit15 = enable（RTL `diag_o` 映射见 `rv_iommu_nonblock_hit.sv:115-131`，观察器 diag bank 见 `dma_nonblock_observer.sv:223`）。
- 诊断计数器位于 diag bank offset 7..15（绝对 word 23..31），只计 event bits 3..11（`dma_nonblock_observer.sv:164-175`、`:223-232`）。窗口按冻结 `(open, close]` 计数；diag bank 复用同一 core seq/cycles；跨窗口 partial probes 合法，因此不得断言完整 lifecycle 守恒。`diag fast` 必须等于 core fast sum（validator `test_fast_accept_must_equal_core_fast_sum`，`out/n-prototype-20261004/v4-diag-validator-regression.log:29`）。
- 已有实际证据：TL exit0；RR 4 KiB proxy 12/12 exit0；MSI actual pass7 / reject10 / accept8 / responses8（含 BP）/ touches8（`out/n-prototype-20261004/v4-msi-regression.log:1`）；observer actual Mode1/2 exit0（`out/n-prototype-20261004/v4-observer-regression-fixed.log:16,27`）；validator 21 项 exit0（`out/n-prototype-20261004/v4-diag-validator-regression.log:42-44`）；static C crosscompile `-static -Wall -Wextra -Werror` exit0。
- Verilator 4.038 兼容临时源只删除 `input logic [15:0] diag_events = 16'd0` 这个 input default；该信号已在 top 显式连线；production 源不变（`out/n-prototype-20261004/v4-observer-regression-fixed.log:1-5`）。
- 边界：仍是 finite unit/proxy 验证；**不是 full DMA integration**，**不是 universal deadlock proof**。
- 容量/布局：S/N 相同 4 KiB buffer（DMA0/1）；旧 DMA2/3 不变；local RAM。原版 3 拍 vs S 512 拍属于结构 + buffer 差异，不能用来分离 N 绕过增量。v3 归档映射见 `v3-accepted-builds/RELOCATION.json`。
- 计划：新增 matched S/N v4，各 15 个 case，按 15 default sequence 并带 `--diagnostics`；不重跑既有样本，不更新 Linux/SD。技术性错误首次出现即停止；`fast0` 是科学负结果，**不是技术性停止条件**。先分析 actual candidate/reject/accept，再谈 cause/benefit。
- 不推测未知 build resource/timing，不新增 novelty 主张。归属：DeepSeek 起草 proposal；Codex 关键 RTL、apply 与实际 test。


### 2026-10-04 v5 后续入口

v4两版30项通过但N候选/探测/快速接收0。v5资格读入口实现已通过有限主机验证，两版FPGA构建中；最新规格、适用范围与冻结板测计划见 [nonblock-v5-qualified-read-plan-20261004.md](nonblock-v5-qualified-read-plan-20261004.md)。实际状态以 `out/n-prototype-20261004/v5-build-state.json` 和工作区CURRENT_STATUS.md为准。


## 2026-10-04 本轮执行结果与暂停状态

v5 S/N两版各15/15完成，runner退出0，全部原始数据已回传Dell；既有分析器退出0、complete=True、quality_issues=0。无新增数据错误/内核警告，逐项taint_after=0。长组每条件3次；N的快速处理计数A8每轮6144/6146/6146、A16每轮8193，Solo/A1及S版均0。N/S速度中位变化A8+4.15%、A16−2.69%；N相对原版A8−14.52%、A16−22.09%。A8三轮104.263/92.987/93.048 MiB/s的波动全部保留。机制已实际触发，但尚不能宣布性能改善成功；不同BIT/新启动、原版结构与缓冲差异限制因果归因，n=3仅描述。

[最终结果复核](../out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md)；[昨夜以来说明](research-overnight-progress-20261004.md)。本节覆盖前文待编译/待上板状态。已完成本轮分析，按用户要求暂停；未启动下一轮，Linux/SD不变。
