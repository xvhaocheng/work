# 四条残余反馈路径：有限正常S1域的源码排除

作者：root，Codex gpt-6.1-sol／ultra。所有数值界是下面限定域的源码推导；本轮没有新运行，也没有重新解析旧trace。AR独占席的完整证明由root核关键原文后接受，见[AR证明](../ar-bound/ar-bound.md)。

## D：证明域与普通控制

从真实空复位开始，两个真实private iDMA各只有一个DEFAULT、对齐4KiB作业、各自512拍buffer；每job两个256拍AR及两个AW，没有先前payload、warmup、batch、CPU、ATOP、pool、额外任务或人为stall。沿用1PTW、IoTLB16、NativeN、原有队列和完整walk。两个固定正常context为S1 Sv39、pdtv0、S2 Bare，ATS/debug/其他DS源为0，页表及context不改变、无错误注入，所有请求走同一DDR目的地。此域不含生产四小buffer、多作业、S2／implicit、其他DS入口、CPU或真实MIG服务。

使用一个完整普通规则O-RR-DSAR：NormalTL沿用原生RR、单头立即处理、首次VALID锁及NativeN；共享AR采用上一轮冻结O-DS-AR（只优先currentVALID DS，否则RR，完整held／spill／fullID，四次DS公平逃逸）；返回采用已批准的全任务DS优先，保留同ID及已held R，W/B服务仍独立。这个组成在结果之前固定，不拼接不同场景的事后最佳规则。

O-DS-AR的零新增注册选择周期是其抽象逻辑合同，**不是已实现的时序／费用验收**。下述排除是该合同与现有条件memory服务下的因果结果；增量端口、真实select延迟、面积/Fmax/CPU/应用仍unknown，不能写成实测费用等价。既有九项完整NormalTL规则保留，本证明不声称逐一求得全部任务最优排序。

完整DS返回优先的具体memory-law参考是上一轮已验私有 `out/pte-safe-stage-causality-20261014/model/safe_pte_stage_probe.cpp:679–719`（`:689` 的窗口为true，held保护及原head准备保持）。原通用tests CPP的旧win/full-era窗口不是本证明的全任务优先合同。本轮只读取该源码，不运行它；DS优先内部queue提升也不是已证明存在于真实MIG的公开硬件接口。

## L1：生产者守恒，不用trace峰值替代

`rv_iommu_ptw.sv:276–278,384–397,507–510`：唯一localID0、len0；AR接受后停在PROC_PTE，只有该response消费后才产生下一层AR。DDTW `:178–185,247–261,299–300,395–408,424–428,469–481`：唯一localID1；非叶消费后续读，正常叶消费最后slot3／slot7后UPDATE；错误也保留RLAST drain。因而每engine未消费的读义务至多1，包括AR、DDR和返回buffers中的全部位置。

```text
全部payload AR ≤ 2×2 = 4
DDR未返回DS AR ≤ 未消费PTW1 + 未消费DDTW1 = 2
Q_DDR ≤ 4+1+1 = 6 < readqueue16
comp任一localID计数 ≤ 4 < demux正常full7
DS localID0／1计数各 ≤ 1 < 7
```

这个保守界独立于返回是否前进，不把DDR pop当walker退休。已接受的metadata／R/W/B义务一个也没有删除。完整AR证明进一步从empty reset、spill更新及DS delayed头间距53得出共享AR输入READY恒1、共享AR held不可达、DS front／route每次新头到达时均空；原生RR可能的当前DS位移由冻结O-DS-AR覆盖，fairness streak≤1，阈值4保留但本域不触发。source2本地spill和锁可以积累，不扩成全部cuts恒READY。

## L2：旧DDTW完整消费，才有新PTW；NativeN不启动另一个walk

TL `rv_iommu_tl_wrap.sv:284–286` 的normal_gate只在IDLE且无旧request开放；`:303–329` 的NativeN窗口只探测另一bank的cache hit，接口无DS启动。`:360,375` 分别限定DDTW init在DDT／ATS-DDTC、PTW init在PT；本top ATS-DDTC为0 (`tests/nonblock_banked_top.sv:625–632`)。

DDTW正常最后slot消费→UPDATE→DDTC更新；TL DDT见DDTC hit后才按context进入PT (`rv_iommu_ddtw.sv:424–428,469–471`；TL `:1103–1107,1153–1166`)。固定context无S2／implicit，因此不会边等待DDTW叶数据边走隐式PTW。PTW全部完成后TL才COMPLETE并响应原owner，新slow owner以后才能进DDT。cache hit旁路不创建另一DDTW／PTE。

于是任一正常PTE出生时：上一层len0 PTE已消费，或该首次walk所需DDTW最后beat已消费；不存在另一engine的未消费DS义务。已被最后消费者消费的该DS事务不可能还在任何上游FIFO／spill／delayer里。它的AR输入准备也早在DDR返回之前完成。此结论来自状态与完整消费关系，不依赖DID数或旧trace中没有DDTW/PTE重叠。

## L3：自有容量只排自身溢出；共同返回恢复另证明

`dma_core_wrap.sv:207,319,339` 配置1D response consumer恒1、NO_ERROR_HANDLING；`idma_backend.sv:531` 将r_dp_rsp_ready接到rsp_ready；`idma_axi_transport_layer.sv:255–257` 的RREADY仅由byte-lane buffer的in_ready限制。对齐DEFAULT头offset／shift0，尾mask为完整字节（`:188–218,232–233`）。`idma_buffer.sv:35–50` 每lane独立512项；实际编译CVA6 `stream_fifo.sv:43–45` 与 `fifo_v3.sv:65,89–123` 按真实计数判full及入／出队。

令c为该源已在DMA接受的R拍数，w为已经实际消费的W数据拍数。某个byte lane的占用q=c−w≤c。只要尚有该作业的一拍payload R在任何上游位置，c≤511，因此q≤511<512，那个R的合法owner具有吸收空间；512拍全部收到后变full不阻挡另一源或DS。对齐全mask下无需借用另一个源／W-owner的storage。这排除自身容量溢出，却**不证明全运行DDR payload永不held**。

旧DDTW8拍可以在DS返回delay及共同R spill留下尾部，从而间接hold某个payload。该历史全部保留。设b为新PTE首次VALID的preedge；L2保证在b之前旧DS最后beat已在真实walker消费，旧DS在整条返回路无剩余token。此时共同路中剩余token只有带原ID的payload，它们各自有上面的真实吸收空间。

本wrapper DDR→payload DMA的有效R storage只有四个非bypass两项spill：

| 阶段 | 原文 |
|---|---|
| shared DDR master mux Rspill | `axi_xbar.sv:150–154`；IOMMU `axi_mux.sv:451–462` |
| source2 route一个multicut Rspill | CVA6 `axi_xbar_unmuxed.sv:231–250`、`axi_multicut.sv:57–74`、`axi_cut.sv:100–111` |
| source2 frontend demux Rspill | xbar_unmuxed `:195–199`；IOMMU `axi_demux.sv:175–186` |
| bank-local TC completion demux Rspill | `rv_iommu_trans_ctl_wrap.sv:541–568` |

bank input／output Rspill明确bypass (`rv_iommu_banked_ctl.sv:298,440`)，四input DMA mux省略SpillR、实际默认为0 (`tests/nonblock_actual_dma_top.sv:680–697`；`axi_mux.sv:55`)；TC响应R为透传（`:361,365`）。没有另一个免费或漏计的共同返回池。

spill的精确式 `ready_o=!a_full||!b_full`、`b_drain=b_full&&ready_i`、`b_fill=a_drain&&!ready_i`、`a_drain=a_full&&!b_full` 位于实际编译 `spill_register_flushable.sv:76–96`。最下游合法payload消费者有空间后，full两项spill在一个接受edge后B清空、READY恢复；下游持续可收时B不再填满。沿四段逆向归纳，READY恢复最多四edge；取保守八拍界，包含阶段采样余量。这个界恢复的是共同payload路径的接收能力，**不是八拍完成全部512R/W/B**。FIFO中仍欠的其他payload和各锁原样保留，持续一拍一拍服务不会再次以自身容量堵塞。

下一PTE的AR必须经DS_IF spill及50输入delay后才到DDR，源码最低准备窗口52，完整AR路径56。因而在它到DDR前已有 `8<52<56` 的恢复余量；旧DDTW导致的payload held不能跨过这整个窗口成为PTE DDR首次返回的额外等待。若改变delay、增加R storage、作业数、异常路径或消费者，这个不等式必须重建，不能照搬。

## L4：四条因果路径的统一裁决

| 路径 | 本域结论 | 不能混入的量 |
|---|---|---|
| 前驱consume→下一PTE birth | 正常non-leaf消费当edge决定MEM_ACCESS，下一preedge首次VALID，间隔1；payload无独立输入 | 整个walk晚开始、IoTLB hit使PTE不存在、不同leaf深度 |
| birth→内部DS／DDR AR接受 | 唯一DS及空输入链，内部0；ODS覆盖currentVALID RR位移，完整路径56 | 假想满队列、撤回held、在DS尚未VALID时用未来信息抢选 |
| DDR AR接受→DDR firstRVALID／握手 | 此时无旧DS／held payload阻挡；完整DS优先选该len0请求，head准备31后firstR为32 | 未被DDR接受的PTE、隐藏MIG状态、DDS长burst自背压 |
| DDR firstR→PTW consume | DS分路空，三xbar Rspills+52返回delay+DS_IF spill=56，PTW在PROC_PTE消费，DDR接受→consume=88 | DDR pop当PTW退休、其它consumer、故障或隐式walk |

因此对同一个实际存在、准确owner/context/stage/level的PTE，在D及固定O-RR-DSAR逻辑合同下，以上相对阶段不因payload先后改变。这里32／56／88是源码推导且与旧已验数据相容，**本轮新测量次数为0**；不存在本轮AB／BA delta或新完成时间。

payload可以利用walk生成／DS准备间隙推进，改变其自身完成时间；翻译排序还可能改变whole-job completion。那些是完整任务调度问题，不能作为“payload延长同PTE相对等待”的正例。也不能在W-owner／独立阶段前提未证时宣称Johnson/O-two-stage最优。本证明没有把候选与所有普通排序的whole-job完成或完整硬件成本判成等价。

## 本轮停点与可推翻条件

首关已有范围排除，无具体未覆盖PTE反馈链，故不寻找另一自然双头、不创建新probe／预测器、不运行A/B或四条件。停止的是D+普通优先合同下的该因果主张；F1全族、真实MIG、不同自然义务域及物理实现成本仍开放。

可推翻本证明的输入必须是一项新原始事实：同一合法源约束下的另一个DS并发来源；超出本计数且原本已拥有的请求；固定返回路中漏列的storage／consumer等待；或者真实共享DDR服务使已有DS优先无法实现上述阶段屏蔽。先指出哪一个明确前提失效及具体普通控制边界，再立新合同。不能手填full/held、增加压力、减少buffer或改条件memory返回策略制造正例。
