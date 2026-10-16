# D来源门：M0中的N放行点、完整收益边界与状态来源

2026-10-15；Codex-only；只读定向源码/已生成模型核查，未运行模型、包装器、构建、goal或板测。建议/请求配置为Sol/high；本执行席未取得实际模型及推理强度的运行元数据，实际字段记unknown，不以任务参数冒充实际记录。root负责机制设计、来源门和新意最终接受。

**结论：N确实可以在另一bank的页表遍历期间，提前接受并返回一个经过真实缓存/权限检查的hit；它没有直接释放COPY完成。当前固定M0缺少有自然来源、同O0而效果反号的两个外生状态。共享FIFO返回、单任务供给和协议承诺会吃掉局部提前量，但源码没有给出“所有合法来源下PN绝不可能让完整任务更快”的证明。现有M0只可作机制资产，不能作为D首个有来源正侧模型准入。**

## 1. 原件与可复核性

完整源文件SHA256、精确行号和必要原文保存在`sources.json`。以下路径均相对`/home/xvhaocheng/work/iommu-platform/`。历史六份完整日志及反号核对沿用输入`out/research-plan-implementation-20261015/d/assessment.json`；本席没有重读六日志，也没有重新接受原数值。

读到的当前HEAD：平台`0a570deda89636631462bb188d39c058409df364`，CVA6`a1b08934391833aed21e6ad63b545634040808ac`，IOMMU`57eb5621802ef9ed8d77c684b9a03205c6630391`。锁文件IOMMU为`238a39e66a07b27a67fb5749df40691682555027`，与当前HEAD不同；因此当前源码SHA、现存生成产物和旧M0日志的构建身份分开，不能自动认定同一字节版本。

现存`out/nonblock-actual-dma/obj-policy-system/Vnonblock_actual_dma_system_top__verFiles.dat:2`记录实际生成命令`-DACTUAL_DMA_SYSTEM -GPolicyEnable=1`，没有其他`-G`覆盖。生成头文件确认两bank、N状态机、8条各512字节深度的每DMA数据lane（总4096B），以及每bank四项AW/AR结构。它证明这些产物曾按相应配置生成；未建立旧六日志到当前二进制/原源hash的完整绑定，也没有本次重建。

## 2. 实际实例与路由

| 层 | 本次读到的配置/作用 | 原文位置 |
|---|---|---|
| SYSTEM顶层 | `NonblockEnable=1`、outstanding=8、W FIFO=32、DMA buffer=512、DS delay=50/50；共享池/反序/同bank/源门等默认关 | `tests/nonblock_actual_dma_system_top.sv:71–96` |
| 参数下传 | SYSTEM→actual DMA→banked top；banked开，BankIdBit=2 | `system_top:1102–1118`；`tests/nonblock_actual_dma_top.sv:788–797` |
| 真实DMA | 两个`dma_core_wrap`，每个NumAxInFlight=8、JobFIFO=8、BufferDepth=512、AxID=0；DID10/11 | `actual_dma_top:352–374,411–433` |
| DMA复用 | 4输入、2活源的真实`axi_mux`，2-bit原ID添源位变4-bit；V/A ID为0/4 | `actual_dma_top:680–696`；`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:243–244` |
| bank资源 | 总8项AW/AR及32项W按bank均分，即每bank AW/AR各4、W FIFO16；不是每bank8/32 | `rv_iommu_banked_ctl.sv:158–164`；`rv_iommu_trans_ctl_wrap.sv:121–149,221–224` |
| bank→TL | 每bank一次翻译生命周期；request RR仲裁，只改`.bank`，response按`.bank`回原bank | `rv_iommu_banked_ctl.sv:367–408` |
| IOTLB策略 | 16项IOTLB、8项DDTC；P/PN相同Victim DID10、2/14分区；更新owner取稳定主请求，不取快探测DID | `tests/nonblock_banked_top.sv:366–380,865–870`；`rv_iommu_tl_wrap.sv:657–676` |
| DS/DDR | 真DS_if→真50/50 delayer→xbar slv3；payload slv2；所有实际地址<4GiB经mst0；xbar两master，8/8、CUT_ALL、stage1、UniqueIds=0 | `system_top:433–480,875–919` |

P/PN为同一个PolicyEnable=1生成模型：runtime `(N,partition)`分别为`(0,1)/(1,1)`，见`tests/nonblock_actual_dma_test.cpp:3359–3368`。`NonblockEnable`是编译参数，P的runtime N=0**没有**使`normal_gate`恢复到另一个编译模式。两者普通路径均在`state=IDLE && !req_valid && !n_intercept`时才能接受新普通请求（`rv_iommu_tl_wrap.sv:284–286`）。

## 3. N究竟提前放行什么

1. 主TL已有一个请求，占有`req_data`及普通spill，处于`PT`，`e3_ptw_active=1`，S1开启/S2关闭，无DDT/PDT隐式翻译；候选必须来自另一bank，无ATS/debug/DDTC旁路请求。窗口公式见`rv_iommu_tl_wrap.sv:303–306`。PTW active定义为PTW状态不为IDLE，涵盖PTE请求/等待，不等于DDR当拍正在服务该owner（`rv_iommu_ptw.sv:100,384–397`）。
2. N模块在IDLE且enable、window、candidate valid、无失效时锁存候选；DC阶段查该候选自己的真实DDTC；LEAF查真实IOTLB。`rv_iommu_nonblock_hit.sv:47–58,143–155`。DDTC/IOTLB端口与主路径时分复用，快probe期间主路径的cache-hit信号被屏蔽（`tl_wrap:249–252,271–277`），不是无限多端口/免费probe。
3. 合格条件包括：DC有效，非PID/priv/debug/ATS/PRI，Sv39/S1-only、canonical、普通R/W、4KiB非superpage、leaf有效且U和对应R/W权限，MSI输入/结果均安全。输入候选、DDTP和DC须与锁存值稳定；无失效。全部条件通过才`accept_o`（`nonblock_hit:84–103`）。
4. **`n_accept`就是TL request握手许可**：`trans_req_ready_o = n_intercept ? n_accept : ...`；未命中或不支持的probe进入REJECT，不握手，等普通路径真正接受或候选消失才重置（`tl_wrap:286–287`；`nonblock_hit:169–172`）。不能将candidate VALID或DC命中当成功许可。
5. 接受边沿形成`resp_q`，echo原bank和物理地址/权限，随后OUT保持VALID直到response ready；fast与slow共享RR响应端口（`nonblock_hit:158–171`；`tl_wrap:346–352`）。匹配bank的CTL仍须AWAIT_RESP消费结果，再到COMPLETE呈现下游AW/AR，直到下游握手才释放该bank当前翻译头（`rv_iommu_trans_ctl_wrap.sv:674–725`）。

窗口只约束`start`，DC/LEAF的稳定性与`accept`不再要求`n_window`或PTW active持续（`nonblock_hit:47–48,100–103,111–112`）。因此一次probe可能在walk结束前开始、结束后才成功；OUT/intercept仍阻止普通新请求。不能将所有fast累计数解释为“尚有walk等待可节省”的数量，更不能把probe/response对正常入口和共享cache的影响记为0。

因此P等待的直接资源是**串行普通TL生命周期**：主请求未返回，候选另一bank不能进入普通路径；PN在额外N状态槽、真实cached DC/leaf及单响应口可用时绕过这段等待。N不是取消当前walk、抢回DDR服务、并发多个miss、重排任意FIFO头或替消费者完成任务。

## 4. 从局部提前到完整正收益：必要链与结构限制

至少要连续满足以下链，才能把“可达hit”变成正侧完整状态：

| 必要条件 | 可能截断的实际结构 | 不能扩大成的结论 |
|---|---|---|
| 另一bank在walk未结束前已选中合格hit，节省量大于DC/LEAF/响应延迟 | 同bank没有此N窗口；每bank头一次翻译，AW优先于AR；N一次probe/response，拒绝后等正常路径 | 不能凭总体hit率或fast累计数确认此请求节省多少 |
| 早response导致更早的下游有效进展 | COMPLETE仍等下游ready；AW/W归属、mux队列、xbar信用与held VALID仍有效 | 不能把早TL handshake直接记作早payload服务 |
| 早进展减少V尾部或形成有用后继工作 | 每DMA frontend一份COPY完成才发下一份；同ID、FIFO响应及COPY读→写依赖可能吸收提前量 | 不能由此推出绝无提前下一COPY的可能 |
| 提前payload的代价不把其他owner关键PTE/后台尾部延后得更多 | 当前DDR把每个请求整段服务完后才服务队头下一请求；新增早payload可能排在PTE之前；DS另有50/50延迟 | 这是代价路径，不是对所有交错的支配定理 |
| 完整向量满足V收益及A/quiet约束，部署后扣全部费用仍正 | 主计时排除了模式/clear、warm、观测/校准、应用准备/consumer/free | 原理时间为正也不等于部署净正 |

**读放行guard必须保留。** `rv_iommu_banked_ctl.sv:246–268`按W所有权限制新读选择，避免另一DMA先填小R→W FIFO而它的AW无法进入，进而堵死共享R/PTE。真正完整容量证书可豁免特定bank DID的读选择：CTL的`fullbuffer_job_i && ar_data.stream_id == FullBufferDID`（`trans_ctl_wrap:646–647`）；per-bank DID=10+b（`banked_ctl:318–319`）。M0默认SharedPoolMode=0，证书来自真实DMA（`actual_dma_top:552–555`）。证书仅在已接受、normal、完整4096B且src/dst页对齐的sole job和4096B buffer下有效；异常重叠/退休撤销直到reset（`idma_fullbuffer_certificate.sv:14–41`；`dma_core_wrap.sv:173–183`）。因此M0并非一概“只有当前写owner能读”；也不能删掉容量证书声称当前fixture已有必然死锁/无N窗口。

**同ID与held VALID。** DMA均AxID0，mux后V/A区分ID0/4；N响应按bank，不给同bank新乱序tag。每bank翻译头须等COMPLETE握手才出队。请求及响应RR仲裁实例使用`LockIn=1`（`stream_arbiter_flushable.sv:34–40`）；已呈现且被阻塞的候选/响应不能重选。AXI demux对同ID在途目的做检查，并锁住首次VALID（`vendor/axi/src/axi_demux_simple.sv:299–335`）；此规则同时约束错误/成功目的，实际本任务地址均在同一DDR目的，不能凭它虚构跨目的收益。xbar的对应demux也沿此生产实现。

**DDR不是并行真实DRAM。** C++ `drive_ddr()`接收队列<16时接受AR，每条都设latency31（`nonblock_actual_dma_test.cpp:668,1301–1305`）；默认仅队头倒数latency，之后整段R响应，最后beat才pop（`699–719,1373–1399`）。其余排队请求的准备不会同时老化。payload一次len255即256拍，PTE一次len0（`rv_iommu_ptw.sv:276–279`）；这构成N早payload阻塞PTE的具体有源路径。可选DS优先只提升最老DS请求，保同ID与先前held R（`C++:681–697`），不改变RTL仲裁、不代表部署免费能力。

## 5. 状态来源：固定子合同没有当前可指认的隐藏外生变化

登记`out/workflow-g0-m0-20261010/baseline-register.json:4–14`固定：64/64、gap0、warm256、lat31、CPUoff、SYSTEM、head-serial、av、所有start delay=0。C++ full guard仅强制其中一部分（`3322–3337`），所以要区分**合法可调接口**与**实际登记合同**：

- `launch-order=av|va`，`start-delay/v-start-delay/a-start-delay`为uint32；解析和合法性见`C++:3220–3244,3296–3308`。t0之后公共delay计费；按order先启动一lane，per-lane delay期间继续police已启动lane，再启动另一lane（`2840–2869`）。有接口不证明真实producer会产生该相位，也不能把手调配置从O0藏起来。
- `v-cold-dst`、`owner-only`、DS优先、write-tail-pause、serial|overlap等另有显式开关（`C++:3158–3174,3190–3212,3245–3259`）；不是当前首批任意扩展菜单或自然源。
- memory setup清内容并重建固定DID/DC/PTE及页内容；reset清mem_q/caw_q/cb_q和RTL，再按固定A-first/V-first轮询warm256并drain（`C++:531–578,1536–1544,1600–1602,2155–2240,2288–2301`）。自主COPY frontend是COUNT握手→WAIT到trans_complete→gap0回COUNT（`idma_reg64_frontend.sv:519–569`）。事前known页环A1/8及服务规则不能充当O0未知态。
- 本范围未发现输入噪声、真实DRAM行/refresh状态、异步其他流或任意历史cache状态入口。RTL内部仲裁/缓存相位会随动作和此前该case的执行而变化；这是同一确定性闭环的内生状态，不能把P产生的轨迹与PN产生的轨迹当成两个动作前外生隐藏态。

可由自然来源影响但**当前未提供来源卡**的最小因素为：另一owner真实就绪时刻/依赖、缓存驻留与复用历史、另一owner正在walk时PTE与payload服务的剩余时长/次序、下游接受/返回能力及consumer完成条件。前两类在现M0由reset+固定warm和固定串行COPY压缩；服务历史由无真实DDR的固定FIFO模型压缩；consumer不在合同中。它们是候选因素列表，不是已取得自然源，也不授权改gap、phase或guard找赢家。

严格范围：①沿用的六日志只证明已登记有限cases无PN正侧；②当前收紧子合同中未找到同O0下可自然变化的未观测外生输入来源，所以不能只从相同case重复产生D需要的反号状态对；③没有证明所有可调输入/所有合法源下PN完整效应恒非正；④确定性系统也能在不同有来源状态中产生反号，确定性本身不否定D。

## 6. quiet配置clear、warm与“上一任务预测下一任务”

板端配置busy含`!quiet`或窗口armed/active及同拍arm/open/close；成功合法配置原子`clear_o`（`dma_composition_control.sv:155–168,182–185`），经`ariane_peripherals_xilinx.sv:1449`到IOMMU policy_clear。TL将其接到IOTLB；IOTLB使所有tag.valid=0并PLRU=0（`tl_wrap:668`；`rv_iommu_iotlb.sv:215–216,424`）。这是缓存状态重置，不是DDTC/整个内存服务状态reset；单凭这根clear不能断言DDTC或真实DDR行状态被清。

板端quiet不是单根TL quiet：`ariane_peripherals_xilinx.sv:1033–1035`实际为`composition_iommu_quiet && research_dma_quiet && (&job_quiet) && !obs_issue && !launch_active && !launch_held`。其中IOMMU top的policy quiet包含TL quiet、CTL !in_flight及TR/COMP/DS外口AW/AR/W/R/B无VALID（`rv_iommu_top.sv:280–287`）；CTL in_flight涵盖AW/AR/W计数、partial W及两bank busy（`banked_ctl:582–586`）。observer quiet排除batch活跃、新AR/AW、坏追踪，并要求每DMA读写在途计数为0（`dma_observer.sv:105–109`，实例连接`ariane_peripherals_xilinx.sv:982–989`）；frontend job quiet含IDLE、backend idle及无VALID/issue/next_id（`idma_reg64_frontend.sv:148–150`）。这些条件监视DMA/IOMMU生命周期，没有全CPU/其他master静止、DDR内部队列空或DRAM row/refresh重置信号；不能由此说真实DDR所有服务历史已消失。其他DS生产端口若有其外部VALID也在IOMMU外口检查中，但这里没有为所有DS来源额外证明下游内部队列全空。

M0的`check_quiet`另有不同scope：先等dbg_in_flight=0、DMA job_quiet=3、合成CPU read/write quiet，再显式断言C++ `mem_q/caw_q/cb_q`均空（`nonblock_actual_dma_test.cpp:1691–1725`）。这不能替代板端检查，也不能把板端TL quiet说成模型队列清空。

M0更强：**每case**setup/reset清队列、RTL及初始内存，再执行固定warm。测试banked top的policy_clear直接是`!rst_ni`（`nonblock_banked_top.sv:863–870`），没有独立“运行上一任务→quiet配置→下一任务”的模型控制序列。warm之后不再清IOTLB；warm建立的驻留是当前case/动作下产生的。绝对`g_cycle`可继续增加，但默认CPUoff/head-serial读服务不据绝对时间查询一个独立持续的外部队列或DRAM历史。

因此当前六独立case不能拼成具有时间相关性的重复任务序列。上一任务观测若要预测本次clear/warm后的下一任务，只能诉诸仍稳定的工作负载结构或清除范围以外的真实服务规律；必须给出它们为何跨clear/warm保留、何时重新可见和观测取得/失效成本。缓存瞬时驻留/未完成队列等被清掉的量不能未经证明跨界继承。本范围没有取得这样的跨任务预测证据；不将未知持续服务状态当不存在，也不将“可能有”写成已存在。

## 7. 供root裁决的最小准入条件

若D首个模型仍用此动作/合同，需先给一张**正侧来源卡**：实际producer/接口/依赖如何自然生成候选hit与另一bank walk重叠；P在哪条确切链等待；PN接受后哪项完整进展提前；早payload为何不抵消V/A/quiet净效果；来源变量是否已在O0、是否跨配置clear/warm保留。然后才有资格冻结模型输入和预算。

接受后继模型的必要证据（本席不授权执行）：来源路径可执行且有原文；保原guard/same-ID/held VALID/真实buffer及原动作；固定共同起终点与双方完整64、成功/quiet；预测完整正侧并明确可推翻结果；所有事前已知参数给baseline；另一状态同合同/O0近似不可分且预测相反；观测与模式/clear/warm/切换的成本和预测失效处理不留作零。

若仅有旧fast次数、启动delay旋钮、改gap/服务规则手工制造赢家、独立case拼连续任务、未来hit真值或忽略后台代价，则**否决该首个模型作为D来源正侧证据**。现材料状态为`not_ready/source_gate_not_passed`；这不是PN普遍无效证明，也不接受/否决整族论文新意。
