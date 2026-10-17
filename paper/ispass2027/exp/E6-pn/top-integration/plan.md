# IOMMU top 集成与完整 quiet：有界源码提案

2026-10-18 JST。**仅方案/固定源证据；没有生产RTL、仿真、综合、BIT或板卡操作。** IOMMU固定 `5b7dfbc70e458244a8e5682e5ae3f8571320254c`，CVA6接口事实固定8aac。32份实际Git blob、SHA256和编号原文见 [source-map.json](source-map.json)；所有读取副本已与固定Git字节核对。Root的最新 `runtime-contract-v1.md`是寄存器合同，A3的observer integration-plan是外层接线参考，均不被当作已经完成quiet的证据。本批不写A2/A3的CVA文件。

## 1. 最小架构与接口

新增 `PnEnable=0`、`HistoryControlEnable=0`，输入拟为 `pn_mode_i[1:0]`（reset语义S）、`pn_history_isolate_i=1`、同步 `pn_policy_clear_i=0`、来自A2真实证书的 `pn_fullbuffer_jobs_i[1:0]=0`；输出 `pn_n_fire_o`、`pn_n_did_o[23:0]`、`pn_quiet_o`。这些是提议端口名，不是已实现接口。完整8-word bank和mode状态仍在外围后批，不在IOMMU造第二份控制状态/observer。

`PnEnable=0`沿原singleTC+原TL，不实例banked或新增DS tracker；PN输出保守置0且外围PN控制块不存在，不能把默认关闭时的常量当完整quiet证明。`PnEnable=1`只按稳定 `pn_mode_i[0]`选择原singleTC或新增restricted bankedTC；P只看bit1，history独立，**不按bit2选择backend/N**。

| 内容 | 当前固定源入口 | 拟接线及保持条件 |
| --- | --- | --- |
| 原singleTC | `rtl/rv_iommu_top.sv:322–372`；TC配置直接RVIOMMUCfg | S/P/P_shared原实例/原8-slot header FIFO、WFifo32及完整MRIF路径；ReadAdmissionEnable保持0。新N支路不回写其READY/FSM。 |
| restricted bankedTC | `rv_iommu_banked_ctl.sv:137–179,332–405` | 仅编译PnEnable时额外实例，RestrictedNormalOnly=1；global仍MSI_BT_MRIF，仅私有bank显式BT_ONLY。原8/32分成4/16，不新增全局credits，不把demux tracker16称16笔翻译。 |
| 真实bank | CVA `ariane_soc_pkg.sv:31`，peripherals `:868–870,990–1011,1031–1035`；vendor/pulp-platform/axi mux `:151`、prepend `:89–90` | 原ID4，四DMA各privateID2，sharedID={port[1:0],privateID[1:0]}，当前privateAxID0。**BankIdBit=2**，DMA0/1为0000/0100；不按DID猜bank。DMA2/3不是本轮N测量源，MODE前仍核四口quiet。 |
| TL请求/响应 | top `:111–139`的原类型无bank；TL `:279–338`已有N/slow merge | 保留原类型作singleTC/软件debug基础类型；PnEnable分支内另定义加1bit bank的PN类型，仅转换/附tag，不改原payload字段。shared TL用PN类型，S请求附bank0，N请求带banked真实tag；响应按模式送正确TC，S剥去tag。ATS/debug请求附bank0，不从当前candidate重标旧请求。 |
| 单个shared TL | top `:378–521`有base/ext两种DC **编译二选一**；实际目标ext | 保持一个TL、一个DDTC/IOTLB和一组walkers，不分别复制S/N缓存。参数PolicyEnable=PnEnable、NonblockEnable=PnEnable、HistoryControlEnable传入；runtime partition=mode[1]、N=mode[0]、history=独立input、clear=同步sideband。global cfg/MSI/PC/debug capability不裁剪。 |
| 原SW写保护 | top `:580`原in_flight接regmap；TC `:101` | S仍传原singleTC in_flight，N传banked实际in_flight。**不把新complete quiet倒接进旧in_flight/DDTP写保护**，避免改变原软件路径；quiet只用于新保护资格。 |
| MRIF/FQ | top `:306–307,365–370,576–578` | 原singleTC MRIF DS/fault持续接原SW/DS（不因选N而掩掉旧义务）；合法切换已证明singleTC无任务，所以N期间这些端口自然无事务。N bank MRIF输出应恒0并有限断言，真实受限错误走旧FQ→error slave。 |

类型/生成范围建议：把原TC/TL域保留为 `!PnEnable`的原类型分支；启用域用原singleTC基础类型+PN TL桥接类型。可以抽一个仅PnEnable实例化的私有TC/TL域模块，避免改SW/debug基础类型。**不能为省类型接线而全局复制第二个TL/cache，不能用零宽bank字段或动态reset。** 如果实现改动层级名，完整构建前必须检查原约束匹配，保持原50MHz/XDC原字节；本提案不改约束。

所有选路是组合旁路，模式由外围持有到真实全quiet才变；未选支路输入VALID清0，响应READY只给选中支路。两个TC共用原reset，不因MODE切换复位/flush FIFO，不撤回已offer VALID。实际Nfire/DID从同一TL原边沿直接导出（`:284–285`），不单独延迟一项。

### 稳定normal来源的MSI限制

当前TL `:287`只按compileN/runtimeN/restricted_bank_i决定restricted MSI，若top直接给modeN会误限制debug。Root已允许最小修订：

```text
restricted_msi = compileN && runtimeN && restricted_bank_i
              && !req_data.is_debug
              && !is_pcie_ats_req && !is_pcie_page_req
```

三项都来自原arbiter→spill保存的**稳定slow req_data**；既有 `is_pcie_ats_req`/`is_pcie_page_req`在 `:405–409` 分别精确判断 `PCIE_ATS_TRANS_REQ(8)` / `PCIE_MSG_REQ(9)`，不能把所有normal地址或当前fast候选当来源。目标InclATS=0，top `:663–678`已按原配置tie-off，保留该事实而不修改全局cap。normal DMA来自私有TR（TC `:586–622`），debug源是SW `:548–575`中的dbg_if_ctl：`:58–64`明确is_debug=1，`:93–104`保持GO/ONGOING直到真实debug response；regmap `:2337–2357`的go/busy会在响应时由原hw2reg清。这样无需新origin状态。

S/P原MSI逻辑完全保留；N/PN仅normal私有bank进入MSI时按原ERROR/FQ发error260。debug MSI继续原BT/MRIF-debug合法故障语义，不能全局关MSI/MRIF/debug或在top吞debug请求。实际code批必须新增debug MSI gold场景，而原normal DTF0/1负例继续通过。

## 2. 完整 IOMMU-domain quiet 与只读tap清单

`state==IDLE`、当前总线VALID全0、CQCSR busy0或CQ head==tail单独均不足。下列输出均仅旁听现状态/库存，不改READY/VALID、不增论文诊断计数。表中路径除vendor外均相对固定IOMMU根；精确原文在source-map。

| 域/拟新增只读输出 | 必要现存事实与固定源码行 | 明确不应要求/不足之处 |
| --- | --- | --- |
| TC `pn_quiet_o` | `rv_iommu_trans_ctl_wrap.sv:101,117–212,287–327,429–475,674–684`：FSM IDLE、AW/AR/W FIFO输出valid均0、w_cnt_q0；无当前TR offer/translation offer或response；MRIF FIFO valid0+handler idle；completion demux真正empty（下行）。 | in_flight只FSM；COMPLETE在AX接受后就IDLE，R/B仍可未返。不看FIFO旧payload，不要求READY0。 |
| vendor `axi_demux_id_counters.empty_o` → simple `idle_o` → demux `idle_o` | 固定AXI e722 `src/axi_demux_simple.sv:210–247,348–381,513–617`：复用两组既有occupied向量归约、w_open0、AW/AR lock0；外层 `axi_demux.sv:78–209` 合AW/AR channel+select spill、W cut、R/B cut及外部VALID。 | 现无公开empty输出。需要这三层只读port，**不新加TC计数器**；counter在cut侧退休后，外部B/R spill仍须另核。UniqueIds=true不能伪称有此empty证明；本目标各TC/外层bank demux均UniqueIds=false。vendor依赖需独立同名branch提交/pin，不能只改build副本或生产hierarchical引用。 |
| banked `pn_quiet_o` | `rv_iommu_banked_ctl.sv:244–280,368–405,522–584`：既有inflight_aw/ar/w及partial全0、w_unfilled0、两bank新增TC quiet、请求/响应与输入/输出VALID0；额外原input-demux idle与output mux无held VALID。 | 保留原in_flight计数语义/32bit容量，不复制到S。真实certificate输入是许可而非库存，不能用许可位替代job/AXI quiet。 |
| MRIF handler `pn_idle_o` | `rv_iommu_mrif_handler.sv:71–91,173–218,261–343`：主FSM IDLE、无新req、无mem/fault offer/held返回；上游MRIF FIFO必须空。 | WAIT_INT_ID即使无当前DS VALID也忙；notice写B返回前不idle。主FSM IDLE后wr_state可保留B_RESP，不单独要求子状态AW_REQ；旧MRIF/intID数据不代表busy。 |
| TL扩展原policy_quiet | `rv_iommu_tl_wrap.sv:288–299`已有req/arb/spill、N busy/accept、response/fault、PTW active、各walk AR/R、update/inval约束；`:542,619`接回已存在DDTW/PDTW active输出；PTW `:99`已有active。MSIPTW新增下行idle。 | 不改walkers功能；DDTW/PDTW/PTW无需新状态/端口。不能只看normal TL state、不能漏debug/ATS来源。pdtc/mrifc update也须由walker-idle/无当前update闭合，不以只查IOTLB update替代。 |
| MSIPTW `pn_idle_o` | `rv_iommu_msiptw.sv:77–100,349–361`：BT主状态IDLE、若MRIF则状态MRIF_PTE，且无init/当前update/error/ignore/AR/R offer。 | **仅此仍不足**：`:163`固定ARLEN1，`:291–293`成功BT首拍即update→IDLE，`:269–272`MRIF交接也先BT→IDLE，尾拍可尚未返回。必须结合第3节DS传输库存；不改原状态功能。 |
| DS `pn_buffers_idle_o` | `rv_iommu_ds_if.sv:77–108,111–219,249–322`：原ar_valid/aw_valid/w_mux_valid/w_valid、外部AR/AW/W/R/B、内部R/B spill valid、全部producer offered/held VALID均0，w_fifo_empty1。 | 原W-owner队列在进入W spill的WLAST时已pop；不证明B已返。`spill_register_flushable.sv:90–93` valid=a_full|b_full，输出valid0足以代表两级empty，不加shadow FIFO计数。 |
| CQ `pn_idle_o` | `rv_iommu_cq_handler.sv:108–155,240–295,297–312,348–395,466–503`：主state IDLE、en_i==en_q、masked tail==head、无mem/ATS/fence/inval当前offer；programmable write同拍见program bridge idle。 | head在REGISTER最后R时已前进，DECODE仍未执行；CQCSR busy只是enable transition。WRITE/B_RESP持续到B；主state回IDLE后wr_state可能仍B_RESP，**不能要求wr_state==AW_REQ**。cmd_q旧bits、sticky fence_w_ip中断状态不代表pending command；原PR/PW fence TODO不在此修复。 |
| fault arbiter `pn_idle_o` | `rv_iommu_fault_arbiter.sv:44–107`：所有fault输入、per-input spill valid、arbited valid、末FIFO valid0。 | 仅top trans_fault_valid0不够；旧fault可在spill/FIFO等待FQ。 |
| FQ/PQ各 `pn_idle_o` | FQ `:112–132,182–218,249–279`；PQ `:84–118,188–283`：主state IDLE、enable稳定、无新record/mem offer/held response。 | **不要求内存ring head==tail**：已经B完成但尚未被软件读的记录不是fabric债务。ERROR保守判非quiet，沿原CSR清错恢复，不能MODE强清/假quiet；reset/terminal错误处理仍原样。ATS0时PQ输入固定0但模块仍检查自身生命周期。 |
| MSI generator `pn_idle_o` | `rv_iommu_msi_ig.sv:65–89,136–224`：主state IDLE、pending_q全0、无`intp & ~edged`且msi_ig_en的新边沿、无mem/fault/held B/R。 | 不要求全部intp0：已edged的sticky IRQ无需重复发MSI。masked pending仍保守非quiet，不能在MODE写中丢掉/清掉它。 |
| debug `pn_idle_o` | `rv_iommu_dbg_if_ctl.sv:47–105`：state IDLE、GO0、无req/response；保持原GO清除路径。SW `:547–583`向top汇总。 | GO1即使尚未TL握手也不能MODE；不因N mode关闭debug capability。 |
| program bridge `pn_idle_o` | `rv_iommu_prog_if.sv:57–135`及 `vendor/modified/rv_iommu_axi2apb_64_32.sv:172–211,214–352,448–455,534–542`：CS IDLE，内部/外部AW/W/AR valid、R/B valid、PENABLE均0。内部buffer输出valid已有empty意义（axi_slice:58–71及single_slice）。 | 外部AW/W已收但APB未执行仍忙；**PSEL恒1(:211)不能当busy**。不看旧AWLEN_Q/ARLEN_Q/data，不要求READY0。这是IOMMU prog总线；MODE在DMA0 +0x78，不能把DMA cfg VALID统一当忙造成自拒。 |
| SW wrapper/top汇总 | `rv_iommu_sw_if_wrap.sv:179–237,243–307,320–406,456–519,547–583`与top DS布线 | 汇总CQ、faultarb、FQ、PQ、MSI、debug、program及相应input/output，保持原regmap/HPM/WSI语义。不要把WSI线高、IRQ latch、HPM counter非零当bus busy。 |

所有inactive TC也须quiet；shared TL及SW只有一份。全局cap仍MRIF/PC/debug，ATS只按原目标InclATS0编译事实处理；不以BT_ONLY/关debug减少需要检查的域。原status field `in_flight`、CQCSR/FQCSR busy均不重定义。

## 3. 必要的纯被动DS传输库存（Root已准纳入方案）

上一表明确现存producer/spill信号无法覆盖MSIPTW两拍间隙；未发现现DS接口有可复用AR→RLAST/AW→B库存。优先复用的TC vendor occupied不覆盖这条独立DS总线。Root已确认该源事实，允许一个独立、只读的小tracker：

```text
read_inc  = ds_req_o.ar_valid && ds_resp_i.ar_ready
read_dec  = ds_resp_i.r_valid && ds_req_o.r_ready && ds_resp_i.r.last
write_inc = ds_req_o.aw_valid && ds_resp_i.aw_ready
write_dec = ds_resp_i.b_valid && ds_req_o.b_ready
```

每类一个8bit**transaction**库存，不数R/W beats；ARLEN不参与计数值。选外部DS边界：AR/AW真正交给总线才inc，R_LAST/B真正进入IOMMU的外层spill才dec。内部未发出的AR/AW/W和已返尚未交给producer的R/B由原spill valid闭合。W-owner empty与各producer生命周期还必须同时成立。

| inc/dec | 新状态/错误规则 |
| --- | --- |
| 0/0 | 保持 |
| 1/1 | 净保持，包括原count0/255；不误判overflow/underflow |
| 1/0且count<255 | +1 |
| 1/0且count255 | 饱和255，置sticky bad，不wrap |
| 0/1且count>0 | -1 |
| 0/1且count0 | 保持0，置sticky bad，不wrap |

两counter共用一个sticky `ds_debt_bad_q`即可合并任一under/overflow；共 **17FF**（Root若要区分under/overflow，可2flags共18FF，但不新增MMIO语义）。只有原reset清counter/bad；ARM、STOP、MODE写和policy_clear均不能清。任一bad后新增quiet永远0直到reset，数据READY/VALID/仲裁/队列**完全不反馈此状态**；255不是新credit上限，超过只标本次保护证据失效，不能停业务或宣称支持容量255。

counter全部置于PnEnable生成域，默认关闭裁掉。它不进旧observer/论文统计bank，不冒充PTW性能计数，也不偷并入194bit2 certificate_fault。bad仅经dataflow_quiet0/eligible0使MODE不能接受；软件按既定超时/原错误路径停止，不伪造可用样本。新transport保护与原stock/ID/overflow检查同时保留。

## 4. quiet/clear/控制依赖无环

```text
iommu_domain_quiet = S_TC_quiet && N_TC_quiet && shared_TL_quiet
                   && MRIF_quiet && SW_control_quiet && program_bridge_quiet
                   && DS_buffers_idle && read_count==0 && write_count==0
                   && !ds_debt_bad && no_current_boundary_VALID

outer_dataflow_quiet = iommu_domain_quiet
                    && all4_frontend_quiet && all4_backend_job_quiet
                    && original_private_stock_validated_zero
                    && no_current_private/shared_DMA_VALID

mode_eligible = outer_dataflow_quiet && !original_observer_armed
              && !original_observer_busy && !capture_busy && !certificate_fault
              && !same_edge_other_functional_launch
accepted_MODE = independent_MODE_decode && full_WSTRB && legal_word && mode_eligible
policy_clear = accepted_MODE
```

`outer_dataflow_quiet`才喂新observer的all_quiet；**不含capture_busy/armed/旧observer busy**，避免等自身冻结的死锁。194bit5保持原busy定义，ARMED另参与bit0，不把它偷并入bit5；194bits0..5/63按Root合同，不增位。

quiet只看旧q、现existing offers/真实external launch；不看accepted_MODE、mode_write_decode、自生policy_clear或由它衍生的flush。专用policy_clear原子清IOTLB tags/PLRU，不OR成CQ external inval再倒进quiet；不把“接收MODE→clear→不quiet→拒MODE”写成组合环。CQ真实invalidation、debug GO、program bridge current/buffered context写继续阻止MODE。只读INDEX/DATA访问属于DMA0 cfg本身，不统一作为dataflow busy；IOMMU另一条prog总线的held访问则须完成。

同拍新DMA launch由frontend raw launch/offer与private VALID保护，不只看旧stock_q；CQ新tail/GO写由program bridge在q更新前就保护。计数同拍净保持不代表quiet：只要当前AR/AW/W/R/B VALID存在，仍非quiet。合法MODE同值也同步clear；busy/非法/半WSTRB均不动mode/cache/counters/phase，只有外围194 last_rejected按已锁规则更新。

## 5. 分批实施清单与有限接受检查（均未执行）

A. **只读quiet tap与transport tracker小批**：IOMMU的DS IF、TC、banked、MRIF、TL/MSIPTW、SW wrapper/CQ/faultarb/FQ/PQ/MSI/debug/program bridge；vendor AXI三个模块只加empty/idle输出、独立component branch/pin。DDTW/PDTW/PTW复用既有active，packages/PTW/DS原修复功能不改。先比tap前后旧外部数据路径逐拍；transport小DUT边界0/255/同拍净算/粘错及reset-only。不要直接合旧composition observer。

B. **top双TC/共享TL接线小批**：实际接mode/history/clear/证书/Nfire/DID/完整域quiet；新增typed bank metadata及选路，保留S原TC。只在Root接受A实际diff/有限证据后继续，不连CVA或MMIO控制状态。此前A2双backend/证书、A3frontend/observer tap各自独占提交，最终整合另由Root定。

C. **真实有限DUT，不能用“建议测试”充通过**：

1. `PnEnable0`和`PnEnable1/runtimeN0/P0/fullmask`分别对**原85b top完整gold**，实际相同RVIOMMUCfg（8/32、MSI_BT_MRIF、PC/debug、ATS0）、MMIO、TR/comp/DS memory输入逐拍比较所有旧外部接口、HPM/fault、原MRIF路径。gold只允许namespace/实例绑定重命名；对修改模块/AXI vendor需独立原字节gold，不让candidate/gold共用被改状态而互相掩错。必须含PTW忙时spill预取、W-before-AW、held R/B、实际FIFO/debt、原BT/MRIF与debug配置。
2. 用真实shared TL+banked+旧error slave证明normal受限MSI **TL→banked SLVERR全链**：实际配置DTF0/1 DC、MSI输入，DTF0有真实FQ记录并最终写完，DTF1无FQ；单/多W拍均排空，slave侧SLVERR一次，无normal comp请求、无MSIPTW/MRIF AR/无fill。证书若仍由harness驱动必须标test-interface，不能称真实DMA full-copy证明。FQ已收record不等于DS已排空，延迟其B时quiet必须0。
3. 原debug通过真实program寄存器GO进入shared TL，runtimeN1时BT MSI保持85b原结果/响应时序，MRIF-debug按原合法故障语义；`req_data.is_debug`真实置1且来自spill。普通normal DTF0/1错误260继续通过；目标ATS0保持cap/tie-off不改，不泛称ATS1已支持。
4. 关键**真实两拍MSI间隙**：实际MSIPTW发ARLEN1、返回合法BT首拍后延迟RLAST至少16拍，确认旧BT/TL可IDLE、所有当前VALID0的中间周期，新增domain quiet仍0（read_count1）。RLAST进入DS R spill后count0但spill未交producer前仍quiet0；完全归还且所有producer idle才quiet1。MRIF交接/ignore和FQ/CQ延迟B也覆盖，不能只驱动一个自写burst公式。
5. 逐一持住TC header/W FIFO、TL fast/slow response、demux B/R spill/accepted owed response、MRIF队列/WAIT_INT_ID、CQ“head==tail但DECODE”和WRITE/B_RESP、fault各spill/FIFO、FQ/PQ写、MSI pending、program AW-only/读返回库存；其他域空时quiet均0。清空后quiet1；PSEL恒1和CQ陈旧wr_state不能永久自拒。masked MSI pending、ERROR conservatively非quiet的恢复口径单列，不靠MODE清错。
6. tracker under/overflow/simultaneous min/max、reset中断、ARM/STOP/policy_clear不清库存；track bad后数据通道与gold仍逐拍相同、quiet不再真。MODE意图/INDEX访问在外围后批测不自拒，self-clear依赖用实际RTL路径核无环。

每次有限compile/run有明确预算、新输出目录、实际完整源码commit/dirty快照/命令及原失败；不启动长仿真/FPGA。本提案不是S15门、物理合格或论文n。完整BIT仍须Root接受各小批后唯一DS0先构建，原50MHz与routed硬门/失败停止不变，S15实际过才可同源DS50。

## 6. 待Root实际代码批锁定的窄点

- 接受只读vendor AXI empty/idle出口及其独立依赖pin，或指定另一份**真实已存在**的同域empty接口；不使用生产hierarchical引用、不重造TC计数。
- top类型组织选原类型defaultoff分支+enabled PN桥接，避免全局扩大原S类型；实际diff需保持一个共享TL。
- tracker建议17FF合并bad；无新MMIO字段。它补已确认MSIPTW尾拍缺口，不能被MSI FSM idle替代。

其余寄存器合法值/193 history/194 flags、debug来源和DS tracker边界已由Root锁定；不重新要求用户普通许可。模型元数据未暴露，Codex-only，未改模型缓存/窗口。
