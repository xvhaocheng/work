# 首 PTE 意图与 legalizer 分段：源码驱动的成对设计与裁决

2026-10-16，`FIRST-PTE-INTERFACE-COST-1`。ARS-Codex 有界机制设计与定向核查；Codex-only。本轮没有新性能模型、旧70条重跑、RTL仿真、综合、生产修改或硬件动作。证据、请求配置与实际执行边界见[本轮接受记录](../out/first-pte-interface-cost-20261016/root/root-result-acceptance.json)。

## 1. 一句话决定

**值得将 M-ID 的同拍捕获版本与复用现有 TC 保持槽的 O-tail-prepared 做一次最小局部成对实现；结束“票据天然比 legalizer 分段少状态、更便宜”的论证，只保留“及时保住原 burst，避免新增 child 翻译与完成回授能否改善完整 COPY”的具体假设。**

这次已解决关联与控制复用的设计问题，并把强普通方案收紧到无需新 header FIFO、只需一个完成 sticky bit 的版本。当前没有证据证明 M 更快、更便宜或已构成论文主线。旧条件模型中普通分段 **2058 比票据2060快2**，继续有效；局部实现的理由是两者实际控制位置与翻译路径不同，不能把这组数当作当前 RTL 的成本比较。

## 2. 两个最小可执行设计

### 共同任务、信息与正常 COPY 资格

固定 DMA0/1、各一个正常、4KiB 对齐的4096B COPY；两个既有512-beat完整 source buffer、原 parent、映射、8192B有效读及8192B有效写不变。只允许这两个任务活动，输入与页表在 pair 排空前稳定。不使用未来 PTE 内容或预知 cold/hot 选择策略。M、O 获得相同公开任务配置，新增接口也向普通策略开放。

**独立性来自 owner 在 quiet 时确认的实际物理范围，而非 bank：**要求 `W_V∩(R_A∪W_A)=∅`、`W_A∩R_V=∅`，也不覆盖页表／控制内存；禁止相互等待的应用依赖。现有驱动确有分别分配源／目的并独立初始化的正常 COPY 来源（[idma.c:354–399](../linux/drivers/misc/idma.c#L354)、[两任务分配:2448](../linux/drivers/misc/idma.c#L2448)），但它**尚未输出 M 的 pair-qualified 信号**。局部验证可以冻结此正常来源；运行部署仍需 quiet arm、选中 A/V、生命周期失效与必要状态/CPU开销，不能偷借未定义 ABI 字段。

M 只门控 exact A **来源**的新 payload read；不挡 V 自己的读写、DS、R/B返回或任何已承诺工作。quiet arm还要确认V的S1根页表整个4KiB位于本profile的DRAM译码范围，根/context及路由在pair内稳定；P1地址由根PPN和已知IOVA顶层index形成，无须未来PTE内容。这是ID-only关联的适用条件，资格产生的成本另计。源完整、V 持有 W owner 是旧数值正例的性能相位，**不是安全资格必须等待的输入**；否则容易把意图拖迟。已有 fullbuffer certificate 仅证明容量及正常4096B资格，不证明源已收齐或跨任务无依赖。详见[资格与循环等待分析](../out/first-pte-interface-cost-20261016/root/copy-qualification.md)。

本设计绑定当前可配置的 banked/fullbuffer 路径，不把源码默认 mode0 当作已启用两 bank。P/PN 运行选择、DS delay、共享选择策略两侧保持相同。当前共享 AR mux 是 round-robin；旧模型的 DS-first/F4 是明确的模型策略，若局部实现采用它，**两侧共同实现、共同计费**，不能冒称 stock 已有。

### M：M-ID-ordered，f=1 同拍捕获

**执行点。**在真正 PTW 接受 normal S1 start 的条件上生成事件，在 TL 将 write 类型、bank、pair epoch 与事件配对，送到 DRAM 目标 mux 的 AR selector，位于其 output spill **之前**。全程同一个 SoC clock，CDC 在后面的 DDR converter；不需要为这条路径添加异步队列。接受一次合格 start，不等待首 PTE 内容，不把 PTW busy 上升沿当 start。

**及时性与身份是两项判定。**令 e0 为真实 start 边缘，Ck 为边缘后的周期区间。去掉人工 DS50 延迟推最短路径，源码给出：

| 事件 | 最早位置 | 与其他事件的区别 |
| --- | --- | --- |
| 真实 start | e0 | PTW 从 IDLE 接受 `init_ptw && !edge_trigger` |
| 首 PTE 的 PTW-local VALID／接受 | C0／e1 | 接进 DS AR spill，不是共享服务接受 |
| DS spill 输出／xbar ingress 接受 | C1／e2 | 之后仍有 ingress spill 与 multicut |
| xbar ingress 输出／multicut 接受 | C2／e3 | 尚未进入目标 selector |
| 首 PTE 在目标 selector 首次 VALID | C3 | 最早在此建立 held 义务 |
| selector 向 output spill 输入接受 | e4 | **消费票据的唯一 P1 commit 点** |
| output spill 向 DRAM/CDC/MIG 继续传播 | C4以后 | 各自是不同端点，不能反过来撤销承诺 |

f 表示控制通路寄存级数，包含 gate 捕获、无排队。主版 f=1 在 e0捕获、C0生效；f≤3 严格早于 P1 首次可选。f=2仍能正确关联 P1，但会错过 C0已经首次 offer 的 L，**不能继承旧模型2060**。f=1源到 gate 的组合长线能否满足时序未测。这个证明利用三段实际前向存储，不借 DS50；修改 cut/profile 必须重算，不将模型时间单位写成 RTL 周期。

P1 用既有 `{IOMMU_MEM, PTW_ID0}` 全 ID 识别，本位宽为6'd48。安全依据一起使用：fresh start 前上一 walk 已串行收完 R；控制严格早于本次 P1；本次第一笔相符 ID 在 e4消耗 pending；P2 必须等 P1 的 R 真正回到 PTW 才能产生。因此无需每笔 DS marker 或另一个 occurrence counter。**单 walker、相同 ID、busy、timeout 单独均不够。**若控制任意延迟，`P1 commit→P1 R→P2→late intent` 会误认 P2；本版不允许这条时序。f≥5/可排队通路应停止 ID-only 版本，必要的 receipt fence 或 M-tag 都另计成本，不作为本轮隐形补丁。

**状态转换。**gate 保存 `pending`、H倒计数、V bank、pair epoch、escape。IDLE遇 fresh 合格 start→LEASE；只在未 locked 的选择中抑制 exact A，倒计数不续租。匹配 P1 在 output-spill 输入握手→清 pending、放行；H到期→PASSIVE，停止 gate，**保留关联**直到 P1 commit 或 PTW 真正 retire。沿相同同步 bundle 导出既有 PTW active，处理真正fault/retire后的未消费记录；active回到0才清，不能用 H 猜完成。**active本身不能修复“P1成功访问另一target，P2才到DRAM”的误认，所以非DRAM根必须跳过，不得只靠retire兜底。**start 与旧 retire 同拍按旧记录优先，宁可跳过新优化，不延迟重放 start。

每次 episode 结束后至少有一次可用 non-DS 接受才允许以后新 start 再 arm；不能把整 walk 变成预约。disable/资格失效只停 gate，不清 held/已接受 AXI；reset 依赖共同 transport reset 或先 drain，单独重置 PTW 不属于本版合同。永久下游不返回时允许 PASSIVE 长留，不能据此声称系统已完成。

**保持和成本。**复用原 arbiter `lock_q/req_q`；locked 请求优先于新 mask。屏蔽 eligibility 时同时限制上游 READY，避免 `AxiVldRdy=1` 对被 mask 的 raw VALID 产生假握手。不能直接在既有 mux 输出对 VALID 做 AND。新增 gate 逻辑存储为 `W_H+3+E` bit，`W_H=ceil(log2(H+1))`，不含 pair 配置／资格部署成本；f=2再增加 `3+E` bit。PTW→TL→top→peripherals→xbar→selector 的连线、比较、timer、扇出和寄存时序都要付费；**这不是净FF/面积测量，也不保证比O少**。原AR/R结构、6-bit全ID、payload RAM均不拓宽。完整偏序和退出规则见[M设计](../out/first-pte-interface-cost-20261016/m/source-design.md)。

fullID可识别 A 来源，**不能分辨同一个 parent 的 prefix 与最后 AR**。此安全版允许保护期暂缓 A 的任一新 AR；若论文只声称“L 已错过后绝不影响 A 的其他 AR”，须另加可证的窗口身份。当前正常 pair/窄末段数值情形不能冒充该能力已免费具备。

### O：legalizer 复用；以 O-tail-prepared 为强普通版本

**直接可复用。**现有 legalizer 有 source/destination 独立长度上限、剩余长度/地址和读写解耦；原 read metadata FIFO、AR fall-through、W计数与最后B的parent完成都保留。frontend当前未暴露 source128/pacing 配置，但无需重写 splitter、增加 payload buffer 或普遍新建返回重组。`NumAxInFlight=8`不是 pacing 开关，也不能直接设1（源码禁止）。

**O-tail-local 的完整控制。**原 A source 是256＋256 beat，只将末256改为128＋128。给 source设7、destination保持256、`decouple_rw=1`；legalizer在 remaining>2048B时仍按256，到 tail才切 source cap128。remaining2048产生L0，remaining1024等待；用现有 metadata FIFO **nonempty** 禁止 L1 descriptor，直到 prefix及L0的真实 accepted RLAST 全部 pop 后才准入。FIFO满时usage截断可能为0，不能用`usage==0`判空。

gate 接在 legalizer descriptor 准入点：`r_ready=metadata_ready && AR_holder_ready && admit`，由同一个 ready-coupled `r_valid`同时进入metadata FIFO与AR holder，避免重复收描述符。已经进入 holder 的 AR 继续原VALID/READY保持，不再受 admit 撤回。注册 empty 版不新增独立 pacing state；下一段首次offer严格在RLAST边缘以后。组合 last-pop lookahead可同边缘准入，但增加 R-ready→legalizer→AR 路径，不能称为严格 after 的线网发布。frontend `trans_complete` 等的是最后写B，**不能拿来释放同parent的第二段读**。

**完成驱动不等于旧模型的立即共享发布。**O-local 到 L0 的DMA端 RLAST 后才生成 L1，后面还要TC→TL→物理AR→xbar。banked路径 N关闭时，TL normal gate在V walk期间不接普通新请求；N打开也只对满足DC/4KiB leaf/权限等条件的另一bank命中绕行，不是免费零延迟。因此“只改读长度”“FIFO空后发下一段”和模型 t=128 已接受物理L1是三件事。

**建设性补强 O-tail-prepared：提前合法化/翻译，复用 TC 的已有保持槽。**不在descriptor入口等 L0；两个128 child都可尽早进入原正常翻译。固定 aligned4096、同一A单parent时，在A bank TC内部以原 `ar_data.id`、offset=0xC00、len127、size3、INCR识别L1。在其 COMPLETE 首次physical offer前等待L0完成；原 ARFIFO head、`trans_resp_data_q`和COMPLETE状态保存它，**不需新增header槽或通用tag FIFO**。必须同时 gate `complete_read` 与COMPLETE退队条件，不能VALID为0却见READY就pop。

L0身份由descriptor remaining2048产生marker；本profile `RAW_COUPLING_AVAIL=0`，可以有条件复用rmetadata中仅disabled coupler读取的 `decouple_aw` bit，FIFO不增宽。DMA backend增加一个 `l0_done` sticky bit：本parent backend接受时清零，marked head的真RLAST pop时置1，保持至该parent结束／下个parent接受；一根level回线送A TC。L0先完成、L1后到COMPLETE也不会丢事件。不拿aggregate RLAST、FIFO empty或下一次同ID响应猜L0。该复用仅对已核RAW=0及单parent成立，其他profile须恢复字段语义或付新marker费用。

这版等待占用A现有TC，阻塞该bank后续AW/AR翻译；不门控已有W/B和V bank。额外lookup若能在V walk前完成，O不受O-local的串行限制；若未赶上，仍可能等待。普通方案得到合法提前准备机会，**不能强制它拖到walk后再翻译以制造M优势**。它依赖实际两child准备时机、完成回线和原TC容量，而非全知/无限缓冲。详细FSM、接线与版本限制见[O设计](../out/first-pte-interface-cost-20261016/o/source-design.md)。

**O-job必须单列。**若两个job全部用128，则各4笔read、仍各2笔write；每段均用metadata FIFO nonempty pacing，免tail比较但改变整个前缀与重叠。不得将其新前缀删去，或套用只拆A末段的2058。

**共同事务与错误责任。**两版均保留原4096 parent、属性、权限、数据次序、四个AW/B以及最后B才退parent。现行wrapper选NO_ERROR_HANDLING，backend强制parent.error=0，这是共同缺项。若要求显式失败报告，可共同增加parent sticky error、真正RLAST才释放及最后B错误发布；不能只向O收全套错误成本，也不能直接开启generic error handler而让当前frontend把中途错误当退parent。当前报告不声称错误/取消/reset已验证。

## 3. 成对比较：已知、推导与未知

| 项目 | M-ID f=1 | O-tail-local／强O-tail-prepared | 证据层次 |
| --- | --- | --- | --- |
| 改变什么 | 原已翻译burst尚未首次共享offer时，为真实首PTE留一次有界先行机会 | 先缩短不可抢占读；local等RLAST再翻译，prepared先翻译后等RLAST发布 | 源码绑定设计；均未实现 |
| 必需信息 | 合格真实start、bank、pair资格、既有PTW ID、active | 普通长度/地址、单parent、真实RLAST；prepared加marker及sticky level | 来源与获得时机已定位；新增接口非免费 |
| 原payload AR／源侧翻译请求 | 4／4 | tail 5／5；job 8／8 | 按两4096任务的结构推导，不是执行计数 |
| cold总AR／前缀payload AR | 模型固定三PTE时7／3 | tail 8／3；job 11／6 | 三PTE只属旧固定翻译史，实际竞争变更未保证仍三PTE |
| 有效数据、写完成 | 8192B读＋8192B写；4AW/4B、两parent和最终drain | 相同；child RLAST由原DMA读metadata消化，W长度仍256 | 工作量相同；错误与实际drain未新测 |
| 新状态/缓冲 | gate角色bit `W_H+3+E`，pair配置另计；0新payload RAM | local可0独立pacing bit；prepared 1个sticky，marker复用已有metadata；0新header/payload RAM | RTL设计角色核账，非合成面积 |
| 额外连线/组合路 | start/active/bank/epoch长线→gate捕获；pending→mask/READY | local空判→ready；prepared marker生成、done→TC gate；tail比较/mux | 寄存边界已明；路径延迟、扇出、Fmax、布线未知 |
| 完整时间 | 及时才有机会，f增加可能直接错过L；不可仅加常数 | 额外child准备可隐藏，也可被TL忙/回授延迟暴露；可能改变服务次序 | 未有新RTL时间，均不填h/a估值 |
| 部署成本 | pair独立性产生、quiet arm/失效、跨多wrapper接口；不改已接受协议 | 现CONF不直接具备：静态策略/新配置、tail接线；prepared需done回线和TC适配 | 真实新增职责；CPU/面积/内存占用未测 |
| 对资源的影响 | 原burst保留、原槽占用可能延长 | 一个额外AR/TL请求；prepared占已有TC更久；job更多 | “占已有槽”不等于新物理RAM；总DDR流量/CPU另测 |

**旧已执行结果原样复用：**下表均是完整起点到quiet的**模型时间单位**，不是RTL周期。新M-ID／O-local／O-prepared均未取得这些数值。

| 冻结case | 原普通 | 原模型票据 | 原模型paced-split，a=0 |
| --- | ---: | ---: | ---: |
| cold | 2066 | 2060 | **2058** |
| hot | 2050 | 2050 | 2050 |
| 短gap反例 | 2059 | 2060 | 2058 |
| 长gap留出 | 2070 | 2061 | 2058 |
| 意图迟到 | 2066 | 2066 | 2058 |

固定G2在cold追平票据、hot慢2；同接口普通if/else与票据逐事件相同。后者在runner中共享实现分支，是能力消融，**不是两份独立实现交叉验证**。2053的提前prepare/细分组织仍只是手工条件构造。全部guard与PTE/L/双方B向量见[旧原始结果](../out/mechanism-construction-20261016/model/results.json)及[定向核对](../out/first-pte-interface-cost-20261016/comparison/evidence-notes.md)。

**修正费用解释。**旧split在L0 RLAST的t=128同边界就接受物理L1；P1早在t=2已接受。runner的`a_split=4`在[128,132)占共享server，连已接受P1的数据服务也推迟，**不是额外AR必有4拍，更不是本地release延迟**。只有仍及时、共同前缀不变、服务次序及critical path不变且费用确在对应不可隐藏路径上时，才可写：

`T_ticket=2060+h_e`，`T_split=2058+a_split`，故主cold票据胜须`a_split>2+h_e`。

h_e是仍保住窗口后的额外暴露费用，a_split是上述位置的暴露serial费用；面积、CPU、Fmax和隐藏开销均不能擅自换算进去。若新延迟只拖L1发布，P1可能照常服务；若L1拖到P2之前/之后、或intent晚于L，竞争顺序已变，必须按实际事件比较完整任务，**不能在旧数上加常数**。当前只保留此算术适用域，不再用不等式作为继续制造文档的理由。模型H=16也不是原生16拍；当前DS50前向配置下，原生H必须按其实际单位与回退合同冻结，不能暗中移用或调参取胜。

## 4. 决定结论的原始证据

顶层实际branch=`main`，输入HEAD=`e79a476c9332d3892a0681957dc85933addda8c5`。CVA6 HEAD=`a1b08934391833aed21e6ad63b545634040808ac`；IOMMU HEAD=`57eb5621802ef9ed8d77c684b9a03205c6630391`，lock仍指向`238a39e...`。目标集成/PTW/TL/DS有既有工作树overlay，本轮绑定**实际文件bytes/hash和目标diff**，不把clean HEAD或既有板载映像当当前文件。顶层无本轮生产修改；其他任务日志、历史未跟踪构建物保留。

| 决定问题 | 直接来源与行号 |
| --- | --- |
| 真正start及AR→R串行 | [PTW:315](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv#L315)–365、384–414；首AR只在MEM_ACCESS，下一PTE依赖R |
| DS/共享spill与held | [DS:81](../cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv#L81)–109；[axi_mux:333](../cva6/vendor/pulp-platform/axi/src/axi_mux.sv#L333)–364；[rr tree:151](../cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv#L151)–197、274–282 |
| P1三段最短前向存储、实际vendor/clock/reset | [M原文摘录与hash](../out/first-pte-interface-cost-20261016/m/evidence.json)：top、DS、axi_demux/axi_multicut/axi_cut，非IOMMU内另一vendor副本；[集成xbar:291](../cva6/corev_apu/fpga/src/ariane_xilinx.sv#L291)–319、1006–1083 |
| 分段原有cap与ready耦合 | [legalizer:139](../cva6/corev_apu/iDMA/src/idma_legalizer.sv#L139)–163、201–249、301–308、449–463 |
| metadata、AR holder、最后B | [backend:471](../cva6/corev_apu/iDMA/src/idma_backend.sv#L471)–473、524–526、554–570、610–625；[transport:257](../cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv#L257)–282；中途error不是RLAST |
| O-prepared无新header槽 | [TC:108](../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv#L108)–164、346–357、674–725：ARFIFO head/PA保持，COMPLETE控制发布和退队 |
| O-local无法假定walk中零延迟hit | [TL:284](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L284)–326；[N资格:84](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv#L84)–103 |
| 两COPY来源、容量非source_done | [driver:354](../linux/drivers/misc/idma.c#L354)–399、2390–2455；[certificate:16](../cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv#L16)–47 |
| 旧结果和费用注入点 | [results:7](../out/mechanism-construction-20261016/model/results.json#L7)、43、381及各case；[runner:51](../out/mechanism-construction-20261016/model/runner.py#L51)–62、77–160；[旧实际执行回执](../out/mechanism-construction-20261016/model/execution-receipt.json) |

输入版本在[input binding](../out/first-pte-interface-cost-20261016/root/input-binding.json)，root新核源码在[critical evidence](../out/first-pte-interface-cost-20261016/root/critical-evidence.json)，M/O各证据manifest记录具体行文、component差异及hash。结构/算术检查不称RTL验证。本轮不核对所有历史hash、不修lock、不重建平台。

## 5. 论文贡献判断

| 层次 | 本轮到达哪里，不能推出什么 |
| --- | --- |
| 功能可实现 | M的关联、晚到、held、有限退出已有具体FSM；O-local与O-prepared已落实到现有ready/metadata/TC保持。**源码可实现设计，未过RTL验证**。 |
| 条件因果链成立 | 旧模型证明一次早P1顺序可改善完整两COPY，也有hot、短gap、长gap、晚到边界；未证明原生共同前缀和有效窗口自然出现。 |
| 相对普通方案有差异 | M保留burst、无需额外child翻译；O-tail复用分段、无需早PTW信息、可提前准备。O-prepared只新增一个释放sticky，不应被画成重型splitter。差异具体，但不是M净胜。 |
| 差异具有实际价值 | 完整时间、布线/时序、全部新增成本未测。旧零暴露split更快；M的少一AR/lookup不是性能或论文价值证明。CPU/内存若有独立改善可作价值，但本轮没有该实测。 |
| 与最近前作独立增量 | 调度算法独占主张已停止。早start→首次共享承诺前的一次有界接口是待验证的窄增量；只有其完整服务/成本或部署能力优于具名普通组织，才有主贡献依据。没有完成穷尽查新，不声称首次。 |

最接近工作的**已核原文**限定了增量：

- **[MASK](../../references/summaries/mask2018.md)**，§5.4 PDF p8：DRAM controller内Golden/Silver/Normal队列、translation优先及app配额，需已有请求分类/并发walk与blocked-warp信息。§7.4 p12的6%是request queue storage增量，不能拿来当本接口面积。它已覆盖“联合考虑翻译与数据”的宽思想；本候选差异只在accepted FIFO不能追越时、P1尚未到达之前的合法控制位置与成本。原文[txt:881](../out/shared-translation-literature-20261006/papers/mask2018.txt#L881)–932、1230–1235。
- **[AXI-REALM v2](../../references/summaries/axi_realm2025.md)**，§III-A pp3–5：已有透明granular fragmentation、header持有、B合并与原parent RLAST；§IV-B p7说active write buffer多1cycle，**不支持每个额外AR必有正泡**。候选不能认领透明合法分段，O有复用/适配机会。原文[txt:287](../out/rule-novelty-check-20261007/papers/axi-realm2025.txt#L287)–304、604–616。
- **[iDMA TC2024](../../references/summaries/idma_tc2024.md)**，§II-C pp266–267：legalizer用户长度限制、读写解耦、FIFO/generic byte stream与可选整transfer SRAM已有；首request 2cycle并非每child固定2cycle。本地新增的是paced控制、tail适配或完成回线，不是购买整套DMA资源。原文[txt:250](../out/paper-driven-innovation-20261008/papers/idma2024.txt#L250)–295、325–347。

这三篇的定向章节未给出本M-ID偏序与身份合同，**不等于证明文献中不存在等价接口**。普通方案拿到同付费intent后同样实现M动作，只排除策略独有性；不是独立旧论文覆盖接口的证据。反过来，本平台尚无接口也不证明论文创新。现有vendor `axi_burst_splitter.sv:18`仅提供single-beat分段，不能冒充现成2×128 pacer，但普通post-TL适配仍允许，按真实新增职责计费。

## 6. 一个最小后继任务

**建议、未执行：FIRST-PTE-PAIRED-LOCAL-1——复用现有完整COPY夹具的局部RTL成对实现与控制锥成本比较。**不重跑旧70条、不新建性能模型、不改生产树、不上板；只在独立测试副本接入上述M-ID f1、O-tail-local、O-tail-prepared，O-job作为独立全job配置，不能用其时间冒充tail。复用[现有system夹具](../tests/nonblock_actual_dma_system_top.sv#L1)的真实DMA/TC/TL/DS/xbar与既有行为DDR服务，关掉无关合成CPU和旧实验控制，不增加服务能力；该夹具无MIG/CDC，不称原生应用。

只冻结一份正常两COPY程序的cold/hot输入和共同服务配置，允许O合法尽早生成/翻译child。从配置/quiet起点走真实前缀，**不强制加载“V源已全、L已翻译”的内部状态，不按M时刻对齐普通方案**。现夹具把0…4GiB全部译码到DDR，而板端窗口是[0x80000000,0xc0000000)；局部副本应对齐冻结profile的译码并将测试table/root地址放入该窗口，不能把原低地址测试root冒充生产DRAM资格。H只按固定前向路径/当前delay单位选一个有界值并公开，不搜索有利相位；旧反例继续保留，无需重跑其Python矩阵。

一次任务内测：实际start→gate、P1各端点、L首次/held、L0 DMA真RLAST→L1翻译/物理firstVALID、双方最后B及最终quiet；核对8192B读写、四B、所有attrs与held保持。O允许复用TC/marker bit的最小版本，不强加新buffer；M不能将不满足时序的长线假设为0费用。对新增控制锥做同约束的局部综合，计增量状态/组合/寄存路径与新增ports，公共policy及既有RAM不重复收费；不做全板综合/布局。工具未测项保留unknown，局部综合不等于全板routing闭合。共同error缺项对称处理，至少不能以mid-error当RLAST提前放行，不称一般恢复证明。

**通过如何改变决定：**若f1及时且关联/保持正确，在同服务下相对最强O-prepared仍有完整完成或可测成本收益，且不是ordinary可免费/低成本消去的接线差，则保留这个窄接口成本版本，收益与剩余物理部署边界分报；不自动宣布论文主线。

**失败如何改变决定：**若f1无法满足所需边界、真实程序无有效窗口，或O-prepared用不更高完整成本达到同等/更好结果，结束本固定COPY的M-ID成本主张，保留普通接线与条件结果；不自动扩成M-tag、协议补丁链、另找相位或升位其他方向。若仅得到未校准不等式，也不判通过、不开完整系统开发。下一任务必须给一次实现裁决，而非再交一份缺项清单。
