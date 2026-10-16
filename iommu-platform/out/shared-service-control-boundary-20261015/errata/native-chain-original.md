# LR1 原生所有权链：源码能力、必要偏序和接口缺项

日期：2026-10-15。目标：`actualgoal1791990424`。执行席：`lr1_native_chain`，Codex-only，Sol 6.1 / ultra。本文只核原生链，候选协议和最终裁决由 root 负责。所有引文的完整原文、文件 SHA-256 和范围在 [source-evidence.json](source-evidence.json)；结构化结论在 [findings.json](findings.json)，执行边界在 [ledger.json](ledger.json)。

**结论：原生链可以产生整页 R 已收完、最后 W/B 尚未完成的时刻，而且不需要增加缓冲或人为延迟。它没有可直接复用的“源 PA 页及代次已经可以回收”ACK。** 新 ACK 必须另付 trusted 全 reader/alias 注册、读收据和错误账本、封新读、旧翻译与物理读关账、晚 fill 同步、PTE/cache 可见性及正常内存排序的成本。本文没有运行 native frontend、RTL、DDR 或板端，不能把源码可达偏序写成已执行见证，更不能推算 ACK 留给 W/B 的实际窗口。

## 固定配置和真实信号

| 项 | 当前源码 | LR1 能用的范围 |
|---|---|---|
| DMA | 4 个；每个私有 AxID=0；source-mux 前缀端口号 | DMA0/1 上游 IOMMU ID 为 0/4，DID 为 10/11；身份不改 |
| 原缓冲 | 非零 IOMMUNonblockMode 的 DMA0/1 各 512 beats；64-bit 数据宽 | 各 4096B，source2/3 仍 3 beats；不增容 |
| iDMA 队列 | Job FIFO=8、NumAxInFlight=8、MemSysDepth=0、RAWCouplingAvail=0 | 维持原配置；AR/AW fall-through 各一槽 |
| IOMMU | NumOutstandingTrans=8、WFifoDepth=32、IOTLB=16 | banked 模式每 bank Ax 队列4、W FIFO16；总量不变 |
| PTW | TL 实例化一个正常 `rv_iommu_ptw` | 共享1正常PTW；上下文/目录 walker 与 MSI walker不能冒称额外正常PTW |
| fullbuffer certificate | capacity≥4096、DEFAULT、length=4096、src/dst 页对齐 | 只是容纳完整 copy 的许可；既不证明 R 已收完，也不证明只读 PA/别名闭合 |
| register completion | DEFAULT 使用最后 B；READ 模式使用 burst read response | LR1 必须保留 DEFAULT；不能通过切 READ 模式伪造整页读完成 |

定位：`ariane_peripherals_xilinx.sv:870–872,1228–1281`、`ariane_soc_pkg.sv:110–130`、`rv_iommu_banked_ctl.sv:158–169`、`dma_core_wrap.sv:173–207,306–319`、`idma_fullbuffer_certificate.sv:16–18`。真实 shared ID 的 0/4 来自 IdWidth=4、DmaAxiIdWidth=2 和 `axi_id_prepend.sv:88–90` 的 `{port, private_id}`，不是给 candidate 另编 ID。

## 原生功能链和信用

1. Frontend 进入 COUNT 后保持 job VALID 和地址，直到 job FIFO 接受；之后 WAIT 保留任务，最终 B 导致 job retirement。现 observer issue/done ID 可作为软件任务身份入口，但没有沿 legalizer、TL、物理 AR、R 返回传播的 job generation / source PA page / epoch 字段。
2. Hardware legalizer 把 4096B、64-bit、非 deburst 的对齐 DEFAULT 分成两个 2048B/256beat burst。AXI 的 `len=255`，每页两个 RLAST。`idma_pkg.sv:107–109` 和 `idma_legalizer.sv:74–83,236–268` 给出这个边界。若打开 deburst，burst 数会变；LR1 的双 RLAST 合同必须明确 deburst=0，不可只看 length=4096。
3. 每个 burst 同时拥有 AR metadata 和 RDP metadata。`idma_backend.sv:471–473` 要求 RDP 队列信用与 AR fall-through 同时就绪；RDP 队列 depth8 (`554–569`)，transport 只在实际 RLAST handshake 后 pop 该 burst 的 RDP 请求 (`idma_axi_transport_layer.sv:268–270`)。两个 burst 不因 RDP 信用不足必须等待最后 B。
4. 原 coupled legalizer 的下一对 burst 同时取决于 read 与 write metadata 接受。write 就绪包括 WDP FIFO、单 AW fall-through 和 last flag FIFO (`backend:472–473,629–649`)，`legalizer:459–464`。所以大 buffer 不能凭空绕过阻塞的第一个 AW：第一 AW 若一直占唯一 fall-through 槽，第二 AR 不会生成。合法 LR1 正例必须让原写 metadata 链确实推进，不能让两段 source read 在两个 destination AW 都未获得原生接受空间时凭空完成。
5. TC 先接收 raw AR 到 bank AR FIFO，选择后发翻译请求；请求期间 header 仍留在 FIFO。正常 TL 可 hit 或 PTW fill 后 hit，N 可接受另 bank 的 leaf hit。结果被 TC 的 `trans_resp_data_q` 持有；TC COMPLETE 发物理 AR，真正物理 AR handshake 后才 pop raw header (`trans_ctl:146–164,692–725`)。
6. R 返回以原 AXI ID 经 completion/源 mux 路由；TC 把 RREADY 直接连到原 DMA (`trans_ctl:359–368`)。DMA 在 `RVALID && RREADY` 时把 DEFAULT oldbytes 写入字节 FIFO。只统计 `RVALID`、翻译响应、物理 AR 接受、单次 RLAST 或 frontend done，都不够形成整页读收据。
7. 当前 read response 通知可在 RLAST 或任一非零 RRESP 出现；`rdata_valid` 则是每个 RLAST 实际接受脉冲。原 backend NO_ERROR_HANDLING 把 response.error 固定0，不保存读错误 (`backend:519–534`)。新收据必须逐 beat 累计 byte/coverage 和 RRESP，成功应为全页4096B、全部声明 burst、全部实际 R handshake、无错、无遗留义务。error pulse 不能计成成功完成。
8. W 从字节 FIFO 转交给 TC W FIFO，然后经已接受的目的 AW 写到内存。字节只有在 W handshake 才离开 DMA；TC 的 W FIFO 也只在自己的后继握手转交。源页复用之后，oldbytes 可仍在 DMA、TC/互连保护的 W 队列、或已经写到不同目的 PA。无需要求 ACK 时完整4096B全部还在一个 buffer，更不应把消耗掉的正确 oldbytes 视为丢失。

### 不靠人为压力的 Rdone→Wpending 偏序

DEFAULT 最后一个 R beat 在接受边沿进入原 byte FIFO；`idma_stream_fifo.sv:105–110` 使用原 stream FIFO，其 `FALL_THROUGH` 默认0 (`common_cells/src/stream_fifo.sv:15,43–53`)。因此这笔最终 oldbytes 不能在同一边沿凭空作为最终 W 被私有 DMA 端接受：至少最终 W 仍在其后，最终 B 又依赖最终 W。这是数据依赖和现有 FIFO 所给的偏序，无需人为添加 W/B 服务延迟、调压力、增队列、调 walker。

允许的源码链为：`原AW metadata推进 → 两段物理AR接受 → 每段256次R接受 → 第2次RLAST/整页读收据 → 最终oldbytes的W接受 → 最终B/jobdone`。期间 oldbytes 的位置可以随着合法 W 转交变化。共享 W-owner、返回和翻译资源会限制实际并发，这条链不声称每次运行的数值延迟。

**整页 Rdone 时 W/Bpending 可以由源码因果说明；新增源页 ACK 时仍 W/Bpending 则没有 native 证据。** seal、旧义务关账、PTE撤销、cache维护、定向失效、ordering所需时间可能使最后 B 先完成。只有实际 native 实现与对应运行能裁决剩余窗口；本文没有做该运行。

## 源页 ACK 所需的精确关账偏序

下面是安全验收边界，不是已存在 RTL 的状态机。root 的新 protocol 必须实现或付费依赖这些接口。

`trusted registry固定已授权parent及完整字节frontier → 封新old-generation parent/未登记读范围 → 排空/终结所有既有授权的剩余字节及old raw/walk/result/held physical AR → 各reader全页实际R成功归档及physical-read obligation=0 → target PTE撤销并完成可见性/必要cache维护 → 最后一次覆盖全部alias的定向invalidate提交且与旧fill同步 → 必要normal-memory ordering证书 → source-page ACK → 新代次CPU/allocator复用`

其中部分 PTE 撤销与旧义务排空可采用另一个有明确保证的合法次序；但不得在 ACK 后再出现 old-generation physical read 或 old stale fill，也不能把已承诺 VALID 丢弃。本证据支持保守的 drain-before-final-invalidate 次序，具体新协议由 root 选择。

| 旧义务 | 必须计入 / 允许的处理 | 当前缺项 |
|---|---|---|
| frontend/Job FIFO/尚可生成的 raw AR | 源 PA binding、job generation、已授权parent全4096B的剩余字节/burst表；seal 禁止新old-generation parent或未登记范围，允许原parent尚未VALID的第二burst继续产生并drain | 无 page/epoch seal；COUNT VALID 已承诺不能撤回 |
| 已可见但未握手 ARVALID | 即使 upstream AR counter尚未增加也属承诺；保持 payload/VALID，完成握手并 drain 或在原协议内返回确定失败 | 原 quiet/counter不覆盖源页的 held-VALID ledger |
| 已接受 raw AR/正常 walk | TC/TL 的旧 header、DC/PC/页表根、walk、leaf结果都需归属旧generation；未知PA的raw/walk由已注册binding保守关联 | 只有bank/IOVA/DID/PSCID；无generation/PA-page obligation接口 |
| N LEAF/OUT | LEAF 同边invalidate会阻止accept；先accept进入OUT的旧结果必须交付并继续 drain | N OUT无invalidate取消，这是既有所有权而非自动bug；新ACK不能忘记OUT |
| normal TL COMPLETE/TC COMPLETE/held physical AR | 翻译已经转成SPA也仍是oldread obligation；held AR按原AXI保持直到接受 | cache失效不收回已翻译结果；需另计drain |
| 已被物理总线接受的 AR | 从真正physical AR handshake到整个burst实际R handshake终结，按reader/jobgen和byte coverage计账 | 全局AR-RLAST counter既不逐页，也不逐错误 |
| R成功收据与故障 | 每个reader的全部4096B已实际交给保护存储；any RRESP、漏beat、异常RLAST、身份不匹配均不得成功ACK | NO_ERROR_HANDLING不会报告这些成功条件；需新sidecar/receipt |
| 旧写 tail | 源PA与所有old目的PA必须不相等；正确oldbytes仍受各队列所有权保护；W/B继续 | 不能把full job quiet或所有B归零设成LR1源ACK默认条件 |
| unrelated DMA | 不在target PA alias/reader集的任务继续合法进度；无全局停DMA作为免费前提 | 全局policy_quiet包含所有W/B、walk、输入VALID；不能复用作局部ACK |

注册必须来自受信任软件/硬件的完整已知 reader 和映射集，不能读未来 trace 来预测谁将访问。已接受4096B parent在JobFIFO中尚未进入legalizer，或legalizer尚未产生第二ARVALID，都不消灭其全4096B读授权和剩余义务：seal前固定这项完整frontier，seal后允许剩余正常AR/W metadata推进。否则前2048B收完后被seal禁止第二burst，整页receipt会永远不成立。禁止的是新增parent、扩大旧frontier或加入未登记old-generation范围。注册后禁止同一旧 epoch 新增alias/context binding；未来合法访问属于新 epoch，并通过正常翻译重新授权。source2/3 仍为原3beat配置；若其 binding可能触达目标PA，则它们也是必须登记/封闭的reader，不能因没有fullbuffer许可而被忽略。LR1中称其“unrelated”必须由trusted注册和权限事实保证。

`dma_core_wrap.sv:177–178`的job_fire在frontend→JobFIFO接受点，`186–203`的深度8队列随后送backend，backend→legalizer接受又是`be_valid && be_ready`。parent/jobgen要从前一接受点开始计入并穿过后者；不能只用当前active backend job漏掉队列中的已授权parent。现frontend COUNT接受后进WAIT，WAIT等每笔B完成才下一COUNT (`frontend:519–564`)，所以给定正常串行frontend的depth8不是8个免费同时已知未来parent；它也不提供跨source完整readerregistry。LR1原型限定两个已注册4096B parent，可将各parent两个declared AR区间和未发byte debt全部先登记；这来自当前已授权任务描述，不需要预测未来任务。

## N OUT、正常晚 fill 与失效

N `accept_o` 在 LEAF 检查candidate稳定、DC权限、leaf权限和 `!invalidate_i` (`rv_iommu_nonblock_hit.sv:100–103`)；它在接受边沿保存SPA与权限并进入OUT，OUT只等consumer READY (`157–171`)。先接受的旧结果在失效后继续有效，符合已接受交易的正常所有权。**本任务不判它为RTL bug**；若新增源页 ACK 不计该OUT，则是新ACK合同不闭合。后继正常TL response arbiter和TC AWAIT_RESP/COMPLETE也会继续产生物理AR，全部需在ACK前终结。

IOTLB 同一边沿的 invalidate 分支优先于 update (`rv_iommu_iotlb.sv:215–218,318–331`)。这仅阻止同边fill，不能阻止旧PTW在后一个边沿才提出update。正常PTW没有invalid/generation输入 (`rv_iommu_ptw.sv:35–84`)，取到合法leaf就发update (`478–484`)，TL把它直接接给IOTLB (`tl_wrap:278–280,759–792`)；CQ的失效pulse没有walk drain acknowledgement。故不能“先发invalidate pulse，旧walk后fill，随后源ACK”。

保守可行合同是先封新增old parent/未登记范围，允许既有完整frontier读完，并排空所有能产生旧fill的raw/walk/result，再做覆盖alias的最终失效；若使用epoch过滤/取消旧fill，应单独实现并付费。原生policy_quiet会检查完整TL/PTW/cache-update生命周期，并加上全局TC、W/B及输入VALID；它是全局配置切换条件，等待它会抹掉LR1希望保留的旧写tail与unrelated进度，不能作为免费局部闭合接口。

## 全物理 alias 闭合和软件范围

IOTLB标签是VPN、PSCID、GSCID和stage，不是PA反向reader表；多个不同IOVA、不同DID、不同PSCID可指向同一PA。不同PSCID只能证明翻译标签分离，不能证明物理不alias。global PTE又绕过PSCID匹配 (`iotlb:105–109`)；PSCV=1定向VMA排除global项 (`227,239–243`)。若源页有global alias，必须选择真正覆盖它的失效或明示LR1禁global。

LR1应至少注册：`source PA page + ownership epoch`、每个reader的device/source mux身份、jobgen、DID、IOVA页、stage、PSCID/GSCID、页表根和绑定代次、权限和global状态、每burst范围、所有已承诺raw/result/physical obligations。安全闭合条件是所有能触达该PA的旧读身份已封且关账；不是只把DMA0/DMA1各自一个IOVA清掉。只读源约束还必须证明没有old writes经任何alias指向该PA。

现 Linux `idma_stream_prepare` 创建src/dst分配和IOVA (`idma.c:370–398`)，实验mutex只序列化本实验 (`153–154`)。`idma_iommu_map_range` 为src/dst统一使用 `IOMMU_READ|IOMMU_WRITE` (`270–279`)，并未实现LR1的只读源或完整物理alias注册。因此这些是新协议的受信任、付费前提，不能写成旧driver已提供。

Linux `riscv_iommu_flush_iotlb_range` 的host-S1 PSCID设置被注释，ADDR范围循环也被注释 (`iommu.c:1752–1770`)；因此当前host S1发的是GV/AV/PSCV全清的广失效，不能写成已提供逐目标PA页失效。源码具备set_addr/set_pscid/set_gscid helper，不等于当前调用路径用了精准字段。`flush_s1_checked`明示要求DMA已quiesced，只核广S1失效pulse；不提供LR1回收ACK (`2151–2162,2264–2272`)。

## read completion、GOP和CPU/DDR一致性

这里的GOP是架构 global ordering point，不是一个免费硬件模块信号。实际RDATA/RLAST成功接受可保守证明该read返回已经进入保护存储，其后copy不需要再读旧源；这个数据依赖结论不自动提供标准global IOFENCE、任意DMA/PCIe端点或CPU缓存的排序保证。

本板源码把IOMMU completion接入SoC AXI crossbar并路由DRAM，后继有atomics wrapper、clock converter与MIG的RDATA/RRESP/RLAST (`ariane_xilinx.sv:291–317,850–852,934–945,1030–1089,1158–1174`)。这是真实普通主存接口入口；本文没有读取封装IP内部，也没有做目标时钟或板端运行。只能对注册为本机普通主存的目标使用该保守数据返回边界，不能外推到设备/MMIO、posted-read语义或外部端点。

现driver明确CVA6无DMA snoop，在初始化buffer交DMA前调用 `local_flush_icache_all` (`idma.c:2490–2495`)。此函数实际发 `fence.i` (`cacheflush.h:11–14`)；本CVA6 controller将FENCE.I连到D-cache flush并等ACK (`controller.sv:100–123`)，WT missunit先等writebuffer empty及MSHR清空，再清cache并ACK (`wt_dcache_missunit.sv:399–404,477–489`)，writebuffer valid在写response返回后才清 (`wt_dcache_wbuffer.sv:407,433–440`)。这些原文支持该平台现有cache维护路径的源码依据，不能把软件注释当通用一致性保证。

但当前readcomplete事件到early sourcepage recycle没有driver ABI、`dma_rmb`/`dma_sync`调用链或native同步实现。新ACK前必须有明确的normal-memory completion/同步契约，CPU重新写源page、PTE store/cache维护/CQ发布和新epoch DMA读各自维持正常排序。root协议的 `target_ordering_proven` 应标为外部付费证书输入且fail-closed；它不是本源码中已存在的免费信号，也不等价globalIOFENCE。

## CQ没有可直接复用的局部源 ACK

CQ IOTINVAL在DECODE发失效pulse，只有输出，没有来自TL/cache/PTW的done/ready输入 (`cq_handler.sv:80–98,312–339`)。IOFENCE处理明确 `TODO: ... PR and PW using R/W counters` (`362`)；它当前处理可选completion-memory write、WSI和ATS同步，没有按读写计数关账。`sw_if_wrap`的 `in_flight_i` 只接regmap，未接CQ handler；regmap拿它延迟DDTP写 (`regmap:3210–3211,3234–3243`)，不能据此推CQ自动等待在途交易。

CQH在命令第二beat fetch结束时已经前进，随后才DECODE (`cq_handler:276–283`)；Linux同步轮询CQH (`iommu.c:516–525`)。现checked-flush注释谨慎说明：在已quiesced、无ATS/S2的实验里，CQH过后继fence足以证明前一VMA pulse已发，不是通用fence execution证据。因此复用这个观测最多证明已知前一个失效pulse发过；它不证明normal旧walk不能再fill、旧N OUT不能发physicalAR、某PA页所有read已收完，也不证明GOP。

LR1新增的 **custom source phase ACK** 应只表示指定PA页及epoch的源读和撤销边界成立，允许其他PA旧W/B和unrelated DMA继续。它必须与标准globalIOFENCE独立命名、独立合同；不能宣称已把原CQ PR/PW实现齐，不能借旧广flush helper把全系统停完再说局部回收完成。

## 新native验收需补的接口

1. 4096B/DEFAULT任务的可信注册与冻结入口、generation分配；现64B launcher不能直接复用。封门在尚未承诺新job/AR的产生点实施，已COUNT、heldAR或物理heldVALID保留原握手，目的W/AW/B不因source seal撤回。
2. 与immutable IDs0/4及jobgen绑定的全页R收据：两burst/512beat/4096B实际handshake、覆盖、RRESP、identity异常；一般化burst描述而非只数最后R。
3. 原metadata信用和oldraw→walk→结果→heldphysicalAR→physicalAR→R的所有权转换账本。既不得同时双计同一交易，也不得在TLresponse或AR握手时提前清账。
4. 目标PA页全部reader/alias/ctx/epoch注册和冻结；只读旧源、目的PA不同；不从未来trace拿资格。
5. target撤销、CPU/PTE cache可见性、覆盖global与所有alias的定向invalidate、最后旧fill同步和可观察completion。至少有一条完整的“oldwalk晚fill仍不能ACK”路径。
6. normal-memory/GOP相关付费证书与新epoch复用排序，故障、超时、重启和cancel下fail-closed；原NO_ERROR_HANDLING、lastB done和全局quiet都不冒充这些接口。
7. 实际nativefrontend冷/热链及源ACK前后old W/B、不相关DMA进度验证。该项本轮未执行；private protocol的运行不实现上述native链，不能替代该项或宣称native已reach。

未改生产源、队列、容量、walker、ID、Linux ABI或FPGA配置；未恢复旧席/分支，未调用DeepSeek/Claude，未运行旧模型/validator/experiment/build。上述缺项并不是本席做出候选创新或收益裁决；它们是root做安全、成本及证据裁决时必须保留的原生边界。
