# SDH1 的具体普通程序 O-NBO

2026-10-14；actual goal `1791986539`；Codex-only 新独占席。没有恢复旧席、关闭分支或运行旧实验。程序先固定，root 的新私有功能协议原型由 root 负责。

**结论：** 在本轮给定的真实信息和严格相同资源上，单 PTW 普通 nonblocking lane 加“已承诺写的当前数据生产者优先”程序 O-NBO，能逐动作覆盖 SDH1 所述 drain-qualified source hit。覆盖证据是本目录独立可执行的 `ordinary_policy.py` 和下述状态/事件映射，不是声称某篇前作已实现整项规则，也不是“普通可以任意加模块”。候选与普通都必须支付原 AR 槽访问、完整源身份、job/cursor 观察和原cell到physical AR接口；当前物理面积/Fmax 未知，不能从逻辑配对推出已测物理差为零。

## 1. 接口与固定程序

`choose(state) -> (source, slot, generation) | None`。只处理 PTW 正常 miss 忙期间的 **一次可选 hit admission**；返回 None 时保留原正常/失效路径。source 是完整、不可改变的返回身份；不以 bank、DID 或仅 source prefix 替代。同 bank 不加禁令。

必须显式给出 `ptw_busy=True`、`result_free=True`、`normal_or_invalidation_pending=False`、`grant_used=False`。active_source 是占 PTW 的原请求完整源。drain.source/job 表示当前真实 W 队头的生产者和 copy；`aw_accepted=True` **只可由该 source/job 的匹配 external physical AW 握手置位**，不能由入口 virtual AW、内部 spill 移交或 firstVALID 推出。若快照另带 aw_source/aw_job，二者都必须匹配。preceding_independent 表示当前 W 前序可独立真实排空，不依赖此次 AR；不能仅指已缓存 WLAST。

head.native_guard_ok 与 head.needed_for_head 必须显式 True；缺失为 False。后者由可信 copy 字节 cursor/区间证明这笔 AR 确为当前 W 头缺数据的读，单纯同 job 不是证书。oldest 是每完整源的最老未退休 AR；同源同时标两项 oldest 时拒绝。hit 与 permissions_ok 是本请求自身真实 DDTC/IOTLB 查找与完整支持范围/权限检查的结果，不能复用 active_source 的上下文或未来翻译结果。

固定程序：

1. 原正常结果、失效、已发布协议义务优先；结果槽不空或本 walk 已使用 grant 就不启动新可选读。
2. 取当前已承诺 physical write 的 W 头生产者 P；若 P 就是 miss source 则无异源动作。
3. 在原 AR cells 中只查 P 的源内最老 cell；其 job 必须是该 W 头 job，且必须是该头真实缺数据的请求。
4. 它若通过真实 hit、权限、native guard、前序独立排空条件，选择该 `(P,slot,generation)`；否则不选后面的同源读或其他来源来冒充这个 grant。

不使用候选函数、oracle、未来 trace、估计 DDR 延迟、另一 walker 或第二结果槽。由于当前 W 只有一个实际 owner，且每源只有一个最老 cell，合格选择至多一个，程序无需另付年龄链或 RR pointer。`grant_used` 在实际许可握手后置位，到该正常 walk 生命周期结束才清；选择/探测本身不消耗 grant。

## 2. 原生源码边界

以下为本席直接读到的关键源，源码根为 `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/`：

|直接源证据|实际含义|
|---|---|
|`riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:282–306`|旧 N window 要求 PT/S1 normal walk，且 candidate.bank != active.bank；没有同 bank N acceptance。|
|`riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:42–46,395–408`|bank 单独够用依赖每 bank 至多一在途 translation；响应仅按 bank valid/ready 路由。|
|`riscv-iommu/rtl/rv_iommu_top.sv:135–165`|请求/响应只含 bank，不含完整 source、slot、generation、job。|
|`riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:146–164,346–357,692–725`|原 AR FIFO 只露一个 head；物理 AR header 仍读该 head，响应登记在单 q，COMPLETE 握手后才 pop。只添 tag 不会自动随机访问。|
|`riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:36–55,84–112,143–172`|已有一个 req/DC/DDTP/resp snapshot 和有限 DC/LEAF/OUT FSM，共享真实 cache 口；接受时核本请求/配置稳定、权限与失效，实际接受才 touch，OUT 持有结果至 ready。|
|`riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:346–352`|已有 fast/slow 两输入、一个返回口；当前 fast 结果容量不能扩充后仍称一个槽。|
|`riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246–277`|现有 W/read guard 只知 bank 和 signed AW-minus-input-WLAST；不是完整 source/job 的 external AW 证书。|
|`fpga/src/ariane_peripherals_xilinx.sv:870–878,1228–1245,1255–1281,1404–1409`；`tb/ariane_soc_pkg.sv:31,110–132`|四 DMA、总 ID4/local ID2，bank bit2，所以原 ID0/8同bank、4/12同bank；AR总8、cache16、W32、一个共享TL。非legacy DMA0/1各512×8B=4096B，DMA2/3仍3×8B。|
|`iDMA/src/idma_fullbuffer_certificate.sv:16–18`；`iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173–183`|fullbuffer只允许完整、页对齐4096B normal copy；64B没有该原生许可。当前已接受 job/退休的观察不自动给新 source/job/cursor 证书。|

旧资料 `docs/research-n-prototype-spec-20261004.md` 只作导航，不据其旧状态推当前构建或板测。旧 BO 的 `control/fixed-controls.md` 对“具体固定程序”和“候选等价重写不能冒当前作”的区分，以及 `docs/r2-ownership-cost-20261014.md` 对字段复用/外部AW/首次VALID的约束，本轮继承但未运行其程序。

## 3. 同资源实现和逐步映射

O-NBO 的普通实现不是银行 FSM 的免费第二 owner。**root实际私有原型采用更紧的direct OUT，两方共同采用这个adapter：** 原 AR 总 cell 数保持8；完整 raw header始终保持在原槽；扫描原槽得到每完整源最老引用；一个真实 N cache lookup lane 和一个结果槽。该原N OUT存PA/许可与完整 `(source,slot,generation)`，直接读匹配原raw header生成被锁physical AR，直到真实physical AR接受才移除原槽；没有给每个raw cell加PA writeback或READY许可槽。normal TC PA寄存器仍独立保留，normal completion与fast OUT可以共存；normal-priority和首次VALID锁由共同adapter处理。原cache/PTW/fast FSM primitive可保留并扩标签/路由，不新增miss walker。READY union写回是双方都可选的另一组织，不能当成candidate或ordinary的必需附加成本。

一个具体、无需年龄数组的普通槽组织是原序 ring + tombstone：normal miss仍占原cell；合法旁路完成的年轻cell标退休；不复用中间hole，只有连续退休前缀被原head回收后才重新分配。总容量始终原8，没有另藏“原FIFO以外的8slot”；代价是hole可能暂占capacity。候选也可用相同组织。若root选择即刻回收任意hole的allocator，两方一起使用并计入该allocator、最老判定和generation，不能以这种实现差异强造普通的额外成本或覆盖失败。

|SDH1事件/状态|普通 O-NBO 的独立实现|同资源约束|
|---|---|---|
|一个normal miss拥有PTW|普通MSHR owner引用原cell，保存active_source|相同一个PTW、相同owner引用/normal snapshots|
|查各完整源最老|原ring按接受序扫描，源内第一活项为head|同8cell、同随机读/mux；无额外request admission|
|source-drain资格|当前已承诺W的数据生产者是P；只读P最老且实际needed区间|同job/cursor/physical-AW证书、同比较器|
|accept合法真实hit|普通nonblocking lane接受P原cell引用，锁generation，置grant_used|同唯一结果槽、同normal/失效优先级、cache仅一次touch|
|结果fullsource回原AR|单N OUT持PA/许可/tag，核P/slot/gen并直接读对应原raw cell|同tag宽、同一OUT、同raw header读口；无percell PA写回|
|物理AR首次VALID|从该OUT与raw cell生成不可更改header，锁offer|同heldVALID adapter；选择程序不能重选已发布offer|
|外部physical AR接受|转交原R/ID owner；cell按共同退休规则释放|同源序/同ID顺序，不能以TL完成冒R完成|
|真实R/W/B和consumer结束|原数据与response trackers消费并退休|同原buffer、信用、error sink、completion counters；AR slot释放不等于R债务退休|
|失效/取消/陈旧snapshot|新hit不接受；已发布义务按共同协议drain，tag防旧结果污染|同epoch/取消scope；不能仅调用choose便称一般安全|

**精确覆盖证明的范围：** 两方看到同可信snapshot，支持类别、native安全检查和发布/退休adapter相同。有效W头确定唯一P；源内最老确定唯一cell。SDH1描述的动作资格正是“PTW忙、结果空、无优先normal/失效、未用grant、异源P、精确job且needed、已接受匹配physicalAW、前序独立排空、本请求合法hit”。O-NBO的步骤1–4和代码逐项检查该集合，因此每个有效snapshot上返回同一引用或同样None。由相同cacheprobe串行、formal hit accept、direct OUT/firstVALID锁、physical AR接受与原R/W/B退休事件归纳，下一个同adapter状态也相同，整个具名事件序列不产生额外可用动作。root原型的instant-hole-reuse可由同generation+age实现；软件untimed整数不证明硬件order字段或ABA回绕免费，两方须按同合同支付。这里是该具名程序的构造覆盖，不是全软件/全nonblocking族定理；也没有证明既有生产N已经安全实现这些adapter。

## 4. 其他指定普通对照的适用范围

|对照|固定具体动作|能说明什么，不能说明什么|
|---|---|---|
|generic hit-under-miss，无owner|按同安全filter收集源内最老合法hit；无W提示时固定来源序/RR/age选一个|若同一时刻只有P合法则同动作；多hit争槽时可选非P，不精确覆盖owner决定。不得为了凸显候选把本可实现的owner程序排除。|
|RR|有合法head时按完整源RR，实际hit许可后推进pointer|有限机会下公平；在一次grant与一个槽下不能保证先服务当前W producer。pointer、selection锁均收费。|
|age|全局最老的合法head先；接受序从原ring取得|保留年龄选择，无需凭空分配大timestamp；年轻P与更老非P均合法时不同。当前源内最老约束不是全局年龄优先。|
|full-return预约|先核证明本读全部返回字节可进原buffer，并为其保留原额度，然后普通hit-under-miss|完整页、无其他占用的4096B copy可安全使用已有fullbuffer；多job/非页对齐/大于原剩余容量不得套permit。预约不自动给slot选择、owner优先或同bankfullsource响应；普通预约也可与O-NBO相同组合，同样付费。|
|DS/Destination-first|在正式发源数据前先通过正常翻译和成功许可，为本copy目的W准备真实drain|若此时physical AW已经合法接受，减少读→写循环；不能解除另源占用本bankFSM或免费证明当前W exact job。DS准备通道与SDH1动作域不同。|
|preprepare|沿已有正常翻译先为目的head准备checked physical结果，再原序派发|可提前打开目的；每bank结果bank标签、原slot owner和真实AW接受仍限制重叠。原位header复用普通可做，前轮O-B5S不能被强迫复制prepared header。|
|single-DMA串行|只开一个source，按AW先行和原R/W/B完成再下一job|消除跨source同bank事件，通常更低并发；不能据此证明共享场景无新动作，也不能在同并发目标下冒等价性能对照。|

RR/age/no-owner的不同决定不是SDH1独立机制证据，因为这里具体O-NBO已经同信息、同动作覆盖。full-return若具名工作流满足其条件，可能连owner过滤都无需，但要实际原剩余容量证明；本席没有运行新性能来扩大该主张。

## 5. 次候选：A/B真实内容双页交换

冻结两页A、B的旧值，DMA0读A写B，DMA1读B写A。两个原4096B buffer分别持有old(A)、old(B)，不以映射交换、修改已发布AW、换generation或消费者重新解释页名代替真实字节交换。

普通两阶段程序 O-SW2（只有同前门/回执接口确实可用时）：

```text
before starting either normal copy:
  set gate0 = gate1 = CLOSED      # BEFORE its first AW VALID to IOMMU
  acquire original buffer0/buffer1 owners; fix source/destination/job generation
  start DMA0 normal(A -> B), DMA1 normal(B -> A)
READ_PHASE:
  accept all legitimate R into original buffer0/1; consume no old source by write
  record complete source receipt, exact 4096B and matching generation for both
  if either read faults/cancels: run the paid error/drain path, do not open writes
  once both old pages are entirely stored: gate0 = gate1 = OPEN
WRITE_PHASE:
  each first physical AW offer is stable, normal permissions and target binding
  original W consumes old(A)->B, old(B)->A, followed by actual target B responses
  declare swap complete only after both full writes and all required consumer receipt
  release both original buffer owners
```

数据证明很短：在任一destination发生首个W之前，两份旧页已经独立存于不会被该写覆盖的原buffer；后续W因此只读取各自旧页。每个source一次4096B R、每个destination一次4096B W、每DMA两次256beat INCR burst，则总8192B R/8192B W、1024 Rbeat/1024 Wbeat、4Rburst/4Wburst/4B是合同算术，**不是实际新native执行**。若copy分段配置不同，burst/B数量要按真实分段重算；本席不把两burst当普遍常量。

条件覆盖：候选若仅做“先两页完整保存，再开目的AW”的两阶段调度，O-SW2在相同可编程gate、read-complete回执和buffer owner条件下逐步复现；这给出普通控制程序，不证明既有软件能够控制这个内部阶段。若原接口没有这些控制/回执，软件不能只靠“先启动read再启动write”使用同一内部4096B buffer；它须新增同一付费前门/回执接口，或使用另一个软件可见scratch buffer及额外数据搬运，后者不属于严格同资源对照。

直接边界：当前 `fpga/src/ariane_peripherals_xilinx.sv:1269–1272` 使两buffer只在非legacy DMA0/1拥有4096B且RAW_COUPLING_AVAIL=0；`iDMA/src/idma_backend.sv:328–353,837–850` 的normal legalizer本身生成R/W并通过AW fall-through向后；`iDMA/src/idma_pkg.sv:13–20` 的READ_ONLY只写rdata寄存器、WRITE_ONLY只取wdata寄存器，不能当成访问原整页buffer的两个software stages。本席没有证明native MMIO已有AW pre-VALID gate，root沿实际控制接口作最终可控性裁决。AWREADY背压发生在已发布VALID之后，不是上述可撤回前门；gate不能撤回任何已发VALID。

root本轮亲核并通过agent交接的新事实另列，**不是本席新增native执行**：每backend AW fall-through有1槽，共享AW spill有2槽；default coupled在gate后第二AR的生成被w_ready阻挡。已有CONF.decouple=1可使R/W独立，first AW held在各DMA fall-through，second AW停在mutable legalizer时，两笔256beat AR可以完整读回；仍须在pre-source-mux首次AWVALID之前增加付费gate，并保证没有旧W tokens让data提前逃逸。这提供有条件可达构造，不证明旧软件已有前门API。O-SW2和硬件候选都使用这份共同配置/存储/旧token约束，不能给普通一侧强加default coupling。完整成本在 `cost.json`。
