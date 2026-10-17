# E6-PN 测量合同：现有字段、分母与缺口

> This original proposal is retained for source evidence. Current runtime words,
> legal0/1/2/3/6, flag meanings and trial counts are frozen in
> [runtime-contract-v1.md](runtime-contract-v1.md). Accepted implementations and
> their limits are indexed by [README](README.md); the separate future writer
> uses [writer-contract-v1.md](writer-contract-v1.md), keeping pn-control readonly.
> “待实现” in the original body describes that proposal's original stage.

## Material Passport

ARS `experiment-agent / plan`；2026-10-18 JST；状态为**源码核查与待审方案**。
本批只读固定 Git 对象和已有 baseline100 证据，没有修改组件、编译、仿真或板操作，新增板点0。
模型请求为 `gpt-6.1-sol / ultra`；本席工具未提供独立运行时模型元数据，实际型号/原生上下文大小记 unknown，不以请求冒充验收，也未修改配置。共享研究保持 Codex-only。

## 固定来源及引用记号

- **B**：platform `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273`。
- **C**：CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`。
- **I**：IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`。
- **L**：Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`。
- **T**：实际 baseline CLI/config 输入 platform `a2e847539bb638662f98dc920de0e8fd63d046e3`。
- **D**：历史 N 算法参考 IOMMU `0b7939445298a23ea3e131ffd4e003cebb15ac59`，不是本次 baseline，也不是授权整树移植。

每个 `C:path:line` 等引用均按该完整 commit 的 `git show COMMIT:path` 编号；不指活动 HEAD。
`tool-source-map.json` 保存实际 blob/SHA、关键行、原始命令、baseline试次引用和当前适用性。
根身份为 `paper/ispass2027/board/baseline-source.json`。实际 software01 的
`selected_platform_software_inputs=T`，CLI SHA为
`f91112f6d844af1fc683266a87a287edcbc42ddd9b3b419f40e37aac16a732f8`；不能拿B的同名bench替代。
证据：`board/manifests/software-build.json:14–30,32–49` 与
`board/manifests/build_baseline.as-dispatched.py:230–254`。

## 实际任务预算与窗口

原命令的核心是：

```text
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor none|1 --aggressor-pages k --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
```

Solo取`none,k=1`，Ak取`1,k=k`。完整原始wrapper（kernel/boot/taint/PRE_SEQ、CLI、CASE_EXIT、dmesg、TAINT_AFTER）在source map中原字节引用；冻结collector为
`board/manifests/collect.baseline100-as-dispatched.py:214–223`。

**只有V是有限4096；A不是另一项有限4096完整任务。** L `drivers/misc/idma.c:941–943`
给V写入输入target、A写入`IDMA_EXPERIMENT_MAX_TRANSFERS`；L `include/uapi/linux/idma.h:9`
为`2^32`。A先启动，V后启动，驱动仅轮询V（L `idma.c:590–615`）；V完成发现后才STOP A，随后双方drain与读最终batch计数（`:963–1000`）。
warmup为旧软件refill256，先完成/排空再开启autonomous batch（`:919–946`）；不是测量段CPU逐copy供给。

定义`W_V=(V首issue边沿,V第4096个done边沿]`：C `dma_observer.sv:148–156,171–204`
只在V首issue后进入active，最后done边沿计入active并结束。`cycles`不含最终软件poll的`done_to_read`（`:163–169`）。
50MHz下`T_V=cycles/50e6`秒，逻辑COPY完成字节为`4096×4096=16MiB`，
`R_V=16/T_V MiB/s`。每次COPY只计一次，不把读+写相加冒称COPY吞吐。

## 用户所需字段逐项资格

| 字段 | 现有baseline能否得到 | 精确定义/证据/最小缺口 |
| --- | --- | --- |
| V硬件时间、吞吐 | 可以 | 上述`W_V`与16MiB；L `idma.c:810–813`输出cycles/h/gap/hmax/gapmax；CLI `victim_elapsed_ns`另属软件时长，T `bench/idma_experiment.c:323–331`明示。 |
| A硬件firstissue→lastdone时间 | baseline没有；**用户已批准最小被动捕获** | C observer只维护V expected/target/done，`:148–156,194–204`；A issue/done输入虽接入，未形成A own计时。`active_cycles`是backend非idle累计（C frontend`:303`），且驱动软件bracket差，不能代替A墙钟时长。新接口见后文。 |
| A最终完成数、字节、正确/排空 | 可以 | L `idma.c:543–568,977–1013`读issued/completed并核一致、末态验证。包括A提前启动、V结束后的STOP/drain；UAPI `idma.h:81–85`明示。不是同窗量。 |
| A own吞吐、双方完整批次共同时间吞吐 | 现有字段不能精确给出；新捕获通过后可给 | 缺A生命周期硬件时间、共同首发至共同最后done的分母。禁止用A最终字节除`W_V`，或将两个独立分母速率相加称共同完整任务吞吐。用户已批准最小捕获；实现/冻结有效性通过前仍N/A。 |
| A在V窗口的有效速度、双方同窗合计 | 可给**响应记账proxy和边界** | 现有`axi_a.b/rlast`同`W_V`累计（C observer`:175–189`），4KiB固定COPY下每突发2048B。`C_A=2048×B_A`；`R_A_proxy=C_A/W_V`，`R_total_proxy=(16MiB+C_A)/W_V`。明确同窗响应记账，不称A own吞吐。 |
| V/A IOTLB miss、S1/S2 PTW | 可以保留原HPM | L `iommu-hpm.c:71–75,133–160,175–195`六槽、每DID三event；`idma.c:952–975`软件顺序前/后差。它们不与硬件`W_V`原子同窗，不能归为精确窗内事件。 |
| N实际触发 | baseline没有；新PN需最小新增 | 不能由probe hit、fast response数或miss差反推。D `rv_iommu_nonblock_hit.sv:100–107`真正接受为`accept_o`，TL `rv_iommu_tl_wrap.sv:287`的`n_accept/nonblock_fire_o`。建议同`W_V`按DID10/11两饱和计数+overflow invalid；不是旧composition observer整块。 |
| V分段等待 | 可以给现有4阶段 | C path `:132–142`的五事件/四段；每role×r/w各4段，字段entered/full/carry/sum/max/start/end/area。只报完整段`sum/full`及max，零full=N/A；不声称请求分位数。 |
| IOMMU入口AW等待 | 可以 | L `idma.c:828–832`的`shared_aw_wait`，C observer`:189`为shared `AWVALID&&!AWREADY`的周期数，窗口为`W_V`。这是入口背压，不是PTW服务时间/具体owner归因；不能与PATH或其他条件累计相加。 |
| 页表AR→R累计 | **跳过** | 现有OBS3接payload TR/COMP端口，没有DS/PTW请求ID/返回配对tap（C peripheral`:924–946`）。HPM PTW是事件次数，不是AR→R延迟。用户禁止为此加硬件，不移植旧诊断bank。 |

HPM event ID为miss`0x4`、S1 PTW`0x7`、S2 PTW`0x8`（L `iommu-bits.h:215–219`）。
I `rv_iommu_tl_wrap.sv:1093–1096`在PT态未命中时置miss及启用stage的PTW位，
I `rv_iommu_hpm.sv:263–274`对持续valid去重复计数。因此不能解释为每一PTE读、PTW占用周期或N接受数。
原六槽selector readback只证明配置，不增加事件板端校准结论（L `iommu-hpm.c:125–131`）。

## 同窗proxy的边界和守恒

原baseline100已审口径为`analysis/scripts/baseline100_extra.py:109–140,375–420`。
64位AXI且最大256beat（C `idma_pkg.sv:107–110`、`idma_legalizer.sv:75–83`），
CONF=0（L `idma.c:342–346`），故固定aligned4096 COPY拆为两个2048B突发。
逻辑完整COPY字节仍按一次4096，响应proxy按实际B事件×2048；不固定假设PN各点A同窗也恰为16MiB。
这一2048B因子是原C配置的源码结论；D1的N/PN新支路也须在Root源码审查/有限波形中保留相同AXI宽度、burst拆分与CONF。
若新支路不满足，不沿用该proxy因子或界的前提，不用旧baseline证据代替新支路验证。

严格私有接口窗口内W有效字节没有采集。合法AXI下令`D`为已W握手但未获B记账的字节，
`S_A=C_A+D_end−D_start`；前端COUNT→WAIT、任务完成后才发下一任务，单路最多一项4KiB任务在途
（C frontend`:332–363`），因此`C_A−4096≤S_A≤C_A+4096`，下界按0截断。
这是私有接口字节边界，不是物理DDR落地、BRESP逐次正确性或整COPY同窗完成证明。
最终data_errors=0/quiet/完整排空原检查仍保留，不能宣称逐copy内容已被独立验证。

所有原私有stock/flow及PATH守恒保留。不得硬断言shared==V私有+A私有：mux有spill边界。
应使用同窗`PATH vr0.full+carry+ar0.full+carry==shared_ar`、
`vw0.full+carry+aw0.full+carry==shared_aw`（两设备）；stage0 enter是首次offer，不是私有accept。
已有Root `board/observer-shared-counts.md`给出此区别；不为了PN复制旧错误公式。
HARDWARE h、frontend gap、PATH段、入口AW等待彼此可能重叠，不能相加解释总耗时。

## 用户已批准的最小被动A own/共同分母接口

Root转达用户明确选择“允许最小被动时长计数（推荐）”；以下正式纳入本批合同，但**尚未实现或板测**。
与原`W_V`独立的被动生命周期捕获从原ARM后首A issue开始，
STOP时仍保持，直到最后A done且前端/后端quiet后冻结；累计`cycles_A_own`。
共同分母为两lane首issue最小值至两lane最后done最大值，记`cycles_union`，不能用`max(T_V,T_A)`替代不同起点的并窗。
每lane最终batch completed_bytes与该被动窗口必须一一对应；warmup不计，timeout/overflow/未drain均invalid。
然后才可报`A_final_bytes/T_A_own`与`(V_final_bytes+A_final_bytes)/T_union`。
不得为获得这些量把A预算改成4096自然结束，因为这会改变原baseline共跑合同。

候选可用两64bit运行elapsed counter（首A issue、首任一路issue起算）加两64bit最近done elapsed latch（A done、任一路done更新），
约256个计数FF及少量seen/running/frozen/overflow标志，无事件FIFO/BRAM；这是未综合设计估计。
用现有ARM/role/hasA、issue/done、batch_active与backend/private quiet信号即可划定生命周期，
warmup排空→ARM→HPM before→A/V启动的L `idma.c:936–957,590–597`顺序保证ARM早于测量A首issue。
只在V target done且STOP后的A全部quiet时冻结，CLI drain完成后稳定读；不在S数据通路引入控制。
新bank197=A own cycles、198=union cycles、199=valid/overflow状态，替换此前待选reserved0。
实现必须保留只读冻结结果、溢出/缺边沿/未quiet拒绝资格；不把未实现的新字段提前写成测量值。
旧同V窗proxy/字节界继续另报，不能与whole-work量混列同名。页表AR→R仍跳过，不加相关硬件。

## 历史证据适用边界

- `composition-history-isolation-20261007.md §3:25–34`是旧composition/isolated、n3、不同BIT/boot；
  A窗口次数相同不等于吞吐相同，也不能拿其PN/P时间作本批S门槛。
- 同文§8 `:88–105`为行为DDR/真实DMA仿真，V64的窗口与下游返回干预；非MIG/Linux完整时序。
  §9 `:113–131`为旧内核/旧V9 BIT的18点，V64、gap改变A工作/触发强度。其`sharedAWwait`仅入口VALID&&!READY。
- `workflow-g0-m0-results-20261010.md:7,33–41`为双方各64自然完成、共同t0的**仿真full-batch**。
  它确有各自最后done/共同完成，但不能证明当前FPGA OBS3具备这些字段。
- `problem-discovery-results-20261017.md:17–21,34,38–44,75–79`为batch-OFF、无warm、各64、A源固定/目的轮转，
  且已有DS-first，不是本批baseline。14项masked PLRU不保证A8热后无walk；实际probe hit也不等于N accept。
- `ddr-service-boundary-20261014.md:5–17`限定实际MIG已接受payload的不可撤销前序债务；
  不用模型DS优先或入口AW等待冒称页表返回实测/因果定量。上述历史后继均未因本批恢复。

S15、正式矩阵、配对与接口草案见 `software-plan.md`。当前没有E6-PN测量结果。
