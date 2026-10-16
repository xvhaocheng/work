# 普通强对照：原始事实、顺序证明与完整成本契约

本席只做新 goal 的强对照源核与有界设计推导。所有结论由 Codex 形成；没有使用 DeepSeek、Claude 或 Astra，没有构建、仿真、综合或板操作，没有修改生产 RTL/driver/ABI，也没有重跑旧 suite。历史运行只引用已给定原件。本目录 `baseline-contract.json` 为可机器读取的契约，`source-manifest.json` 保存阅读原件的 SHA-256，`verify_contract.py` 只校核新字段与静态算术，不是 RTL 性能实验。

## 决定结论

1. **Mode7 本身的 current/next 决定是固定队头与紧邻第二项查验，不是完整队列扫描。** 新事件索引若只复现这些决定，须和这个 O(1) 普通控制比较。
2. **当前每 burst 256 拍、每 bank TC W 容量 16 拍的正常共同分段路径，跨 bank 的未完成写不会在 physical AW 次序上逆转。** 证明依赖 burst 长度大于 TC 容量和 AW 后方可出 W，不能把结论扩大到任意短 burst。
3. **对成功的 current virtual-W-head，只要准确匹配的 physical AW 已真正握手，全部 physical W 前序所需 payload 已供给。** 这是比全局 AW 顺序相同更弱、也更适用的源结构不变量。当前目的准备普通门即可表达这个放行能力，完整前缀扫描对该资格没有增加动作。原严格双头门存在合法额外等待，但不能据此认定扫描为新机制。
4. **一般 AXI wrapper 的短包 AW 逆序有合法条件 trace；真实四 iDMA 短包/独立分段路径的全部时间序仍未亲证。** 不把 wrapper trace、两真实 DMA 历史仿真或四来源抽象模型当成实际四 DMA 实验。
5. **R2 的成功路径可由独立普通“AW 接收 FIFO + 后台正常翻译准备 + FIFO 队头 issue + W 选择 FIFO”表达。** 显式票据序号可由 FIFO 位置代替；这构成功能强对照，不是发表前作或实测成本声明。完整错误/cancel/ATOP/epoch 等价与实际面积仍 unknown，不能宣布完整同预算等价。

## 1. 实际拓扑与原生额度

- 生产四 iDMA：`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:870-872,1214-1252,1255-1281,1404-1409`。`NumDMADevices=4`，源 local ID 固定 0，DID=10+i；四路 mux prepend 来源索引，`SlvAxiIDWidth=DmaAxiIdWidth`、`MaxWTrans=8`。`cva6/corev_apu/tb/ariane_soc_pkg.sv:31,111-112` 给 ID 宽 4、全局 TC head 额度 8、W 深度 32。所以 DmaAxiIdWidth=2，原 ID 为 0/4/8/12，BankIdBit=2，bank0={DMA0,DMA2}，bank1={DMA1,DMA3}。
- `rv_iommu_banked_ctl.sv:162-164,243-244,280-309,418-449` 平分容量：每 bank AW FIFO 4、AR FIFO 4、W FIFO 16；输入 demux 不加 AW/W spill，输出 mux `MaxWTrans=8` 且 AW/W/AR spill 都为 0。两个 bank 各至多一个活动 translation；共享正常 TL 请求口 1、响应口 1，按 bank echo 路由（`:314-357,379-408`）。这些原生槽和两 bank 在途须给每种强对照使用，不人为单 child 限流。
- `tests/nonblock_actual_dma_top.sv:349-434,471-548,680-705,788-797` 只有两个真实 iDMA，另外两个 mux 来源系零。常规 BankIdBit=2；SameBankDmasEnable 则用 bit3 使原 IDs0/4 同 bank。不能用同 bank 测试的 bit2 源身份代替生产四 DMA 的完整来源身份。
- 生产 `ariane_peripherals_xilinx.sv:1263-1277` 在非阻塞 mode 只给前两路 512 拍返回 FIFO 与 launch 能力，后两路保持 3 拍；四路现有生产配置的活跃 DMA payload 是 `(512+512+3+3)*8=8240B`，不是全四路 Mode7 的 96B。假设四路均换为 3 拍时才是 96B，未实施。
- 原生 AW 输入 mux 的 W 选择 FIFO 8 项存源索引 2 bit；输出 mux 8 项存 bank 1 bit。两个都按 AW **首次选择** push，按真实 WLAST pop，并保留首次 VALID 承诺。不是 AW 首次选择已获物理授权。准备证据须取 physical AW handshake。
- 输入 demux `MaxTrans=16` 的 ID 计数宽是 `idx_width(16)=4`（`axi_demux_simple.sv:69`），每完整 ID 计数满时全局 AW 准入受限（`:557,614-615`）。它是 per-ID 计数/OR-full，不能当作一个“全局只有16笔直到 B”的队列。新的 B 退休控制不能偷换成只容纳8个未返 B。

关键原文：

```systemverilog
// rv_iommu_banked_ctl.sv:162-164
NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
// axi_demux_simple.sv:175-181
if (slv_req_i.aw_valid &&
      ((w_open == '0) || (w_select == slv_aw_select_i)) &&
      (!aw_select_occupied || (slv_aw_select_i == lookup_aw_select))) begin
  aw_valid = 1'b1;
  w_cnt_up = 1'b1;
```

## 2. 三个不同的顺序命题

### L1：同 bank AW 保序

`rv_iommu_trans_ctl_wrap.sv:121-139` 的 FIFO 保留原 AW 头。控制器只从头发 translation（`:628-642`），COMPLETE 到 completion AW 握手后才 pop（`:716-725`）。所以任意来源 ID 的同 bank 成功 AW 都依该 bank 的输入接受序 issue，异步翻译不改变同 bank AW 顺序。

`axi_mux.sv` 的两份 vendor 不是全文件同 hash，但只有单输入退化分支及 include 不同；本题四输入/两输入 `gen_mux` 块相同。生产版本 `:225-257,277-290` 对应测试 IOMMU vendor 版本 `:301-333,353-365`：第一次选择时 push W owner，AW stall 则锁住 VALID，W owner 直到最后拍 handshake 才 pop。哈希均保存在 manifest，不谎称两个文件全字节相同。

### L2：长 burst 全局 AW 序不逆

正常成功路径的关键源：

```systemverilog
// rv_iommu_trans_ctl_wrap.sv:178-179,220
assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
assign decr_w_cnt = (w_valid & w_ready & w_data.last);
assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);
// :222-239 输入 W 可独立写入有界 FIFO
.DEPTH(RVIOMMUCfg.WFifoDepth)
.valid_i(tr_axi_req_i.w_valid)
.ready_o(tr_w_ready)
```

设 T 是某 bank 连续 run 的最后一个写，长度 `B_T>C_TC`。若 T 的 physical AW 尚未握手，T 的 W 无可用 AW credit，旧写 credit 在旧 WLAST 时耗尽。T 在 TC 内最多驻留 C_TC 拍，所以 T 的 virtual WLAST 不可能接受。输入 demux 仅在 run 的全部 WLAST 已接受、`w_open=0` 后允许下一 bank AW；因此下一 bank 的 AW 接受前 T 的 physical AW 已握手。L1 又保证 run 内其他 AW 早于 T。对 bank run 归纳，成功 AW 的全局顺序一致。

当前共同分段正常 4KiB copy 每 burst 是 256 拍、C_TC=16，满足该条件。证明不要求 source R 的精确延迟，也不要求全返回缓冲。翻译错误走内部 error slave、短包、ATOP、W spill 改变及取消不在本命题内。

### L3：current virtual W 头的所有 physical 前序已经供齐

设 T 仍是全局 virtual W 队头，且 T 的准确 matching physical AW 已 handshake。令 X 为 physical W 队列中 T 之前的任一成功写：

1. X 和 T 同 bank：由 L1，X 的 bank 输入 AW 在 T 前，故其全局 virtual AW 也在 T 前；全局输入 W FIFO 非交织，T 成为 virtual W 队头意味着 X 的 virtual WLAST 已接受。
2. X 与 T 异 bank：若 X 的 input AW 在 T 前，则输入 demux 在接纳 T 的 bank 前已接受 X 所属旧 run 全部 virtual WLAST。若 X 的 input AW 在 T 后，则 demux 要接纳 X 的 bank 必须先接受 T 的 virtual WLAST，和 T 仍是 virtual W 头矛盾。

所以任意 physical 前序 X 都已得到全部 payload，有限/公平 W 服务下可自行排完，不再需要后面的 R。这个结论不需要 L2 的“全局全部 AW 同序”，故即使短包产生过去的 AW 逆序，对 current 资格也不必扫描整个 physical 队列。它不放行非 current 的任意 future 写，不保证无限 downstream 背压下完成，且同 ID R 顺序、既有读的完全返回/排空仍必须独立满足。

这里“已供齐”指 virtual WLAST 已送进 TC，不等于 DDR 接受完、B 返回或 job_done。

## 3. 源结构允许的最短事件 trace，和未关闭的 native 范围

### T-short：一般 wrapper 的短 W 逆序（条件性、未运行）

只有两个异 bank 写 A/B 即足以否定“输入 W 锁单独强制所有长度全局 AW 顺序相同”。输入 AW 保持 A→B：

1. 接受 AW_A；bankA 的 translation 未完成，尚未向共享 completion mux 首次 offer AW_A。
2. 输入一个合法单拍 W_A/LAST；空 bankA W FIFO 接受，w_cnt=0 因而只缓存，不向输出转发。输入 demux `w_open` 降为 0。
3. 接受异 bank AW_B；bankB 的正常 translation 先完成；共享 output AW offer/handshake B。
4. A 后完成；共享 output AW offer/handshake A。

这条 trace 使用源码实际有界 FIFO 与握手，未随意指定不可达 free-model 队列状态。它要求 W_A 的 payload 已在 source 侧可用，未证明当前真实 iDMA/CONF=0/串行无预存 payload 自己能发出；不是性能准入见证。

### T-tail：严格双头门可多等，但普通 current 门已覆盖（条件性源推导）

1. AW_A 已 input/physical 接受；长 W_A 送到 virtual LAST，但物理端尾部≤16拍在 TC 中，物理 WREADY 有限暂停。两 domain 头分别即将指向 B 和仍指向 A。
2. 输入锁已清；AW_B 接受、正常成功翻译，physical AW_B handshake，把 B 加到输出 W 队列尾。
3. B 的源 AR 已翻译且可见。strict `can_drain` 要求两个头都是 B，故拒绝；普通 current virtual-W-head + exact matching accepted-AW 由 L3 放行，A 尾独立排完后 B 的小 FIFO 即可排空。

这是严格门的可达额外 stall，不是新实验。历史原件 `docs/composition-history-isolation-20261007.md:502-514` 已有两真实 DMA 的有限尾停顿场景，完整扫描与普通 current 门 P/PN 两对 CSV 逐字相同；本席没有重跑，亦不把此历史等同四 DMA 定理或面积测量。

### native 短包补核

- actual 测试 `nonblock_actual_dma_test.cpp:1501,2025` 配置 CONF=0。frontend `idma_reg64_frontend.sv:366-373`：decouple_rw 来自 CONF.decouple，源/目的减长度来自同一个 deburst；legalizer `:201-214,453-464` 在 decouple_rw=0 采用共同边界并同步推进。这排除本 Mode7 受限包中“一次 source R 覆盖两个独立 destination 段”的尾部设想。
- 生产允许 CONF.bit0=1 的独立边界：legalizer `:209-214`。transport `idma_axi_transport_layer.sv:314-318` 明确 R/W 1-to-2 映射，`:343-360` 的 W 来自 buffer；早先 source R 尾部在该组件里可以供应后续 destination 段。
- **decouple_aw=0 不能单独推出 AW 等 first R。** production/test 都 `RAWCouplingAvail=0`，backend `:837-854` 使用 AW fall-through bypass；AR 独立 fall-through 是 `:612-625`。两个通道在外部 mux 的等待可不同。TC AW 优先也不能只凭直觉抹去已经在途的 source R 或已经预填的 payload。
- 因为后续 miss AW 是否能在首 R 已在途时被 bank 接纳、短 W 是否在 miss 期间全部接受、另一 bank 是否能更早译完涉及实际 mux 序，本席保留 native 短包状态 **unresolved**。当前长 burst 无逆序证明已独立成立；不为此扩四 DMA 夹具。

## 4. 四项现有强对照与 O(1) 普通扩展

### B1：Mode7 准确 current/next，无返回池

`nonblock_completion_pool_late.sv:94-124` 是决定原件。current 要 job active、源/目的 job 相对偏移相等、len/size/INCR64 相同、两 W 头为该源、该目标 physical AW 已 handshake、physical/virtual 同源已完序号一致。next 要仅一个旧读、launch=tx+1=vw_done+1，**两 domain 紧邻第二项**均为该源，下一 physical AW 真正握手，旧读同 ID，并且旧 STREAM 或旧 POOL 已全部返回。Mode7 不给 pool grant（`:207-226`）。两套 W 顺序账本深度8按首次 AW offer push、真实 WLAST pop，逐拍和实际 mux 核对（`:356-374`）。

元数据每真实 DMA 8 项（`:46-64,266-270`）；Mode7 同 ID 流式 R 仅透传旧最早 metadata（`:274-291`）。armed 保持到 AR handshake；在途 physical AR 字段必须稳定（`:448-474`）；R id/last/长度逐拍核（`:489-492`）。不能把 b 当 production bank 身份；本模块的 b 是两源身份，production 四源需完整 source ID 与 epoch/segment。

### B1v：普通 current virtual-W-head + matching prepared AW，加原 B1 next

`frontier_drain(...,check_prefix=0)`（`:162-168`）直接实现 current 普通条件；其扩展保留 B1 next 的完整限定。`FrontierScanEnable=0` 准入不靠全前缀扫描，但 `:449-450` 仍执行 `check_prefix=1` 作为 fault16 见证；历史 test 实现仍有诊断扫描，不能拿它的 sim wall time 或轨迹推面积。优化后的普通实现可由 L3 撤去该诊断扫描，不是本席实施/综合事实。

### B2：普通共享可选择头队列 + 旁路

实际实现位于 `nonblock_completion_banked_ctl.sv:448-584`，不是另一个名字为 nonblock_header_park 的文件。每 bank 共 H 个 full physical AR header；按每源 seq 最早项形成候选，不同源可选择；同源有更早 parked 项就不准直通（`:482-495`）。空槽容纳捕获，满槽仍可 current 直接派发。下游首次 VALID 后锁住选择直到 handshake（`:505-524,546-565`）。每 bank 最多一头进共享输出 AR mux，两个 bank 可并行 capture、共享物理 AR 仍单口。

H=2+direct、每 DMA2槽、共享3槽无direct 的历史区别见文档`:478-486`。不能只和每 DMA4总槽的弱对照比较。此代码只接受原 IDs0/4（`:571-573`）；生产四源映射需扩 full-source key，不是当前实现。

### B3：普通每 DMA 头队列

同一实现 HeaderParkMode=2，每 bank为两个测试源分配各 H 个槽（`:449,500-502`），总 physical 配置两 bank共4H槽；不旁路。只从每源最早项派发，same-ID/VALID规则同B2。四真实 DMA 的自然普通控制是每源 H 个槽、所在 bank 的两个源头仲裁；H=2时四源共8槽的**推导实现**，没有 RTL实测。native TC每bank4 AR头与当前COMPLETE占用仍另计；不能把 parked H 说成所有可见头额度。

### B4：普通完整返回预约

Mode5 `nonblock_completion_pool_late.sv:212-218` 在本例每源一个完整256拍槽，真实 AR首次 VALID 前预留整响应，物理返回到storage直到 private DMA可消费；同 ID按最早 metadata回放（`:274-291,483-492`）。两真实 DMA附加payload4096B、DMA小FIFO48B，合4144B；R字段还存 ID/resp/last/user，未计入历史payload数。一次一个完整槽是实际Mode5配置，不是强对照永远只能一个child。广义普通控制可用 M_d 个完整槽和原生8metadata/8DMA在途，只要支付全部容量并先reserve后AR；四源完整job 4KiB/源私有返回也须对比，不能压成一槽制造收益。

## 5. B5：独立普通“接收、准备、原序 issue”算法

这是源核后形成的**手推普通强对照**，当前未实现；不是把 R2 移植到 baseline 后称前作。目标接口与预算显式给定：一个 global AW接收口、一个 physical AW输出口，原生 upstream W-selection depth8和每bankW16，正常 TL request/response单口、每bank至多一个正在准备的 owner。Q个owned AW头可用已有bank4+4 header存储作候选复用，若不复用则Q个完整头全部新增计费。

独立算法：

1. **接收**：有自由AW-cell、W-disposition和B响应记录信用时才握手接收virtual AW。按实际握手顺序把cell引用 append 到全局FIFO；FIFO位置就是顺序。原始 full header/ID/属性/length、bank、epoch/slot generation归该cell所有。AW只可接收一次；首次VALID stall后源必须稳定。允许W早于AW的接口情形只背压W到对应AW被接收，不撤销VALID。
2. **准备**：每bank从已有接受FIFO引用中取该bank最早未准备cell，取得bank准备owner；通过正常TL发送该cell虚拟地址及DID/PID/权限字段；正常成功响应写回该cell的physical AW header、ready/error状态。两个bank各可有owner，共享TL单口仲裁。不发数据AR，也不声称这个準备入口已经存在于生产TC。当前TC的COMPLETE要等AW issue才pop；新adapter必须付出owned准备头结果存储、bankowner引用和正常TL调度集成成本。
3. **原序 issue**：只有全局最老未issue cell为ready-success时，才锁定其完整physical AW并首次assert VALID；直到AWREADY不得换头/撤回。真实AW handshake后释放仅供准备/地址派发使用的字段，W及B仍活跃的信息迁移到各自已付费记录。literal FIFO无需每cell存32bit全局票据号。
4. **W**：W-disposition FIFO仍按原AW接收顺序，head含源/完整原ID、bank、success/error、len、B-record引用。非交织W逐拍更新、只由真实 WLAST推进。成功W不能在对应physicalAW实际接受前出物理口；可留在该bankTC有界W FIFO，但不可在满时继续发R而无drain/reserve证据。错误head不发physicalAW/W，应向已计费的内部error sink提交该错误AW描述并消费该写的全部W，包括在错误翻译返回前已进入TC的缓存拍；sink真正consume最后拍后才准备SLVERR。virtual WLAST已入缓存不能冒充error sink消耗完。后继同bankW不能越过尚未消耗的错误W头；sink入口VALID也须锁到握手。
5. **B归属与同ID**：保留原ID，成功返回按ID归原源；每完整ID一条接受序B记录FIFO。physical B依该ID最早尚无返回的issued-success记录捕获；error空洞在消耗W最后拍后记SLVERR。只从该ID最老ready记录发upstreamB，首次VALID锁到握手。这样较年轻成功B不能越过较早error。不能在AW issue/WLAST时无证据删B debt，也不能只给8个B记录限制原生未返B额度。
6. **R**：仍只按B1/B1v的current/next资格或已支付完整响应reserve发AR；来源/epoch/segment/read-ID作为身份。ordered AW并不免费授权任何future source R。同ID更早parked/已在途读不能被越过，首次AR VALID保持。
7. **退休/复用**：只有相关owner响应已落实、已发VALID已握手、W/B归属迁移已完成才复用cell。TL错误要等该错误写的W收齐与有信用的SLVERR队列；generation防止迟到response写复用槽。已经首次VALID的AW不能被cancel抽走。取消未发的AW但W已被上游承诺时必须转入error/drop并保序回复，不能简单删除FIFO项。缺失payload永不出现的cancel、ATOP/MRIF、异步reset/invalidation与新接口异常整合仍未证明，不能宣称一般AXI/完整job无死锁。

**与root R2成功路径的状态对应**：global ticket最小序号 ↔ FIFO head；ticket bank ↔ header ID[BankIdBit]；translation owner ticket tag ↔ cell reference+generation；prepared physicalAW ↔ ready-success cell；global最老ready-success发AW ↔ FIFO head issue；原W顺序 ↔已有W-selection+W-disposition。相同接受/TL响应/READY事件下两者成功路径AW发射决定相同，R资格沿用相同门。FIFO算法允许翻译多个已接受AW、每bank一个native正在准备owner，没有人为每DMA只能一个child。

**成本可证范围**：Q个普通header cell可替换Q个ticket header cell；Q=8复用bank4+4时每bank仍只拥有4个cell，不免费把另bank空间借成单bank8项。globalFIFO可存8个(bank,bank-slot)引用，即每引用3bit共24bit另加指针/占用；原始地址字段在翻译结束后允许合法复用作physical地址，先前原相对段偏移与B引用若仍需用必须保留并计费；显式32bit/global seq不必重复。若R2也用implicit queue位置，两者同状态家族；若R2保留32bit seq，普通上界可少这些seq位，不能把这项推导说成总面积更小。完整errorB/history/generation状态与root R2异常接口未固定，**同预算完整异常等价=unknown**，成功路径表达覆盖=proved-by-mapping。无新颖性或性能升级声明。

## 6. 成本字段与验证范围

全部基线必须报告同一成本字段集合，详见JSON `cost_field_catalog`。至少分清：DMA活跃data payload；公共TC W payload与strb/last/user；完整R槽的data与ID/resp/last/user；raw/prepared AR与AW完整header；job/epoch/segment/源/ID/权限元数据；sequence/valid/read-write pointers/occupancy；armed/held头；错误W消费/B debt/迟到TL归属；FAST分类diagnostic；每cycle capture/dispatch/metadata读写口；full宽payload mux/回放口；比较/仲裁/fanout/扫描；CPU供给/轮询；LUT/FF/BRAM/面积/时序/能耗。未综合值null，不把48B当总存储或面积。

静态参考算术（test widths，非综合）：local扩展AR146bit，completion AR98bit，raw扩展AW4-ID154bit，physicalAW104bit，local R70bit。16个virtual AR metadata字段本体2336bit；16个AW address/len/size记录1200bit；Mode7两个8项源顺序数组共16bit、4个3bit指针+2个4bit计数20bit，合36bit（额外fault计数等另计）。四源数组若保持同8项仅owner标签扩大为2bit，共32bit加20bit控制=52bit，未实现。两个3拍小DMA仅48Bdata；公共TC两bankW总32拍data=256B，字段总`32*(64+8+1+1)=2368bit`；Mode5两个完整R槽全字段`2*256*70=35840bit`，其中data4096B、额外字段384B。

这些declared字段可以因Mode7不使用POOL/fixture等而经综合消除；也可能因端口与布线实现加成本。完整module area不能由字段相加验收。B1/B1v的普通实现可直接引用原生W-owner FIFO的current/next项；复制的两套顺序数组是当前测试参考实现，不是普通控制的强制最小额外状态。直接引用要付实际metadata读口/连线，不能声称成本免费。B2同bank有效H=2是196bit完整头+64bit seq+2valid等，但源码两个bank都物理declared该资源，须计两份；四真实DMA需要重新映射source key并验字段，不能照历史“一个bank活跃”消掉真实其他bank。

当前输入证据已经足以关闭“扫描提供current安全能力”和“只用复杂扫描作为R1比较对象”；也足以给R2成功路径一个可执行普通对照。剩余问题为真正不同的成功动作或已固定完整成本下的优势、native短/独立分段具体trace、异常接口与完整面积/时序。没有性能准入见证，停止建新夹具/扩旧矩阵。

## 7. 原件入口

历史事实只读入口：`out/completion-reserve-late-20261007/codex-review.md:7-30,42-52`；`docs/composition-history-isolation-20261007.md:474-545`。前者文件当前实际共58行，任务中提及的61-70不存在；没有伪造行号或从别文件补作其原文。

完整hash表由 `source-manifest.json` 给出；本稿末尾附关键原件hash。本席未下载/查新论文，普通算法覆盖不等于该具体方法已经发表。

|关键原件|SHA-256|
|---|---|
|`out/completion-reserve-late-20261007/codex-review.md`|`0e9ac8ea14030970d8a094fadf02184715e68165970023863eb50fafc53fe5e0`|
|`docs/composition-history-isolation-20261007.md`|`e12ad5e3da0bdf4bf22bda80051d05a9e565d2b0ec087a7eb9ae750959248f1f`|
|`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`|`613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`|
|`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`|`4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`|
|`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`|`9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`|
|`tests/nonblock_actual_dma_top.sv`|`81d9566595e20f208a22db27b9b4e9e90678f8394aedb1be13f2a89fd2275c5a`|
|`tests/nonblock_completion_banked_ctl.sv`|`251a1e3b3df265934d826535fdc6166857d2b98e626a53461b5de5b96b6c710b`|
|`tests/nonblock_completion_pool_late.sv`|`c9029e1bc63df1a45c50b46a6e276920944b027545544c952a0580233527c90a`|
